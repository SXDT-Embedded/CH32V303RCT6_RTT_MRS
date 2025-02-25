/*
 * File      : board.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-09-22     Bernard      add board.h to this bsp
 * 2017-10-20     ZYH          emmm...setup for HAL Libraries
 */
#ifndef __BOARD_H__
#define __BOARD_H__

#include "ch32v30x.h"

#define CH32V30X_PIN_NUMBERS   (64U)

// 最大堆大小开关
// 参考：https://club.rt-thread.org/ask/article/001065082e9ae611.html
#define USING_MAX_HEAP_SIZE     1

#if  (USING_MAX_HEAP_SIZE == 0)
/* board configuration */
#define SRAM_SIZE  64
#define SRAM_END (0x20000000 + SRAM_SIZE * 1024)

extern int _ebss;
#define HEAP_BEGIN  ((void *)&_ebss)
#define HEAP_END    (SRAM_END-_stack_size)
#else
extern int _ebss, _heap_end;
#define HEAP_BEGIN  ((void *)&_ebss)
#define HEAP_END    ((void *)&_heap_end)
#endif  // !USING_MAX_HEAP_SIZE



void rt_hw_board_init(void);

#endif /* __BOARD_H__ */
