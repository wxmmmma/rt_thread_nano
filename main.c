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


FATFS fs;                        /* FatFs �ļ�ϵͳ���� */
FIL fnew;                        /* �ļ����� */
FRESULT res_flash;               /* �ļ�������� */
UINT fnum;                       /* �ļ��ɹ���д���� */
BYTE buffer[1024]= {0};          /* �������� */
BYTE textFileBuffer[] = "��ӭʹ��Ұ�� STM32 ������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";

int main(void)
{
    //fs = rt_malloc(sizeof(FATFS));
   /* res_flash = f_mount(&fs, "1:", 1);
    rt_kprintf("res_flash���%d\r\n", res_flash);
     if (res_flash == FR_NO_FILESYSTEM){
        rt_kprintf("�������%d\r\n", res_flash);
        res_flash = f_mkfs("1:", 0, 0);
        if (res_flash == FR_OK){
            rt_kprintf("�ɹ���ʽ���ļ�ϵͳ\r\n");
            res_flash = f_mount(NULL, "1:", 1);
            rt_kprintf("res_flash = %d\r\n", res_flash);
            res_flash = f_mount(&fs, "1:", 1);
            rt_kprintf("res_flash = %d\r\n", res_flash);
        }
        else{
            rt_kprintf("�����ⲿ Flash �����ļ�ϵͳʧ�ܡ�");
        }
    }
    else if(res_flash != FR_OK){
        rt_kprintf("�����ⲿ Flash �����ļ�ϵͳʧ�ܡ� (%d)\r\n", res_flash);
        rt_kprintf("!!!����ԭ��SPI FLASH ��ʼ�����ɹ�");
    }
    else{
        rt_kprintf("�ļ�ϵͳ���سɹ������Խ��ж�д���ԡ�\r\n");
    }
    rt_kprintf("\r\n****** ���������ļ�д�����... ******\r\n\r\n");
     res_flash = f_mount(NULL, "1:", 1);
   if(res_flash != FR_OK){
       rt_kprintf("ж��ϵͳ�ļ�ʧ�ܣ�\r\n");
    }
    else{
        rt_kprintf("ж��ϵͳ�ļ��ɹ���\r\n");
    }
     res_flash = f_open(&fnew, "1:FatFs as.txt", FA_CREATE_ALWAYS | FA_WRITE );
    if(res_flash == FR_OK) {
        rt_kprintf("����/���� FatFs ��д�����ļ�.txt �ļ��ɹ������ļ�д�����ݡ� \r\n");
        res_flash = f_write(&fnew, textFileBuffer, sizeof(textFileBuffer), &fnum);
        if(res_flash == FR_OK) {
            rt_kprintf("���ļ�д��ɹ���д���ֽ����ݣ� %d\n", fnum);
            rt_kprintf("�����ļ�д�������Ϊ�� \r\n%s\r\n", textFileBuffer);
        }
        else{
            rt_kprintf("�����ļ�д��ʧ�ܣ� (%d)\n",res_flash);
        }
        f_close(&fnew);
    }
    else{
        rt_kprintf("������/�����ļ�ʧ�ܡ� \r\n");
    }
    while(1)
    {
        rt_thread_mdelay(1000);
        //rt_kprintf("���\r\n");
    }*/
    return RT_EOK;
}





