/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*                                                                       */
/*   Portions COPYRIGHT 2017 STMicroelectronics                          */
/*   Portions Copyright (C) 2014, ChaN, all right reserved               */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/


#include "diskio.h"
#include "ff_gen_drv.h"
#include "spi_flash_w25qx.h"

#define ATA 0                                          // 预留 SD 卡使用
#define SPI_FLASH 1                                    // 外部 SPI Flash

extern Disk_drvTypeDef  disk;



//pdrv ATA sd卡        SPI_FLASH flash   返回值0确定id成功  1失败
DSTATUS disk_status (BYTE pdrv)
{
    DSTATUS status = STA_NOINIT;
    switch (pdrv) {
        case ATA:
            break;
        case SPI_FLASH:
            if (0x14 == SPI_FLASH_ReadID(&w25q256))status &= ~STA_NOINIT; //设备初始化成功
            else status = STA_NOINIT;;                            //设备初始化失败
            break;
        default:
            status = STA_NOINIT;
            break;
    }
    return status;
}


DSTATUS disk_initialize (BYTE pdrv)
{
    DSTATUS status = STA_NOINIT;
    uint16_t i;
    switch (pdrv) {
        case ATA:
            break;
        case SPI_FLASH:
            SPI_FLASH_Init(&w25q256.w25qx_sample);
            i = 500;
            while (--i);
            SPI_Flash_WAKEUP();
            status = disk_status(SPI_FLASH);
            break;
        default:
                status = STA_NOINIT;
                break;
    }
    return status;
}

//pdrv 设备   buff 数据   sector 地址  count 扇区数
DRESULT disk_read (BYTE pdrv, BYTE *buff, DWORD sector,	UINT count)
{
    DRESULT status = RES_PARERR;
    switch (pdrv) {
        case ATA:
            break;
        case SPI_FLASH:
            W25QXX_Read(buff, (sector + 128) * 4096, count * 4096);
            status = RES_OK;
            break;
        default:
            status = RES_PARERR;
            break;
    }
    return status;
}


#if _USE_WRITE == 1

//pdrv 设备   buff 数据   sector 地址  count 扇区数
DRESULT disk_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
    DRESULT status = RES_PARERR;
    DWORD write_addr = sector + 128;
    if (!count) {
        return RES_PARERR;
    }
    switch (pdrv) {
        case ATA:
            break;
        case SPI_FLASH:
            W25QXX_Write((uint8_t *)buff, write_addr * 4096, count * 4096);
            status = RES_OK;
            break;
        default:
            status = RES_PARERR;
            break;
    }
    return status;
}
#endif


#if _USE_IOCTL == 1


DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void *buff)
{
    DRESULT status = RES_PARERR;
    switch (pdrv) {
    case ATA:
        status = RES_OK;
        break;
    case SPI_FLASH:
        switch (cmd) {
            case CTRL_SYNC :
                status = RES_OK;
                break;
            case GET_SECTOR_COUNT:            //扇区数
                *(DWORD *)buff = 400;
                break;
            case GET_SECTOR_SIZE:             //扇区大小
                *(WORD *)buff = 4096;
                break;
            case GET_BLOCK_SIZE:              //扇区单位
                *(DWORD *)buff = 1;
                break;
            default:
                status = RES_OK;
                break;
            }
        status = RES_OK;
        break;
    default:
        status = RES_PARERR;
        break;
    }
    return status;
}
#endif


__weak DWORD get_fattime (void)
{
    return ((DWORD)(2015 - 1980) << 25)
            | ((DWORD)1 << 21)
            | ((DWORD)1 << 16)
            | ((DWORD)0 << 11)
            | ((DWORD)0 << 5)
            | ((DWORD)0 >> 1);
}



