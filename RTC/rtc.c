/*
 * rtc.c
 *
 *  Created on: 12/11/2016
 *      Author: Enrique
 */


#include "MKL25Z4.h"
#include "fsl_common.h"
#include "fsl_clock.h"
#include "rtc.h"

uint32_t sec;
uint32_t currSeconds;
rtc_datetime_t date;
rtc_config_t rtcConfig;
rtc_datetime_t Temp_date;

void rtc_init(void)
{


	/* Set PTC1 as RTC_CLKIN and select 32 KHz clock source for the RTC module*/
	PORT_SetPinMux(PORTC, 1u, kPORT_MuxAsGpio);

	PORT_SetPinMux(PORTC, 3u, kPORT_MuxAlt5);

	SIM->SOPT1 = SIM_SOPT1_OSC32KSEL(2);

	/*Selects either the RTC 1 Hz clock or the OSC clock to be output on the RTC_CLKOUT pin.
	0 RTC 1 Hz clock is output on the RTC_CLKOUT pin.
	1 OSCERCLK clock is output on the RTC_CLKOUT pin*/
	CLOCK_SetRtcClkOutClock(0u);

	CLOCK_SetClkOutClock(4u);

	RTC_GetDefaultConfig(&rtcConfig);

	RTC_Init(RTC, &rtcConfig);
	//RTC_StopTimer(RTC);

	date.year = 2016;
	date.day = 13;
	date.hour = 13;
	date.minute = 50;
	date.second = 00;

	rtc_SetDatetime(&date);
}

void rtc_Start(void)
{
	RTC_StartTimer(RTC);
}

void rtc_Stop(void)
{
	RTC_StopTimer(RTC);
}

void rtc_SetDatetime(rtc_datetime_t *Datetime )
{
	rtc_Stop();
	RTC_SetDatetime(RTC, Datetime);
	rtc_Start();

}

void rtc_GetDatetime(rtc_datetime_t *Datetime )
{
	RTC_GetDatetime(RTC, Datetime);
}

void rtc_SetAlarm(rtc_datetime_t * Datetime )
{}
void rtc_GetAlarm(rtc_datetime_t * Datetime )
{}
