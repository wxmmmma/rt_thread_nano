/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-20     LEGION       the first version
 */
#include "drv_gpio.h"
#include <board.h>
#include <drv_common.h>
#include "i2c_config.h"



void i2c_gpio_init(GPIO_TypeDef *GPIO_SCL, uint32_t GPIO_PIN_SCL, GPIO_TypeDef *GPIO_SDA, uint32_t GPIO_PIN_SDA)
{
    My_Dev_Enable(GPIO_SCL);
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin   = GPIO_PIN_SCL;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(GPIO_SCL, &GPIO_InitStruct);

    My_Dev_Enable(GPIO_SDA);
    GPIO_InitStruct.Pin   = GPIO_PIN_SDA;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(GPIO_SDA, &GPIO_InitStruct);
}



void I2C_SCL_H(struct rt_hw_i2c *I2Cx)
{
    HAL_GPIO_WritePin(I2Cx->scl_gpio, I2Cx->scl_pin_name, GPIO_PIN_SET);
}

void I2C_SCL_L(struct rt_hw_i2c *I2Cx)
{
    HAL_GPIO_WritePin(I2Cx->scl_gpio, I2Cx->scl_pin_name, GPIO_PIN_RESET);
}

void I2C_SDA_H(struct rt_hw_i2c *I2Cx)
{
    HAL_GPIO_WritePin(I2Cx->sda_gpio, I2Cx->sda_pin_name, GPIO_PIN_SET);
}

void I2C_SDA_L(struct rt_hw_i2c *I2Cx)
{
    HAL_GPIO_WritePin(I2Cx->sda_gpio, I2Cx->sda_pin_name, GPIO_PIN_RESET);
}

GPIO_PinState I2C_SDA_R(struct rt_hw_i2c *I2Cx)
{
    return HAL_GPIO_ReadPin(I2Cx->sda_gpio, I2Cx->sda_pin_name);
}




static void i2c_Delay(void)
{
    //uint8_t i;
    //for (i = 0; i < 10; i++);
    rt_hw_us_delay(2);
}

void IIC_Start(struct rt_hw_i2c *i2cx)
{
    I2C_SCL_H(i2cx);
    I2C_SDA_H(i2cx);
    i2c_Delay();
    I2C_SDA_L(i2cx);
    i2c_Delay();
    I2C_SCL_L(i2cx);
    i2c_Delay();
}

void IIC_Stop(struct rt_hw_i2c *i2cx)
{
    I2C_SDA_L(i2cx);
    I2C_SCL_H(i2cx);
    i2c_Delay();
    I2C_SDA_H(i2cx);
}

uint8_t IIC_Wait_Ack(struct rt_hw_i2c *i2cx)
{
    uint8_t re;
    I2C_SDA_H(i2cx);
    i2c_Delay();
    I2C_SCL_H(i2cx);
    i2c_Delay();
    if(I2C_SDA_R(i2cx))re = 1;
    else re = 0;
    I2C_SCL_L(i2cx);
    return re;
}

void IIC_Ack(struct rt_hw_i2c *i2cx)
{
    I2C_SDA_L(i2cx);
    i2c_Delay();
    I2C_SCL_H(i2cx);
    i2c_Delay();
    I2C_SCL_L(i2cx);
    i2c_Delay();
    I2C_SDA_H(i2cx);
}

void IIC_NAck(struct rt_hw_i2c *i2cx)
{
    I2C_SDA_H(i2cx);
    i2c_Delay();
    I2C_SCL_H(i2cx);
    i2c_Delay();
    I2C_SCL_L(i2cx);
}

void IIC_Send_Byte(struct rt_hw_i2c *i2cx,  uint8_t txd)
{
    for(int i=0; i < 8; i++)
    {
        if(txd & 0x80){
            I2C_SDA_H(i2cx);
        }
        else{
            I2C_SDA_L(i2cx);
        }
        i2c_Delay();
        I2C_SCL_H(i2cx);
        i2c_Delay();
        I2C_SCL_L(i2cx);
        txd<<=1;
        i2c_Delay();
    }
}



uint8_t IIC_Read_Byte(struct rt_hw_i2c *i2cx)
{
    uint8_t i,receive=0;
    for(i = 0; i < 8; i++ )
    {
        receive <<= 1;
        I2C_SCL_H(i2cx);
        i2c_Delay();
        if(I2C_SDA_R(i2cx))receive++;
        I2C_SCL_L(i2cx);
        i2c_Delay();
    }
    return receive;
}

uint8_t i2c_CheckDEV(struct rt_hw_i2c *i2cx)
{
    uint8_t ack;
    IIC_Start(i2cx);
    IIC_Send_Byte(i2cx, i2cx->slave_addr);
    ack = IIC_Wait_Ack(i2cx);
    IIC_Stop(i2cx);
    return ack;
}

uint8_t IIC_Send_Data(struct rt_hw_i2c *i2cx, uint8_t reg, uint8_t *data, int len)
{
    uint8_t res = 0;
    do {
        rt_interrupt_enter();
        IIC_Start(i2cx);
        IIC_Send_Byte(i2cx, (i2cx->slave_addr << 1) | 0);
        if(IIC_Wait_Ack(i2cx)){
            res = 1;
            break;
        }
        IIC_Send_Byte(i2cx, reg);
        if(IIC_Wait_Ack(i2cx)){
            res = 1;
            break;
        }
        for(int i = 0; i < len; i++)
        {
            IIC_Send_Byte(i2cx, data[i]);
            IIC_Wait_Ack(i2cx);
        }
    } while (0);
    IIC_Stop(i2cx);
    rt_interrupt_leave();
    rt_thread_mdelay(10);
    return res;
}


uint8_t IIC_Read_Data(struct rt_hw_i2c *i2cx, uint8_t reg, uint8_t *data, int len)
{
    uint8_t res = 0;
    do {
        rt_interrupt_enter();
        IIC_Start(i2cx);
        IIC_Send_Byte(i2cx, (i2cx->slave_addr << 1) | 0);
        if(IIC_Wait_Ack(i2cx)){
            res = 1;
            break;
        }
        IIC_Send_Byte(i2cx, reg);
        if(IIC_Wait_Ack(i2cx)){
            res = 1;
            break;
        }
        IIC_Start(i2cx);
        IIC_Send_Byte(i2cx, (i2cx->slave_addr << 1) | 1);
        IIC_Wait_Ack(i2cx);
        for(int i = 0; i < len; i++)
        {
            data[i] = IIC_Read_Byte(i2cx);
            if(i != len - 1){
                IIC_Ack(i2cx);
            }
            else{
                IIC_NAck(i2cx);
            }
        }
    } while (0);
    IIC_Stop(i2cx);
    rt_interrupt_leave();
    rt_thread_mdelay(10);
    return res;
}




void MX_I2Cx_Init(struct rt_hw_i2c *i2cx)
{
    i2c_gpio_init(i2cx->scl_gpio, i2cx->scl_pin_name, i2cx->sda_gpio, i2cx->sda_pin_name);
    I2C_SCL_H(i2cx);
    I2C_SDA_H(i2cx);
}





