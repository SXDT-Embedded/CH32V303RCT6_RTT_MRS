#ifndef __BSP_HISTORY_READING_H__
#define __BSP_HISTORY_READING_H__

#include "board.h"
#include "rtdef.h"

#define HR_THREAD_STACK_SIZE    (1028)
#define HR_THREAD_PRIORITY      (8)
#define HR_THREAD_TIMESLICE     (5)

#define HR_BAUDRATE (4800)

/*************可燃气体探测器报警历史记录读取串口通信协议****************/
// 帧起始符(1)  控制码(2)       数据域长度(1)  数据域(n)    校验码(1)   结束符(1)
// 0xAA        C1、C2(低四位)      L          DATA         CS          0x55
#define FRAME_HEADER    (0xAA)
#define FRAME_TAIL      (0x55)

#define HOST_FRAME_MIN_LEN   (6U)

#define HR_ALARM_MAX_NUM              (200)
#define HR_ALARM_RCY_MAX_NUM          (200)
#define HR_FAULT_MAX_NUM              (100)
#define HR_FAULT_RCY_MAX_NUM          (100)
#define HR_POWER_FAILURE_MAX_NUM      (50)
#define HR_POWER_ON_MAX_NUM           (50)
#define HR_SENSOR_FAILURE_MAX_NUM     (1)

typedef enum
{
    kNumOfRecords = 0U,
    kAlarmRecord,           // at least 200
    kAlarmRcyRecord,        // at least 200
    kFaultRecord,           // at least 100
    kFaultRcyRecord,        // at least 100
    kPowerFailureRecord,    // at least 50
    kPowerOnRecord,         // at least 50
    kSensorFailureRecord,   // at least 1
    kGetCurrentTime,

    // 以下是在国标原基础上所扩展的
    kGetCurrentTimeSecond,  // include second

    kSetCurrentTime,
    kSetFactoryTime,
    kSetExpirationTime,

    kSetAlarmValue,

    kOutSysStatus,  // 0x0E 不能动，因为说明书里面写好了

    kGetAlarmValue,

    kGetMqVoltage,

    kSetSysStatus,
    kGetSysStatus,

    kEraseRecords,

    kGetFactoryTime,
    kGetExpirationTime,

    // kGetEsig,

    kReboot,
} TeFrameC2;

typedef struct __attribute__((packed))
{
    uint8_t year_h;
    uint8_t year_l;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
} TsRecordsTime;

typedef struct __attribute__((packed))
{
    uint8_t c1;
    TeFrameC2 c2;
    uint8_t len;
    uint8_t data[];
} TsFrameData;

typedef struct __attribute__((packed))
{
    uint8_t len;
    uint8_t buf[256];
} TsRawFrameData;


extern struct rt_semaphore hr_rx_sem;
int BSP_HR_Init(void);

TsFrameData* HR_GetFrameData(const uint8_t *p_src, const uint8_t src_len);
// FlagStatus HR_ProcessData(const TsFrameData *pHostFrameData);
// uint8_t HR_ProcessData(const TsFrameData *pHostFrameData);
uint8_t HR_ProcessData(const TsFrameData *pHostFrameData);

uint8_t HR_GenerateRawFrame(TsRawFrameData* pRawData
    , uint8_t c1,TeFrameC2 c2, const uint8_t* p_src, uint8_t src_len);




#endif //!__BSP_HISTORY_READING_H__
