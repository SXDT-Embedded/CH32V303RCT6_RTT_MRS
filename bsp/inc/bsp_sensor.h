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
    char concentration[16];//Ũ��
    char temperature[16];//�¶�
    char sensor_temperature[16];//�������¶�
    char signal_strength[16];//�ź�ǿ��
    char mu_factor[16];//�Ŵ�ϵ��
    char sensor_status[4];//������״̬
    char checksum[4];//У��

} TsSensoStrData;


typedef struct
{

    double concentration;//Ũ��
    double temperature;//�¶�
    double sensor_temperature;//�������¶�
    double signal_strength;//�ź�ǿ��
    double mu_factor;//�Ŵ�ϵ��
    rt_uint8_t sensor_status;//������״̬
    rt_uint8_t checksum;//У��

} TsSensorData;

//typedef struct
//{
//    float concentration;//Ũ��
//    float temperature;//�¶�
//    float sensor_temperature;//�������¶�
//    float signal_strength;//�ź�ǿ��
//    float mu_factor;//�Ŵ�ϵ��
//    int sensor_status;//������״̬
//    int checksum;//У��
//
//} TsSensorData;

extern TsSensorData SensorData;
extern struct rt_semaphore uart4_rx_sem;









#endif //!__BSP_UART_H__

