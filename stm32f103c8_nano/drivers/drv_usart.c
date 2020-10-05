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
#include "uart_config.h"
#include <string.h>
#include <stdio.h>

#define DBG_TAG              "drv.usart"

#ifdef DRV_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO
#endif

#include <rtdbg.h>

#ifdef RT_USING_CONSOLE




//============================================================
#ifdef BSP_USING_UART1
struct rt_hw_uart   my_uart1;
#endif

#ifdef BSP_USING_UART2
struct rt_hw_uart   my_uart2;
#endif

#ifdef BSP_USING_UART3
struct rt_hw_uart   my_uart3;
#endif

struct rt_semaphore rx_sem;                    //串口返回数据信号量，记录返回命令个数
UsartCallBack       CallBack1, CallBack2, CallBack3;
usart_list          *uart_list_head;            //串口接收返回返回数据的双向链表的头节点


//==============================================================================
//创建头链表
rt_inline usart_list *HN_List_Create(void)
{
    usart_list *node = (usart_list*)rt_malloc(sizeof(usart_list));
    node->next = node->prev = node;
    return node;
}
//向双链表表头后面插入一个节点
//l为表头 n为插入节点
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
//向双链表表头前面插入一个节点
//l为表头 n为插入节点
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

//删除节点
rt_inline void list_delete_note(usart_list *n)
{

    usart_list *p = n;
    p->next->prev = p->prev;
    p->prev->next = p->next;
    p->next = p->prev = n;
    rt_free(p->data);
    rt_free(p);
}


//=====================================================================



void my_hw_usart_init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();
#ifdef BSP_USING_UART1
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
//    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 1, 0);
//    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
#endif
#ifdef BSP_USING_UART2
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
//    HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 4, 0);
//    HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
#endif
#ifdef BSP_USING_UART3
    __HAL_RCC_USART3_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
//    HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 1, 0);
//    HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
    HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
#endif
}

void MY_UART_GPIIO_DMA_INIT(struct rt_hw_uart *my_uart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin   = my_uart->conset.tx_pin_name;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(my_uart->conset.GPIOx, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = my_uart->conset.rx_pin_name;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(my_uart->conset.GPIOx, &GPIO_InitStruct);
/*
    my_uart->hdma_usart_tx.Instance                 = my_uart->conset.DMA_TX_Channel;
    my_uart->hdma_usart_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    my_uart->hdma_usart_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    my_uart->hdma_usart_tx.Init.MemInc              = DMA_MINC_ENABLE;
    my_uart->hdma_usart_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    my_uart->hdma_usart_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    my_uart->hdma_usart_tx.Init.Mode                = DMA_NORMAL;
    my_uart->hdma_usart_tx.Init.Priority            = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&my_uart->hdma_usart_tx) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_LINKDMA(&my_uart->handle, hdmatx, my_uart->hdma_usart_tx);
*/
    my_uart->hdma_usart_rx.Instance                 = my_uart->conset.DMA_RX_Channel;
    my_uart->hdma_usart_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    my_uart->hdma_usart_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    my_uart->hdma_usart_rx.Init.MemInc              = DMA_MINC_ENABLE;
    my_uart->hdma_usart_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    my_uart->hdma_usart_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    my_uart->hdma_usart_rx.Init.Mode                = DMA_NORMAL;
    my_uart->hdma_usart_rx.Init.Priority            = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&my_uart->hdma_usart_rx) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_LINKDMA(&my_uart->handle, hdmarx, my_uart->hdma_usart_rx);
    HAL_NVIC_SetPriority(my_uart->conset.IRQn_Type, my_uart->conset.PreemptPriority, 0);
    HAL_NVIC_EnableIRQ(my_uart->conset.IRQn_Type);
    __HAL_UART_ENABLE_IT(&my_uart->handle, UART_IT_IDLE);

}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{

    if(huart->Instance == USART1){
    #ifdef BSP_USING_UART1
        MY_UART_GPIIO_DMA_INIT(&my_uart1);
    #endif
    }
    else if(huart->Instance == USART2){
    #ifdef BSP_USING_UART2
        MY_UART_GPIIO_DMA_INIT(&my_uart2);
    #endif
    }
    else if(huart->Instance == USART3){
    #ifdef BSP_USING_UART3
        MY_UART_GPIIO_DMA_INIT(&my_uart3);
    #endif
    }

}

__weak void my_uart_ops(struct rt_hw_uart *my_uart)
{
    my_uart->handle.Instance          = my_uart->conset.Instance;
    my_uart->handle.Init.BaudRate     = 115200;
    my_uart->handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    my_uart->handle.Init.Mode         = UART_MODE_TX_RX;
    my_uart->handle.Init.OverSampling = UART_OVERSAMPLING_16;
    my_uart->handle.Init.WordLength   = UART_WORDLENGTH_8B;
    my_uart->handle.Init.StopBits     = UART_STOPBITS_1;
    my_uart->handle.Init.Parity       = UART_PARITY_NONE;
}

static rt_err_t stm32_configure(struct rt_hw_uart *my_uart)
{

    my_uart_ops(my_uart);
    if (HAL_UART_Init(&my_uart->handle) != HAL_OK)
    {
        return -RT_ERROR;
    }
    return RT_EOK;
}


int rt_hw_usart_init(void)
{
    uart_list_head = HN_List_Create();
    my_hw_usart_init();
    #ifdef BSP_USING_UART1
    my_uart1.conset = (struct rt_hw_uart_conset)UART1_CONFIG;
    stm32_configure(&my_uart1);
    my_uart1.rx_buffer = (uint8_t*)rt_malloc(my_uart1.conset.dma_data_len *sizeof(char));
    HAL_UART_Receive_DMA(&my_uart1.handle, my_uart1.rx_buffer, my_uart1.conset.dma_data_len);
    #endif
    #ifdef BSP_USING_UART2
    my_uart2.conset = (struct rt_hw_uart_conset)UART2_CONFIG;
    stm32_configure(&my_uart2);
    my_uart2.rx_buffer = (uint8_t*)rt_malloc(my_uart2.conset.dma_data_len *sizeof(char));
    HAL_UART_Receive_DMA(&my_uart2.handle, my_uart2.rx_buffer, my_uart2.conset.dma_data_len);
    #endif
    #ifdef BSP_USING_UART3
    my_uart3.conset = (struct rt_hw_uart_conset)UART3_CONFIG;
    stm32_configure(&my_uart3);
    my_uart3.rx_buffer = (uint8_t*)rt_malloc(my_uart3.conset.dma_data_len *sizeof(char));
    HAL_UART_Receive_DMA(&my_uart3.handle, my_uart3.rx_buffer, my_uart3.conset.dma_data_len);
    #endif
    return RT_EOK;
}
INIT_BOARD_EXPORT(rt_hw_usart_init);

void rt_hw_uart_register(UART_HandleTypeDef handle, char *name)
{

}

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


/*
void u1_printf(const char* fmt,...)
{
    char USART3_TX_BUF[128];
    rt_size_t i;
    va_list ap;
    va_start(ap, fmt);
    my_vsprintf((char*)USART3_TX_BUF, fmt, ap);
    i = strlen((const char*)USART3_TX_BUF);
    HAL_UART_Transmit(&my_uart1.handle, (uint8_t *)USART3_TX_BUF, i, 0xff);
    va_end(ap);
}
*/

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

#endif /* RT_USING_CONSLONE */

