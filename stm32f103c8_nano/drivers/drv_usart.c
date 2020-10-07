/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-09     xiangxistu   first version
 * 2020-05-18     chenyaxing   modify usart3 remap check
 */

#include "stdlib.h"
#include "drv_common.h"
#include "drv_usart.h"
#include <string.h>
#include <stdio.h>

#define DBG_TAG              "drv.usart"

#ifdef DRV_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO
#endif

#include <rtdbg.h>


//==============================================================================
/*
rt_inline usart_list *HN_List_Create(void)
{
    usart_list *node = (usart_list*)rt_malloc(sizeof(usart_list));
    node->next = node->prev = node;
    return node;
}
//
//
rt_inline usart_list *list_inster_after(usart_list *l, uint16_t data_len, uint8_t *buf)
{
    usart_list *n = (usart_list*)rt_malloc(sizeof(usart_list));;
    n->data = (uint8_t*)rt_malloc(data_len *sizeof(char));
    memcpy(n->data, buf, data_len);
    l->next->prev = n;
    n->next = l->next;
    n->prev = l;
    l->next = n;
    return n;
}
//
//
rt_inline usart_list *list_inster_before(usart_list *l, uint16_t data_len, uint8_t *buf, USART_TypeDef *Instance)
{
    usart_list *n;
    n = (usart_list*)rt_malloc(sizeof(usart_list));
    l->prev->next = n;
    n->prev = l->prev;
    n->next = l;
    l->prev = n;
    n->data = (uint8_t*)rt_malloc(data_len *sizeof(char));
    memcpy(n->data, buf, data_len);
    n->data_len = data_len;
    n->Instance = Instance;
    return n;
}

//
rt_inline void list_delete_note(usart_list *n)
{

    usart_list *p = n;
    p->next->prev = p->prev;
    p->prev->next = p->next;
    p->next = p->prev = n;
    rt_free(p->data);
    rt_free(p);
}
*/

//=====================================================================




//UART_HandleTypeDef          handle;
struct rt_hw_uart my_uart1 = {"uart1", 1, 115200};
//DMA_HandleTypeDef hdma_usart_rx;
void USART_INIT(struct rt_hw_uart *rt_hw_uart)
{

    //__HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();
/*
    hdma_usart_rx.Instance                 = DMA1_Channel5;
    hdma_usart_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_usart_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_usart_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_usart_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_usart_rx.Init.Mode                = DMA_NORMAL;
    hdma_usart_rx.Init.Priority            = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart_rx) != HAL_OK)
    {
        Error_Handler();
    }
    __HAL_LINKDMA(&rt_hw_uart.handle, hdmarx, hdma_usart_rx);
*/
    rt_hw_uart->handle.Instance          = USART1;
    rt_hw_uart->handle.Init.BaudRate     = 115200;
    rt_hw_uart->handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    rt_hw_uart->handle.Init.Mode         = UART_MODE_TX_RX;
    rt_hw_uart->handle.Init.OverSampling = UART_OVERSAMPLING_16;
    rt_hw_uart->handle.Init.WordLength   = UART_WORDLENGTH_8B;
    rt_hw_uart->handle.Init.StopBits     = UART_STOPBITS_1;
    rt_hw_uart->handle.Init.Parity       = UART_PARITY_NONE;

    if (HAL_UART_Init(&rt_hw_uart->handle) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    //HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 2, 0);
    //HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
    __HAL_UART_ENABLE_IT(&rt_hw_uart->handle, UART_IT_IDLE);

}


int rt_hw_usart_init(void)
{
    __USART_GPIO_INIT(A, 9, A, 10);
    USART_INIT(&my_uart1);
    return 0;
}
INIT_BOARD_EXPORT(rt_hw_usart_init);

void UART1_SEND_DATA(uint8_t *Data, uint16_t Len, uint32_t Timeout)
{
    HAL_UART_Transmit(&my_uart1.handle, Data, Len, Timeout);
}

void rt_hw_uart_register(UART_HandleTypeDef handle, char *name)
{

}
/*
#ifdef BSP_USING_UART1
void USART1_IRQHandler(void)
{

    if(__HAL_UART_GET_FLAG(&my_uart1.handle, UART_FLAG_IDLE) != RESET){
        //rt_interrupt_enter();
        __HAL_UART_CLEAR_IDLEFLAG(&my_uart1.handle);
        HAL_UART_DMAStop(&my_uart1.handle);
        uint16_t data_length  = my_uart1.conset.dma_data_len - __HAL_DMA_GET_COUNTER(&my_uart1.hdma_usart_rx);
        usart_list *n = list_inster_before(uart_list_head, data_length, my_uart1.rx_buffer, my_uart1.conset.Instance);
        rt_sem_release(&rx_sem);
        HAL_UART_Receive_DMA(&my_uart1.handle, my_uart1.rx_buffer, my_uart1.conset.dma_data_len);
        //rt_interrupt_leave();
    }

}

void UART1_SEND_DATA(uint8_t *Data, uint16_t Len, uint32_t Timeout)
{
    HAL_UART_Transmit(&my_uart1.handle, Data, Len, Timeout);
}




void rt_hw_console_output(const char *str)
{
    rt_size_t i = 0, size = 0;
    char a = '\r';

    size = rt_strlen(str);
    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            HAL_UART_Transmit(&my_uart1.handle, (uint8_t *)&a, 1, 1);
        }
        HAL_UART_Transmit(&my_uart1.handle, (uint8_t *)(str + i), 1, 1);
    }
}

void Uasrt1CallBackReg(UsartCallBack Back)
{
    CallBack1 = Back;
}

#endif

#ifdef BSP_USING_UART2

void USART2_IRQHandler(void)
{
    if(__HAL_UART_GET_FLAG(&my_uart2.handle, UART_FLAG_IDLE) != RESET){
        //rt_interrupt_enter();
        __HAL_UART_CLEAR_IDLEFLAG(&my_uart2.handle);
        HAL_UART_DMAStop(&my_uart2.handle);
        uint16_t data_length  = my_uart2.conset.dma_data_len - __HAL_DMA_GET_COUNTER(&my_uart2.hdma_usart_rx);
        usart_list *n = list_inster_before(uart_list_head, data_length, my_uart2.rx_buffer, my_uart2.conset.Instance);
        rt_sem_release(&rx_sem);
        HAL_UART_Receive_DMA(&my_uart2.handle, my_uart2.rx_buffer, my_uart2.conset.dma_data_len);
        //rt_interrupt_leave();
    }

}

void UART2_SEND_DATA(uint8_t *Data, uint16_t Len, uint32_t Timeout)
{
    HAL_UART_Transmit(&my_uart2.handle, Data, Len, Timeout);
}

void Uasrt2CallBackReg(UsartCallBack Back)
{
    CallBack2 = Back;
}

#endif

#ifdef BSP_USING_UART3
void USART3_IRQHandler(void)
{
    if(__HAL_UART_GET_FLAG(&my_uart3.handle, UART_FLAG_IDLE) != RESET){
       // rt_interrupt_enter();
        __HAL_UART_CLEAR_IDLEFLAG(&my_uart3.handle);
        HAL_UART_DMAStop(&my_uart3.handle);
        uint16_t data_length  = my_uart3.conset.dma_data_len - __HAL_DMA_GET_COUNTER(&my_uart3.hdma_usart_rx);
        usart_list *n = list_inster_before(uart_list_head, data_length, my_uart3.rx_buffer, my_uart3.conset.Instance);
        rt_sem_release(&rx_sem);
        HAL_UART_Receive_DMA(&my_uart3.handle, my_uart3.rx_buffer, my_uart3.conset.dma_data_len);
       // rt_interrupt_leave();
    }
}

void UART3_SEND_DATA(uint8_t *Data, uint16_t Len, uint32_t Timeout)
{
    HAL_UART_Transmit(&my_uart3.handle, Data, Len, Timeout);
}

void Uasrt3CallBackReg(UsartCallBack Back)
{
    CallBack3 = Back;
}

#endif

static void serial_thread_entry(void *parameter)
{
    while(1)
    {
        rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        if(uart_list_head->next->Instance == USART1)
            CallBack1(uart_list_head->next->data, uart_list_head->next->data_len);
        else if(uart_list_head->next->Instance == USART2)
            CallBack2(uart_list_head->next->data, uart_list_head->next->data_len);
        else if(uart_list_head->next->Instance == USART3)
            CallBack3(uart_list_head->next->data, uart_list_head->next->data_len);
        list_delete_note(uart_list_head->next);

    }
}


static int uart3_init(void)
{

    rt_sem_init(&rx_sem, "uart_rx_sem", 0, RT_IPC_FLAG_FIFO);
    rt_thread_t usart_thread = rt_thread_create("serial", serial_thread_entry, RT_NULL, 1024, 24, 20);
    if (usart_thread != RT_NULL)
    {
        rt_thread_startup(usart_thread);
    }
    return 0;
}



INIT_DEVICE_EXPORT(uart3_init);
*/


