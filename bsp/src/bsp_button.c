#include <rtthread.h>
#include "flexible_button.h"
#include "drv_gpio.h"
#include "bsp_led.h"
#include "user_sys.h"


#define LOG_TAG    "bsp_button"
#define  LOG_LVL    LOG_LVL_DBG
#include <ulog.h>

#define BUTTON_LOGIC_LEVEL               0
#define  BUTTON_THREAD_PRIORITY          5 //优先级拉高,这时按键响应快点
#define  BUTTON_THREAD_TICKS             10
#define BUTTON_THREAD_STACK_SIZE        512 /**< button_thread 线程堆栈大小 */
static rt_uint8_t button_thread_stack[BUTTON_THREAD_STACK_SIZE]; /**< button_thread 线程堆栈 */
static struct rt_thread button_0;



#ifndef BUTTON_0
#define BUTTON_0 GET_PIN(B, 12)
#endif


#define ENUM_TO_STR(e) (#e)

typedef enum
{
    USER_BUTTON_0 = 0,
    USER_BUTTON_MAX
} user_button_t;

static char *enum_event_string[] = {
    ENUM_TO_STR(FLEX_BTN_PRESS_DOWN),
    ENUM_TO_STR(FLEX_BTN_PRESS_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_DOUBLE_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_REPEAT_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_SHORT_START),
    ENUM_TO_STR(FLEX_BTN_PRESS_SHORT_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_START),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_HOLD),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_HOLD_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_MAX),
    ENUM_TO_STR(FLEX_BTN_PRESS_NONE),
};

static char *enum_btn_id_string[] = {
    ENUM_TO_STR(USER_BUTTON_0),
    ENUM_TO_STR(USER_BUTTON_1),
    ENUM_TO_STR(USER_BUTTON_2),
    ENUM_TO_STR(USER_BUTTON_3),
    ENUM_TO_STR(USER_BUTTON_MAX),
};

static flex_button_t user_button[USER_BUTTON_MAX];

static uint8_t common_btn_read(void *arg)
{
    uint8_t value = 0;

    flex_button_t *btn = (flex_button_t *)arg;

    switch (btn->id)
    {
    case USER_BUTTON_0:

        return rt_pin_read(BUTTON_0);
        break;

    default:
        RT_ASSERT(0);
    }

    return value;
}

/*事件回调*/
static void common_btn_evt_cb(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;

    LOG_D("id: [%d - %s]  event: [%d - %30s]  repeat: %d\n",
                btn->id, enum_btn_id_string[btn->id], btn->event,
                enum_event_string[btn->event], btn->click_cnt);

    //也可以使用组合键
    if (flex_button_event_read(&user_button[USER_BUTTON_0]) == FLEX_BTN_PRESS_CLICK)//单击
    {
        Send_Laser_Alarm_Event(kSelfInspectionEvents);
    }
    if (flex_button_event_read(&user_button[USER_BUTTON_0]) == FLEX_BTN_PRESS_DOUBLE_CLICK)//双击
    {

        Send_Laser_Alarm_Event(kFlexBtnPressDoubleClick);
    }
    if (flex_button_event_read(&user_button[USER_BUTTON_0]) == FLEX_BTN_PRESS_SHORT_START)//短按开始
    {

        Send_Laser_Alarm_Event(kFlexBtnPressShoartStart);
    }
}

static void BSP_Button_thread(void *parameter)
{
    log_i("button_scan_thread\r\n");
    while(1)
    {
        flex_button_scan();
        rt_thread_mdelay(20); // 20 ms
    }
}

static void BSP_Button_Init(void)
{
    int i;
    
    rt_memset(&user_button[0], 0x0, sizeof(user_button));

    rt_pin_mode(BUTTON_0, PIN_MODE_INPUT_PULLUP); /* set KEY pin mode to input */

    for (i = 0; i < USER_BUTTON_MAX; i ++)
    {
        user_button[i].id = i;
        user_button[i].usr_button_read = common_btn_read;
        user_button[i].cb = common_btn_evt_cb;
        user_button[i].pressed_logic_level = BUTTON_LOGIC_LEVEL;
        user_button[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(1500);
        user_button[i].long_press_start_tick = FLEX_MS_TO_SCAN_CNT(3000);
        user_button[i].long_hold_start_tick = FLEX_MS_TO_SCAN_CNT(4500);

        flex_button_register(&user_button[i]);
    }
}                            /**< button_thread 线程控制块 */

int flex_button_main(void)
{
//    rt_thread_t button = RT_NULL;
    rt_err_t ret = RT_ERROR;
    BSP_Button_Init();

    /* Create background ticks thread */
//    button = rt_thread_create("button_0", BSP_Button_thread, RT_NULL,
//                            BUTTON_THREAD_STACKSIZE, BUTTON_THREAD_PRIORITY, BUTTON_THREAD_TICKS);

    ret = rt_thread_init(&button_0,
                       "button_0",
                       BSP_Button_thread,
                       RT_NULL,
                       &button_thread_stack[0],
                       sizeof(button_thread_stack),
                       BUTTON_THREAD_PRIORITY,
                       BUTTON_THREAD_TICKS);

    if(ret == RT_EOK)
    {
        rt_thread_startup(&button_0);
        return RT_EOK;
    }
    else
    {
        LOG_E("button_thread create failed...\r\n");
        return RT_ERROR;
    }

    return RT_ERROR;
}
#ifdef FINSH_USING_MSH
INIT_PREV_EXPORT(flex_button_main);
//MSH_CMD_EXPORT(flex_button_main, RT_NULL);
#endif
