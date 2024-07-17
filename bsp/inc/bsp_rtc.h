#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__


#include "board.h"
#include "rtdef.h"


typedef enum
{
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday,
}TeWeek;

typedef struct
{
  uint8_t   second;
  uint8_t   minute;
  uint8_t   hour;
  uint8_t   day;
  uint8_t   month;
  uint16_t  year;
  uint8_t   rtc_wday;
  TeWeek   week;

}TsRtcDateTime;


extern char week[7][10];
extern uint8_t endtimer_check_flag;
extern TsRtcDateTime RtcDateTime;

rt_uint8_t BSP_Rtc_Init(void);
void BSP_Get_RTC_Time(void);
rt_uint8_t RTC_Set(rt_uint16_t syear, rt_uint8_t smon, rt_uint8_t sday, rt_uint8_t hour, rt_uint8_t min, rt_uint8_t sec);


























#endif //!__BSP_RTC_H__



