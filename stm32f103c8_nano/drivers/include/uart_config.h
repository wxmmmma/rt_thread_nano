/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-10-30     SummerGift   first version
 * 2019-01-03     zylx         modify dma support
 * 2020-06-03     chenyaxing   modify uart config
 */

#ifndef __UART_CONFIG_H__
#define __UART_CONFIG_H__


#include <rtthread.h>
#include <board.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BSP_USING_UART1
#define BSP_USING_UART2
#define BSP_USING_UART3

#define my_usart_rev_len  512
#define my_usart_sen_len  512


/*
typedef void (*UsartCallBack) (uint8_t *ch, uint16_t  USART_RX_STA); //


#ifdef BSP_USING_UART2

void UART2_SEND_DATA(uint8_t *Data, uint16_t Len, uint32_t Timeout);

void Uasrt2CallBackReg(UsartCallBack Back);

#define UART2_CONFIG                                                \
    {                                                               \
        .name = "uart2",                                            \
        .dma_data_len = my_usart_rev_len,                           \
        .PreemptPriority = 2,                                       \
        .Instance = USART2,                                         \
        .tx_pin_name = GPIO_PIN_2,                                  \
        .rx_pin_name = GPIO_PIN_3,                                  \
        .GPIOx = GPIOA,                                             \
        .DMA_TX_Channel = DMA1_Channel7,                            \
        .DMA_RX_Channel = DMA1_Channel6,                            \
        .IRQn_Type = USART2_IRQn                                    \
    }                                                               \

#endif

#ifdef BSP_USING_UART3

void UART3_SEND_DATA(uint8_t *Data, uint16_t Len, uint32_t Timeout);

void Uasrt3CallBackReg(UsartCallBack Back);

#define UART3_CONFIG                                                \
    {                                                               \
        .name = "uart3",                                            \
        .dma_data_len = my_usart_rev_len,                           \
        .PreemptPriority = 1,                                       \
        .Instance = USART3,                                         \
        .tx_pin_name = GPIO_PIN_10,                                 \
        .rx_pin_name = GPIO_PIN_11,                                 \
        .GPIOx = GPIOB,                                             \
        .DMA_TX_Channel = DMA1_Channel2,                            \
        .DMA_RX_Channel = DMA1_Channel3,                            \
        .IRQn_Type = USART3_IRQn                                    \
    }                                                               \

#endif


*/


#endif
