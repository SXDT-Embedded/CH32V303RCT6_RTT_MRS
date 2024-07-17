#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "board.h"
#include "drv_gpio.h"
#include "agile_led.h"

#define LED_R_PIN     GET_PIN(A, 0)
#define LED_G_PIN     GET_PIN(A, 1)
#define LED_B_PIN     GET_PIN(B, 9)

#define LED_ON(x)     agile_led_on(led_##x)
#define LED_OFF(x)    agile_led_off(led_##x)
#define LED_TOGGLE(x) agile_led_toggle(led_##x)

#define LED_START(x)  agile_led_start(led_##x)

#define LED_STOP(x)              \
    do                           \
    {                            \
        LED_OFF(x);              \
        agile_led_stop(led_##x); \
    } while (0U)

#define LED_CTRL(x, light_mode, loop_cnt)                        \
    do                                                           \
    {                                                            \
        agile_led_set_light_mode(led_##x, light_mode, loop_cnt); \
    } while (0U)

/************************预设的一些LED闪烁模式**************/
void LED_NetInComm(void);
void LED_NetFail(void);

#endif //!__BSP_LED_H__
