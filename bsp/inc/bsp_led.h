#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "board.h"
#include "drv_gpio.h"
#include "agile_led.h"



extern agile_led_t *led_r;
extern agile_led_t *led_g;
extern agile_led_t *led_y;
extern agile_led_t *emv;//电磁阀
extern agile_led_t *fj;//风机


#define RED_LED_PIN             GET_PIN(C, 7)
#define GREEN_LED_PIN           GET_PIN(C, 6)
#define YELLOW_LED_PIN          GET_PIN(B, 15)
#define EMV_LED_PIN             GET_PIN(B, 0)
#define FJ_LED_PIN              GET_PIN(B, 1)

#define LED_ON(x)               agile_led_on(led_##x)
#define LED_OFF(x)              agile_led_off(led_##x)
#define LED_TOGGLE(x)           agile_led_toggle(led_##x)

#define LED_START(x)    agile_led_start(led_##x)

#define EMV_ON(x)        agile_led_on(x)
#define FJ_ON()          agile_led_off(x)

#define EMV_OFF(x)        agile_led_on(x)
#define FJ_OFF(x)         agile_led_off(x)

#define DEV_CTRL(x, light_mode, loop_cnt)    do                 \
    {                                                           \
        agile_led_set_light_mode(x, light_mode, loop_cnt);      \
        agile_led_start(x);                                      \
    } while (0U)


#define LED_STOP(x)    do                                   \
    {                                                       \
        LED_OFF(x);                                         \
        agile_led_stop(led_##x);                             \
    } while (0U)

#define LED_CTRL(x, light_mode, loop_cnt)    do                 \
    {                                                           \
        agile_led_set_light_mode(led_##x, light_mode, loop_cnt); \
    } while (0U)


/************************预设的一些LED闪烁模式**************/
// 预热绿灯闪烁
#define GREEN_LED_PREAT    do             \
    {                               \
        LED_STOP(r);                \
        LED_STOP(y);                \
        LED_CTRL(g, "500,500", -1); \
        LED_START(g);               \
    } while (0U)

//  正常绿灯常亮
#define GREEN_LED_NORMAL    do                                    \
    {                                                       \
        LED_STOP(r);                                        \
        LED_STOP(y);                                        \
        agile_led_stop(led_g);                                                \
        LED_ON(g);                                          \
    } while (0U)

// 故障状态 黄灯常亮
#define YELLOW_LED_FAULT    do                                     \
    {                                                       \
        LED_STOP(r);                                        \
        LED_STOP(g);                                        \
        LED_ON(y);                                          \
    } while (0U)

// 报警状态 红灯闪烁
#define RED_LED_ALAEM    do                                     \
    {                                                       \
        LED_STOP(g);                                        \
        LED_STOP(y);                                        \
        LED_CTRL(r, "200,200", -1);                         \
        LED_START(r);                                       \
    } while (0U)

// 寿命到期状态 黄灯闪烁
#define YELLOW_LED_END_OF_LIFE    do                               \
    {                                                       \
        LED_STOP(r);                                        \
        LED_STOP(g);                                        \
        LED_CTRL(y, "500,500", -1);                         \
        LED_START(y);                                       \
    } while (0U)




#endif //!__BSP_LED_H__

