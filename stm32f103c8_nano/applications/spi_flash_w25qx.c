/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-25     LEGION       the first version
 */


/*
0XEF13,芯片型号为W25Q80       8M    1M        256                  4096                     16           256扇区
0XEF14,芯片型号为W25Q16       16M   2M        256                  8192                     32           512
0XEF15,芯片型号为W25Q32       32M   4M        256                  16384                    64           1024
0XEF16,芯片型号为W25Q64       64M   8M        256                  32768                   128           2048
0XEF17,芯片型号为W25Q128  共有256M位   16M字节  每页有256位 16字节  共256x256 = 65536页   其字节地址共有256x256x256个  因此需要3个8字节的数据做为地址
*/
#include <stdio.h>
#include <board.h>
#include <string.h>
#include "spi_flash_w25qx.h"
#include "uart_config.h"
union w25qx_addr{
    uint32_t addr;
    struct addr_k_t{
        uint8_t l_addr;           //低地址位
        uint8_t m_addr;           //中地址位
        uint8_t h_addr;           //高地址位
        uint8_t n_addr;           //此位为空
    }addr_k;
};


union w25qx_addr w25qx_addr_t;    //w25q要操作的地址位
uint8_t W25QXX_BUFFER[4096];



//获取FLASH id  0XEF14 0XEF15 0XEF16 0XEF13 0XEF17
uint8_t SPI_FLASH_ReadID(struct w25qx *w25qx)
{
    uint8_t W25X_DeviceaID[4] = {0x90, 0, 0, 0};
    uint8_t flash_id[2];
    rt_spi_send_then_recv(&w25qx->w25qx_sample, W25X_DeviceaID, 4, flash_id, 2);
    return flash_id[1];
}



//读取W25QXX的状态寄存器  1繁忙 0空闲
static uint8_t W25QXX_ReadSR(void)
{
    uint8_t byte = 0;
    uint8_t W25X_ReadStatusReg = 0x05;
    rt_spi_send_then_recv(&w25q256.w25qx_sample, &W25X_ReadStatusReg, 1, &byte, 1);
    return byte;
}

//判断是否繁忙  繁忙就每隔timeout毫秒判断一次
static void W25QXX_Wait_Busy(uint32_t timeout)
{
    while((W25QXX_ReadSR()&0x01) == 0x01)
    {
        rt_thread_mdelay(timeout);
    }
}


//写使能
void W25QXX_Write_Enable(void)
{
    uint8_t a = 0x06;
    rt_spi_send(&w25q256.w25qx_sample, &a, 1);
    W25QXX_Wait_Busy(1);
}

//写一页地址   无校验是否有数据
static void W25QXX_Write_Page(uint32_t WriteAddr, uint8_t *pBuffer, uint16_t NumByteToWrite)
{
    w25qx_addr_t.addr = WriteAddr;
    w25qx_addr_t.addr_k.n_addr =  w25qx_addr_t.addr_k.m_addr;
    w25qx_addr_t.addr_k.m_addr =  w25qx_addr_t.addr_k.h_addr;
    w25qx_addr_t.addr_k.h_addr =  w25qx_addr_t.addr_k.n_addr;
    w25qx_addr_t.addr_k.n_addr =  w25qx_addr_t.addr_k.l_addr;
    w25qx_addr_t.addr_k.l_addr =  0x02;
    W25QXX_Write_Enable();                                                                        //写使能
    rt_spi_send_then_send(&w25q256.w25qx_sample, &w25qx_addr_t.addr_k.l_addr, 4, pBuffer, NumByteToWrite);
    W25QXX_Wait_Busy(10);
}


//无校验是否擦除   写数据
void W25QXX_Write_NoCheck(uint32_t WriteAddr, uint8_t *pBuffer, uint16_t NumByteToWrite)
{
    uint16_t pageremain;

    pageremain = 256 - WriteAddr % 256;
    if(NumByteToWrite <= pageremain)pageremain = NumByteToWrite;
    while(1)
    {
        W25QXX_Write_Page(WriteAddr, pBuffer, pageremain);
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


//擦除一片扇区
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
    W25QXX_Wait_Busy(20);                                                 //10毫秒检测一下是否繁忙
}

//读取任意长度的地址                          ReadAddr地址                    pBuffer存储空间                    长度
//返回值 0无数据  1有数据
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

//向W25QX写入任意长度的数据    自动翻页   自动检查擦除
void W25QXX_Write( uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff, secremain, i;
    uint8_t  *W25QXX_BUF;
    secpos     = WriteAddr / 4096;                              //地址在第几片
    secoff     = WriteAddr % 4096;                              //第几片的第几位
    secremain  = 4096 - secoff;                                 //当片还剩多少地址
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
            W25QXX_Write_NoCheck(secpos * 4096, W25QXX_BUF, 4096);
        }
        else W25QXX_Write_NoCheck(WriteAddr, pBuffer, secremain);
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

/*
static void w25qx_thread_entry(void *parameter)
{
    //uint8_t a[5] = {0x01, 0x01, 0x04, 0x03, 0x05};
    //uint8_t b[10];

    while(1)
    {
        //W25QXX_Write(0xFFE, a, 5);
        rt_thread_mdelay(100);
        //W25QXX_Read(0xFFE, b, 5);
        rt_thread_mdelay(1000);
    }
}



static int spi_w25qx_sample(void)
{
    SPI_FLASH_Init(&w25q256.w25qx_sample);//初始化配置
    rt_thread_t w25qx_thread = rt_thread_create("w25qx", w25qx_thread_entry, RT_NULL, 1024, 24, 20);
    if (w25qx_thread != RT_NULL)
    {
        rt_thread_startup(w25qx_thread);
    }

    return 0;
}

INIT_APP_EXPORT(spi_w25qx_sample);
*/


