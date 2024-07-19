/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 * 2018-11-12     Ernest Chen  modify copyright
 */
#include "board.h"

#include <stdint.h>
// #include "drv_uart.h"
#include "drv_usart.h"

#include <rthw.h>
#include <rtthread.h>

// Holds the system core clock, which is the system clock
// frequency supplied to the SysTick timer and the processor
// core clock.
extern uint32_t SystemCoreClock;

static uint32_t _SysTick_Config(rt_uint32_t ticks)
{
    // 抢占位:1 优先级:111B = 7
    NVIC_SetPriority(SysTicK_IRQn, 0xf0);
    NVIC_SetPriority(Software_IRQn, 0xf0);
    NVIC_EnableIRQ(SysTicK_IRQn);
    NVIC_EnableIRQ(Software_IRQn);

    SysTick->CTLR = 0;
    SysTick->SR   = 0;
    SysTick->CNT  = 0;
    SysTick->CMP  = ticks - 1;
    //    SysTick->CTLR = 0xF;        // 向上计数
    // 改成向下计数，搞得和CM3类似
    SysTick->CTLR = 0x001F; // 向下计数   0----    1 1111
    return 0;
}

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#if (USING_MAX_HEAP_SIZE == 0)
#define RT_HEAP_SIZE (1024)
static uint32_t rt_heap[RT_HEAP_SIZE]; // heap default size: 4K(1024 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}
RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#else
// RT_WEAK void *rt_heap_begin_get(void)
// {
//     return HEAP_BEGIN;
// }
// RT_WEAK void *rt_heap_end_get(void)
// {
//     return HEAP_END;
// }
#endif /* !USING_MAX_HEAP_SIZE*/
#endif

/**
 * This function will initial your board.
 */
void rt_hw_board_init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* System Tick Configuration */
    _SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    // rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
    rt_system_heap_init((void *) HEAP_BEGIN, (void *) HEAP_END);
#endif
    /* USART driver initialization is open by default */
#ifdef RT_USING_SERIAL
    rt_hw_usart_init();
#endif
#ifdef RT_USING_CONSOLE
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
#ifdef RT_USING_PIN
    /* pin must initialized before i2c */
    rt_hw_pin_init();
#endif

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
}

void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void)
{
    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();
    SysTick->SR = 0;
    rt_tick_increase();
    /* leave interrupt */
    rt_interrupt_leave();
    FREE_INT_SP();
}

// #define DBG_TAG "buzzer"
// #define DBG_LVL DBG_ERROR
// #include <rtdbg.h>

// 参考：[001] [RT-Thread 学习笔记] 高精度延时函数 rt_hw_us_delay 的陷阱与优化
// https://blog.csdn.net/kouxi1/article/details/122581857

// 其中入口参数 us 指示出需要延时的微秒数目，这个函数只能支持低于 1 OS Tick 的延时。

// 可以支持≥ 1 os tick 的延时时间！但是不建议这样使用
// ，如果需要延时时间≥ 1 os tick，请使用 rt_thread_delay() 或 rt_thread_mdelay 函数
void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t ticks;
    rt_uint32_t told, tnow, tcnt = 0;
    rt_uint32_t reload = SysTick->CMP;

    /* 获得延时经过的 tick 数 */
    ticks = us * reload / (1000000 / RT_TICK_PER_SECOND);
    /* 获得当前时间 */
    told = SysTick->CNT;
    while (1)
    {
        /* 循环获得当前时间，直到达到指定的时间后退出循环 */
        tnow = SysTick->CNT;
        if (tnow != told)
        {
            if (tnow < told)
            {
                // 取决于 SysTick->CNT 递增或递减
                // 通过一个 tcnt 变量将当前计数值 tnow 与上一时刻的计数值 told 的差值进行累加（注意 SysTick->VAL 为递减还是递增计数器）
                // ，当累加值 tcnt≥延时节拍 ticks 时跳出循环，而 tcnt 最大值为 0xffff ffff，不可能出现死循环的情况
                tcnt += told - tnow; // SysTick 递增的话
                //tcnt += reload - told + tnow; // SysTick 递减的话
            }
            else
            {
                tcnt += reload - tnow + told; // SysTick 递增的话
                //tcnt += tnow - told;          // SysTick 递减的话
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}
