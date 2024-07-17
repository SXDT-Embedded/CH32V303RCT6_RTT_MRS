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
#include "user_sys.h"
#include "bsp_flash.h"
#include "rtdef.h"
#include "bsp_voice.h"




/* Global typedef */

/* Global define */
#define LOG_TAG "main"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>


#define ALARM_THREAD_PRIORITY      20
#define ALARM_THREAD_TIMESLICE     50

#define LINKAGE_DEVICE_THREAD_PRIORITY      19
#define LINKAGE_DEVICE_THREAD_TIMESLICE     10


#define RT_ALARM_THREAD_STACK_SIZE     (2048)
#define RT_LINKDEV_THREAD_STACK_SIZE   (512)

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t alarm_thread_stack[RT_ALARM_THREAD_STACK_SIZE];
static struct rt_thread alarm_thread;

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t linkage_device_thread_stack[RT_LINKDEV_THREAD_STACK_SIZE];
static struct rt_thread linkage_device_thread;


struct rt_semaphore fj_sem;
struct rt_semaphore emv_sem;

/* Global Variable */
static rt_uint32_t event_flags[kMaxEventcnt] = {0};


/*报警器事件集控制块*/
struct rt_event alarm_event;
static struct rt_timer warm_up_timer;

static rt_bool_t is_event_initialized = RT_FALSE;//是否初始化完成

typedef struct
{
    AlarmEvent current_event;//当前事件
    AlarmEvent last_event;//上次事件
}EventIndex;

EventIndex event_index;
/*********************************************************************
 * @fn      main
 *
 * @brief   main is just one of the threads, in addition to tshell,idle
 * This main is just a flashing LED, the main thread is registered in 
 * rtthread_startup, tshell uses the serial port to receive interrupts, 
 * and the interrupt stack and thread stack are used separately.
 *
 * @return  none
 */
int main(void)
{
    SystemCoreClockUpdate();
    LOG_I(" MCU: CH32V303\r\n");
    LOG_I(" SysClk: %dHz\r\n",SystemCoreClock);
    LOG_I(" ChipID: %08x\r\n",DBGMCU_GetCHIPID());

//	while(1)
//	{
//
//	    rt_thread_mdelay(5000);
//	}
}

static void Set_Event(AlarmEvent event_type)
{
    if (event_type < kMaxEventcnt)
    {
        event_flags[event_type] = (1 << event_type);
    }
}


void Alarm_Init_event(void)
{
    if (!is_event_initialized)
    {
        rt_event_init(&alarm_event, "alarm_event", RT_IPC_FLAG_FIFO);
        is_event_initialized = RT_TRUE;

        for (AlarmEvent event = kPowerOnEvent; event < kMaxEventcnt; event++)
       {
           Set_Event(event);
       }
    }
}

rt_uint32_t Get_Sys_Event_Flag(AlarmEvent event_type)
{
    if (event_type < kMaxEventcnt)
    {
        return event_flags[event_type];
    }
    return 0;
}

static void Warm_Up_Timer_Timeout_Cb(void *parameter)
{
    LOG_D("预热完成\n");
    AudioPlayControl_Ctrl(&audio_play_control, kBuzzerVoice, 500);//播放蜂鸣器前要先设置参数，才能播放，否则可能会出现问题
    Send_Voice_Event(kBuzzerVoice);
    Send_Laser_Alarm_Event(kNormalDetectionEvents);

}

static rt_bool_t Is_Event_Init_Func(void)
{
    return is_event_initialized;
}


void Send_Laser_Alarm_Event(AlarmEvent event_type)
{
    if (Is_Event_Init_Func())
    {
        rt_event_send(&alarm_event, Get_Sys_Event_Flag(event_type));
    }
    else
    {
        rt_kprintf("Alarm event is not initialized!\n");
    }
}

void _Self_Check_Mode(void)
{
    LED_STOP(r);
    LED_STOP(g);
    LED_STOP(y);

    LED_CTRL(r, "500,1000", 6);
    LED_START(r);
    rt_thread_mdelay(500);

    LED_CTRL(g, "500,1000", 6);
    LED_START(g);
    rt_thread_mdelay(500);

    LED_CTRL(y, "500,1000", 6);
    LED_START(y);
    rt_thread_mdelay(500 + 4500);

    // 第6s开始蜂鸣器叫
    AudioPlayControl_Ctrl(&audio_play_control, kBuzzerVoice, -1);//播放蜂鸣器前要先设置参数，才能播放，否则可能会出现问题
    Send_Voice_Event(kBuzzerVoice);
    rt_thread_mdelay(3000);

    // 第9s关闭电磁阀
    DEV_CTRL(emv, "500,500", 1);
    DEV_CTRL(fj, "500,500", 1);
    rt_thread_mdelay(2000);

}
static void Alarm_Thread_Recv_Event(void *param)
{
    rt_err_t result;
    rt_uint32_t received_event;

    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
        LOG_D("上电/掉电复位");
        RCC_ClearFlag();
        Send_Laser_Alarm_Event(kPowerOnEvent);
    }
    else
    {
        Send_Laser_Alarm_Event(kPreheatingEvent);
    }
    while(1)
       {
            result = rt_event_recv(&alarm_event,
                                    Get_Sys_Event_Flag(kPowerOnEvent)|          //上电
                                    Get_Sys_Event_Flag(kPreheatingEvent)|       //预热
                                    Get_Sys_Event_Flag(kNormalDetectionEvents)| //正常检测
                                    Get_Sys_Event_Flag(kAlarmEvent)|            //报警
                                    Get_Sys_Event_Flag(kAlarmRcyEvent)|         //报警恢复
                                    Get_Sys_Event_Flag(kSelfInspectionEvents)|  //自检
                                    Get_Sys_Event_Flag(kFaultEvent)|            //故障模式
                                    Get_Sys_Event_Flag(kFaultRcyEvent),         //故障恢复
                                    RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                                    RT_WAITING_FOREVER, &received_event);

           if (result == RT_EOK)
           {
               if (received_event & Get_Sys_Event_Flag(kPowerOnEvent))//上电
               {
                   LOG_D("上电模式\n");

                   event_index.current_event = kPowerOnEvent;

                   Flash_Write_Record(kRecordPowerOn);
                   Send_Laser_Alarm_Event(kPreheatingEvent);
               }
               else if (received_event & Get_Sys_Event_Flag(kPreheatingEvent))//预热
               {
                   LOG_D("预热模式\n");

                   event_index.last_event = event_index.current_event;
                   event_index.current_event = kPreheatingEvent;

                   GREEN_LED_PREAT;

                   rt_timer_init(&warm_up_timer,
                                 "warm_up",
                                 Warm_Up_Timer_Timeout_Cb,
                                 RT_NULL,
                                 WARM_UP_TIMER_TICKS,
                                 RT_TIMER_FLAG_ONE_SHOT);

                   if (&warm_up_timer != RT_NULL)
                   {
                       LOG_D("warm_up_timer 5s start\n");
                       rt_timer_start(&warm_up_timer);
                   }
                   else
                   {
                       LOG_E("warm_up_timer create Failed...\n");
                   }
               }
               else if (received_event & Get_Sys_Event_Flag(kNormalDetectionEvents))//正常检测
               {
                   LOG_D("正常检测模式\n");

                   event_index.last_event = event_index.current_event;
                   event_index.current_event = kNormalDetectionEvents;

                   GREEN_LED_NORMAL;

                   if (event_index.last_event == kPreheatingEvent)//如果是从预热到正常检测模式，则等个1秒钟，让蜂鸣器叫一下再关闭
                   {
                       rt_thread_delay(1000);
                   }
                   Send_Voice_Event(kVoiceStop);
               }
               else if (received_event & Get_Sys_Event_Flag(kAlarmEvent))//报警
               {
                   LOG_D("报警模式\n");

                   event_index.last_event = event_index.current_event;
                   event_index.current_event = kAlarmEvent;

                   Send_Voice_Event(kLowConcentrationVoice);

                   RED_LED_ALAEM;

                   rt_sem_release(&emv_sem);//电磁阀动作
                   rt_sem_release(&fj_sem);//风机动作

                   Flash_Write_Record(kRecordAlarm);//写入flash报警信息

                   rt_thread_delay(8000);//等待语音播报完成，再蜂鸣器响所以这个延时时间一定要大于等于语音播放时间，不然会播放中断，播放下一条

                   AudioPlayControl_Ctrl(&audio_play_control, kBuzzerVoice, -1);//播放蜂鸣器前要先设置参数，才能播放，否则可能会出现问题
                   Send_Voice_Event(kBuzzerVoice);//播放蜂鸣器

               }
               else if (received_event & Get_Sys_Event_Flag(kAlarmRcyEvent))//报警恢复
               {
                   LOG_D("报警恢复模式\n");

                   event_index.last_event = event_index.current_event;
                   event_index.current_event = kAlarmRcyEvent;

                   Flash_Write_Record(kRecordAlarmRcy);
                   Send_Laser_Alarm_Event(kNormalDetectionEvents);
               }
               else if (received_event & Get_Sys_Event_Flag(kFaultEvent))//故障
               {
                   LOG_D("故障模式\n");

                   event_index.last_event = event_index.current_event;
                   event_index.current_event = kFaultEvent;

                   YELLOW_LED_FAULT;

                   Flash_Write_Record(kRecordFault);

               }
               else if (received_event & Get_Sys_Event_Flag(kFaultRcyEvent))//故障恢复
               {
                   LOG_D("故障恢复模式\n");

                   event_index.last_event = event_index.current_event;
                   event_index.current_event = kFaultRcyEvent;

                   Flash_Write_Record(kRecordFaultRcy);

                   Send_Laser_Alarm_Event(kNormalDetectionEvents);
               }
               else if (received_event & Get_Sys_Event_Flag(kPowerDownEvent))//掉电
               {
                   LOG_D("掉电模式\n");

                   event_index.last_event = event_index.current_event;
                   event_index.current_event = kPowerDownEvent;

//                    写入掉电记录
                   Flash_Write_Record(kRecordPowerDown);
                   LOG_E("Enter AnalogWatchdog Interrupt");

                   for (rt_uint16_t i = 0; i < 65535; i++)
                   {
                       rt_thread_mdelay(1);
                       LOG_D("i = %d", i);
                   }
                   rt_thread_mdelay(1000);
                   // 还是没断电的话就重启
                   NVIC_SystemReset();
               }
               else if (received_event & Get_Sys_Event_Flag(kSelfInspectionEvents))//自检
               {
                   LOG_D("自检模式\n");

                   event_index.last_event = event_index.current_event;
                   event_index.current_event = kSelfInspectionEvents;

                   _Self_Check_Mode();
                   Send_Laser_Alarm_Event(event_index.last_event);

               }
               else if (received_event & Get_Sys_Event_Flag(kSensorFailureEvent))//失效
               {
                   LOG_D("传感器失效模式\n");

                   event_index.last_event = event_index.current_event;
                   event_index.current_event = kSensorFailureEvent;

                   Flash_Write_Record(kRecordSensoEndOfLife);
               }
           }
       }
}


int Alarm_Handle(void)
{
    Alarm_Init_event();

    rt_thread_init(&alarm_thread,
                  "alarm_thread",
                  Alarm_Thread_Recv_Event,
                  RT_NULL,
                  &alarm_thread_stack[0],
                  sizeof(alarm_thread_stack),
                  ALARM_THREAD_PRIORITY, ALARM_THREAD_TIMESLICE);
    if (&alarm_thread != RT_NULL)
    {
        rt_thread_startup(&alarm_thread);
    }
    else
    {
        LOG_E("alarm_thread init failed...\n");
    }

    return RT_EOK;
}
INIT_APP_EXPORT(Alarm_Handle);

static void Alarm_Linkage_Device_Thread_Entry(void *param)
{
    while(1)
    {
        if(rt_sem_trytake(&emv_sem) == RT_EOK)
        {
            if (event_index.current_event == kAlarmEvent)
            {
                DEV_CTRL(emv, "500,500", 1);
                LOG_I("电磁阀关闭\n");
            }
        }
        if (rt_sem_trytake(&fj_sem) == RT_EOK)
        {
            if (event_index.current_event == kAlarmEvent)
            {
                DEV_CTRL(fj, "500,500", 1);
                LOG_I("风机打开\n");
            }
        }

        rt_thread_mdelay(10);
    }
}


int Alarm_Linkage_Device_Thread(void)
{
    rt_sem_init(&emv_sem, "emv_sem", 0, RT_IPC_FLAG_FIFO);
    if (&emv_sem == RT_NULL)
    {
         rt_kprintf("create emv_sem semaphore failed.\n");

         return -1;
    }
     rt_sem_init(&fj_sem, "fj_sem", 0, RT_IPC_FLAG_FIFO);
    if (&fj_sem == RT_NULL)
    {
        rt_kprintf("create fj_sem semaphore failed.\n");

        return -1;
    }
    rt_thread_init(&linkage_device_thread,
                  "link_eque",
                  Alarm_Linkage_Device_Thread_Entry,
                  RT_NULL,
                  &linkage_device_thread_stack[0],
                  sizeof(linkage_device_thread_stack),
                  LINKAGE_DEVICE_THREAD_PRIORITY, LINKAGE_DEVICE_THREAD_TIMESLICE);
    if (&linkage_device_thread != RT_NULL)
    {
        rt_thread_startup(&linkage_device_thread);
    }
    else
    {
        LOG_E("Alarm_Linkage_Device_Thread_Entry init failed...\n");
    }

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(Alarm_Linkage_Device_Thread);

