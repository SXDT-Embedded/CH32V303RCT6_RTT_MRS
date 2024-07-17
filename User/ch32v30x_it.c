/*
 * @Author: mbw
 * @Date: 2024-06-27 10:52:38
 * @LastEditors: mbw && 1600520629@qq.com
 * @LastEditTime: 2024-06-27 11:01:56
 * @FilePath: \SCV_ControlBox_CH32V303RCT6_BC260\User\ch32v30x_it.c
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
/********************************** (C) COPYRIGHT *******************************
* File Name          : ch32v10x_it.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : Main Interrupt Service Routines.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "ch32v30x_it.h"
#include "board.h"
#include <rtthread.h>



void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();
    rt_kprintf(" NMI Handler\r\n");
    rt_interrupt_leave();
    FREE_INT_SP();
}


/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
    GET_INT_SP();
    rt_interrupt_enter();

    rt_kprintf(" hardfult\r\n");
    rt_kprintf("mepc:%08x\r\n",__get_MEPC());
    rt_kprintf("mcause:%08x\r\n",__get_MCAUSE());
    rt_kprintf("mtval:%08x\r\n",__get_MTVAL());

    while(1);

    rt_interrupt_leave();
    FREE_INT_SP();

}


