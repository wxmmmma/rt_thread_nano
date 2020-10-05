/**
  ******************************************************************************
  * @file    sd_diskio.c
  * @author  MCD Application Team
  * @version V1.4.1
  * @date    14-February-2017
  * @brief   SD Disk I/O driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"

#include "spi_flash_w25qx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
static __IO DSTATUS Stat = STA_NOINIT;
/* Private variables ---------------------------------------------------------*/
/* Disk status */
#define SPI_FLASH_REBUILD           1
#define SPI_FLASH_SECTOR_SIZE    4096
#define SPI_FLASH_START_SECTOR   1792
#define SPI_FLASH_SECTOR_COUNT   2304

/* Private function prototypes -----------------------------------------------*/
DSTATUS SPIFLASH_initialize(BYTE);
DSTATUS SPIFLASH_status(BYTE);
DRESULT SPIFLASH_read(BYTE, BYTE*, DWORD, UINT);
#if _USE_WRITE == 1
  DRESULT SPIFLASH_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */

#if _USE_IOCTL == 1
  DRESULT SPIFLASH_ioctl (BYTE, BYTE, void*);
#endif

const Diskio_drvTypeDef  SPIFLASH_Driver =
{
  SPIFLASH_initialize,                    
  SPIFLASH_status,                       
  SPIFLASH_read,                         
#if  _USE_WRITE == 1
  SPIFLASH_write,                        
#endif
  
#if  _USE_IOCTL == 1
  SPIFLASH_ioctl,                        
#endif
};


DSTATUS SPIFLASH_initialize(BYTE lun)
{
#if SPI_FLASH_REBUILD == 1
  static uint8_t startflag = 1;
#endif
  Stat = STA_NOINIT;
  SPI_FLASH_Init(&w25q256.w25qx_sample);
  SPI_Flash_WAKEUP();
  if(SPI_FLASH_ReadID(&w25q256) == 0x14)
  {
#if SPI_FLASH_REBUILD == 1
      if(startflag)
      {
        SPI_FLASH_SectorErase(SPI_FLASH_START_SECTOR);
        startflag=0;
      }
#endif
    Stat &=~STA_NOINIT;
  }
  return Stat;
}


DSTATUS SPIFLASH_status(BYTE lun)
{
  Stat = STA_NOINIT;

  if(SPI_FLASH_ReadID(&w25q256) == 0X14)
  {
    Stat &= ~STA_NOINIT;
  }
  return Stat;
}


DRESULT SPIFLASH_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{ 
  sector+=SPI_FLASH_START_SECTOR;      
  W25QXX_Read(buff, sector*SPI_FLASH_SECTOR_SIZE, count*SPI_FLASH_SECTOR_SIZE);
  return RES_OK;
}


#if _USE_WRITE == 1
DRESULT SPIFLASH_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
  uint32_t write_addr; 
  sector+=SPI_FLASH_START_SECTOR;
  write_addr = sector*SPI_FLASH_SECTOR_SIZE;    
  SPI_FLASH_SectorErase(write_addr);
  W25QXX_Write((uint8_t *)buff,write_addr,count*SPI_FLASH_SECTOR_SIZE);
  return RES_OK;
}
#endif



#if _USE_IOCTL == 1
DRESULT SPIFLASH_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  
  if (Stat & STA_NOINIT) return RES_NOTRDY;
  
  switch (cmd)
  {
  case CTRL_SYNC :
    res = RES_OK;
    break;
  case GET_SECTOR_COUNT :
    *(DWORD * )buff = SPI_FLASH_SECTOR_COUNT;	
    res = RES_OK;
    break;
  case GET_SECTOR_SIZE :
    *(WORD * )buff = SPI_FLASH_SECTOR_SIZE;
    res = RES_OK;
    break;
  case GET_BLOCK_SIZE :
    *(DWORD * )buff = 1;
    res = RES_OK;
    break;
  
  default:
    res = RES_PARERR;
  }
  
  return res;
}
#endif
  

