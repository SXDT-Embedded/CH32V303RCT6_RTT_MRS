/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2024-07-17 09:54:43
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2024-07-17 16:45:17
 * @FilePath     : \CH32V303RCT6_RTT_MRS\bsp\inc\bsp_lcd_1621.h
 * @Description  :
 *
 * Copyright (c) 2024 by yzy, All Rights Reserved.
 */
#ifndef __BSP_LCD_1621_H__
#define __BSP_LCD_1621_H__

#include "bsp_sys.h"
#include "drv_gpio.h"
#include <rtthread.h>

// 参考：https://blog.csdn.net/wyf9003/article/details/121791860
// 一般情况下，为了节约引脚，可以省掉读时钟
#define LCD_CS_PIN          GET_PIN(A, 6)	    // CS为片选，拉低为使能
#define LCD_DATA_PIN        GET_PIN(C, 5)	    // 传输的数据
#define LCD_RD_PIN          GET_PIN(A, 7)		  // 读时钟
#define LCD_WR_PIN          GET_PIN(C, 4)		  // 写时钟

#define LCD_BUFFER_LEN  ((uint8_t)14U)          // 14个SEG

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
  kLcdSeg0 = 0U,
  kLcdSeg1,
  kLcdSeg2,
  kLcdSeg3,
  kLcdSeg4,
  kLcdSeg5,
  kLcdSeg6,
  kLcdSeg7,
  kLcdSeg8,
  kLcdSeg9,
  kLcdSeg10,
  kLcdSeg11,
  kLcdSeg12,
  kLcdSeg13
} TeLcdSeg;

typedef enum
{
  kLcdCom1 = 1U,
  kLcdCom2,
  kLcdCom3,
  kLcdCom4
} TeLcdCom;

// 图标
typedef enum {
  kS1OverPress = 0U,       //  超压
  kS2UnderPress,          //  欠压
  kS3OverCurrent,            //  过流
  kS4Leakage,                 //  泄露
  kS5PressTimingValves,  //  压力、定时、自闭阀、手阀
  kS6Network,                 //  联网
  kS7LeakHunting,            //  检漏
  kS8FFault,                   //  故障
  kS9Alarm,                   //  报警

  kL1Line,                    //  下划线1
  kL2Line,                    //  下划线2
  kL3Line,                    //  下划线3
  kL4Line,                    //  下划线4

  kK1SelfCloseingValveOn,   //  自闭阀开
  kK2SelfCloseingValveOff,  //  自闭阀关
  kK3HandValveOn,           //  手阀开
  kK4HandValveOff,          //  手阀关

  kIconPoint,               //  小数点
  kIconKpa,                 //  kPa单位
  kIconMin                  //  min单位
} TeLcdIconIndex;

typedef enum
{
  kKpaNum1 = kLcdSeg1,     //  kpa的十位,A的SEG为基准
  kKpaNum2 = kLcdSeg3,     //  kpa的个位
  kKpaNum3 = kLcdSeg5,     //  kpa的小数点后1位

  kMinNum1 = kLcdSeg9,     //  min的十位
  kMinNum2 = kLcdSeg11    //  min的个位
} TeLcdNumIndex;

typedef enum {
  kShowClose = 0U,
  kShowOpen
} TeLcdSegmentShowControl;


void LCD_Init(void);
void LCD_Refresh(void);
void LCD_Clear(void);

void LCD_ShowSegment(TeLcdSeg seg, TeLcdCom com, TeLcdSegmentShowControl mode);
void LCD_ShowIcon(TeLcdIconIndex index, TeLcdSegmentShowControl mode);
void LCD_ShowNum(TeLcdNumIndex index, uint8_t num, TeLcdSegmentShowControl mode);

void LCD_ShowPressure(uint32_t pressure, TeLcdSegmentShowControl mode);
void LCD_ShowTime(uint8_t time, TeLcdSegmentShowControl mode);


#endif  //__BSP_LCD_1621_H__
