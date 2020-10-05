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



typedef void (*UsartCallBack) (uint8_t *ch, uint16_t  USART_RX_STA); //串口回调函数指针

#ifdef BSP_USING_UART1

void UART1_SEND_DATA(uint8_t *Data, uint16_t Len, uint32_t Timeout);  //串口1发送函数

void u1_printf(const char* fmt,...);

void Uasrt1CallBackReg(UsartCallBack Back);                           //串口1回调函数

#define UART1_CONFIG                                                \
    {                                                               \
        .name = "uart1",                                            \
        .dma_data_len = my_usart_rev_len,                           \
        .PreemptPriority = 0,                                       \
        .Instance = USART1,                                         \
        .tx_pin_name = GPIO_PIN_9,                                  \
        .rx_pin_name = GPIO_PIN_10,                                 \
        .GPIOx = GPIOA,                                             \
        .DMA_TX_Channel = DMA1_Channel4,                            \
        .DMA_RX_Channel = DMA1_Channel5,                            \
        .IRQn_Type = USART1_IRQn                                    \
    }                                                               \

#endif

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

typedef struct Doubly_Linked_List
{
    uint8_t                   *data;                       //串口传送回来的数据
    uint16_t                  data_len;                    //串口接收的数据长度
    USART_TypeDef             *Instance;                   //串口号 USART1 USART2 USART3
    struct Doubly_Linked_List *next;                       //指针指向下一个节点
    struct Doubly_Linked_List *prev;                       //指针指向上一个节点
}usart_list;


//串口常量
struct rt_hw_uart_conset{
    char                        *name;                      //串口标志名  uart uart2 uart3
    uint16_t                    dma_data_len;               //DMA储存数据长度 串口返回数据容量
    uint32_t                    PreemptPriority;            //串口中断抢占优先级
    USART_TypeDef               *Instance;                  //串口号 USART1 USART2 USART3
    uint32_t                    tx_pin_name;                //引脚名称  GPIO_PIN_9
    uint32_t                    rx_pin_name;                //
    GPIO_TypeDef                *GPIOx;                     //
    DMA_Channel_TypeDef         *DMA_TX_Channel;            //DMA通道
    DMA_Channel_TypeDef         *DMA_RX_Channel;            //
    IRQn_Type                   IRQn_Type;                  //中断类型
};

//串口变量
struct rt_hw_uart{
    uint8_t                     *rx_buffer;                 //数据接收buf
    uint8_t                     *tx_buffer;                 //数据发送
    struct rt_hw_uart_conset    conset;                     //串口常量
    UART_HandleTypeDef          handle;                     //
    DMA_HandleTypeDef           hdma_usart_tx;              //
    DMA_HandleTypeDef           hdma_usart_rx;              //


};


#endif
