/*
 * rtc.c
 *
 *  Created on: 12/11/2016
 *      Author: Enrique
 */


#include "MKL25Z4.h"
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "rtc.h"

uint32_t sec;
uint32_t currSeconds;
rtc_datetime_t date;
rtc_config_t rtcConfig;
rtc_datetime_t Temp_date;
rtc_datetime_t RTC_Alarm;


volatile uint8_t alarm_ringing = 0;

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
	date.month = 11;
	date.day = 20;
	date.hour = 13;
	date.minute = 50;
	date.second = 00;

	RTC_Alarm.year = 2016;
	RTC_Alarm.month = 11;
	RTC_Alarm.day = 20;
	RTC_Alarm.hour = 13;
	RTC_Alarm.minute = 50;
	RTC_Alarm.second = 55;

	rtc_SetDatetime(&date);

	rtc_SetAlarm(&RTC_Alarm);
	GPIO_WritePinOutput(GPIOC,7,0);
	rtc_Start();
	EnableIRQ(RTC_IRQn);
	//EnableIRQ(RTC_Seconds_IRQn);
	RTC_EnableInterrupts(RTC, kRTC_AlarmInterruptEnable);
	//RTC_EnableInterrupts(RTC, kRTC_SecondsInterruptEnable);
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

}

void rtc_GetDatetime(rtc_datetime_t *Datetime )
{
	if(Datetime != NULL)
	{
		RTC_GetDatetime(RTC, Datetime);
	}
}

uint8_t rtc_SetAlarm(rtc_datetime_t * Datetime )
{
	uint8_t loc_status = 0;
	status_t loc_ret = kStatus_Fail;
	if (Datetime != NULL)
	{

		loc_ret = RTC_SetAlarm(RTC, Datetime);
		if(loc_ret != kStatus_Success)
		{
			loc_status = 1;
		}
	}
	return loc_status;
}


void rtc_GetAlarm(rtc_datetime_t * Datetime )
{
	if (Datetime != NULL)
	{
		RTC_GetAlarm(RTC, Datetime);
	}
}


void RTC_IRQHandler(void)
{
	GPIO_WritePinOutput(GPIOC,7,1);
	alarm_ringing = 1;
	rtc_Stop();
	RTC_ClearStatusFlags(RTC, kRTC_AlarmInterruptEnable);
	rtc_Start();
}

void RTC_Seconds_IRQHandler(void)
{
	GPIO_WritePinOutput(GPIOC,7,1);
	alarm_ringing = 1;
	rtc_Stop();
	RTC_ClearStatusFlags(RTC, kRTC_SecondsInterruptEnable);
	rtc_Start();
}


