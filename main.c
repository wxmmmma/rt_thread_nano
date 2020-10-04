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


FATFS fs;                        /* FatFs ÎÄ¼şÏµÍ³¶ÔÏó */
FIL fnew;                        /* ÎÄ¼ş¶ÔÏó */
FRESULT res_flash;               /* ÎÄ¼ş²Ù×÷½á¹û */
UINT fnum;                       /* ÎÄ¼ş³É¹¦¶ÁĞ´ÊıÁ¿ */
BYTE buffer[1024]= {0};          /* ¶Á»º³åÇø */
BYTE textFileBuffer[] = "»¶Ó­Ê¹ÓÃÒ°»ğ STM32 ¿ª·¢°å ½ñÌìÊÇ¸öºÃÈÕ×Ó£¬ĞÂ½¨ÎÄ¼şÏµÍ³²âÊÔÎÄ¼ş\r\n";

int main(void)
{
    //fs = rt_malloc(sizeof(FATFS));
   /* res_flash = f_mount(&fs, "1:", 1);
    rt_kprintf("res_flash¿¿¿%d\r\n", res_flash);
     if (res_flash == FR_NO_FILESYSTEM){
        rt_kprintf("¿¿¿¿¿¿¿%d\r\n", res_flash);
        res_flash = f_mkfs("1:", 0, 0);
        if (res_flash == FR_OK){
            rt_kprintf("³É¹¦¸ñÊ½»¯ÎÄ¼şÏµÍ³\r\n");
            res_flash = f_mount(NULL, "1:", 1);
            rt_kprintf("res_flash = %d\r\n", res_flash);
            res_flash = f_mount(&fs, "1:", 1);
            rt_kprintf("res_flash = %d\r\n", res_flash);
        }
        else{
            rt_kprintf("£¡£¡Íâ²¿ Flash ¹ÒÔØÎÄ¼şÏµÍ³Ê§°Ü¡£");
        }
    }
    else if(res_flash != FR_OK){
        rt_kprintf("£¡£¡Íâ²¿ Flash ¹ÒÔØÎÄ¼şÏµÍ³Ê§°Ü¡£ (%d)\r\n", res_flash);
        rt_kprintf("!!!¿ÉÄÜÔ­Òò£ºSPI FLASH ³õÊ¼»¯²»³É¹¦");
    }
    else{
        rt_kprintf("ÎÄ¼şÏµÍ³¹ÒÔØ³É¹¦£¬¿ÉÒÔ½øĞĞ¶ÁĞ´²âÊÔ¡£\r\n");
    }
    rt_kprintf("\r\n****** ¼´½«½øĞĞÎÄ¼şĞ´Èë²âÊÔ... ******\r\n\r\n");
     res_flash = f_mount(NULL, "1:", 1);
   if(res_flash != FR_OK){
       rt_kprintf("Ğ¶ÔØÏµÍ³ÎÄ¼şÊ§°Ü£¡\r\n");
    }
    else{
        rt_kprintf("Ğ¶ÔØÏµÍ³ÎÄ¼ş³É¹¦£¡\r\n");
    }
     res_flash = f_open(&fnew, "1:FatFs as.txt", FA_CREATE_ALWAYS | FA_WRITE );
    if(res_flash == FR_OK) {
        rt_kprintf("¡·´ò¿ª/´´½¨ FatFs ¶ÁĞ´²âÊÔÎÄ¼ş.txt ÎÄ¼ş³É¹¦£¬ÏòÎÄ¼şĞ´ÈëÊı¾İ¡£ \r\n");
        res_flash = f_write(&fnew, textFileBuffer, sizeof(textFileBuffer), &fnum);
        if(res_flash == FR_OK) {
            rt_kprintf("¡·ÎÄ¼şĞ´Èë³É¹¦£¬Ğ´Èë×Ö½ÚÊı¾İ£º %d\n", fnum);
            rt_kprintf("¡·ÏòÎÄ¼şĞ´ÈëµÄÊı¾İÎª£º \r\n%s\r\n", textFileBuffer);
        }
        else{
            rt_kprintf("£¡£¡ÎÄ¼şĞ´ÈëÊ§°Ü£º (%d)\n",res_flash);
        }
        f_close(&fnew);
    }
    else{
        rt_kprintf("£¡£¡´ò¿ª/´´½¨ÎÄ¼şÊ§°Ü¡£ \r\n");
    }
    while(1)
    {
        rt_thread_mdelay(1000);
        //rt_kprintf("ÄãºÃ\r\n");
    }*/
    return RT_EOK;
}





