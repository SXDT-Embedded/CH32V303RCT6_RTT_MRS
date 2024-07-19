/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "ch32v30x.h"
#include <rtthread.h>
#include <rthw.h>
#include "drivers/pin.h"
#include "bsp_led.h"
#include "bsp_flash.h"
#include "rtdef.h"

#define LOG_TAG "main"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>


int main(void)
{
    SystemCoreClockUpdate();
    LOG_I(" MCU: CH32V303\r\n");
    LOG_I(" SysClk: %dHz\r\n",SystemCoreClock);
    LOG_I(" ChipID: %08x\r\n",DBGMCU_GetCHIPID());

	while(1)
	{
	    rt_thread_mdelay(5000);
	}
}
