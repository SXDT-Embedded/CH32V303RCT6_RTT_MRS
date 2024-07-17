/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2024-07-17 09:54:43
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2024-07-17 09:58:02
 * @FilePath     : \SCV_ControlBox_CH32V303RCT6_BC260\bsp\inc\bsp_lcd_1621.h
 * @Description  :
 *
 * Copyright (c) 2024 by yzy, All Rights Reserved.
 */
#ifndef __BSP_LCD_1621_H__
#define __BSP_LCD_1621_H__

// #include "user_sys.h"
#include "drv_gpio.h"
#include <rtthread.h>

// 参考：https://blog.csdn.net/wyf9003/article/details/121791860
// 一般情况下，为了节约引脚，可以省掉读时钟
#define LCD_CS_PIN          23	    // CS为片选，拉低为使能
#define LCD_DATA_PIN        19	    // 传输的数据
#define LCD_RD_PIN          22		  // 读时钟
#define LCD_WR_PIN          21		  // 写时钟

#define LCD_BUFFER_LEN  (14U)          // 14个SEG

#define SYS_DIS         0x00        // 同时关闭系统振荡器和LCD偏置发生器
#define SYS_EN          0x01        // 开启系统振荡器
#define LCD_OFF         0x02        // 关闭LCD偏置发生器
#define LCD_ON          0x03        // 开启LCD偏置发生器
#define TIMER_DIS       0x04        // 禁止时间基准输出
#define WDT_DIS         0x05        // 禁止WDT暂停标志输出
#define TIMER_EN        0x06        // 允许时间基准输出
#define WDT_EN          0x07        // 允许WDT暂停标志输出
#define TONE_OFF        0x08        // 关闭蜂鸣器输出
#define TONE_ON         0x09        // 打开蜂鸣器输出
#define CLR_TIMER       0x0B        // 清除时基发生器的内容
#define CLR_WDT         0x0F        // 清除WDT内容
#define XTAL_32K        0x17        // 系统时钟为晶体振荡器
#define RC_256K         0x1B		 	  // 系统时钟为片内RC振荡器
#define EXT_256K        0x1F        // 系统时钟为外部时钟LCD 1/2
#define BIAS_2_2COM     0x22        // 2COM端 LCD:1/2偏压
#define BIAS_2_3COM     0x26        // 3COM端 LCD:1/2偏压
#define BIAS_2_4COM     0x2A        // 4COM端 LCD:1/2偏压
#define BIAS_3_2COM     0x23        // 2COM端 LCD:1/3偏压
#define BIAS_3_3COM     0x27        // 3COM端 LCD:1/3偏压
#define BIAS_3_4COM     0x2B        // 4COM端 LCD:1/3偏压
#define TONE_4K         0x5F        // 蜂鸣器频率:4KHz
#define TONE_2K         0x7F        // 蜂鸣器频率:2KHz
#define IRQ_DIS         0x97        // 禁止IRQ输出
#define IRQ_EN          0x9F        // 允许IRQ输出
#define TIMER_WDT_F1    0xB8        // 时基/WDT时钟输出:1Hz;  WDT暂停标志延时:4s
#define TIMER_WDT_F2    0xB9        // 时基/WDT时钟输出:1Hz;  WDT暂停标志延时:4s
#define TIMER_WDT_F4    0xBA        // 时基/WDT时钟输出:1Hz;  WDT暂停标志延时:4s
#define TIMER_WDT_F8    0xBB        // 时基/WDT时钟输出:1Hz;  WDT暂停标志延时:4s
#define TIMER_WDT_F16   0xBC        // 时基/WDT时钟输出:1Hz;  WDT暂停标志延时:4s
#define TIMER_WDT_F32   0xBD        // 时基/WDT时钟输出:1Hz;  WDT暂停标志延时:4s
#define TIMER_WDT_F64   0xBE        // 时基/WDT时钟输出:1Hz;  WDT暂停标志延时:4s
#define TIMER_WDT_F128  0xBF        // 时基/WDT时钟输出:1Hz;  WDT暂停标志延时:4s
#define TOPT            0xE0        // 测试模式
#define TNORMAL         0xE3        // 标准模式

#define WR_CMD          0x80        // 控制发送命令标识的宏
#define WR_DATA         0xA0        // 控制发送写数据的宏

#define LCD_WR_PIN_LOW       rt_pin_write(LCD_WR_PIN, PIN_LOW)
#define LCD_WR_PIN_HIGH      rt_pin_write(LCD_WR_PIN, PIN_HIGH)
#define LCD_CS_PIN_LOW       rt_pin_write(LCD_CS_PIN, PIN_LOW)
#define LCD_CS_PIN_HIGN      rt_pin_write(LCD_CS_PIN, PIN_HIGH)
#define LCD_DATA_PIN_LOW     rt_pin_write(LCD_DATA_PIN, PIN_LOW)
#define LCD_DATA_PIN_HIGH    rt_pin_write(LCD_DATA_PIN, PIN_HIGH)

typedef enum
{
  LCD_SEG0 = 0,
  LCD_SEG1,
  LCD_SEG2,
  LCD_SEG3,
  LCD_SEG4,
  LCD_SEG5,
  LCD_SEG6,
  LCD_SEG7,
  LCD_SEG8,
  LCD_SEG9,
  LCD_SEG10,
  LCD_SEG11,
  LCD_SEG12,
  LCD_SEG13
} LcdSeg;

typedef enum
{
  LCD_COM1 = 1,
  LCD_COM2,
  LCD_COM3,
  LCD_COM4
} LcdCom;

// 图标
typedef enum {
  S1_OVER_PRESSURE = 0,       //  超压
  S2_UNDER_PRESSURE,          //  欠压
  S3_OVER_CURRENT,            //  过流
  S4_LEAKAGE,                 //  泄露
  S5_PRESSURE_TIMING_VALVES,  //  压力、定时、自闭阀、手阀
  S6_NETWORK,                 //  联网
  S7_LEAK_HUNTING,            //  检漏
  S8_FAULT,                   //  故障
  S9_ALARM,                   //  报警

  L1_LINE,                    //  下划线1
  L2_LINE,                    //  下划线2
  L3_LINE,                    //  下划线3
  L4_LINE,                    //  下划线4

  K1_SELF_CLOSING_VALVE_ON,   //  自闭阀开
  K2_SELF_CLOSING_VALVE_OFF,  //  自闭阀关
  K3_HAND_VALVE_ON,           //  手阀开
  K4_HAND_VALVE_OFF,          //  手阀关

  ICON_POINT,               //  小数点
  ICON_KPA,                 //  kPa单位
  ICON_MIN                  //  min单位
} LcdIconIndex;

typedef enum
{
  KPA_NUM_1 = LCD_SEG1,     //  kpa的十位,A的SEG为基准
  KPA_NUM_2 = LCD_SEG3,     //  kpa的个位
  KPA_NUM_3 = LCD_SEG5,     //  kpa的小数点后1位

  MIN_NUM_1 = LCD_SEG9,     //  min的十位
  MIN_NUM_2 = LCD_SEG11    //  min的个位
} LcdNumIndex;

typedef enum {
  SHOW_CLOSE = 0,
  SHOW_OPEN
} LcdSegmentShowControl;

/**
 * @description: GN1621初始化
 * @param {*}
 * @return {*}
 */
void LCD_Init(void);

/**
 * @description: 屏幕刷新
 * @param {*}
 * @return {*}
 */
void LCD_Refresh(void);

/**
 * @description: 清屏
 * @param {*}
 * @return {*}
 */
void LCD_Clear(void);

/**
 * @description:
 * @param {LcdSeg} seg     0-13(LCD_BUFFER_LEN-1)
 * @param {LcdCom} com      1-4
 * @param {LcdSegmentShowControl} mode    SHOW_CLOSE：熄灭；SHOW_OPEN:点亮
 * @return {*}
 */
void LCD_ShowSegment(LcdSeg seg, LcdCom com, LcdSegmentShowControl mode);

/**
 * @description: 显示图标
 * @param {LcdIconIndex} index
 * @param {LcdSegmentShowControl} mode  SHOW_CLOSE：熄灭；SHOW_OPEN:点亮
 * @return {*}
 */
void LCD_ShowIcon(LcdIconIndex index, LcdSegmentShowControl mode);

/**
 * @description: LCD显示数字
 * @param {LcdNumIndex} index
 * @param {uint8_t} num (0-9)
 * @param {LcdSegmentShowControl} mode
 * @return {*}
 */
void LCD_ShowNum(LcdNumIndex index, uint8_t num, LcdSegmentShowControl mode);

/**
 * @description: LCD显示压力数值
 * @param {uint32_t} pressure  (0-99999)
 * @param {LcdSegmentShowControl} mode
 * @return {*}
 */
void LCD_ShowPressure(uint32_t pressure, LcdSegmentShowControl mode);

/**
 * @description: LCD显示定时时间
 * @param {uint8_t} time  (0-99)
 * @param {LcdSegmentShowControl} mode
 * @return {*}
 */
void LCD_ShowTime(uint8_t time, LcdSegmentShowControl mode);




































#endif  //__BSP_LCD_1621_H__
