/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-26     LEGION       the first version
 */

#ifndef DRIVERS_DRV_W25QX_H_
#define DRIVERS_DRV_W25QX_H_

#include "drv_spi.h"

/*
//#define SPI_FLASH_PageSize            4096
#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

typedef enum w25qx_state_c{
    W25X_FLSH_ID,                       //芯片ID            90
    W25X_WriteStatusReg,                //写状态                               01
    W25X_PageProgram,                   //页编程                               02
    W25X_ReadData,                      //读数据                               03
    W25X_WriteDisable,                  //禁止写入                           04
    W25X_ReadStatusReg,                 //读状态                               05
    W25X_WriteEnable,                   //写使能                               06
    W25X_SectorErase,                   //扇区擦除  4KB       20
    W25X_BlockErase,                    //块擦除      64KB      D8
    W25X_ChipErase,                     //全片擦除                           C7
    W25X_PowerDown,                     //掉电模式                           B9  低功耗模式
    W25X_StopErase,                     //停止擦除                           75
    W25X_StartErase,                    //恢复擦除                           7A
    W25X_ReleasePowerDown,              //解除低功耗或高性能模式 AB
    W25X_FastReadData,                  //快速读数据                        0B
    W25X_FastReadDual,                  //双倍快速度数据                 3B
}w25qx_state;


struct w25qxx_state_f{

};
*/
struct w25qx{
    struct rt_hw_spi w25qx_sample;
};

struct w25qx w25q256;

void SPI_Flash_WAKEUP(void);
void W25QXX_Erase_Sector(uint32_t ReadAddr);
uint8_t SPI_FLASH_ReadID(struct w25qx *w25qx);
void SPI_FLASH_Init(struct rt_hw_spi *rt_hw_spi_t);
uint8_t W25QXX_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void W25QXX_Write( uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);

#endif
