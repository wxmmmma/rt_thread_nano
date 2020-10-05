/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-08     LEGION       the first version
 */
#include <rtthread.h>
#include "uart3.h"

#include "uart_config.h"



void esp8266_reve1(uint8_t *ch, uint16_t  USART3_RX_STA)
{
    UART1_SEND_DATA(ch, USART3_RX_STA, 100);
}

void esp8266_reve2(uint8_t *ch, uint16_t  USART3_RX_STA)
{
    UART2_SEND_DATA(ch, USART3_RX_STA, 100);
}

void esp8266_reve3(uint8_t *ch, uint16_t  USART3_RX_STA)
{
    UART3_SEND_DATA(ch, USART3_RX_STA, 100);
}


static int my_uart1_init(void)
{
    Uasrt1CallBackReg(esp8266_reve1);
    return 0;
}


static int my_uart2_init(void)
{
    Uasrt2CallBackReg(esp8266_reve2);
    return 0;
}

static int my_uart3_init(void)
{
    Uasrt3CallBackReg(esp8266_reve3);
    return 0;
}

INIT_DEVICE_EXPORT(my_uart1_init);

INIT_DEVICE_EXPORT(my_uart2_init);

INIT_DEVICE_EXPORT(my_uart3_init);

