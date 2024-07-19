#include "bsp_rtc.h"

#define LOG_TAG     "bsp_rtc"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>

#define RTC_FIRST_INIT_VALUE        (0x1314)

TsRtcDateTime RtcDateTime;

rt_uint8_t const table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
const rt_uint8_t mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

char week[7][10] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

void RTC_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));



rt_uint8_t RTC_Get_Week(rt_uint16_t year, rt_uint8_t month, rt_uint8_t day)
{
    rt_uint16_t temp2;
    rt_uint8_t  yearH, yearL;

    yearH = year / 100;
    yearL = year % 100;
    if(yearH > 19)
        yearL += 100;
    temp2 = yearL + yearL / 4;
    temp2 = temp2 % 7;
    temp2 = temp2 + day + table_week[month - 1];
    if(yearL % 4 == 0 && month < 3)
        temp2--;
    return (temp2 % 7);
}

rt_uint8_t Is_Leap_Year(u16 year)
{
    if(year % 4 == 0)
    {
        if(year % 100 == 0)
        {
            if(year % 400 == 0)
                return 1;
            else
                return 0;
        }
        else
            return 1;
    }
    else
        return 0;
}



rt_uint32_t RTC_Get(void)
{
    static rt_uint16_t daycnt = 0;
    rt_uint32_t        timecount = 0;
    rt_uint32_t        temp = 0;
    rt_uint16_t        temp1 = 0;
    timecount = RTC_GetCounter();
    temp = timecount / 86400;

    if(daycnt != temp)
    {
        daycnt = temp;
        temp1 = 1970;
        while(temp >= 365)
        {
            if(Is_Leap_Year(temp1))
            {
                if(temp >= 366)
                {
                    temp -= 366;
                }
                else
                {
                    temp1++;
                    break;
                }
            }
            else
            {
                temp -= 365;
            }

            temp1++;
        }
        RtcDateTime.year = temp1;
        temp1 = 0;
        while(temp >= 28)
        {
            if(Is_Leap_Year(RtcDateTime.year) && temp1 == 1)
            {
                if(temp >= 29)
                    temp -= 29;
                else
                    break;
            }
            else
            {
                if(temp >= mon_table[temp1])
                    temp -= mon_table[temp1];
                else
                    break;
            }
            temp1++;
        }
        RtcDateTime.month = temp1 + 1;
        RtcDateTime.day = temp + 1;

    }
    temp = timecount % 86400;
    RtcDateTime.hour = temp / 3600;
    RtcDateTime.minute = (temp % 3600) / 60;
    RtcDateTime.second = (temp % 3600) % 60;
    RtcDateTime.rtc_wday = RTC_Get_Week(RtcDateTime.year, RtcDateTime.month, RtcDateTime.day);
    RtcDateTime.week = (RtcDateTime.rtc_wday - 1);
    return timecount;
}

rt_uint8_t RTC_Set(rt_uint16_t syear, rt_uint8_t smon, rt_uint8_t sday, rt_uint8_t hour, rt_uint8_t min, rt_uint8_t sec)
{
    rt_uint16_t t;
    rt_uint32_t seccount = 0;
    if(syear < 1970 || syear > 2099)
        return 1;
    for(t = 1970; t < syear; t++)
    {
        if(Is_Leap_Year(t))
            seccount += 31622400;
        else
            seccount += 31536000;
    }
    smon -= 1;
    for(t = 0; t < smon; t++)
    {
        seccount += (rt_uint32_t)mon_table[t] * 86400;
        if(Is_Leap_Year(syear) && t == 1)
            seccount += 86400;
    }
    seccount += (rt_uint32_t)(sday - 1) * 86400;
    seccount += (rt_uint32_t)hour * 3600;
    seccount += (rt_uint32_t)min * 60;
    seccount += sec;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    RTC_SetCounter(seccount);
    RTC_WaitForLastTask();

    return 0;
}
#include "stdlib.h"

void BSP_Rtc_Set(int argc, char **argv)
{
    if (argc == 7)
    {
        /*年 月 日 时 分 秒*/
        RTC_Set((rt_uint16_t)(atoi(argv[1])), (rt_uint8_t)(atoi(argv[2])), (rt_uint8_t)(atoi(argv[3])),
                (rt_uint8_t)(atoi(argv[4])), (rt_uint8_t)(atoi(argv[5])), (rt_uint8_t)(atoi(argv[6])));
    }
}
MSH_CMD_EXPORT(BSP_Rtc_Set, "y m d h m s");


void BSP_Get_RTC_Time(void)
{
    RTC_Get();
    LOG_I("RTC Timer : %04d-%02d-%02d  %02d:%02d:%02d, %s\r\n", RtcDateTime.year, RtcDateTime.month, RtcDateTime.day,
            RtcDateTime.hour, RtcDateTime.minute, RtcDateTime.second, &week[RtcDateTime.week][0]);
}
MSH_CMD_EXPORT(BSP_Get_RTC_Time, "get rtc timer");

static void RTC_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;        //RTC全局中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级1位,从优先级3位
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;    //先占优先级0位,从优先级4位
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;       //使能该通道中断
  NVIC_Init(&NVIC_InitStructure);       //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}


static uint8_t RTC_First_Init(void)
{
    uint8_t temp = 0;

    RCC_LSEConfig(RCC_LSE_ON);
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && temp < 250)
    {
        temp++;
        rt_thread_mdelay(5);
    }
    if(temp >= 250)
        return 1;

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForLastTask();
    RTC_WaitForSynchro();
    RTC_WaitForLastTask();
    RTC_SetPrescaler(32767);
    RTC_WaitForLastTask();


//    RTC_Set(2000, 1, 1, 0, 0, 0);  //设置出厂日期
//    RTC_WaitForLastTask();
//
//    RTC_SetAlarm(20 + RTC_GetCounter());    //闹钟值设定为当前时间的10秒后
//    RTC_WaitForLastTask();
//
//    RTC_ExitConfigMode(); //退出配置模式


    // RTC_ITConfig(RTC_IT_ALR, ENABLE);
    // RTC_ITConfig(RTC_IT_SEC, ENABLE);
    // RTC_WaitForLastTask();

    /* Is it the first configuration */
    BKP_WriteBackupRegister(BKP_DR1, RTC_FIRST_INIT_VALUE);

    return 0;
}


int BSP_RTC_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    LOG_D("RCC_GetFlagStatus(RCC_FLAG_LSERDY): %d", RCC_GetFlagStatus(RCC_FLAG_LSERDY));
    if(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET || BKP_ReadBackupRegister(BKP_DR1) != RTC_FIRST_INIT_VALUE)
    {
        RTC_First_Init();
    }
    else
    {
        // 外部手动复位
        if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
            LOG_D("外部手动复位");
        }
        // 上电/掉电复位标志
        else if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
            LOG_D("上电/掉电复位");
//            Send_Laser_Alarm_Event(kPowerOnEvent);
        }
        // 软件复位
        else if(RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)
        {
            LOG_D("软件复位");
        }
        // 独立看门狗复位
        else if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
        {
            LOG_D("独立看门狗复位");
        }
        // 窗口看门狗复位
        else if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
        {
            LOG_D("窗口看门狗复位");
        }
        // 低功耗复位
        else if(RCC_GetFlagStatus(RCC_FLAG_LPWRRST) != RESET)
        {
            LOG_D("低功耗复位");
        }
//         RCC_ClearFlag();

        // 虽然RTC模块不需要重新配置，且掉电后依靠后备电池依然运行
        // 但是每次上电后，还是要使能RTCCLK
        RCC_RTCCLKCmd(ENABLE);  // 使能RTCCLK
        RTC_WaitForSynchro();   // 等待RTC时钟与APB1时钟同步
        // RTC_ITConfig(RTC_IT_ALR, ENABLE);
        // RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }

    RTC_NVIC_Config();
    BSP_Get_RTC_Time();
    return 0;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_PREV_EXPORT(BSP_RTC_Init);
#endif



void RTC_IRQHandler(void)
{
    GET_INT_SP();
   /* enter interrupt */
   rt_interrupt_enter();

    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//秒钟中断
     {
        RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);       //清秒中断
         RTC_Get();   //更新时间
         RTC_WaitForLastTask();  //等待写入完成
     }

    if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//闹钟中断
     {
        RTC_ClearITPendingBit(RTC_IT_ALR);      //清闹钟中断
        RTC_Get();      //更新时间
        RTC_WaitForLastTask();
     }

    rt_interrupt_leave();
    FREE_INT_SP();
}

