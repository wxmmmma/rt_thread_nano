/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-08     RT-Thread    first version
 */
#include "stm32f1xx_hal.h"
#include <rtthread.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "uart_config.h"

int main(void)
{
    while(1)
    {
        rt_thread_mdelay(1000);
        rt_kprintf("ÄãºÃ");
    }
    return RT_EOK;
}


