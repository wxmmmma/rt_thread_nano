/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-09     RT-Thread    first version
 */

#include <rtthread.h>
#include <drv_common.h>
#include <string.h>
#include "ff.h"
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>


FATFS fs;                        /* FatFs 文件系统对象 */
FIL fnew;                        /* 文件对象 */
FRESULT res_flash;               /* 文件操作结果 */
UINT fnum;                       /* 文件成功读写数量 */
BYTE buffer[1024]= {0};          /* 读缓冲区 */
BYTE textFileBuffer[] = "欢迎使用野火 STM32 开发板 今天是个好日子，新建文件系统测试文件\r\n";

int main(void)
{
    //fs = rt_malloc(sizeof(FATFS));
   /* res_flash = f_mount(&fs, "1:", 1);
    rt_kprintf("即将格式化%d\r\n", res_flash);
     if (res_flash == FR_NO_FILESYSTEM){
        rt_kprintf("即将格式化%d\r\n", res_flash);
        res_flash = f_mkfs("1:", 0, 0);
        if (res_flash == FR_OK){
            rt_kprintf("成功格式化文件系统\r\n");
            res_flash = f_mount(NULL, "1:", 1);
            rt_kprintf("res_flash = %d\r\n", res_flash);
            res_flash = f_mount(&fs, "1:", 1);
            rt_kprintf("res_flash = %d\r\n", res_flash);
        }
        else{
            rt_kprintf("！！外部 Flash 挂载文件系统失败。");
        }
    }
    else if(res_flash != FR_OK){
        rt_kprintf("！！外部 Flash 挂载文件系统失败。 (%d)\r\n", res_flash);
        rt_kprintf("!!!可能原因：SPI FLASH 初始化不成功");
    }
    else{
        rt_kprintf("文件系统挂载成功，可以进行读写测试。\r\n");
    }
    rt_kprintf("\r\n****** 即将进行文件写入测试... ******\r\n\r\n");
     res_flash = f_mount(NULL, "1:", 1);
   if(res_flash != FR_OK){
       rt_kprintf("卸载系统文件失败！\r\n");
    }
    else{
        rt_kprintf("卸载系统文件成功！\r\n");
    }
     res_flash = f_open(&fnew, "1:FatFs as.txt", FA_CREATE_ALWAYS | FA_WRITE );
    if(res_flash == FR_OK) {
        rt_kprintf("》打开/创建 FatFs 读写测试文件.txt 文件成功，向文件写入数据。 \r\n");
        res_flash = f_write(&fnew, textFileBuffer, sizeof(textFileBuffer), &fnum);
        if(res_flash == FR_OK) {
            rt_kprintf("》文件写入成功，写入字节数据： %d\n", fnum);
            rt_kprintf("》向文件写入的数据为： \r\n%s\r\n", textFileBuffer);
        }
        else{
            rt_kprintf("！！文件写入失败： (%d)\n",res_flash);
        }
        f_close(&fnew);
    }
    else{
        rt_kprintf("！！打开/创建文件失败。 \r\n");
    }
    while(1)
    {
        rt_thread_mdelay(1000);
        //rt_kprintf("你好\r\n");
    }*/
    return RT_EOK;
}





