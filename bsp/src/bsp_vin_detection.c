/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2024-06-14 15:28:09
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2024-06-18 16:19:27
 * @FilePath     : \wuxi_alarm_ch32v303rct6_rtt\bsp\src\bsp_vin_detection.c
 * @Description  :
 *
 * Copyright (c) 2024 by yzy, All Rights Reserved.
 */
#include "bsp_vin_detection.h"
#include "bsp_rtc.h"
#include "bsp_flash.h"
#include "rthw.h"
#include "pin.h"
#include "drv_gpio.h"


#define LOG_TAG     "bsp_vin_detection"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

//static rt_sem_t sem_vin;
static struct rt_semaphore sem_vin;

ALIGN(RT_ALIGN_SIZE)
static char vin_thread_stack[VIN_THREAD_STACK_SIZE];
static struct rt_thread vin_thread;

uint16_t adc_buffer[ADC_BUFFER_SIZE] = {0};
static int16_t adc_calibrattion = 0;


void ADC1_2_IRQHandler(void) __attribute__((interrupt()));

static uint16_t _Get_ConversionVal(int16_t val)
{
    if((val + adc_calibrattion) < 0)
        return 0;
    if((adc_calibrattion + val) > 4095 || val == 4095)
        return 4095;
    return (val + adc_calibrattion);
}

/**
 * @description: ADC平均值计算
 * @param {TeAdcIndex} index
 * @return {*}
 */
float Get_ADC_Average(void)
{
    uint32_t sum = 0;
    float average = 0;

    for (uint8_t i = 0; i < ADC_BUFFER_SIZE; i++)
    {
        sum += _Get_ConversionVal(adc_buffer[i]);
    }
    average = sum / ADC_TIMES;

    return average;
}

// 这里测的是原始的ADC电压，没有经过缩放
float Get_ADC_Voltage(void)
{
    return (Get_ADC_Average() * 3.3 / 4095);
}

float Get_VIN_Voltage(void)
{
    float vin_voltage = Get_ADC_Voltage() * VIN_VOLTAGE_RATIO;

    LOG_D("vin_voltage = %0.2f\n", vin_voltage);

    return vin_voltage;
}
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(Get_VIN_Voltage, "Get_VIN_Voltage(V)");
#endif // !FINSH_USING_MSH

uint16_t Get_VIN_VoltageInt1000x(void)
{
    uint16_t voltage = Get_VIN_Voltage() * 1000;

    LOG_D("Get_VIN_VoltageInt1000x = %04d", voltage);
    return voltage;
}
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(Get_VIN_VoltageInt1000x, "Get_VIN_VoltageInt1000x");
#endif // !FINSH_USING_MSH

static void vin_thread_entry(void *param)
{
    while (1)
    {
        if (rt_sem_take(&sem_vin, RT_WAITING_FOREVER))
        {
            Get_VIN_Voltage();
//            Send_Laser_Alarm_Event(kNormalDetectionEvents);
        }
        rt_thread_delay(500);
    }
}

static void _USED_ADC_GPIO_Init(void)
{
//    GPIO_InitTypeDef GPIO_InitStructure = {0};
//
//    // 使能 GPIO 时钟 GPIOA
//    USED_ADC_GPIO_CLK_ENABLE();
//
//    // 配置 IO
//    GPIO_InitStructure.GPIO_Pin = VIN_ADC_GPIO_PIN;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//    GPIO_Init(USED_ADC_GPIO_PORT, &GPIO_InitStructure);
    rt_pin_mode(GET_PIN(A,0), PIN_MODE_INPUT);
}

static void _USED_ADC_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure = {0};

    USED_ADC_DMA_CLK_ENABLE();

    DMA_DeInit(USED_ADC_DMA_STREAM);
    DMA_InitStructure.DMA_PeripheralBaseAddr = USED_ADC_DR_ADDRESS;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adc_buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(USED_ADC_DMA_STREAM, &DMA_InitStructure);
}

int BSP_VIN_Detection_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    _USED_ADC_GPIO_Init();
    _USED_ADC_DMA_Init();

    // 开启ADC时钟
   USED_ADC_CLK_ENABLE();
   RCC_ADCCLKConfig(RCC_PCLK2_Div8);

   ADC_DeInit(USED_ADC);
   ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // ADC1和ADC2工作在独立模式
   ADC_InitStructure.ADC_ScanConvMode = DISABLE;        // 使能扫描
   ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // 连续转换
   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 禁止外部边沿触发
   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // 数据右对齐
   ADC_InitStructure.ADC_NbrOfChannel = NUM_OF_CHANNEL;    // 转换通道 (1) 个
   ADC_Init(USED_ADC, &ADC_InitStructure);

   ADC_RegularChannelConfig(USED_ADC, VIN_ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5);

    /* Higher Threshold: 14V, Lower Threshold: 10V */
    ADC_AnalogWatchdogThresholdsConfig(USED_ADC, VIN_ADC_HIGH_THRESHOLD, VIN_ADC_LOW_THRESHOLD);
    ADC_AnalogWatchdogSingleChannelConfig(USED_ADC, VIN_ADC_CHANNEL);
    ADC_AnalogWatchdogCmd(USED_ADC, ADC_AnalogWatchdog_SingleRegEnable);

    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    ADC_ITConfig(USED_ADC, ADC_IT_AWD, ENABLE);

    ADC_DMACmd(USED_ADC, ENABLE);
    ADC_Cmd(USED_ADC, ENABLE);

   ADC_BufferCmd(USED_ADC, DISABLE); //disable buffer
   ADC_ResetCalibration(USED_ADC);
   while(ADC_GetResetCalibrationStatus(USED_ADC));
   ADC_StartCalibration(USED_ADC);
   while(ADC_GetCalibrationStatus(USED_ADC));
   adc_calibrattion = Get_CalibrationValue(USED_ADC);

   DMA_Cmd(DMA1_Channel1, ENABLE);
   ADC_SoftwareStartConvCmd(ADC1, ENABLE);

   LOG_D("adc_calibrattion = %d", adc_calibrattion);
   LOG_I("BSP_ADC_Init");

    rt_thread_mdelay(50);
    rt_sem_init(&sem_vin, "sem_vin", 0, RT_IPC_FLAG_PRIO);
//    sem_vin = rt_sem_create("sem_vin", 0, RT_IPC_FLAG_PRIO);
    if (&sem_vin == RT_NULL)
    {
        LOG_E("create sem_vin fail");
    }

    rt_thread_init(&vin_thread,
                   "vin_thread",
                   vin_thread_entry,
                   RT_NULL,
                   &vin_thread_stack[0],
                   sizeof(vin_thread_stack),
                   VIN_THREAD_PRIORITY, VIN_THREAD_TIMESLICE);
    rt_thread_startup(&vin_thread);

    LOG_D("BSP_VIN_Detection_Init");

    return RT_EOK;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_APP_EXPORT(BSP_VIN_Detection_Init);
#endif

/*********************************************************************
 * @fn      ADC1_2_IRQHandler
 *
 * @brief   This function handles analog wathdog exception.
 *
 * @return  none
 */
void ADC1_2_IRQHandler(void)
{
    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();

    if(ADC_GetITStatus(ADC1, ADC_IT_AWD))
    {
        ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);

        ADC_ITConfig(USED_ADC, ADC_IT_AWD, DISABLE);
//        Send_Laser_Alarm_Event(kNormalDetectionEvents);
        rt_sem_release(&sem_vin);
    }

    /* leave interrupt */
    rt_interrupt_leave();
    FREE_INT_SP();
}



