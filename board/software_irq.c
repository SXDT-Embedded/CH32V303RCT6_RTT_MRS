/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2024-07-19 12:09:42
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2024-07-19 12:09:57
 * @FilePath     : \CH32V303RCT6_RTT_MRS\rtthread\software_irq.c
 * @Description  :
 *
 * Copyright (c) 2024 by yzy, All Rights Reserved.
 */
/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-09     WCH        the first version
 * 2023-01-04     WangShun   Remove redundant files
 */
#include "rtconfig.h"
#include "ch32v30x.h"

void rt_trigger_software_interrupt(void)
{
    SysTick->CTLR |= (1 << 31);
}

void rt_hw_do_after_save_above(void)
{
    __asm volatile ("li t0,0x20" );
    __asm volatile ("csrs 0x804, t0");

    SysTick->CTLR &= ~(1 << 31);
}
