#include "rtthread.h"
#include "bsp_wdg.h"



#define LOG_TAG     "bsp_wdg"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>


ALIGN(RT_ALIGN_SIZE)
static char wdg_thread_stack[WDG_THREAD_STACK_SIZE];
static struct rt_thread wdg_thread;

#if USED_WWDG
//  This routine demonstrates that the watchdog is fed between the window 0x40(64) and 0x7F(127) to
//  prevent the watchdog from being reset.
#define WWDG_CNT    127
uint8_t wwdg_tr, wwdg_wr;
void WWDG_IRQHandler(void) __attribute__((interrupt("")));
#endif

#if USED_IWDG
/*************************************IWDG**********************************************/
/**
 * @description: Initializes IWDG.
 * @param {uint16_t} prescaler   specifies the IWDG Prescaler value.
 *            IWDG_Prescaler_4: IWDG prescaler set to 4.
 *            IWDG_Prescaler_8: IWDG prescaler set to 8.
 *            IWDG_Prescaler_16: IWDG prescaler set to 16.
 *            IWDG_Prescaler_32: IWDG prescaler set to 32.
 *            IWDG_Prescaler_64: IWDG prescaler set to 64.
 *            IWDG_Prescaler_128: IWDG prescaler set to 128.
 *            IWDG_Prescaler_256: IWDG prescaler set to 256.
 * @param {uint16_t} reload      specifies the IWDG Reload value.
 *             This parameter must be a number between 0 and 0x0FFF(4095).
 * @return {*}
 */
void IWDG_Init(uint16_t prescaler, uint16_t reload)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(prescaler);
    IWDG_SetReload(reload);
    IWDG_ReloadCounter();
    IWDG_Enable();

    LOG_I("IWDG_Feed_Init");
}

inline void IWDG_Feed(void)
{
    // LOG_D("IWDG_Feed!");
    IWDG_ReloadCounter();   //Feed dog
}

#elif USED_WWDG
/*************************************WWDG**********************************************/
/*********************************************************************
 * @fn      WWDG_NVIC_Config
 *
 * @brief   WWDG INT init.
 *
 * @return  none
 */
static void WWDG_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @description:
 * @param {uint32_t} prv 设置预分频器值
 *          WWDG_Prescaler - specifies the WWDG Prescaler
 *              WWDG_Prescaler_1 - WWDG counter clock = (PCLK1/4096)/1
 *              WWDG_Prescaler_2 - WWDG counter clock = (PCLK1/4096)/2
 *              WWDG_Prescaler_4 - WWDG counter clock = (PCLK1/4096)/4
 *              WWDG_Prescaler_8 - WWDG counter clock = (PCLK1/4096)/8
 * @param {uint8_t} wr 设置上窗口值
 *          WindowValue - specifies the window value to be compared to the
 *              downcounter,which must be lower than 0x80
 * @param {uint8_t} tr 设置计数器的值
 *          Counter - specifies the watchdog counter value,which must be a
 *              number between 0x40(64) and 0x7F(127)
 * @return {*}
 */
void WWDG_Init(uint32_t prv, uint8_t wr, uint8_t tr)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

    WWDG_SetPrescaler(prv);
    WWDG_SetWindowValue(wr);
    WWDG_SetCounter(tr);

    WWDG_Enable(WWDG_CNT);
    WWDG_ClearFlag();

    WWDG_NVIC_Config();
    WWDG_EnableIT();

    wwdg_wr = WWDG->CFGR & 0x7F;

    LOG_D("WWDG_Feed_Init");
}

/*********************************************************************
 * @fn      WWDG_Feed
 *
 * @brief   Feed WWDG.
 *
 * @return  none
 */
void WWDG_Feed(void)
{
    WWDG_SetCounter(WWDG_CNT);
    // LOG_D("WWDG_Feed");
}

void WWDG_Loop(void)
{
    wwdg_tr = WWDG->CTLR & 0x7F;
    if(wwdg_tr < wwdg_wr)
    {
        WWDG_Feed();
    }
}
#endif

static void wdg_thread_entry(void *param)
{
    while (1)
    {
    #if USED_IWDG
        IWDG_Feed();
        rt_thread_mdelay(IWDG_FEED_INTERVAL_MS);
    #elif USED_WWDG
        WWDG_Loop();
    #endif
    }
}

int BSP_WDG_Init(void)
{
#if USED_IWDG
    // t = 4000 * （IWDG计数时基 = LSI / 分频系数 = 40 KHz / 32 = 1250 Hz）= 3.2 s
    IWDG_Init(IWDG_Prescaler_32, 4000);    // 3.2s IWDG reset
#elif USED_WWDG
    // WWDG counter clock = (PCLK1 / 4096) / 8 = (SystemCoreClock(96MHz) / 2) / 4096 / 8 ~= 1464 Hz
    WWDG_Init(WWDG_Prescaler_8, 80, WWDG_CNT);
#endif
    // /* 设置空闲线程钩子函数 */
    // return rt_thread_idle_sethook(BSP_WDG_Loop);
    if (RT_EOK == rt_thread_init(&wdg_thread,
                   "wdg_thread",
                   wdg_thread_entry,
                   RT_NULL,
                   &wdg_thread_stack[0],
                   sizeof(wdg_thread_stack),
                   WDG_THREAD_PRIORITY, WDG_THREAD_TIMESLICE))
    {
        rt_thread_startup(&wdg_thread);
    }

    return 0;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_PREV_EXPORT(BSP_WDG_Init);
#endif

#if USED_WWDG
/*********************************************************************
 * @fn      WWDG_IRQHandler
 *
 * @brief   This function handles WWDG exception.
 *
 * @return  none
 */
void WWDG_IRQHandler(void)
{
    GET_INT_SP();
    /* enter interrupt */
    rt_interrupt_enter();

    WWDG_ClearFlag();
    // WWDG_SetCounter(WWDG_CNT);

    #error "此处加入在复位前需要处理的工作或保存数据"

    /* leave interrupt */
    rt_interrupt_leave();
    FREE_INT_SP();
}
#endif // !USED_WWDG



