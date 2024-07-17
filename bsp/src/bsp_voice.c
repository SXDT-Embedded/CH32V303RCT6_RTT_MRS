#include "bsp_voice.h"
#include "rtthread.h"
#include "rtdef.h"
#include "stdlib.h"
#include "debug.h"

#define LOG_TAG     "bsp_voice"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面


#define VOICE_THREAD_PRIORITY      24
#define VOICE_THREAD_TICKS         50
#define VOICE_THREAD_STACK_SIZE    1024
static struct rt_thread voice_thread;
static rt_uint8_t voice_thread_stack[VOICE_THREAD_STACK_SIZE];

static struct rt_thread voice_play_thread;
static rt_uint8_t voice_play_thread_thread_stack[VOICE_THREAD_STACK_SIZE];

//static rt_sem_t  voice_sem;
static struct rt_timer  Voice_Timer;
static rt_mutex_t voice_send_mux;
/* 邮箱控制块 */
static struct rt_mailbox voice_mb;
/* 用于放邮件的内存池 */
static char voice_mb_pool[128];
static char voice_stop_play_cmd[] = "stop";

/*报警器事件集控制块*/
struct rt_event voice_event;
static rt_uint32_t voice_event_flags[kVoiceMaxEventcnt] = {0};
static rt_bool_t is_event_initialized = RT_FALSE;//是否初始化完成

TsAudioPlayControl audio_play_control = {0};

static int BSP_Voice_Init(void)
{
    rt_pin_mode(VOICE_CLK_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(VOICE_DATA_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(VOICE_BUSY_PIN, PIN_MODE_INPUT);

    return RT_EOK;
}

/**
 * @description: 发送数据到2A-WT588F
 * @param {uint8_t} data
 * @return {*}
 */
void Line_2A_WT588F(rt_uint8_t data)
{
    rt_uint8_t value = 0;

    VOICE_CLK_HIGH();
    VOICE_DATA_HIGH();
    rt_thread_delay(5); // 5ms
    value = data;
    VOICE_CLK_LOW();
    rt_thread_delay(5); // 5ms
    for (rt_uint8_t j = 0; j < 8; j++)
    {
        VOICE_CLK_LOW(); // CLK
        VOICE_DATA_SET(value & 0X01);
        rt_thread_delay(1); //350us
        VOICE_CLK_HIGH(); // CLK
        rt_thread_delay(1);

        value >>= 1;
    }
    VOICE_CLK_LOW();
    VOICE_DATA_LOW();
    rt_thread_delay(2);
}

void BSP_Voice_Set_Volume(rt_uint8_t data)
{
    if ((data >= 0xE0) && (data <= 0xFE))
    {
        Line_2A_WT588F(data);
    }
}

void BSP_Voice_Stop_Play(rt_uint8_t data)
{
    if (data == VOICE_CMD_STOP_PLAY)
    {
        Line_2A_WT588F(VOICE_CMD_STOP_PLAY);
    }
}

void BSP_Play_Specified_Voice(rt_uint8_t data)
{
    if ((data >= 0x00) && (data <= 0xE0))
    {
        rt_mutex_take(voice_send_mux, 20);
        Line_2A_WT588F(data);
        rt_mutex_release(voice_send_mux);
    }
}

//循环播放当前音频，当第二次发送这个命令时，停止当前循环播放功能
void Loop_Playback(void)
{
    Line_2A_WT588F(0xF1);
    rt_thread_delay(5);
}


static void Voice_Timer_Cb(void *parameter)
{
    TsAudioPlayControl *ctrl = (TsAudioPlayControl *)parameter;

    if (ctrl->is_playing == RT_TRUE)
    {
          ctrl->is_playing = RT_FALSE;
          /* 发送 voice_stop_play_cmd 地址到邮箱中 */
          rt_mb_send(&voice_mb, (rt_uint32_t)&voice_stop_play_cmd);
    }
}

// 初始化音频播放控制结构体
void AudioPlayControl_Ctrl(TsAudioPlayControl *ctrl,//结构体
                            rt_uint8_t audio_data,//播放的音频
                            rt_int32_t play_duration_ms)//播放的音频时长
{
    ctrl->audio_data = audio_data;
    ctrl->play_duration = play_duration_ms;
    ctrl ->is_playing = RT_FALSE;
}

// 开始播放音频并启动定时器
void AudioPlayControl_Start(TsAudioPlayControl *ctrl)
{
    if (ctrl->audio_data >= 0x00 && ctrl->audio_data <= 0xE0)
    {
        BSP_Play_Specified_Voice(ctrl->audio_data);
        ctrl ->is_playing = RT_TRUE;

        if (ctrl->play_duration == -1)
        {
            Loop_Playback();//第一次发这个命令启动循环播放
        }
        else
        {
            rt_timer_init(&Voice_Timer, "Voice_Timer", Voice_Timer_Cb,  ctrl, ctrl->play_duration, RT_TIMER_FLAG_ONE_SHOT);
            /* 启动定时器 Voice_Timer */
            if (&Voice_Timer != RT_NULL)
            {
                rt_timer_start(&Voice_Timer);
            }
            else
            {
                LOG_E("Voice_Timer init failed...\n");
            }
        }
    }
}


void BSP_Play_Voice(TePlayVoiceEvent data)
{
    switch (data)
    {
    case kBuzzerVoice:
        AudioPlayControl_Start(&audio_play_control);//调用这个之前要先调用AudioPlayControl_Ctrl函数进行参数设置
        break;
    case kConnectNetWorkVoice:
        BSP_Play_Specified_Voice(kConnectNetWorkVoice);//
        break;
    case kConnectNetWorkSuccessVoice:
        BSP_Play_Specified_Voice(kConnectNetWorkSuccessVoice);//
        break;
    case kDisConnectNetWorkVoice:
        BSP_Play_Specified_Voice(kDisConnectNetWorkVoice);//
        break;
    case kLowConcentrationVoice:
        BSP_Play_Specified_Voice(kLowConcentrationVoice);//
        break;
    case kHighConcentrationVoice:
        BSP_Play_Specified_Voice(kHighConcentrationVoice);//
        break;
    case kVoiceStop:
        BSP_Voice_Stop_Play(VOICE_CMD_STOP_PLAY);
        if (&Voice_Timer != RT_NULL)//如果有启动定时器,则关闭它
            rt_timer_stop(&Voice_Timer);
        break;
    default:
        break;
    }
}

static rt_bool_t Is_Event_Init_Func(void)
{
    return is_event_initialized;
}


static rt_uint32_t Get_Vocie_Event_Flag(TePlayVoiceEvent event_type)
{
    if (event_type < kVoiceMaxEventcnt)
    {
        return voice_event_flags[event_type];
    }
    return 0;
}

void Send_Voice_Event(TePlayVoiceEvent event_type)
{
    if (Is_Event_Init_Func())
    {
        rt_event_send(&voice_event, Get_Vocie_Event_Flag(event_type));
    }
    else
    {
        rt_kprintf("Alarm event is not initialized!\n");
    }
}

static void Set_Event(TePlayVoiceEvent event_type)
{
    if (event_type < kVoiceMaxEventcnt)
    {
        voice_event_flags[event_type] = (1 << event_type);
    }
}

static void Voice_Init_Event(void)
{
    if (!is_event_initialized)
    {
        rt_event_init(&voice_event, "voice_event", RT_IPC_FLAG_FIFO);
        voice_send_mux = rt_mutex_create("voice_mux", RT_IPC_FLAG_PRIO);
        is_event_initialized = RT_TRUE;
//        voice_sem = rt_sem_create("voice_sem", 0, RT_IPC_FLAG_FIFO);
//        if (voice_sem == RT_NULL)
//        {
//            LOG_E("voice_sem create failed\n");
//        }
        for (TePlayVoiceEvent event = kBuzzerVoice; event < kVoiceMaxEventcnt; event++)
       {
           Set_Event(event);
       }
    }
}

void Voice_Thread_Entry(void *parameter)
{
    rt_uint32_t received_event;
    rt_err_t result;

    LOG_I("Voice_Thread_Entry\n");

    while(1)
    {
        result = rt_event_recv(&voice_event,
                                Get_Vocie_Event_Flag(kBuzzerVoice)|
                                Get_Vocie_Event_Flag(kVoiceStop)|
                                Get_Vocie_Event_Flag(kConnectNetWorkVoice)|
                                Get_Vocie_Event_Flag(kConnectNetWorkSuccessVoice)|
                                Get_Vocie_Event_Flag(kDisConnectNetWorkVoice)|
                                Get_Vocie_Event_Flag(kLowConcentrationVoice)|
                                Get_Vocie_Event_Flag(kHighConcentrationVoice),
                                RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                                RT_WAITING_FOREVER, &received_event);

       if (result == RT_EOK)
       {
          if (received_event & Get_Vocie_Event_Flag(kBuzzerVoice))//报警声
          {
               LOG_D("play alarm voice\n");
               BSP_Play_Voice(kBuzzerVoice);
          }
          else if (received_event & Get_Vocie_Event_Flag(kVoiceStop))
          {
             LOG_D("kVoiceStop\n");
             BSP_Play_Voice(kVoiceStop);
          }
          else if (received_event & Get_Vocie_Event_Flag(kConnectNetWorkVoice))
          {
               BSP_Play_Voice(kConnectNetWorkVoice);
          }
          else if (received_event & Get_Vocie_Event_Flag(kConnectNetWorkSuccessVoice))
          {
               BSP_Play_Voice(kConnectNetWorkSuccessVoice);
          }
          else if (received_event & Get_Vocie_Event_Flag(kDisConnectNetWorkVoice))
          {
               BSP_Play_Voice(kDisConnectNetWorkVoice);
          }
          else if (received_event & Get_Vocie_Event_Flag(kLowConcentrationVoice))
          {
               LOG_D("kLowConcentrationVoice\n");
               BSP_Play_Voice(kLowConcentrationVoice);
          }
          else if (received_event & Get_Vocie_Event_Flag(kHighConcentrationVoice))
          {
               BSP_Play_Voice(kHighConcentrationVoice);
          }
       }
    }
}

void Voice_Play_Thread_Entry(void *parameter)
{
   char *ptr = RT_NULL;

   while (1)
   {
       if (rt_mb_recv(&voice_mb, (rt_uint32_t *)&ptr, RT_WAITING_FOREVER) == RT_EOK)
       {
           if (ptr == "stop")//停止播放
           {
               LOG_D("voice_stop_play_cmd \n");
               Loop_Playback();//第二次发送停止当前循环播放功能
               BSP_Voice_Stop_Play(0XFE);
               break;
           }
           rt_thread_mdelay(100);
       }

   }
    /* 执行邮箱对象脱离 */
    rt_mb_detach(&voice_mb);
}

static int Rt_Voice_Thread_Init(void)
{
    rt_err_t result;
    BSP_Voice_Init();
    Voice_Init_Event();

    /* 初始化一个 mailbox */
    result = rt_mb_init(&voice_mb,
                       "voice_mb",                       /* 名称是voice_mb */
                       &voice_mb_pool[0],                /* 邮箱用到的内存池是 mb_pool */
                       sizeof(voice_mb_pool) / 4,        /* 邮箱中的邮件数目，因为一封邮件占 4 字节 */
                       RT_IPC_FLAG_FIFO);                /* 采用 FIFO 方式进行线程等待 */
    if (result != RT_EOK)
    {
       rt_kprintf("init mailbox failed.\n");
       return -1;
    }
    rt_thread_init(&voice_play_thread,
                    "voice_play",
                    Voice_Play_Thread_Entry,
                    RT_NULL,
                    &voice_play_thread_thread_stack[0],
                    sizeof(voice_play_thread_thread_stack),
                    VOICE_THREAD_PRIORITY, VOICE_THREAD_TICKS);

    rt_thread_startup(&voice_play_thread);

    rt_thread_init(&voice_thread,
                    "voice",
                    Voice_Thread_Entry,
                    RT_NULL,
                    &voice_thread_stack[0],
                    sizeof(voice_thread_stack),
                    VOICE_THREAD_PRIORITY,
                    VOICE_THREAD_TICKS);

    if (&voice_thread == RT_NULL)
    {
        LOG_E("Voice_Thread_Entry init failed\n");
        return RT_ENOMEM;
    }
    rt_thread_startup(&voice_thread);
    return RT_EOK;
}
INIT_DEVICE_EXPORT(Rt_Voice_Thread_Init);


#if 1
void TEST_Play_Voice(int argc, char **argv)
{
    if (argc == 3)
    {
        LOG_E("BSP_Play_Voice(%d, %d)",atoi(argv[2]), atoi(argv[2]));
        if (atoi(argv[1]) == 0)
        {
            AudioPlayControl_Ctrl(&audio_play_control, atoi(argv[1]), atoi(argv[2]));
        }
        BSP_Play_Voice((TePlayVoiceEvent)atoi(argv[1]));
    }
    else {
        LOG_E("value(0~5)");
    }
}
MSH_CMD_EXPORT(TEST_Play_Voice, data ms);

void TEST_Set_Volume(int argc, char **argv)
{
    if (argc == 2)
    {
        LOG_E("Set_Volume(%d)", atoi(argv[1]));

        BSP_Voice_Set_Volume(atoi(argv[1]));
    }
    else {
        LOG_E("0xE0!0xFE");
    }
}
MSH_CMD_EXPORT(TEST_Set_Volume, volume);

#endif




