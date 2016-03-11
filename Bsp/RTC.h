#ifndef RTC_H
#define RTC_H


#include "stm32f10x.h"
#include "time.h"

/*********************************************************************
*
*       Functions
*
**********************************************************************
*/
u32 Time_GetRTCTimeByUnix(void);
void Time_SetRTCTimeByCalendar(struct tm t);
void Time_SetRTCTimeByUnix(time_t t);
void RTC_Init(struct tm time_now);
struct tm Time_ConvUnixToCalendar(time_t t);


#endif







