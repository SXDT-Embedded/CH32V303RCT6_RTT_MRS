#ifndef __USER_SYS_H__
#define __USER_SYS_H__

#include "rtdef.h"



#define WARM_UP_TIMER_TICKS             (5000U)//Ԥ���¼�5s


typedef enum
{
    kPowerOnEvent,//�ϵ��¼�
    kPreheatingEvent,//Ԥ���¼�
    kNormalDetectionEvents,//�������״̬
    kAlarmEvent,//�����¼�
    kAlarmRcyEvent,//�����ָ��¼�
    kFaultEvent,//�����¼�
    kFaultRcyEvent,//���ϻָ��¼�
    kSensorFailureEvent,//������ʧЧ�¼�
    kPowerDownEvent,//�����¼�
    kSelfInspectionEvents,//�Լ��¼�

    kFlexBtnPressDown,        // �����¼�
    kFlexBtnPressClick,           // �����¼�
    kFlexBtnPressDoubleClick,    // ˫���¼�
    kFlexBtnPressPepeatClick,    // �����¼���ʹ�� flex_button_t �е� click_cnt �϶���������
    kFlexBtnPressShoartStart,     // �̰���ʼ�¼�
    kFlexBtnPressShortUp,        // �̰�̧���¼�
    kFlexBtnPressLongStart,      // ������ʼ�¼�
    kFlexBtnPressLongUp,         // ����̧���¼�
    kFlexBtnPressLongHold,       // ���������¼�
    kFlexBtnPressLongHoldUp,    // �������ֵ�̧���¼�
    kMaxEventcnt,//�¼���
}AlarmEvent;//�������¼�



extern struct rt_event alarm_event;

void Send_Laser_Alarm_Event(AlarmEvent event);//��Ϊһ��ȫ�ֺ�����ͨ��������¼�����



















#endif //!__USER_SYS_H__


