/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2024-07-17 10:41:45
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2024-07-17 16:26:52
 * @FilePath     : \SCV_ControlBox_CH32V303RCT6_BC260\bsp\inc\bsp_sys.h
 * @Description  :
 *
 * Copyright (c) 2024 by yzy, All Rights Reserved.
 */
#ifndef __BSP_SYS_H__
#define __BSP_SYS_H__

#include "board.h"
#include "rtthread.h"

//定义结构和联合
typedef union
{
  struct
  {
    uint8_t low_byte;
    uint8_t mlow_byte;
    uint8_t mhigh_byte;
    uint8_t high_byte;
  }float_byte;
  float value;
}FLAOT_UNION;

//仿二进制赋值
#define  HEX__(n)   0x##n##UL
#define  B8__(x)   ( (x & 0x0000000FUL) ? 1:0 )\
    +( (x & 0x000000F0UL) ? 2:0 )\
    +( (x & 0x00000F00UL) ? 4:0 )\
    +( (x & 0x0000F000UL) ? 8:0 )\
    +( (x & 0x000F0000UL) ? 16:0 )\
    +( (x & 0x00F00000UL) ? 32:0 )\
    +( (x & 0x0F000000UL) ? 64:0 )\
    +( (x & 0xF0000000UL) ? 128:0 )
#define  B8(x)                      ((unsigned char)B8__(HEX__(x)))
#define  B16(x_msb,x_lsb)           (((unsigned int)B8(x_msb)<<8) + B8(x_lsb))
#define  B32(x_msb,x_2,x_3,x_lsb)   (((unsigned long)B8(x_msb)<<24) + ((unsigned long)B8(x_2)<<16) + ((unsigned long)B8(x_3)<<8) + B8(x_lsb))


#endif // !__BSP_SYS_H__





















