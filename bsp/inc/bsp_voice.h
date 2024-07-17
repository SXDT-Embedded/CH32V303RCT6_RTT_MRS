#ifndef __BSP_VOICE_H__
#define __BSP_VOICE_H__



#include "board.h"
#include "pin.h"
#include "drv_gpio.h"
#include "ch32v30x_gpio.h"



#define     VOICE_CLK_PIN     GET_PIN(C,9)
#define     VOICE_DATA_PIN    GET_PIN(A,8)
#define     VOICE_BUSY_PIN    GET_PIN(C,8)


#define     VOICE_CLK_HIGH()            rt_pin_write(VOICE_CLK_PIN, PIN_HIGH)
#define     VOICE_CLK_LOW()             rt_pin_write(VOICE_CLK_PIN, PIN_LOW)

#define     VOICE_DATA_HIGH()           rt_pin_write(VOICE_DATA_PIN, PIN_HIGH)
#define     VOICE_DATA_LOW()            rt_pin_write(VOICE_DATA_PIN, PIN_LOW)

#define VOICE_DATA_SET(x)               rt_pin_write(VOICE_DATA_PIN, x)

/*BUSY位高电平时表示正在处理语音数据，低电平时表示处理结束，可用于判断音频数据是否处理完毕*/
#define     VOICE_BUSY_GET()            rt_pin_read(VOICE_BUSY_PIN)

#define  VOICE_CMD_STOP_PLAY             (0XFEU)//停止播放音频

//这里的顺序要和bin文件的保持一致，否则会出错
typedef enum
{
    /***********bin文件语音顺序*************/
    kBuzzerVoice,               //报警音频
    kConnectNetWorkVoice,       //连接网络音频
    kConnectNetWorkSuccessVoice,//网络连接成功音频
    kDisConnectNetWorkVoice,    //断开网络连接音频
    kLowConcentrationVoice,     //低浓度报警音频
    kHighConcentrationVoice,    //高浓度报警音频
    /***********************************/
    kVoiceStop,//关闭播放
    kVoiceMaxEventcnt,
}TePlayVoiceEvent;

// 音频播放控制的结构体
typedef struct
{
    TePlayVoiceEvent audio_data;//音频
    uint64_t start_time;//开始时间
    int32_t play_duration;//持续时间
    rt_bool_t is_playing;//是否正在播放

} TsAudioPlayControl;

extern TsAudioPlayControl audio_play_control;

void AudioPlayControl_Ctrl(TsAudioPlayControl *ctrl,//结构体
                            rt_uint8_t audio_data,//播放的音频
                            rt_int32_t play_duration_ms);//播放的音频时长
void Send_Voice_Event(TePlayVoiceEvent event_type);










#endif //!__BSP_VOICE_H__


