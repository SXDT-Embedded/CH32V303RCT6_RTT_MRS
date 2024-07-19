/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2024-06-14 13:26:13
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2024-06-14 13:27:02
 * @FilePath     : \wuxi_alarm_ch32v303rct6_rtt\common\AT Command\at_port.c
 * @Description  :
 *
 * Copyright (c) 2024 by yzy, All Rights Reserved.
 */
/**
 * @Brief: The AT component drives the interface implementation
 * @Author: roger.luo
 * @Date: 2021-04-04
 * @Last Modified by: roger.luo
 * @Last Modified time: 2021-11-27
 */
#include <stddef.h>
#include <stdlib.h>

#include "bsp_sys.h"

/**
 * @brief Custom malloc for AT component.
 */
void *at_malloc(unsigned int nbytes)
{
//    return malloc(nbytes);
    return rt_malloc(nbytes);
}

/**
 * @brief Custom free for AT component.
 */
void  at_free(void *ptr)
{
//    free(ptr);
    rt_free(ptr);
}

/**
 * @brief Gets the total number of milliseconds in the system.
 */
unsigned int at_get_ms(void)
{
//   return get_tick();
    return rt_tick_get();
}
