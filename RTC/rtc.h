/*
 * rtc.h
 *
 *  Created on: 12/11/2016
 *      Author: Enrique
 */

#ifndef RTC_H_
#define RTC_H_
#include "fsl_rtc.h"
#include "fsl_port.h"

extern rtc_datetime_t Temp_date;

void rtc_init(void);
void rtc_Start(void);
void rtc_Stop(void);
void rtc_SetDatetime(rtc_datetime_t * Datetime );
void rtc_GetDatetime(rtc_datetime_t * Datetime );
uint8_t rtc_SetAlarm(rtc_datetime_t * Datetime );
void rtc_GetAlarm(rtc_datetime_t * Datetime );


#endif /* RTC_H_ */
