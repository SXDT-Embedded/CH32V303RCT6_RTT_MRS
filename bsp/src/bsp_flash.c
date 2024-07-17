#include "bsp_flash.h"
// #include "bsp_bkp.h"
#include "lwutil.h"
#include "bsp_rtc.h"
// #include "bsp_wdg.h"
#include "bsp_hr.h"


#include <rtthread.h>
#include <rthw.h>

#define LOG_TAG     "bsp_flash"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

//用到了atoi
#include <stdlib.h>

TsTotalRecords TotalRecords;

const rt_uint32_t hr_start_addr[7] = {FLASH_HR_ALARM_START_ADDR, FLASH_HR_ALARM_RCY_START_ADDR, \
        FLASH_HR_FAULT_START_ADDR, FLASH_HR_FAULT_RCY_START_ADDR, \
        FLASH_HR_POWER_FAILURE_START_ADDR, FLASH_HR_POWER_ON_START_ADDR, \
        FLASH_HR_SENSOR_FAILURE_START_ADDR
};

const rt_uint8_t hr_record_max_num[7] = {HR_ALARM_MAX_NUM, HR_ALARM_RCY_MAX_NUM, \
        HR_FAULT_MAX_NUM, HR_FAULT_RCY_MAX_NUM, \
        HR_POWER_FAILURE_MAX_NUM, HR_POWER_ON_MAX_NUM, \
        HR_SENSOR_FAILURE_MAX_NUM
};

const rt_uint8_t hr_record_pages[7] = {HR_ALARM_PAGES, HR_ALARM_RCY_PAGES, \
        HR_FAULT_PAGES, HR_FAULT_RCY_PAGES, \
        HR_POWER_FAILURE_PAGES, HR_POWER_ON_PAGES, \
        HR_SENSOR_FAILURE_PAGES
};

static rt_base_t interrupt_value;
static void BSP_Flash_UnLock(void)
{
#if (SystemCoreClock > SYSCLK_FREQ_96MHz_HSE)
    RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV2;
#endif
    interrupt_value = rt_hw_interrupt_disable();

    FLASH_Unlock();
}

static void BSP_Flash_Lock(void)
{
    FLASH_Lock();

#if (SystemCoreClock > SYSCLK_FREQ_96MHz_HSE)
    RCC->CFGR0 &= ~(uint32_t)RCC_HPRE_DIV2;
#endif
    rt_hw_interrupt_enable(interrupt_value);
}

static void BSP_Flash_FastUnLock(void)
{
#if (SystemCoreClock > SYSCLK_FREQ_96MHz_HSE)
    RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV2;
#endif
    interrupt_value = rt_hw_interrupt_disable();

    FLASH_Unlock_Fast();
}

static void BSP_Flash_FastLock(void)
{
    FLASH_Lock_Fast();

#if (SystemCoreClock > SYSCLK_FREQ_96MHz_HSE)
    RCC->CFGR0 &= ~(uint32_t)RCC_HPRE_DIV2;
#endif
    rt_hw_interrupt_enable(interrupt_value);
}

/**
 * @description: 在通用地址空间内进行直接寻址，
 *          任何8/16/32位数据的读操作都能访问闪存模块的内容并得到相应的数据
 * @param {uint32_t} addr
 * @param {uint8_t} *buf
 * @param {size_t} len
 * @return {*}
 */
static rt_size_t Flash_Read(rt_uint32_t addr, rt_uint8_t *buf, rt_size_t len)
{
    rt_size_t read_len = 0;

    for (rt_size_t i = 0; i < len; i++, buf++, addr++, read_len++)
    {
        *buf = *(rt_uint8_t *) addr;
    }

    return read_len;
}

/**
 * @description:
 * @param {uint32_t} addr
 * @param {uint8_t} *buf
 * @param {size_t} len
 * @return {*}
 */
static rt_size_t Flash_Write(rt_uint32_t addr, rt_uint8_t *buf, rt_size_t len)
{
    rt_size_t i = 0;
    __attribute__ ((aligned (2))) rt_uint16_t write_data;

    if(addr % 2 != 0)
       return 0;

    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP |FLASH_FLAG_WRPRTERR);

    BSP_Flash_UnLock();

    for (i = 0; i < len; i += 2, buf += 2, addr += 2)
    {
        rt_memcpy(&write_data, buf, 2); //用以保证HAL_FLASH_Program的write_data是内存首地址对齐
        FLASH_ProgramHalfWord(addr, write_data);

        /* You can add your code under here. */
        if (*(rt_uint16_t *)addr != write_data)
        {
            BSP_Flash_Lock();
            LOG_E("Flash_Write Error");
            return 0;
        }
        // else
        // {
            //FLash操作可能非常耗时，如果有看门狗需要喂狗，以下代码由用户实现
            // HAL_FeedDog();
        // }
    }

    BSP_Flash_Lock();

    return i;
}

/**
 * @description: Flash Erase Page
 * @param {TeRecord} record
 * @param {uint8_t} page
 * @return {*}
 */
static ErrorStatus Flash_ErasePage_Records(TeRecord record, rt_uint8_t page_offset)
{
    ErrorStatus flag = READY;

    rt_uint8_t erase_page = 0;

    if(page_offset <= hr_record_pages[record] - 1)
    {
        erase_page = page_offset;
    }
    else
    {
        erase_page = hr_record_pages[record] - 1;
    }

    BSP_Flash_FastUnLock();
    FLASH_ErasePage_Fast(hr_start_addr[record] + FLASH_PAGE_SIZE * erase_page);
    BSP_Flash_FastLock();

    for (rt_uint8_t i = 0; i < (FLASH_PAGE_SIZE / 2); i++)
    {
        if(*(rt_uint16_t*)((hr_start_addr[record] + FLASH_PAGE_SIZE * erase_page) + 2 * i) != 0xE339)
        {
            flag = NoREADY;
        }
    }

    LOG_D("/**Flash_Erase(%d)Page(%d)=%d**/", record, erase_page, flag);

    return flag;
}

void Flash_Erase_Records(TeRecord record)
{
    for (rt_uint8_t erase_page = 0; erase_page < hr_record_pages[record]; erase_page++)
    {
        Flash_ErasePage_Records(record, erase_page);
    }
    LOG_D("Flash_Erase_Records(%d)", record);
}

static void TEST_Flash_Erase_Records(int argc, char **argv)
{
    if (argc == 2)
    {
        int record = atoi(argv[1]);
        Flash_Erase_Records(record);
    }
    else
    {
        LOG_E("TEST_Flash_Erase_Records      --use _cmd_ [record](0~6)");
    }
}
MSH_CMD_EXPORT(TEST_Flash_Erase_Records, "TEST_Flash_Erase_Records");

/*
 * @description:
 * @param {TeRecord} record
 * @return {*}
 */
static rt_uint16_t Flash_GetMaxIndex_Records(TeRecord record)
{
    rt_uint16_t index_max = 0;

    for (rt_uint8_t page = 0; page < hr_record_pages[record]; page++)
    {
        for (rt_uint8_t i = 0; i < FLASH_PAGE_HR_RECORD_NUM; i++)
        {
            if(*(rt_uint16_t*)(hr_start_addr[record] + FLASH_PAGE_SIZE * page + FLASH_HR_ONE_RECORD_SIZE * i) == HR_RECORD_FRAME_HEADER)
            {
                index_max = LWUTIL_MAX(index_max, *(rt_uint16_t *)(hr_start_addr[record] + FLASH_PAGE_SIZE * page + FLASH_HR_ONE_RECORD_SIZE * i + 2));
            }
        }
    }

    LOG_D("Flash_Get_Records(%d)MaxNum = %d", record, index_max);

    return index_max;
}

static void TEST_Flash_GetMaxIndex_Records(int argc, char **argv)
{
    if (argc == 2)
    {
        rt_uint8_t record = atoi(argv[1]);
        LOG_D("Flash_GetMaxIndex_Records(%d) = %d", record, Flash_GetMaxIndex_Records(record));
    }
    else
    {
        LOG_E("TEST_Flash_GetMaxIndex_Records      --use _cmd_ [record](0~6)");
    }
}
MSH_CMD_EXPORT(TEST_Flash_GetMaxIndex_Records, "TEST_Flash_GetMaxIndex_Records");

/**
 * @description:
 * @param {TeRecord} record
 * @return {*}
 */
rt_uint8_t Flash_GetNum_Records(TeRecord record)
{
    rt_uint8_t num = 0;
    rt_uint16_t num_max = Flash_GetMaxIndex_Records(record);

    if(num_max <= hr_record_max_num[record])
    {
        num = num_max;
    }
    else
    {
        num = hr_record_max_num[record];
    }

    LOG_D("Flash_GetNum_(%d)Records = %d", record, num);

    return num;
}

static void TEST_Flash_GetNum_Records(int argc, char **argv)
{
    if (argc == 2)
    {
        int record = atoi(argv[1]);
        if (0 <= record && record <= 6)
        {
            LOG_D("TEST_Flash_GetNum_Records(%d) = %d", record, Flash_GetNum_Records(record));
        }
        else
        {
            LOG_E("TEST_Flash_GetNum_Records      --use _cmd_ [record](0~6)");
        }
    }
    else
    {
        LOG_E("TEST_Flash_GetNum_Records      --use _cmd_ [record](0~6)");
    }
}
MSH_CMD_EXPORT(TEST_Flash_GetNum_Records, "TEST_Flash_GetNum_Records");

/**
 * @description:
 * @param {TuFlashHrRecordFrame*} pHrRecord
 * @param {TeRecord} record
 * @param {uint8_t} index
 * @return {*}
 */
void Flash_Read_Record(TuFlashHrRecordFrame* pHrRecord, TeRecord record, rt_uint8_t index)
{
    LOG_D("/*********Flash_Read_(%d)Record(%d)***************/", record, index);
    uint16_t index_max = Flash_GetMaxIndex_Records(record);

    rt_uint8_t page_offset = 0 ;
    rt_uint8_t in_page_offset = 0;

    if(index <= index_max)
    {
        if(index_max > hr_record_max_num[record])
        {
            if(record < kRecordSensoEndOfLife)
            {
                // index_redirect = index_max - (hr_record_max_num[record] - index);
                uint16_t index_redirect = index_max - (50 - index);
                LOG_D("index_redirect = %d", index_redirect);
                page_offset = ((index_redirect - 1) / FLASH_PAGE_HR_RECORD_NUM) % hr_record_pages[record];
                in_page_offset = ((index_redirect - 1) % FLASH_PAGE_HR_RECORD_NUM) * FLASH_HR_ONE_RECORD_SIZE;
            }
            else
            {
                page_offset = 0;
                in_page_offset = ((index_max - 1) % FLASH_PAGE_HR_RECORD_NUM) * FLASH_HR_ONE_RECORD_SIZE;
            }
        }
        else
        {
            page_offset = ((index - 1) / FLASH_PAGE_HR_RECORD_NUM) % hr_record_pages[record];
            in_page_offset = ((index - 1) % FLASH_PAGE_HR_RECORD_NUM) * FLASH_HR_ONE_RECORD_SIZE;
        }
        LOG_D("page_offset = %d", page_offset);
        LOG_D("in_page_offset = %d", in_page_offset);

        uint32_t addr = (hr_start_addr[record] + FLASH_PAGE_SIZE * page_offset + in_page_offset);
        LOG_D("addr = %04X", addr);

        Flash_Read(addr, pHrRecord->buf, HR_RECORD_FRAME_LEN);

        LOG_D("Flash_Read_(%d)Record(%d):%04d-%02d-%02d,%02d:%02d", record, index, \
            pHrRecord->Struct.year, pHrRecord->Struct.month, pHrRecord->Struct.day,
            pHrRecord->Struct.hour, pHrRecord->Struct.minute);
    }
    else
    {
        rt_memset(pHrRecord->buf, 0, HR_RECORD_FRAME_LEN);
        LOG_E("Flash_GetMaxNum_(%d)Records Error!: (index_max)%d < %d", record, index_max, index);
    }
}

/**
 * @description:
 * @param {TuFlashHrRecordFrame*} pHrRecord
 * @param {uint16_t} index
 * @return {*}
 */
void Flash_Write_RecordIndex(TuFlashHrRecordFrame* pHrRecord, TeRecord record, rt_uint16_t index)
{
    LOG_D("/*********Flash_Write_RecordIndex***************/");

    rt_uint8_t page_offset = ((index - 1) / FLASH_PAGE_HR_RECORD_NUM) % hr_record_pages[record];
    LOG_D("page_offset = %d", page_offset);
    rt_uint8_t in_page_offset = ((index - 1) % FLASH_PAGE_HR_RECORD_NUM) * FLASH_HR_ONE_RECORD_SIZE;
    LOG_D("in_page_offset = %d", in_page_offset);

    rt_uint32_t addr = (hr_start_addr[record] + FLASH_PAGE_SIZE * page_offset + in_page_offset);
    LOG_D("addr = %04X", addr);
    Flash_Write(addr, pHrRecord->buf, HR_RECORD_FRAME_LEN);

    TuFlashHrRecordFrame pReadRecord;
    Flash_Read(addr, pReadRecord.buf, HR_RECORD_FRAME_LEN);

    LOG_D("(%04X)Flash_Write_(%d)RecordIndex(%d):%04d-%02d-%02d,%02d:%02d", \
            pReadRecord.Struct.header, record, pReadRecord.Struct.index, \
            pReadRecord.Struct.year, pReadRecord.Struct.month, pReadRecord.Struct.day,
            pReadRecord.Struct.hour, pReadRecord.Struct.minute);
}

void Flash_Write_Record(TeRecord record)
{
    LOG_D("/*********Flash_Write_Record***************/");

    rt_uint16_t index_max = Flash_GetMaxIndex_Records(record);
    rt_uint16_t index_new = index_max + 1;
    LOG_D("index_new = %d", index_new);

    rt_uint16_t index_start;
    rt_uint8_t index_start_page_offset;

    rt_uint8_t index_max_page_offset;
    rt_uint8_t index_max_in_page_offset;

    if(index_max <= hr_record_max_num[record])
    {
        index_start = 0;
        index_start_page_offset = 0;
        index_max_page_offset = ((index_max - 1) / FLASH_PAGE_HR_RECORD_NUM) % hr_record_pages[record];
        LOG_D("index_max_page_offset = %d", index_max_page_offset);
    }
    else
    {
        index_start = index_max - (hr_record_max_num[record] - 1);
        LOG_D("index_start = %d", index_start);

        index_start_page_offset = ((index_start - 1) / FLASH_PAGE_HR_RECORD_NUM) % hr_record_pages[record];
        LOG_D("index_start_page_offset = %d", index_start_page_offset);

        index_max_page_offset = ((index_max - 1) / FLASH_PAGE_HR_RECORD_NUM) % hr_record_pages[record];
    }
    index_max_in_page_offset = (index_max - 1) % FLASH_PAGE_HR_RECORD_NUM;
    LOG_D("index_max_in_page_offset = %d", index_max_in_page_offset);

    if((index_max_in_page_offset + 1) == (FLASH_PAGE_HR_RECORD_NUM - 1) \
        && index_max_page_offset == (hr_record_pages[record] - 1))
    {
        Flash_ErasePage_Records(record, 0);
    }
    else if ((index_max_in_page_offset + 1) == (FLASH_PAGE_HR_RECORD_NUM - 1) \
        && index_max_page_offset == index_start_page_offset - 1)
    {
        Flash_ErasePage_Records(record, index_max_page_offset + 1);
    }

    BSP_Get_RTC_Time();

    TuFlashHrRecordFrame HrRecord;
    HrRecord.Struct.header = HR_RECORD_FRAME_HEADER;
    HrRecord.Struct.index = index_new;
    HrRecord.Struct.year = RtcDateTime.year;
    HrRecord.Struct.month = RtcDateTime.month;
    HrRecord.Struct.day = RtcDateTime.day;
    HrRecord.Struct.hour = RtcDateTime.hour;
    HrRecord.Struct.minute = RtcDateTime.minute;

    Flash_Write_RecordIndex(&HrRecord, record, index_new);
}

static void TEST_Flash_Write_Record(int argc, char **argv)
{
    if (argc == 3)
    {
        int record = atoi(argv[1]);
        int num = atoi(argv[2]);
        for (rt_uint8_t i = 0; i < num; i++)
        {
            Flash_Write_Record(record);
            rt_thread_mdelay(2);
            // BSP_WDG_Loop();
        }
    }
    else
    {
        LOG_E("TEST_Flash_Write_Record      --use _cmd_ [record(0~6)] [num]");
    }
}
MSH_CMD_EXPORT(TEST_Flash_Write_Record, "TEST_Flash_Write_Record");

void Flash_ErasePage_ConfigInfo(void)
{
    BSP_Flash_FastUnLock();
    FLASH_ErasePage_Fast(FLASH_CONFIG_INFO_START_ADDR);
    BSP_Flash_FastLock();
}

/**
 * @description: 擦除配置页前保存当前数据到page_buf
 * @param {uint8_t} *page_buf
 * @return {*}
 */
void Flash_ErasePage_ReadConfigInfo(rt_uint8_t *page_buf)
{
    Flash_Read(FLASH_CONFIG_INFO_START_ADDR, page_buf, FLASH_PAGE_SIZE);

    Flash_ErasePage_ConfigInfo();
}

void Flash_Write_ConfigInfo(rt_uint8_t *page_buf)
{
    Flash_Write(FLASH_CONFIG_INFO_START_ADDR, page_buf, FLASH_PAGE_SIZE);
}

ErrorStatus Flash_GetProductTimeLimit(TuFlashProductTimeLimitFrame *pLimitTime,
                                TeFlashProductTimeLimitId id)
{
    ErrorStatus flag = NoREADY;
    rt_uint32_t addr = 0;

    if(id == kFactoryTimeId)
    {
        addr = FLASH_FACTORY_TIME_START_ADDR;
    }
    else if(id == kExpirationTimeId)
    {
        addr = FLASH_EXPIRATION_TIME_START_ADDR;
    }
    if (addr != 0)
    {
        Flash_Read(addr, pLimitTime->buf, sizeof(TuFlashProductTimeLimitFrame));

        if(pLimitTime->Struct.header == FLASH_PRODUCT_TIME_FRAME_HEADER)
        {
            flag = READY;
            LOG_D("Flash_GetProductTimeLimit(%d):%04d-%02d-%02d,%02d:%02d:%02d", id, \
                    pLimitTime->Struct.year, pLimitTime->Struct.month, pLimitTime->Struct.day,
                    pLimitTime->Struct.hour, pLimitTime->Struct.minute, pLimitTime->Struct.second);
        }
        else
        {
            LOG_E("Flash_GetProductTimeLimit Error!");
        }
    }

    return flag;
}

static void TEST_Flash_GetProductTimeLimit(int argc, char **argv)
{
    if (argc == 2)
    {
        int id = atoi(argv[1]);
        TuFlashProductTimeLimitFrame LimitTime;
        Flash_GetProductTimeLimit(&LimitTime, id);
    }
    else
    {
        LOG_E("TEST_Flash_GetProductTimeLimit      --use _cmd_ [id(0:FACTORY; 1:EXPIRATION)]");
    }
}
MSH_CMD_EXPORT(TEST_Flash_GetProductTimeLimit, "TEST_Flash_GetProductTimeLimit");

void Flash_SetProductTimeLimit(rt_uint16_t year, rt_uint8_t mon, rt_uint8_t day
        ,rt_uint8_t hour, rt_uint8_t min, rt_uint8_t second, TeFlashProductTimeLimitId id)
{
    TuFlashProductTimeLimitFrame LimitTime;

    rt_uint8_t page_buf[FLASH_PAGE_SIZE];
    Flash_ErasePage_ReadConfigInfo(page_buf);

    LimitTime.Struct.header = FLASH_PRODUCT_TIME_FRAME_HEADER;
    LimitTime.Struct.year = year;
    LimitTime.Struct.month = mon;
    LimitTime.Struct.day = day;
    LimitTime.Struct.hour = hour;
    LimitTime.Struct.minute = min;
    LimitTime.Struct.second = second;

    rt_uint8_t in_page_offset = 0;
    if(id == kFactoryTimeId)
    {
        in_page_offset = (FLASH_FACTORY_TIME_START_ADDR - FLASH_CONFIG_INFO_START_ADDR);
    }
    else if(id == kExpirationTimeId)
    {
        in_page_offset = (FLASH_EXPIRATION_TIME_START_ADDR - FLASH_CONFIG_INFO_START_ADDR);
    }

    for (rt_uint8_t i = 0; i < sizeof(TuFlashProductTimeLimitFrame); i++)
    {
        page_buf[in_page_offset + i] = LimitTime.buf[i];
    }

    Flash_Write_ConfigInfo(page_buf);

    TuFlashProductTimeLimitFrame ReadLimitTime;
    Flash_GetProductTimeLimit(&ReadLimitTime, id);
}

static void TEST_Flash_SetProductTimeLimit(int argc, char **argv)
{
    if (argc == 8)
    {
        int year = atoi(argv[1]);
        int mon = atoi(argv[2]);
        int day = atoi(argv[3]);
        int hour = atoi(argv[4]);
        int min = atoi(argv[5]);
        int second = atoi(argv[6]);
        int id = atoi(argv[7]);
        Flash_SetProductTimeLimit(year, mon, day, hour, min, second, id);
    }
    else
    {
        LOG_E("TEST_Flash_SetProductTimeLimit      --use _cmd_ [y] [m] [d] [h] [m] [s] [id(0:FACTORY; 1:EXPIRATION)]");
    }
}
MSH_CMD_EXPORT(TEST_Flash_SetProductTimeLimit, "TEST_Flash_SetProductTimeLimit");

void Set_FactoryRtcTime(void)
{
    BSP_Get_RTC_Time();
    LOG_D("%4d-%02d-%02d, %02d:%02d:%02d", RtcDateTime.year, RtcDateTime.month, RtcDateTime.day, \
            RtcDateTime.hour, RtcDateTime.minute, RtcDateTime.second);

    Flash_SetProductTimeLimit(RtcDateTime.year, RtcDateTime.month, RtcDateTime.day
        , RtcDateTime.hour, RtcDateTime.minute, RtcDateTime.second, kFactoryTimeId);
}
MSH_CMD_EXPORT(Set_FactoryRtcTime, "Use RTC time Set_FactoryRtcTime");

//void Set_ExpirationTime(uint16_t days)
//{
//    uint32_t factory_seconds, expiration_seconds;
//    TuFlashProductTimeLimitFrame LimitTime;
//
//    if (Flash_GetProductTimeLimit(&LimitTime, kFactoryTimeId) == READY)
//    {
//        factory_seconds = DateTime2Seconds(LimitTime.Struct.year
//            , LimitTime.Struct.month, LimitTime.Struct.day, LimitTime.Struct.hour
//            , LimitTime.Struct.minute, LimitTime.Struct.second);
//        LOG_D("FactoryTime:%04d-%02d-%02d,%02d:%02d"
//            , LimitTime.Struct.year, LimitTime.Struct.month, LimitTime.Struct.day
//            , LimitTime.Struct.hour, LimitTime.Struct.minute, LimitTime.Struct.second);
//
//        expiration_seconds = factory_seconds + days * 24 * 3600;
//        // LOG_D("factory_seconds = %d, expiration_seconds = %d", factory_seconds, expiration_seconds);
//
//        TsRtcDateTime DateTime;
//        Seconds2DateTime(expiration_seconds, &DateTime);
//        Flash_SetProductTimeLimit(DateTime.year, DateTime.month, DateTime.day
//            , DateTime.hour, DateTime.minute, DateTime.second, kExpirationTimeId);
//
//        if (Flash_GetProductTimeLimit(&LimitTime, kExpirationTimeId) == READY)
//        {
//            LOG_D("Flash_GetProductTimeLimit:%04d-%02d-%02d,%02d:%02d:%02d"
//                , LimitTime.Struct.year, LimitTime.Struct.month, LimitTime.Struct.day
//                , LimitTime.Struct.hour, LimitTime.Struct.minute, LimitTime.Struct.second);
//        }
//    }
//}

//static void TEST_Flash_Set_ExpirationTime(int argc, char **argv)
//{
//    if (argc == 2)
//    {
//        int days = atoi(argv[1]);
//        Set_ExpirationTime(days);
//    }
//    else
//    {
//        LOG_E("TEST_Flash_Set_ExpirationTime      --use _cmd_ [days]");
//    }
//}
//MSH_CMD_EXPORT(TEST_Flash_Set_ExpirationTime, "TEST_Flash_Set_ExpirationTime");

rt_uint16_t Flash_Get_AlarmLel(void)
{
    rt_uint16_t value = 0;

    TuFlashMqAlarmFrame SensorCal;

    Flash_Read(FLASH_ALARM_VALUE_ADDR, SensorCal.buf, sizeof(TuFlashMqAlarmFrame));

    if(SensorCal.Struct.header == FLASH_CONFIG_INFO_FRAME_HEADER)
    {
        value = SensorCal.Struct.value;
        LOG_D("Flash_Get_AlarmValue: %d", value);
    }
    else
    {
        LOG_E("Flash_Get_AlarmValue Error!");
    }

    return value;
}

static void TEST_Flash_Get_AlarmValue(void)
{
    LOG_I("Flash_Get_AlarmValue = %d LEL", Flash_Get_AlarmLel());
}
MSH_CMD_EXPORT(TEST_Flash_Get_AlarmValue, "TEST_Flash_Get_AlarmValue");

void Flash_Set_AlarmLel(rt_uint16_t value)
{
    TuFlashMqAlarmFrame SensorCal;

    rt_uint8_t page_buf[FLASH_PAGE_SIZE];
    Flash_ErasePage_ReadConfigInfo(page_buf);

    SensorCal.Struct.header = FLASH_CONFIG_INFO_FRAME_HEADER;
    SensorCal.Struct.value = value;

    rt_uint8_t in_page_offset = (FLASH_ALARM_VALUE_ADDR - FLASH_CONFIG_INFO_START_ADDR);

    for (rt_uint8_t i = 0; i < sizeof(TuFlashMqAlarmFrame); i++)
    {
        page_buf[in_page_offset + i] = SensorCal.buf[i];
    }

    Flash_Write_ConfigInfo(page_buf);

//    LOG_D("Flash_SetMQ_AlarmValue() = %d", Flash_Get_AlarmLel());
}

static void TEST_Flash_Set_AlarmValue(int argc, char **argv)
{
    if (argc == 2)
    {
        rt_uint16_t value = atoi(argv[1]);
        if (0 < value && value < 100)
        {
            Flash_Set_AlarmLel(value);
        }
        else
        {
            LOG_E("value(0~100)");
        }
    }
    else
    {
        LOG_E("TEST_Flash_SetMQ_AlarmValue      --use __cmd__ [value]");
    }
}
MSH_CMD_EXPORT(TEST_Flash_Set_AlarmValue, "TEST_Flash_Set_AlarmValue");

ErrorStatus Flash_GetTotalRecord(TsTotalRecords *pTotalRecords)
{
    ErrorStatus flag = NoREADY;

    pTotalRecords->alarm = Flash_GetNum_Records(kRecordAlarm);
    pTotalRecords->alarm_rcy = Flash_GetNum_Records(kRecordAlarmRcy);
    pTotalRecords->fault = Flash_GetNum_Records(kRecordFault);
    pTotalRecords->fault_rcy = Flash_GetNum_Records(kRecordFaultRcy);
    pTotalRecords->power_failure =Flash_GetNum_Records(kRecordPowerDown);
    pTotalRecords->power_on = Flash_GetNum_Records(kRecordPowerOn);
    pTotalRecords->sensor_failure = Flash_GetNum_Records(kRecordSensoEndOfLife);

    flag = READY;

    return flag;
}

ErrorStatus Flash_GetRecord(TeFrameC2 record_type, rt_uint8_t index, TsRecordsTime *pRecordsTime)
{
    ErrorStatus flag = NoREADY;

    if(kNumOfRecords < record_type && record_type < kGetCurrentTime)
    {
        TuFlashHrRecordFrame pHrReadRecord;
        Flash_Read_Record(&pHrReadRecord, record_type - 1, index);

        LOG_D("(%d)Flash_GetRecord(%d):%04d-%02d-%02d,%02d:%02d", \
            record_type - 1, pHrReadRecord.Struct.index, \
            pHrReadRecord.Struct.year, pHrReadRecord.Struct.month, pHrReadRecord.Struct.day,
            pHrReadRecord.Struct.hour, pHrReadRecord.Struct.minute);

        pRecordsTime->year_h = pHrReadRecord.Struct.year / 256;
        pRecordsTime->year_l = pHrReadRecord.Struct.year % 256;
        pRecordsTime->month = pHrReadRecord.Struct.month;
        pRecordsTime->day = pHrReadRecord.Struct.day;
        pRecordsTime->hour = pHrReadRecord.Struct.hour;
        pRecordsTime->minute = pHrReadRecord.Struct.minute;

        flag = READY;
    }

    return flag;
}

/**
 * @description:
 * @return {*}
 */
void BSP_Flash_EraseRecodrs(void)
{
    for (rt_uint8_t record = 0; record <= kRecordSensoEndOfLife; record++)
    {
        for (rt_uint8_t i = 0; i < hr_record_pages[record]; i++)
        {
            Flash_ErasePage_Records(record, i);
        }
        // LOG_D("/**BSP_Flash_First_Init (%d)**/", record);
    }
    Flash_ErasePage_ConfigInfo();
}

int BSP_Flash_Init(void)
{
    if(*(rt_uint16_t *)FLASH_INIT_FLAG_ADDR != FLASH_FIRST_INIT_VALUE)
    {
        LOG_D("BSP_Flash_EraseRecodrs!");
    // if (SysControl.status == kSysProductionModeStatus)
    // {
        // SYS_SetStatus(kSysProductionModeStatus);

        rt_uint8_t flash_init_flag[2];

        flash_init_flag[0] = FLASH_FIRST_INIT_VALUE % 256;
        flash_init_flag[1] = FLASH_FIRST_INIT_VALUE / 256;

        BSP_Flash_EraseRecodrs();

        Flash_Write(FLASH_INIT_FLAG_ADDR, flash_init_flag, sizeof(flash_init_flag));

        Flash_SetProductTimeLimit(2023, 3, 22, 13, 58, 20, kFactoryTimeId);
        Flash_SetProductTimeLimit(2028, 3, 22, 13, 58, 20, kExpirationTimeId);
    }
    return 0;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_BOARD_EXPORT(BSP_Flash_Init);
#endif
