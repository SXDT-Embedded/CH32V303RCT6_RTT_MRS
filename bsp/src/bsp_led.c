/*
 * @Author       : yzy
 * @Date         : 2023-04-04 14:45:50
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2024-07-17 17:46:33
 * @FilePath     : \SCV_ControlBox_CH32V303RCT6_BC260\bsp\src\bsp_led.c
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#include "bsp_led.h"
#include <rtthread.h>
#include <stdlib.h>
// #include "bsp_sys.h"

#define LOG_TAG "bsp_led"   // ?????????????????NO_TAG
#define LOG_LVL LOG_LVL_DBG // ?????????????????????????
#include <ulog.h>           // ??? LOG_TAG ? LOG_LVL ??

agile_led_t *led_r = RT_NULL;
agile_led_t *led_g = RT_NULL;
agile_led_t *led_b = RT_NULL;

// ???????
inline void LED_NetInComm(void)
{
    LED_STOP(r);
    LED_STOP(b);
    LED_CTRL(g, "3200,10", 1);
    LED_START(g);
}

// ????
inline void LED_NetFail(void)
{
    LED_STOP(r);
    LED_STOP(b);
    LED_CTRL(g, "250,250", 6);
    LED_START(g);
}

int BSP_LED_Init(void)
{
    led_r = agile_led_create(LED_R_PIN, PIN_LOW, "1000,2000", 1);
    led_g = agile_led_create(LED_G_PIN, PIN_LOW, "1000,2000", 1);
    led_b = agile_led_create(LED_B_PIN, PIN_LOW, "1000,2000", 1);

    LED_STOP(r);
    LED_STOP(g);
    LED_STOP(b);

    LOG_I("BSP_LED_Init");

    return 0;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_DEVICE_EXPORT(BSP_LED_Init);
#endif

static void TEST_LED(int argc, char **argv)
{
    if (argc == 2)
    {
        int a = atoi(argv[1]);
        if (a == 1)
        {
            LED_NetInComm();
        }
        else if (a == 2)
        {
            LED_NetFail();
        }
    }
    else
    {
        LOG_E("TEST_LED      --use TEST_LED [mode]");
    }
}
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(TEST_LED, TEST_LED);
#endif // !FINSH_USING_MSH
