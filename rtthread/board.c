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
#include "drv_usart.h"

#include <rthw.h>
#include <rtthread.h>


// core clock.
extern uint32_t SystemCoreClock;

static uint32_t _SysTick_Config(rt_uint32_t ticks)
{
    NVIC_SetPriority(SysTicK_IRQn,0xf0);
    NVIC_SetPriority(Software_IRQn,0xf0);
    NVIC_EnableIRQ(SysTicK_IRQn);
    NVIC_EnableIRQ(Software_IRQn);
    SysTick->CTLR=0;
    SysTick->SR=0;
    SysTick->CNT=0;
    SysTick->CMP=ticks-1;
    SysTick->CTLR=0xF;
    return 0;
}

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE (1024)
static uint32_t rt_heap[RT_HEAP_SIZE];     // heap default size: 4K(1024 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{
    /* System Tick Configuration */
    _SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif

#ifdef RT_USING_SERIAL
    rt_hw_usart_init();
#endif

#ifdef RT_USING_CONSOLE
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
}


void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void)
{
    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();
    SysTick->SR=0;
    rt_tick_increase();
    /* leave interrupt */
    rt_interrupt_leave();
    FREE_INT_SP();

}


void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t ticks;
    rt_uint32_t told, tnow, tcnt = 0;
    rt_uint32_t reload = SysTick->CMP;

    /* �����ʱ������ tick �� */
    ticks = us * reload / (1000000 / RT_TICK_PER_SECOND);
    /* ��õ�ǰʱ�� */
    told = SysTick->CNT;
    while (1)
    {
        /* ѭ����õ�ǰʱ�䣬ֱ���ﵽָ����ʱ����˳�ѭ�� */
        tnow = SysTick->CNT;
        if (tnow != told)
        {
            if (tnow < told)
            {
                // ȡ���� SysTick->CNT ������ݼ�
                // ͨ��һ�� tcnt ��������ǰ����ֵ tnow ����һʱ�̵ļ���ֵ told �Ĳ�ֵ�����ۼӣ�ע�� SysTick->VAL Ϊ�ݼ����ǵ�����������
                // �����ۼ�ֵ tcnt����ʱ���� ticks ʱ����ѭ������ tcnt ���ֵΪ 0xffff ffff�������ܳ�����ѭ�������
//                tcnt += told - tnow;            // SysTick �����Ļ�
                tcnt += reload - told + tnow; // SysTick �ݼ��Ļ�
            }
            else
            {
//                tcnt += reload - tnow + told;   // SysTick �����Ļ�
                tcnt += tnow - told;          // SysTick �ݼ��Ļ�
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}


