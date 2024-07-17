#ifndef __USER_SYS_H__
#define __USER_SYS_H__

#include "rtdef.h"



#define WARM_UP_TIMER_TICKS             (5000U)//预热事件5s


typedef enum
{
    kPowerOnEvent,//上电事件
    kPreheatingEvent,//预热事件
    kNormalDetectionEvents,//正常检测状态
    kAlarmEvent,//报警事件
    kAlarmRcyEvent,//报警恢复事件
    kFaultEvent,//故障事件
    kFaultRcyEvent,//故障恢复事件
    kSensorFailureEvent,//传感器失效事件
    kPowerDownEvent,//掉电事件
    kSelfInspectionEvents,//自检事件

    kFlexBtnPressDown,        // 按下事件
    kFlexBtnPressClick,           // 单击事件
    kFlexBtnPressDoubleClick,    // 双击事件
    kFlexBtnPressPepeatClick,    // 连击事件，使用 flex_button_t 中的 click_cnt 断定连击次数
    kFlexBtnPressShoartStart,     // 短按开始事件
    kFlexBtnPressShortUp,        // 短按抬起事件
    kFlexBtnPressLongStart,      // 长按开始事件
    kFlexBtnPressLongUp,         // 长按抬起事件
    kFlexBtnPressLongHold,       // 长按保持事件
    kFlexBtnPressLongHoldUp,    // 长按保持的抬起事件
    kMaxEventcnt,//事件数
}AlarmEvent;//报警器事件



extern struct rt_event alarm_event;

void Send_Laser_Alarm_Event(AlarmEvent event);//作为一个全局函数，通过其进行事件发送



















#endif //!__USER_SYS_H__


