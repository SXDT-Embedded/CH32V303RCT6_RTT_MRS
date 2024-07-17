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

/*BUSYλ�ߵ�ƽʱ��ʾ���ڴ����������ݣ��͵�ƽʱ��ʾ����������������ж���Ƶ�����Ƿ������*/
#define     VOICE_BUSY_GET()            rt_pin_read(VOICE_BUSY_PIN)

#define  VOICE_CMD_STOP_PLAY             (0XFEU)//ֹͣ������Ƶ

//�����˳��Ҫ��bin�ļ��ı���һ�£���������
typedef enum
{
    /***********bin�ļ�����˳��*************/
    kBuzzerVoice,               //������Ƶ
    kConnectNetWorkVoice,       //����������Ƶ
    kConnectNetWorkSuccessVoice,//�������ӳɹ���Ƶ
    kDisConnectNetWorkVoice,    //�Ͽ�����������Ƶ
    kLowConcentrationVoice,     //��Ũ�ȱ�����Ƶ
    kHighConcentrationVoice,    //��Ũ�ȱ�����Ƶ
    /***********************************/
    kVoiceStop,//�رղ���
    kVoiceMaxEventcnt,
}TePlayVoiceEvent;

// ��Ƶ���ſ��ƵĽṹ��
typedef struct
{
    TePlayVoiceEvent audio_data;//��Ƶ
    uint64_t start_time;//��ʼʱ��
    int32_t play_duration;//����ʱ��
    rt_bool_t is_playing;//�Ƿ����ڲ���

} TsAudioPlayControl;

extern TsAudioPlayControl audio_play_control;

void AudioPlayControl_Ctrl(TsAudioPlayControl *ctrl,//�ṹ��
                            rt_uint8_t audio_data,//���ŵ���Ƶ
                            rt_int32_t play_duration_ms);//���ŵ���Ƶʱ��
void Send_Voice_Event(TePlayVoiceEvent event_type);










#endif //!__BSP_VOICE_H__


