/*** 
 * @Author: mbw
 * @Date: 2024-06-27 10:55:21
 * @LastEditors: mbw && 1600520629@qq.com
 * @LastEditTime: 2024-06-27 10:55:31
 * @FilePath: \MY_RTT_CH32V303_DEMO\drivers\drv_usart.h
 * @Description: 
 * @
 * @Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-09     WCH        the first version
 */
#ifndef __DRV_USART_H__
#define __DRV_USART_H__
#include <rtthread.h>
#include "rtdevice.h"
#include <rthw.h>
#include "board.h"
#include "drv_gpio.h"
#include "lwrb.h"

/* ch32 config class */
struct ch32_uart_config
{
    const char *name;
    USART_TypeDef *Instance;
    IRQn_Type irq_type;
};


/* ch32 uart dirver class */
struct ch32_uart
{
    struct ch32_uart_config *config;
    USART_InitTypeDef Init;		
    struct rt_serial_device serial;
//    struct _DMA_HandleTypeDef *dma_rx, *dma_tx;
//    rt_size_t last_index;
};


/***********************UART1**************************** */
#define UART1_TX_GPIO_PIN              GPIO_Pin_9
#define UART1_TX_GPIO_PORT             GPIOA

#define UART1_RX_GPIO_PIN              GPIO_Pin_10
#define UART1_RX_GPIO_PORT             GPIOA

#define UART1_RX_BUFFER_LENGTH         (128U)
#define UART1_RX_RB_LENGTH             (256U)
#define UART1_TX_RB_LENGTH             (512U)

#define UART1_DMA_RX_CHANNEL           DMA1_Channel5
#define UART1_DMA_TX_CHANNEL           DMA1_Channel4

#define UART1_DMA_RX_IRQ_CHANNEL       DMA1_Channel5_IRQn
#define UART1_DMA_TX_IRQ_CHANNEL       DMA1_Channel4_IRQn

/***********************UART2 **************************** */
#define UART2_TX_GPIO_PIN              GPIO_Pin_2
#define UART2_TX_GPIO_PORT             GPIOA

#define UART2_RX_GPIO_PIN              GPIO_Pin_3
#define UART2_RX_GPIO_PORT             GPIOA

#define UART2_RX_BUFFER_LENGTH         (128U)
#define UART2_RX_RB_LENGTH             (256U)
#define UART2_TX_RB_LENGTH             (256U)

#define UART2_DMA_RX_CHANNEL           DMA1_Channel6
#define UART2_DMA_TX_CHANNEL           DMA1_Channel7

#define UART2_DMA_RX_IRQ_CHANNEL       DMA1_Channel6_IRQn
#define UART2_DMA_TX_IRQ_CHANNEL       DMA1_Channel7_IRQn

/***********************UART3 4G**************************** */
#define UART3_TX_GPIO_PIN              GPIO_Pin_10
#define UART3_TX_GPIO_PORT             GPIOB

#define UART3_RX_GPIO_PIN              GPIO_Pin_11
#define UART3_RX_GPIO_PORT             GPIOB

#define UART3_RX_BUFFER_LENGTH         (256U)
#define UART3_RX_RB_LENGTH             (512U)
#define UART3_TX_RB_LENGTH             (512U)

#define UART3_DMA_RX_CHANNEL           DMA1_Channel3
#define UART3_DMA_TX_CHANNEL           DMA1_Channel2

#define UART3_DMA_RX_IRQ_CHANNEL       DMA1_Channel3_IRQn
#define UART3_DMA_TX_IRQ_CHANNEL       DMA1_Channel2_IRQn

/***********************UART4 SENSOR**************************** */

#define SENSOR_POWER_PIN  GET_PIN(B, 4)


#define UART4_TX_GPIO_PIN              GPIO_Pin_10
#define UART4_TX_GPIO_PORT             GPIOC

#define UART4_RX_GPIO_PIN              GPIO_Pin_11
#define UART4_RX_GPIO_PORT             GPIOC

#define UART4_RX_BUFFER_LENGTH         (64U)
#define UART4_RX_RB_LENGTH             (64U)

#define UART4_DMA_RX_CHANNEL           DMA2_Channel3
#define UART4_DMA_TX_CHANNEL           DMA2_Channel5

#define UART4_DMA_RX_IRQ_CHANNEL       DMA2_Channel3_IRQn
#define UART4_DMA_TX_IRQ_CHANNEL       DMA2_Channel5_IRQn

//#define SENSOR_POWER_ON              GPIO_SetBits(SENSOR_POWER_PORT, SENSOR_POWER_PIN);
//#define SENSOR_POWER_OFF             GPIO_ResetBits(SENSOR_POWER_PORT, SENSOR_POWER_PIN);

/***********************UART5 bt**************************** */
#define UART5_TX_GPIO_PIN              GPIO_Pin_12
#define UART5_TX_GPIO_PORT             GPIOC

#define UART5_RX_GPIO_PIN              GPIO_Pin_2
#define UART5_RX_GPIO_PORT             GPIOD

#define UART5_RX_BUFFER_LENGTH         (256U)
#define UART5_RX_RB_LENGTH             (512U)
#define UART5_TX_RB_LENGTH             (512U)

#define UART5_DMA_RX_CHANNEL           DMA2_Channel2
#define UART5_DMA_TX_CHANNEL           DMA2_Channel4

#define UART5_DMA_RX_IRQ_CHANNEL       DMA2_Channel2_IRQn
#define UART5_DMA_TX_IRQ_CHANNEL       DMA2_Channel4_IRQn

extern lwrb_t uart2_rx_rb;
extern lwrb_t uart4_rx_rb;


int rt_hw_usart_init(void);
void UART2_RxCheck(void);
void UART4_RxCheck(void);
#endif


