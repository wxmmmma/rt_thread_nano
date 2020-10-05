/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-24     LEGION       the first version
 */
#include "drv_gpio.h"

void My_Dev_Enable(GPIO_TypeDef  *GPIOx)
{
    if(GPIOx == GPIOA){
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    else if(GPIOx == GPIOB){
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    else if(GPIOx == GPIOC){
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    else if(GPIOx == GPIOD){
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
}
