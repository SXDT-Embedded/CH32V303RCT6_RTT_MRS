#include <bsp_sensor.h>
#include "ringbuffer.h"
#include "drv_usart.h"
#include <rtdevice.h>
#include <rtthread.h>
#include "stdlib.h"
#include "stdio.h"
#include "user_sys.h"


#define LOG_TAG "bsp_uart"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

#define SENSOR_UART "uart4"


#define SENSOR_RX_BUF_LEN 64
#define THREAD_STACK_SIZE 2560

#define THREAD1_PRIORITY 25
#define THREAD2_PRIORITY 24
#define THREAD_TIMESLICE 5

ring_buf_t Sensor_Rx_t = {0};
TsSensorData SensorData = {0};
rt_uint16_t uart4_rx_cnt = 0;
Ts_StateStruct Usart4_State;


/* ���ڽ�����Ϣ���ź��� */
struct rt_semaphore uart4_rx_sem;

static rt_device_t rt_sensor_device;

static struct rt_thread sensor_thread1 = {0};


ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t sensor_thread1_stack[THREAD_STACK_SIZE] = {0};


// ���ڽ���
unsigned int BSP_Uart4_Receive_Data(void *buf, unsigned int len)
{
    return ring_buf_get(&Sensor_Rx_t, (unsigned char *)buf, len);
}

// ���ַ����������У��
static unsigned char BSP_Xor_Checksum(char *str, int len)
{
    unsigned char xorvalue = 0;

    if (str == RT_NULL || len < 2)
    {
        // �������򷵻�
        return 0; // ����ѡ���׳��쳣
    }
    for (int i = 0; i < len - 4; i++) // У��ֵǰ��Ŀո�Ҫ��
    {
        xorvalue ^= str[i];
    }

    return xorvalue;
}

// ���ַ�������ȡ���������ݲ��洢���ṹ����
TsSensorData Extract_SensorData(const char *str)
{
    TsSensoStrData SensoStrData = {0};
    TsSensorData data = {0};
    //A+000.00 +31.1 0657.80 +18.1 B+008.0 00 12
    if (sscanf(str, "A+%6s %6s %8s %6s B+%6s %2s %2s",
            SensoStrData.concentration, SensoStrData.temperature,SensoStrData.sensor_temperature, SensoStrData.signal_strength,
            SensoStrData.mu_factor, SensoStrData.sensor_status, SensoStrData.checksum) == 7)
    {
        data.concentration = (float)atof(SensoStrData.concentration);
        data.temperature = (float)atof(SensoStrData.temperature);
        data.sensor_temperature = (float)atof(SensoStrData.sensor_temperature);
        data.signal_strength = (float)atof(SensoStrData.signal_strength);
        data.mu_factor = (float)atof(SensoStrData.mu_factor);
        data.sensor_status = (rt_uint8_t)strtol(SensoStrData.sensor_status, NULL, 16);
        data.checksum = (rt_uint8_t)strtol(SensoStrData.checksum, NULL, 16);
    }
    else
    {
        LOG_E("Extract_SensorData failed...");
    }

   return data;
}

// ��ӡ�洢���������ݵĽṹ��
void Print_SensorData(TsSensorData data)
{
    //A+000.00 +31.1 0657.80 +18.1 B+008.0 00 12
    LOG_D("Sensor Data: %.2f, %.1f, %.2f, %.1f, %.1f, %02X, %02X\r\n",
            data.concentration, data.temperature, data.sensor_temperature, data.signal_strength, data.mu_factor, data.sensor_status, data.checksum);
}
MSH_CMD_EXPORT(Print_SensorData, data);


/* �������ݻص����� */
static rt_err_t Uart4_Rec_Cb(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&uart4_rx_sem);//�����ж�ʱ�ͷ��ź���
    return RT_EOK;
}

int Update_Sensor_Data(void)
{
    uint16_t read_length = 0;
    unsigned char xor_value = 0;
    int check_value = 0;
   char laser_sensor_rx_data[64] = {0};

   read_length = lwrb_get_full(&uart4_rx_rb);
   uart4_rx_cnt = 0;
   if (read_length < 35)
   {
       lwrb_free(&uart4_rx_rb);
       return -1;
   }
   else
   {
       lwrb_read(&uart4_rx_rb, laser_sensor_rx_data, read_length);
       xor_value = BSP_Xor_Checksum(laser_sensor_rx_data, read_length);    // �����ݽ������У��

       sscanf((&laser_sensor_rx_data[0] + read_length - 4), "%02X", &check_value); // ���ַ����л�ȡУ����
//
       if (xor_value == check_value)
       {
           SensorData = Extract_SensorData(laser_sensor_rx_data);
//           LOG_I("%s", laser_sensor_rx_data);//��ӡ���յ����ַ���
//           Print_SensorData(SensorData);
           return 0;
       }
       else
       {
           LOG_E("check error\r\n");
           LOG_I("%s", laser_sensor_rx_data);//��ӡ���յ����ַ���
           LOG_E("xor_value:%#X, check_value:%#X\r\n", xor_value, check_value);

           return -1;
       }
   }
    return 0;
}

static int Sensor_Gpio_Cfg(void)
{
    rt_pin_mode(SENSOR_POWER_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(SENSOR_POWER_PIN, PIN_HIGH);

    return RT_EOK;
}
INIT_PREV_EXPORT(Sensor_Gpio_Cfg);


static void Check_Concentration(rt_base_t *alarm_active)
{
    if (SensorData.concentration >= 0.5 && !(*alarm_active))
    {
        Send_Laser_Alarm_Event(kAlarmEvent);
        *alarm_active = RT_TRUE;  // ���±���״̬Ϊ����
    }
    else if (SensorData.concentration < 0.5 && *alarm_active)
    {
        Send_Laser_Alarm_Event(kAlarmRcyEvent);
        *alarm_active = RT_FALSE;  // ���±���״̬Ϊδ����
    }
}

static void Check_Fault(rt_base_t *fail_code)
{
    if (SensorData.sensor_status > 0 && !(*fail_code))
    {
        Send_Laser_Alarm_Event(kFaultEvent);
        *fail_code = RT_TRUE;  // ����״̬����
    }
    else if (SensorData.sensor_status == 0 && *fail_code)
    {
        Send_Laser_Alarm_Event(kFaultRcyEvent);
        *fail_code = RT_FALSE;  // ���ϻָ�״̬
    }
}

static void Sensor_Thread_Entry(void *parameter)
{
    rt_base_t alarm_active = RT_FALSE;  // ��ʼʱ����״̬Ϊδ����
    rt_base_t fail_code = RT_FALSE;  // ��ʼʱ����״̬Ϊ0

    LOG_I("Sensor_Thread_Entry\r\n");
    while (1)
    {
        rt_sem_take(&uart4_rx_sem, RT_WAITING_FOREVER);
        UART4_RxCheck();
        Update_Sensor_Data();
       if (SensorData.sensor_status > 0 || SensorData.concentration > 0)
       {
           Print_SensorData(SensorData);
       }
       Check_Concentration(&alarm_active);// ���Ũ��
       Check_Fault(&fail_code); // ������״̬

    }
}


static int Sensor_Rec_Data(void)
{
    rt_err_t ret = RT_EOK;
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  /* ��ʼ�����ò��� */

    /* ����ϵͳ�еĴ����豸 */
    rt_sensor_device = rt_device_find("uart4");
    if (!rt_sensor_device)
    {
        rt_kprintf("find %s failed!\n", SENSOR_UART);
        return RT_ERROR;
    }
    else
    {
        rt_kprintf("find %s OK!\n", SENSOR_UART);
    }

    config.baud_rate = BAUD_RATE_9600; //�޸Ĳ�����
    if (rt_device_control(rt_sensor_device, RT_DEVICE_CTRL_CONFIG, &config) != RT_EOK)
    {
        rt_kprintf("rt_device_control failed!\n");
        return RT_ERROR;
    }

    /* ��ʼ���ź��� */
    rt_sem_init(&uart4_rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    if (&uart4_rx_sem == RT_NULL)
    {
        rt_kprintf("create rx_sem semaphore failed.\n");
        return -1;
    }

    /* ���жϽ��ռ���ѯ����ģʽ�򿪴����豸��ֻ��һ�ξ���,��������λᵼ��Ӳ�����ϣ��������ص� */
    if (rt_device_open(rt_sensor_device, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
    {
        rt_kprintf("rt_device_open failed!\n");
        return RT_ERROR;
    }
    /* ���ý��ջص����� */
    if (rt_device_set_rx_indicate(rt_sensor_device, Uart4_Rec_Cb) != RT_EOK)
    {
        rt_kprintf("rt_device_set_rx_indicate failed!\n");
        return RT_ERROR;
    }

    /* ��̬��ʼ���߳� 1*/
      ret = rt_thread_init(&sensor_thread1,//���߳��������ݽ���
                           "uart4_rx",
                           Sensor_Thread_Entry,
                           RT_NULL,
                           &sensor_thread1_stack[0],
                           sizeof(sensor_thread1_stack),
                           THREAD1_PRIORITY,
                           THREAD_TIMESLICE);
      /* �����ɹ��������߳� */
        if (ret == RT_EOK)
       {
           LOG_I("rt_thread_startup sensor_thread1");
           rt_thread_startup(&sensor_thread1);
       }
       else
       {
           LOG_I("rt_thread_init sensor_thread Failed...");
           return ret;
       }
    return ret;
}
INIT_APP_EXPORT(Sensor_Rec_Data);
///* ������ msh �����б��� */
//MSH_CMD_EXPORT(Sensor_Rec_Data, example);
