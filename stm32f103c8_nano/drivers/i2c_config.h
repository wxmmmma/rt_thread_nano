/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-20     LEGION       the first version
 */
#ifndef DRIVERS_I2C_CONFIG_H_
#define DRIVERS_I2C_CONFIG_H_

#include "stm32f1xx_hal.h"

#define BSP_USING_I2C_24L02


struct rt_hw_i2c{
    uint8_t               slave_addr;                    //元件地址
    GPIO_TypeDef          *scl_gpio;                     //SCL GPIOX
    uint32_t              scl_pin_name;                  //SCL PINX
    GPIO_TypeDef          *sda_gpio;                     //SDA GPIOX
    uint32_t              sda_pin_name;                  //SDA PINX

};

struct rt_hw_i2c my_i2c[] = {
#ifdef BSP_USING_I2C_24L02
        {0XD0, GPIOB, GPIO_PIN_4, GPIOB, GPIO_PIN_6},    //24C02
#endif
#ifdef BSP_USING_I2C_MPU6050
        {0X68, GPIOB, GPIO_PIN_4, GPIOB, GPIO_PIN_6},    //MPU6050
#endif
};

/*
 * i2c发送数据函数
 * reg 要写寄存器地址  data要发送的数据  len数据长度
 * 返回值0成功 1失败
 */
uint8_t IIC_Send_Data(struct rt_hw_i2c *i2cx, uint8_t reg, uint8_t *data, int len);

/*
 * i2c读取数据函数
 * reg 要读寄存器地址  data要读的数据  len数据长度
 * 返回值0成功 1失败
 */
uint8_t IIC_Read_Data(struct rt_hw_i2c *i2cx, uint8_t reg, uint8_t *data, int len);


#endif /* DRIVERS_I2C_CONFIG_H_ */
