/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 17:51:11
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2024-06-19 15:51:40
 * @FilePath     : \wuxi_alarm_ch32v303rct6_rtt\bsp\inc\bsp_vin_detection.h
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#ifndef __BSP_VIN_DETECTION_H__
#define __BSP_VIN_DETECTION_H__

#include "board.h"

// ADC要采集的通道个数
#define NUM_OF_CHANNEL              (1U)
// ADC要采集的次数
#define ADC_TIMES                   (30U)
#define ADC_BUFFER_SIZE             (NUM_OF_CHANNEL * ADC_TIMES)

// ADC GPIO 宏定义
#define USED_ADC_GPIO_PORT          GPIOA
#define USED_ADC_GPIO_CLK_ENABLE()  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

// ADC 序号宏定义
#define USED_ADC                    ADC1
#define USED_ADC_CLK_ENABLE()       RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

// ADC DMA 通道宏定义，这里我们使用DMA传输
#define USED_ADC_DR_ADDRESS         ((uint32_t)&USED_ADC->RDATAR)
#define USED_ADC_DMA_CLK_ENABLE()   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
#define USED_ADC_DMA_STREAM         DMA1_Channel1

/*****************机械手/电磁阀驱动电流的检测***********************/
// #define EMV_ADC_GPIO_PORT              GPIOA
// #define EMV_ADC_GPIO_PIN               GPIO_Pin_0
// #define EMV_ADC_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

// #define EMV_ADC                        ADC1
// #define EMV_ADC_CLK_ENABLE()           RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
// #define EMV_ADC_CHANNEL                ADC_Channel_0

/***************************12V输入的检测，用来实现掉电检测****************************/
// #define VIN_ADC_GPIO_PORT              GPIOA
#define VIN_ADC_GPIO_PIN               GPIO_Pin_0
// #define VIN_ADC_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

// #define VIN_ADC                        ADC1
// #define VIN_ADC_CLK_ENABLE()           RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
#define VIN_ADC_CHANNEL                ADC_Channel_0

/****************************TGS2610的检测****************************/
// #define TGS_ADC_GPIO_PORT              GPIOA
#define TGS_ADC_GPIO_PIN               GPIO_Pin_6
// #define TGS_ADC_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

// #define MQ_ADC                        ADC1
// #define TGS_ADC_CLK_ENABLE()           RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
#define TGS_ADC_CHANNEL                ADC_Channel_6


typedef enum
{
    kVinAdc = 0U,
} TeAdcIndex;

// 存储采集的数据
extern uint16_t adc_buffer[ADC_BUFFER_SIZE];


#define VIN_THREAD_STACK_SIZE    (2024)
#define VIN_THREAD_PRIORITY      (10)
#define VIN_THREAD_TIMESLICE     (10)

#define VIN_VOLTAGE_RATIO        (4.19148936F)

// 不含输入二极管的压降
// (4.7/19.7) * 13V = 3.1V, 3.1/3.3*4096 = 3723
#define VIN_ADC_HIGH_THRESHOLD      (uint16_t)3847  // 13V
// (4.7/19.7) * 10V = 2.39V, 2.39/3.3*4096 = 2966
#define VIN_ADC_LOW_THRESHOLD       (uint16_t)2966  // 10V


float Get_VIN_Voltage(void);
uint16_t Get_VIN_VoltageInt1000x(void);

int BSP_VIN_Detection_Init(void);

#endif // !__BSP_VIN_DETECTION_H__
