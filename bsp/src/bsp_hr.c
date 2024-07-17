#include "bsp_hr.h"
#include "bsp_flash.h"
#include "bsp_rtc.h"
#include "drv_usart.h"
#include "lwrb.h"


#define LOG_TAG     "bsp_hr"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面


ALIGN(RT_ALIGN_SIZE)
static char hr_thread_stack[HR_THREAD_STACK_SIZE];
static struct rt_thread hr_thread;

/* 用于接收消息的信号量 */
struct rt_semaphore hr_rx_sem;
static rt_device_t rt_hr_device;

#define HR_UART "uart2"




/**
 * @description: 从帧起始符开始到校验码之前所有字节的和的模256，即各字节不计超过255的溢出值的二进制算术和。
 * @param {uint8_t} *data
 * @param {uint16_t} len
 * @return {*}
 */
static rt_uint8_t HR_CheckSum(const rt_uint8_t *data, rt_size_t len)
{
    rt_uint8_t sum = 0;

    for (rt_size_t i = 0; i < len; i++)
      {
          sum += data[i];
      }
    return sum;
}

/**
 * @description: 从接收缓冲区中获取一帧有效数据
 * @param {TsFrameData} *pFrameData 主机帧
 * @param {uint8_t} *p_src      接收缓冲区
 * @param {uint16_t} src_len    接收缓冲区长度
 * @return {*}
 */
TsFrameData* HR_GetFrameData(const rt_uint8_t *p_src, const rt_uint8_t src_len)
{
    rt_uint8_t data_field_len = 0;
    rt_uint8_t check_sum = 0;

    TsFrameData *get_buffer = RT_NULL;

    for(rt_size_t i = 0; i < src_len; i++)
    {
        if(p_src[i] == FRAME_HEADER)
        {
            data_field_len = p_src[i + 3];

            if(p_src[i + 3 + data_field_len + 2] == FRAME_TAIL)
            {
                check_sum = HR_CheckSum(&p_src[i] , (4 + data_field_len));

                if(p_src[i + 3 + data_field_len + 1] == check_sum)
                {
                    get_buffer = (TsFrameData *)rt_malloc(sizeof(TsFrameData) + sizeof(rt_uint8_t) * data_field_len);
                    if (get_buffer == RT_NULL)
                    {
                        LOG_E("get space fail\r\n");

                        return RT_NULL;
                    }
                    get_buffer->c1 = p_src[i + 1];
                    get_buffer->c2 = p_src[i + 2];
                    get_buffer->len = data_field_len;

                    if(data_field_len > 0)
                    {
                        // p_data = &p_src[i + 4];
//                        rt_strncpy(get_buffer->data, &p_src[i + 4], data_field_len);
                        rt_memmove(get_buffer->data, &p_src[i + 4], data_field_len);
                    }
                    LOG_D("HR_GetDataFrame Success!");

                    return get_buffer;
                }
            }
        }
    }
    LOG_E("HR_GetDataFrame Fail!");

    return get_buffer;
}

rt_uint8_t HR_GenerateRawFrame(TsRawFrameData* pRawData , rt_uint8_t c1, TeFrameC2 c2, const rt_uint8_t* p_src, rt_uint8_t src_len)
{
    pRawData->len = src_len + 6;

    rt_memset(pRawData->buf, 0 , sizeof(pRawData->buf));

    pRawData->buf[0] = FRAME_HEADER;
    pRawData->buf[1] = c1;
    pRawData->buf[2] = c2;
    pRawData->buf[3] = src_len;

    rt_memmove(&pRawData->buf[4], p_src, src_len);

    // 从帧起始符开始到校验码之前所有字节的和的模256
    // ，即各字节不计超过255的溢出值的二进制算术和。
    pRawData->buf[pRawData->len - 2] = HR_CheckSum(&pRawData->buf[0], pRawData->len - 2);
    pRawData->buf[pRawData->len - 1] = FRAME_TAIL;

//     LOG_HEX("HrRawData", 16, &pRawData->buf[0], pRawData->len);

    return RT_EOK;
}

rt_uint8_t HR_ProcessData(const TsFrameData *pHostFrameData)
{
    TuFlashProductTimeLimitFrame ReadLimitTime = {0};
    TsRawFrameData RawData;
    rt_uint16_t value;
    rt_uint8_t data_field[64] = {0};
    rt_uint8_t c1, c2, data_len = 0;

    c1 = pHostFrameData->c1;
    c2 = pHostFrameData->c2;

    switch (c2)
    {
        case kNumOfRecords: // 查询各类记录的总数 AA 00 00 00 AA 55
        {
            LOG_D("kNumOfRecords");
            c1       = 0;
            data_len = sizeof(TsTotalRecords);
            if (Flash_GetTotalRecord((TsTotalRecords *)&data_field[0]) != RESET)
            {
                goto send;
            }
            break;
        }
        case kAlarmRecord: // 查询第n条探测器报警记录AA 01 01 00 AC 55
        {
            LOG_D("kAlarmRecord(%d)", c1);
            data_len = sizeof(TsRecordsTime);
            if (Flash_GetRecord(kAlarmRecord, c1, (TsRecordsTime *)data_field) != RESET)
            {
                if (data_field[0] == 0)
               {
                   rt_memset(data_field, 0, data_len);
               }
                goto send;
            }
            break;
        }
        case kAlarmRcyRecord: // 查询第n条探测器报警恢复记录  AA 01 02 00 AD 55
        {
            LOG_D("kAlarmRcyRecord(%d)", c1);
            data_len = sizeof(TsRecordsTime);
            if (Flash_GetRecord(kAlarmRcyRecord, c1, (TsRecordsTime *)data_field) != RESET)
            {
                goto send;
            }
            break;
        }
        case kFaultRecord: // 查询第n条探测器故障记录  AA 01 03 00 AE 55
        {
            LOG_D("kFaultRecord(%d)", c1);
            data_len = sizeof(TsRecordsTime);
            if (Flash_GetRecord(kFaultRecord, c1, (TsRecordsTime *)&data_field) != RESET)
            {
                goto send;
            }
            break;
        }
        case kFaultRcyRecord: // 查询第n条探测器故障恢复记录  AA 01 04 00 AF 55   AA 66 04 00 14 55
        {
            LOG_D("kFaultRcyRecord(%d)", c1);
            data_len = sizeof(TsRecordsTime);
            if (Flash_GetRecord(kFaultRcyRecord, c1, (TsRecordsTime *)data_field) != RESET)
            {
                goto send;
            }
            break;
        }
        case kPowerFailureRecord: // 查询第n条探测器掉电记录 AA 01 05 00 B0 55
        {
            LOG_D("kPowerFailureRecord(%d)", c1);
            data_len = sizeof(TsRecordsTime);
            if (Flash_GetRecord(kPowerFailureRecord, c1, (TsRecordsTime *)data_field) != RESET)
            {
                goto send;
            }

            break;
        }
        case kPowerOnRecord: // 查询第n条探测器上电记录  AA 01 06 00 B1 55
        {
            LOG_D("kPowerOnRecord(%d)", c1);
            data_len = sizeof(TsRecordsTime);
            if (Flash_GetRecord(kPowerOnRecord, c1, (TsRecordsTime *)data_field) == READY)
            {
                goto send;
            }
            break;
        }
        case kSensorFailureRecord: // 查询气体传感器失效记录  AA 00 07 00 B1 55
        {
            LOG_D("kSensorFailureRecord(%d)", c1);

            data_len = 7;
            if (Flash_GetRecord(kSensorFailureRecord, 1, (TsRecordsTime *)&data_field[0]) != RESET)
            {

                if (c1 != 0)//接收到的数据不对，直接返回0
                {
                    rt_memset(data_field, 0, data_len);
                }
                else
                {
                    if (data_field[1] == 0)// 未失效
                    {
                        rt_memset(data_field, 0, data_len);
                    }
                    else//失效
                    {
                        data_field[0] = 1;
                    }
                }
                goto send;
            }
            break;
        }
        case kGetCurrentTime: // AA 00 08 00 B2 55
        {
            LOG_D("kGetCurrentTime");

            data_len = sizeof(TsRecordsTime);
            BSP_Get_RTC_Time();

            data_field[0] = (uint8_t)((RtcDateTime.year >> 8)&(0XFF));
            data_field[1] = (uint8_t)((RtcDateTime.year) & (0xFF));
            data_field[2] = RtcDateTime.month;
            data_field[3] = RtcDateTime.day;
            data_field[4] = RtcDateTime.hour;
            data_field[5] = RtcDateTime.minute;

            goto send;

            break;
        }
        /*扩展命令*/
        case kGetCurrentTimeSecond: // AA 00 09 00 B3 55
        {
            LOG_D("kGetCurrentTimeSecond");

            data_len = 7;
            BSP_Get_RTC_Time();

            data_field[0] = (uint8_t)((RtcDateTime.year >> 8)&(0XFF));
            data_field[1] = (uint8_t)((RtcDateTime.year) & (0xFF));
            data_field[2] = RtcDateTime.month;
            data_field[3] = RtcDateTime.day;
            data_field[4] = RtcDateTime.hour;
            data_field[5] = RtcDateTime.minute;
            data_field[6] = RtcDateTime.second;

            goto send;

            break;
        }
        case kSetCurrentTime: /*2024-03-25 17:34:56   AA 00 0A 07 07 E8 03 19 11 22 38 31 55*/
        {
            LOG_D("kSetCurrentTime");

            data_len        = 7;

            RtcDateTime.hour   = pHostFrameData->data[4];
            RtcDateTime.minute = pHostFrameData->data[5];
            RtcDateTime.second = pHostFrameData->data[6];
            RtcDateTime.day    = pHostFrameData->data[3];
            RtcDateTime.month   = pHostFrameData->data[2];
            RtcDateTime.year    = (rt_uint16_t)(pHostFrameData->data[0] << 8 | pHostFrameData->data[1]);

           RTC_Set(RtcDateTime.year, RtcDateTime.month, RtcDateTime.day,
                   RtcDateTime.hour, RtcDateTime.minute, RtcDateTime.second);

            BSP_Get_RTC_Time();
            data_field[0] = (uint8_t)((RtcDateTime.year >> 8)&(0XFF));
            data_field[1] = (uint8_t)((RtcDateTime.year) & (0xFF));
            data_field[2] = RtcDateTime.month;
            data_field[3] = RtcDateTime.day;
            data_field[4] = RtcDateTime.hour;
            data_field[5] = RtcDateTime.minute;
            data_field[6] = RtcDateTime.second;

            goto send;

            break;
        }
        case kSetFactoryTime: /*2024-03-26 9:30:00  AA 00 0B 07 07 E8 03 1A 09 1E 00 EF 55*/
        {
            LOG_D("kSetFactoryTime\r\n");
            data_len = 7;
            Flash_SetProductTimeLimit((rt_uint16_t)(pHostFrameData->data[0] << 8 | pHostFrameData->data[1]), pHostFrameData->data[2],
                                    pHostFrameData->data[3], pHostFrameData->data[4], pHostFrameData->data[5], pHostFrameData->data[6], kFactoryTimeId);
            Flash_GetProductTimeLimit(&ReadLimitTime, kFactoryTimeId);

            data_field[0] = (uint8_t)(ReadLimitTime.Struct.year >> 8);
            data_field[1] = (uint8_t)(ReadLimitTime.Struct.year);
            data_field[2] = ReadLimitTime.Struct.month;
            data_field[3] = ReadLimitTime.Struct.day;
            data_field[4] = ReadLimitTime.Struct.hour;
            data_field[5] = ReadLimitTime.Struct.minute;
            data_field[6] = ReadLimitTime.Struct.second;

            goto send;

            break;
        }
        case kSetExpirationTime: /*2028-3-26 9:30:00  AA 00 0C 07 07 EC 03 1A 09 1E 00 F4 55*/
        {
            LOG_D("kSetExpirationTime");

            data_len = 7;

            Flash_SetProductTimeLimit((uint16_t)(pHostFrameData->data[0] << 8 | pHostFrameData->data[1]), pHostFrameData->data[2], pHostFrameData->data[3], pHostFrameData->data[4], pHostFrameData->data[5], pHostFrameData->data[6], kExpirationTimeId);
            Flash_GetProductTimeLimit(&ReadLimitTime, kExpirationTimeId);

            data_field[0] = (uint8_t)(ReadLimitTime.Struct.year >> 8);
            data_field[1] = (uint8_t)(ReadLimitTime.Struct.year);
            data_field[2] = ReadLimitTime.Struct.month;
            data_field[3] = ReadLimitTime.Struct.day;
            data_field[4] = ReadLimitTime.Struct.hour;
            data_field[5] = ReadLimitTime.Struct.minute;
            data_field[6] = ReadLimitTime.Struct.second;

            goto send;

            break;
        }
       case kSetAlarmValue: /* 3000 AA 00 0D 02 0B B8 7C 55 */
       {
           // logDebug("设置传感器标定的报警阈值");
           data_len = 2;
           value    = (uint16_t)(pHostFrameData->data[0] << 8) | pHostFrameData->data[1];
           Flash_Set_AlarmLel(value);
           LOG_D("kSetAlarmLelValue: %d\r\n", Flash_Get_AlarmLel());
           data_field[0] = (uint8_t)(value >> 8);
           data_field[1] = (uint8_t)(value);

           goto send;

           break;
       }
       case kGetAlarmValue: //*AA 00 0F 00 B9 55*/
       {
           // logDebug("获取传感器标定的报警阈值");
           data_len = 2;
           value    = Flash_Get_AlarmLel();
           LOG_D("kGetAlarmValue: %d\r\n", value);
           data_field[0] = (uint8_t)(value >> 8);
           data_field[1] = (uint8_t)(value);

           goto send;
           break;
       }
        default:
            break;
    }

send:
    {
        if ((c1 == 0)&&((0 < c2)&(c2 < kSensorFailureRecord)))
        {
            rt_memset(data_field, 0, data_len);
        }
        HR_GenerateRawFrame(&RawData, c1, (TeFrameC2)c2, data_field, data_len);
        rt_device_write(rt_hr_device, 0, &RawData.buf[0], RawData.len);
        return 0;
    }

    return RT_EOK;
}


/* 接收数据回调函数 */
static rt_err_t Uart2_Rec_Cb(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&hr_rx_sem);
    return RT_EOK;
}

static void hr_thread_entry(void *param)
{
    rt_uint8_t buf[32];
    while (1)
    {
        rt_sem_take(&hr_rx_sem, RT_WAITING_FOREVER);//等待信号量
        UART2_RxCheck();
        LOG_D("uart2_revok_sem");

        rt_uint8_t buf_len = lwrb_get_full(&uart2_rx_rb);
        LOG_D("buf_len = %d\n", buf_len);
        lwrb_read(&uart2_rx_rb, buf, buf_len);
        LOG_HEX("uart2_rx_rb", 16, &buf[0], buf_len);

        if (buf_len >= HOST_FRAME_MIN_LEN)
        {
            TsFrameData *HostFrameData = HR_GetFrameData(buf, buf_len);
            if(HostFrameData != RT_NULL)
            {
                LOG_HEX("HostFrameData", 16, &HostFrameData->data[0], HostFrameData->len);//数据段
                HR_ProcessData(HostFrameData);
            }
            rt_free(HostFrameData);
            HostFrameData = RT_NULL;
        }
//        rt_thread_delay(100);
    }
}


int BSP_HR_Init(void)
{
    rt_err_t ret = RT_EOK;
   struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  /* 初始化配置参数 */

   /* 查找系统中的串口设备 */
   rt_hr_device = rt_device_find("uart2");
   if (!rt_hr_device)
   {
       rt_kprintf("find %s failed!\n", HR_UART);
       return RT_ERROR;
   }
   else
   {
       rt_kprintf("find %s ok!\n", HR_UART);
   }

   config.baud_rate = BAUD_RATE_4800; //修改波特率
   config.parity = PARITY_EVEN;//偶校验
   config.data_bits = DATA_BITS_9;//第9位为校验位

   if (rt_device_control(rt_hr_device, RT_DEVICE_CTRL_CONFIG, &config) != RT_EOK)//初始化设置
   {
       rt_kprintf("rt_device_control failed!\n");
       return RT_ERROR;
   }

   /* 初始化信号量 */
   rt_sem_init(&hr_rx_sem, "hr_sem", 0, RT_IPC_FLAG_FIFO);
   /* 以中断接收模式打开串口设备 */
   if (rt_device_open(rt_hr_device, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
   {
       rt_kprintf("rt_device_open failed!\n");
       return RT_ERROR;
   }
   /* 设置接收回调函数 */
  if (rt_device_set_rx_indicate(rt_hr_device, Uart2_Rec_Cb) != RT_EOK)
  {
      rt_kprintf("rt_device_set_rx_indicate failed!\n");
      return RT_ERROR;
  }

    rt_thread_init(&hr_thread,
                   "hr_thread",
                   hr_thread_entry,
                   RT_NULL,
                   &hr_thread_stack[0],
                   sizeof(hr_thread_stack),
                   HR_THREAD_PRIORITY, HR_THREAD_TIMESLICE);

    /* 创建成功则启动线程 */
     if (ret == RT_EOK)
    {
        LOG_I("rt_thread_startup hr_thread");
        rt_thread_startup(&hr_thread);
    }
    else
    {
        LOG_I("rt_thread_init sensor_thread Failed...");
        ret = RT_ERROR;
    }
    return RT_EOK;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_PREV_EXPORT(BSP_HR_Init);
#endif



