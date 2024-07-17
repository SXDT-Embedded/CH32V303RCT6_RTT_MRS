#include "bsp_led.h"
#include "rtthread.h"
#include "pin.h"

#define LOG_TAG    "bsp_led"
#define  LOG_LVL    LOG_LVL_DBG
#include <ulog.h>


agile_led_t *led_r = RT_NULL;
agile_led_t *led_g = RT_NULL;
agile_led_t *led_y = RT_NULL;
agile_led_t *emv = RT_NULL;//µç´Å·§
agile_led_t *fj = RT_NULL;//·ç»ú

int BSP_Led_Init(void)
{
    led_r = agile_led_create(RED_LED_PIN, PIN_LOW, "100,100", 0);
    led_g = agile_led_create(GREEN_LED_PIN, PIN_LOW, "100,100", 0);
    led_y = agile_led_create(YELLOW_LED_PIN, PIN_LOW, "100,100", 0);

    emv = agile_led_create(EMV_LED_PIN, PIN_HIGH, "100,100", 0);
    fj    = agile_led_create(FJ_LED_PIN, PIN_HIGH, "100,100", 0);

    return RT_EOK;
}
INIT_DEVICE_EXPORT(BSP_Led_Init);






