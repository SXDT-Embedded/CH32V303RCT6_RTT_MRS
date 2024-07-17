#ifndef __BSP_FLASH_H__
#define __BSP_FLASH_H__



#include "board.h"
#include "rtdef.h"
#include "bsp_hr.h"


#define FLASH_PAGE_SIZE             (256U)
#define FLASH_PAGE_HR_RECORD_NUM    (25U)
#define FLASH_HR_ONE_RECORD_SIZE    (10U)

#define HR_ALARM_MAX_NUM              (200)
#define HR_ALARM_RCY_MAX_NUM          (200)
#define HR_FAULT_MAX_NUM              (100)
#define HR_FAULT_RCY_MAX_NUM          (100)
#define HR_POWER_FAILURE_MAX_NUM      (50)
#define HR_POWER_ON_MAX_NUM           (50)
#define HR_SENSOR_FAILURE_MAX_NUM     (1)

#define HR_ALARM_PAGES              (9U)     // 8 + 1 page
#define HR_ALARM_RCY_PAGES          (9U)     // 8 + 1 page
#define HR_FAULT_PAGES              (5U)     // 4 + 1 page
#define HR_FAULT_RCY_PAGES          (5U)     // 4 + 1 page
#define HR_POWER_FAILURE_PAGES      (3U)     // 2 + 1 page
#define HR_POWER_ON_PAGES           (3U)     // 2 + 1 page
#define HR_SENSOR_FAILURE_PAGES     (1U)     // 1 page

#define FLASH_CONFIG_INFO_PAGES     (1U)

#define HR_RECORD_FRAME_HEADER      (0xAA55U)
#define HR_RECORD_FRAME_LEN         (10U)

#define FLASH_START_ADDR            (0x08000000U)
// code flash = 64 KB
#define FALSH_SIZE_KB               (256U)
#define FLASH_LAST_PAGE_START_ADDR  (FLASH_START_ADDR + (FALSH_SIZE_KB * 1024U) - FLASH_PAGE_SIZE)

#define FLASH_HR_ALARM_START_ADDR           (FLASH_HR_ALARM_RCY_START_ADDR - FLASH_PAGE_SIZE * HR_ALARM_PAGES)
#define FLASH_HR_ALARM_RCY_START_ADDR       (FLASH_HR_FAULT_START_ADDR - FLASH_PAGE_SIZE * HR_ALARM_RCY_PAGES)

#define FLASH_HR_FAULT_START_ADDR           (FLASH_HR_FAULT_RCY_START_ADDR - FLASH_PAGE_SIZE * HR_FAULT_PAGES)
#define FLASH_HR_FAULT_RCY_START_ADDR       (FLASH_HR_POWER_FAILURE_START_ADDR - FLASH_PAGE_SIZE * HR_FAULT_RCY_PAGES)

#define FLASH_HR_POWER_FAILURE_START_ADDR   (FLASH_HR_POWER_ON_START_ADDR - FLASH_PAGE_SIZE * HR_POWER_FAILURE_PAGES)
#define FLASH_HR_POWER_ON_START_ADDR        (FLASH_HR_SENSOR_FAILURE_START_ADDR - FLASH_PAGE_SIZE * HR_POWER_ON_PAGES)

#define FLASH_HR_SENSOR_FAILURE_START_ADDR  (FLASH_FACTORY_TIME_START_ADDR - FLASH_PAGE_SIZE)

/*
*   FLASH_CONFIG_INFO_START_ADDR    FLASH_LAST_PAGE_START_ADDR  (0)
*   FLASH_FACTORY_TIME_START_ADDR       (0~9)       10B (出厂时间)
*   FLASH_EXPIRATION_TIME_START_ADDR    (10~19)     10B (到期时间)
*   FLASH_MQ_ALARM_VALUE_ADDR           ()          10B (标定值)
*   FLASH_INIT_FLAG_ADDR                (254~255)   2B  (FLASH_FIRST_INIT_VALUE)
*/
#define FLASH_CONFIG_INFO_START_ADDR        FLASH_LAST_PAGE_START_ADDR
#define FLASH_FACTORY_TIME_START_ADDR       FLASH_CONFIG_INFO_START_ADDR
#define FLASH_EXPIRATION_TIME_START_ADDR    (FLASH_FACTORY_TIME_START_ADDR + sizeof(TuFlashProductTimeLimitFrame))
#define FLASH_ALARM_VALUE_ADDR           (FLASH_EXPIRATION_TIME_START_ADDR + sizeof(TuFlashProductTimeLimitFrame))
#define FLASH_INIT_FLAG_ADDR                (FLASH_CONFIG_INFO_START_ADDR + 254)

// #define FLASH_VIN_ADC_HIGH_THRESHOLD
// #define FLASH_VIN_ADC_LOW_THRESHOLD

#define FLASH_FIRST_INIT_VALUE          (0xA2A2U)
#define FLASH_SYS_PRODUCT_VALUE         (0xA3A3U)
#define FLASH_SYS_USER_VALUE            (0xA4A4U)

#define FLASH_CONFIG_INFO_FRAME_HEADER      (0xA5A5U)

#define FLASH_PRODUCT_TIME_FRAME_HEADER     (0xA5U)

typedef enum
{
    kFactoryTimeId = 0,
    kExpirationTimeId
} TeFlashProductTimeLimitId;

typedef struct __attribute__((packed))
{
    uint8_t header;
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} TsFlashProductTimeLimitFrame;

typedef union
{
    uint8_t buf[sizeof(TsFlashProductTimeLimitFrame)];
    TsFlashProductTimeLimitFrame Struct;
} TuFlashProductTimeLimitFrame;

typedef struct __attribute__((packed))
{
    uint16_t header;
    uint16_t value;
} TsFlashMqAlarmFrame;

typedef union
{
    uint8_t buf[sizeof(TsFlashMqAlarmFrame)];
    TsFlashMqAlarmFrame Struct;
} TuFlashMqAlarmFrame;

typedef struct __attribute__((packed))
{
    uint16_t header;
    uint16_t index;
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
} TsFlashHrRecordFrame;

typedef union
{
    uint8_t buf[HR_RECORD_FRAME_LEN];
    TsFlashHrRecordFrame Struct;
} TuFlashHrRecordFrame;

typedef struct
{
    uint8_t alarm;
    uint8_t alarm_rcy;
    uint8_t fault;
    uint8_t fault_rcy;
    uint8_t power_failure;
    uint8_t power_on;
    uint8_t sensor_failure;
} TsTotalRecords;

extern TsTotalRecords TotalRecords;

typedef enum
{
    kRecordAlarm = 0,       // at least 200
    kRecordAlarmRcy,        // at least 200
    kRecordFault,           // at least 100
    kRecordFaultRcy,        // at least 100
    kRecordPowerDown,       // at least 50
    kRecordPowerOn,         // at least 50
    kRecordSensoEndOfLife   // at least 1
} TeRecord;

void Flash_Write_Record(TeRecord record);

rt_uint8_t Flash_GetNum_Records(TeRecord record);

void Flash_ErasePage_ConfigInfo(void);
void Flash_ErasePage_ReadConfigInfo(rt_uint8_t *page_buf);
void Flash_Write_ConfigInfo(rt_uint8_t *page_buf);

ErrorStatus Flash_GetProductTimeLimit(TuFlashProductTimeLimitFrame *pLimitTime,
                                TeFlashProductTimeLimitId id);
void Flash_SetProductTimeLimit(uint16_t year, rt_uint8_t mon, rt_uint8_t day
        ,rt_uint8_t hour, rt_uint8_t min, rt_uint8_t second, TeFlashProductTimeLimitId id);

void Set_FactoryRtcTime(void);
void Set_ExpirationTime(rt_uint16_t days);

rt_uint16_t Flash_Get_AlarmLel(void);
void Flash_Set_AlarmLel(rt_uint16_t value);

ErrorStatus Flash_GetTotalRecord(TsTotalRecords *pTotalRecords);
ErrorStatus Flash_GetRecord(TeFrameC2 record_type, rt_uint8_t index, TsRecordsTime *pRecordsTime);

void Flash_Erase_Records(TeRecord record);

void BSP_Flash_EraseRecodrs(void);
int BSP_Flash_Init(void);





























#endif //!@__BSP_FLASH_H__


