/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-06     LEGION       the first version
 */
#ifndef DRIVERS_INCLUDE_DRV_USART_H_
#define DRIVERS_INCLUDE_DRV_USART_H_

#define __USART_GPIO_INIT(GPIO_RX, PIN_RX, GPIO_TX, PIN_TX)          \
do{                                                                  \
    GPIO_InitTypeDef GPIO_InitStruct = {0};                          \
    __HAL_RCC_GPIO##GPIO_RX##_CLK_ENABLE();                          \
    __HAL_RCC_GPIO##GPIO_TX##_CLK_ENABLE();                          \
                                                                     \
    GPIO_InitStruct.Pin   = GPIO_PIN_##PIN_RX;                       \
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;                         \
    GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;                   \
    HAL_GPIO_Init(GPIO##GPIO_RX, &GPIO_InitStruct);                  \
                                                                     \
    GPIO_InitStruct.Pin   = GPIO_PIN_##PIN_TX;                       \
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;                         \
    GPIO_InitStruct.Pull = GPIO_NOPULL;                              \
    HAL_GPIO_Init(GPIO##GPIO_TX, &GPIO_InitStruct);                  \
}while(0)                                                            \


#define __USART_INIT(rt_hw_uart, USARTx, DMAx, NUM)                  \
do{                                                                  \
    DMA_HandleTypeDef hdma_usart_rx;                                 \
    __HAL_RCC_##DMAx##_CLK_ENABLE();                                 \
    __HAL_RCC_##USARTx##_CLK_ENABLE();                               \
                                                                     \
    hdma_usart_rx.Instance                 = DMAx##_Channel##NUM;    \
    hdma_usart_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;   \
    hdma_usart_rx.Init.PeriphInc           = DMA_PINC_DISABLE;       \
    hdma_usart_rx.Init.MemInc              = DMA_MINC_ENABLE;        \
    hdma_usart_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;    \
    hdma_usart_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;    \
    hdma_usart_rx.Init.Mode                = DMA_NORMAL;             \
    hdma_usart_rx.Init.Priority            = DMA_PRIORITY_LOW;       \
    if (HAL_DMA_Init(&hdma_usart_rx) != HAL_OK)                      \
    {                                                                \
        Error_Handler();                                             \
    }                                                                \
    __HAL_LINKDMA(&rt_hw_uart.handle, hdmarx, hdma_usart_rx);        \
                                                                     \
    rt_hw_uart.handle.Instance          = USARTx;                    \
    rt_hw_uart.handle.Init.BaudRate     = rt_hw_uart.BaudRate;       \
    rt_hw_uart.handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;       \
    rt_hw_uart.handle.Init.Mode         = UART_MODE_TX_RX;           \
    rt_hw_uart.handle.Init.OverSampling = UART_OVERSAMPLING_16;      \
    rt_hw_uart.handle.Init.WordLength   = UART_WORDLENGTH_8B;        \
    rt_hw_uart.handle.Init.StopBits     = UART_STOPBITS_1;           \
    rt_hw_uart.handle.Init.Parity       = UART_PARITY_NONE;          \
                                                                     \
    if (HAL_UART_Init(&rt_hw_uart.handle) != HAL_OK)                 \
    {                                                                \
        return -RT_ERROR;                                            \
    }                                                                \
                                                                     \
    HAL_NVIC_SetPriority(USARTx##_IRQn, rt_hw_uart.Priority, 0);     \
    HAL_NVIC_EnableIRQ(USARTx##_IRQn);                               \
    HAL_NVIC_SetPriority(DMAx##_Channel##NUM##_IRQn, rt_hw_uart.Priority + 1, 0);\
    HAL_NVIC_EnableIRQ(DMAx##_Channel##NUM##_IRQn);                  \
    __HAL_UART_ENABLE_IT(&rt_hw_uart.handle, UART_IT_IDLE);          \
                                                                     \
}while(0)                                                            \


void UART1_SEND_DATA(uint8_t *Data, uint16_t Len, uint32_t Timeout);


struct rt_hw_uart{
    char                        *name;                      //
    uint32_t                    Priority;
    uint32_t                    BaudRate;
    UART_HandleTypeDef          handle;                     //
};

#endif /* DRIVERS_INCLUDE_DRV_USART_H_ */
