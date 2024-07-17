#ifndef __BSP_UART_H__
#define  __BSP_UART_H__


#include "ch32v30x.h"
#include "rtthread.h"



typedef enum
{
    kUsartRxStart,
    kUsartRxComplate,
}Te_State_e;

typedef struct
{
    Te_State_e usart_rx_state;
}Ts_StateStruct;



//QX
typedef struct
{
    char concentration[16];//浓度
    char temperature[16];//温度
    char sensor_temperature[16];//激光器温度
    char signal_strength[16];//信号强度
    char mu_factor[16];//放大系数
    char sensor_status[4];//传感器状态
    char checksum[4];//校验

} TsSensoStrData;


typedef struct
{

    double concentration;//浓度
    double temperature;//温度
    double sensor_temperature;//激光器温度
    double signal_strength;//信号强度
    double mu_factor;//放大系数
    rt_uint8_t sensor_status;//传感器状态
    rt_uint8_t checksum;//校验

} TsSensorData;

//typedef struct
//{
//    float concentration;//浓度
//    float temperature;//温度
//    float sensor_temperature;//激光器温度
//    float signal_strength;//信号强度
//    float mu_factor;//放大系数
//    int sensor_status;//传感器状态
//    int checksum;//校验
//
//} TsSensorData;

extern TsSensorData SensorData;
extern struct rt_semaphore uart4_rx_sem;









#endif //!__BSP_UART_H__

