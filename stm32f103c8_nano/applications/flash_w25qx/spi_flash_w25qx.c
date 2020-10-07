/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-25     LEGION       the first version
 */


/************************************************************************************
0XEF13,     25Q80          8M           1M           256             256
0XEF14,     W25Q16          16M          2M           256             512
0XEF15,      W25Q32          32M          4M           256             1024
0XEF16,      W25Q64          64M          8M           256             2048
0XEF17,      W25Q128         128M         16M          256             4096
***********************************************************************************/
#include <stdio.h>
#include <board.h>
#include <string.h>
#include "spi_flash_w25qx.h"

union w25qx_addr{
    uint32_t addr;
    struct addr_k_t{
        uint8_t l_addr;           //
        uint8_t m_addr;           //
        uint8_t h_addr;           //
        uint8_t n_addr;           //
    }addr_k;
};


union w25qx_addr w25qx_addr_t;    //
uint8_t W25QXX_BUFFER[4096];



//
uint8_t SPI_FLASH_ReadID(struct w25qx *w25qx)
{
    uint8_t W25X_DeviceaID[4] = {0x90, 0, 0, 0};
    uint8_t flash_id[2];
    rt_spi_send_then_recv(&w25qx->w25qx_sample, W25X_DeviceaID, 4, flash_id, 2);
    return flash_id[1];
}



//
static uint8_t W25QXX_ReadSR(void)
{
    uint8_t byte = 0;
    uint8_t W25X_ReadStatusReg = 0x05;
    rt_spi_send_then_recv(&w25q256.w25qx_sample, &W25X_ReadStatusReg, 1, &byte, 1);
    return byte;
}

//
static void W25QXX_Wait_Busy(uint32_t timeout)
{
    while((W25QXX_ReadSR()&0x01) == 0x01)
    {
        rt_thread_mdelay(timeout);
    }
}


//
void W25QXX_Write_Enable(void)
{
    uint8_t a = 0x06;
    rt_spi_send(&w25q256.w25qx_sample, &a, 1);
    W25QXX_Wait_Busy(1);
}

//
static void W25QXX_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    w25qx_addr_t.addr = WriteAddr;
    w25qx_addr_t.addr_k.n_addr =  w25qx_addr_t.addr_k.m_addr;
    w25qx_addr_t.addr_k.m_addr =  w25qx_addr_t.addr_k.h_addr;
    w25qx_addr_t.addr_k.h_addr =  w25qx_addr_t.addr_k.n_addr;
    w25qx_addr_t.addr_k.n_addr =  w25qx_addr_t.addr_k.l_addr;
    w25qx_addr_t.addr_k.l_addr =  0x02;
    W25QXX_Write_Enable();                                                                        //
    rt_spi_send_then_send(&w25q256.w25qx_sample, &w25qx_addr_t.addr_k.l_addr, 4, pBuffer, NumByteToWrite);
    W25QXX_Wait_Busy(10);
}


//
void W25QXX_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t pageremain;

    pageremain = 256 - WriteAddr % 256;
    if(NumByteToWrite <= pageremain)pageremain = NumByteToWrite;
    while(1)
    {
        W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
        if(NumByteToWrite == pageremain)break;
        else
        {
            pBuffer        += pageremain;
            WriteAddr      += pageremain;
            NumByteToWrite -= pageremain;
            if(NumByteToWrite > 256)pageremain = 256;
            else pageremain = NumByteToWrite;
        }
    }
}


//
void W25QXX_Erase_Sector(uint32_t ReadAddr)
{
    ReadAddr  = (ReadAddr / 4096) * 4096;
    w25qx_addr_t.addr = ReadAddr;
    w25qx_addr_t.addr_k.n_addr =  w25qx_addr_t.addr_k.m_addr;
    w25qx_addr_t.addr_k.m_addr =  w25qx_addr_t.addr_k.h_addr;
    w25qx_addr_t.addr_k.h_addr =  w25qx_addr_t.addr_k.n_addr;
    w25qx_addr_t.addr_k.n_addr =  0;
    w25qx_addr_t.addr_k.l_addr =  0X20;
    W25QXX_Write_Enable();
    rt_spi_send(&w25q256.w25qx_sample, &w25qx_addr_t.addr_k.l_addr, 4);
    W25QXX_Wait_Busy(20);                                                 //
}

//®³
//
uint8_t W25QXX_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    uint8_t res = 0;
    w25qx_addr_t.addr = ReadAddr;
    w25qx_addr_t.addr_k.n_addr =  w25qx_addr_t.addr_k.m_addr;
    w25qx_addr_t.addr_k.m_addr =  w25qx_addr_t.addr_k.h_addr;
    w25qx_addr_t.addr_k.h_addr =  w25qx_addr_t.addr_k.n_addr;
    w25qx_addr_t.addr_k.n_addr =  w25qx_addr_t.addr_k.l_addr;
    w25qx_addr_t.addr_k.l_addr =  0X03;
    rt_spi_send_then_recv(&w25q256.w25qx_sample, &w25qx_addr_t.addr_k.l_addr, 4, pBuffer, NumByteToRead);
    for(uint16_t i = 0; i < NumByteToRead; i++)
    {
        if(pBuffer[i] != 0xFF){
            res = 1;
            break;
        }
    }
    return res;
}

//
void W25QXX_Write( uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff, secremain, i;
    uint8_t  *W25QXX_BUF;
    secpos     = WriteAddr / 4096;                              //
    secoff     = WriteAddr % 4096;                              //
    secremain  = 4096 - secoff;                                 //
    W25QXX_BUF = W25QXX_BUFFER;
    if(NumByteToWrite <= secremain)secremain = NumByteToWrite;  //
    while(1)
    {
        if(W25QXX_Read(W25QXX_BUF, WriteAddr, secremain)){
            W25QXX_Read(W25QXX_BUF, secpos * 4096, 4096);
            W25QXX_Erase_Sector(WriteAddr);
            for(i = 0; i < secremain; i++)
            {
                W25QXX_BUF[i + secoff] = pBuffer[i];
            }
            W25QXX_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096);
        }
        else W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain);
        if(NumByteToWrite == secremain)break;
        else
        {
            secpos++;
            secoff          = 0;
            pBuffer        += secremain;
            WriteAddr      += secremain;
            NumByteToWrite -= secremain;
            if(NumByteToWrite > 4096)secremain = 4096;
            else secremain = NumByteToWrite;
        }
    }

}


void SPI_Flash_WAKEUP(void)
{
    uint8_t W25X_ReleasePowerDown = 0xAB;
    rt_spi_send(&w25q256.w25qx_sample, &W25X_ReleasePowerDown, 1);
}


void SPI_FLASH_Init(struct rt_hw_spi *rt_hw_spi_t)
{
    __MY_SPI_INIT(rt_hw_spi_t, SPI1, A, 5, A, 6, A, 7, C, 13);
    SPI_MODE(rt_hw_spi_t, HIGH, 2, 4);
    MX_SPIx_Init(rt_hw_spi_t);
}




