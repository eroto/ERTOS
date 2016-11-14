/*
 * apptasks.c
 *
 *  Created on: 25/07/2016
 *      Author: uid87057
 */

#include <string.h>
#include "apptasks.h"
#include "fsl_gpio.h"
#include "uart.h"
#include "rtc.h"

uint8_t value = 1;

void apptask_5ms(void)
{
    GPIO_TogglePinsOutput(GPIOC, 1<<7);
}

void apptask_20ms()
{
	GPIO_TogglePinsOutput(GPIOC, 1<<0);
}

void apptask_100ms(void)
{
    GPIO_TogglePinsOutput(GPIOC, 1<<3);

    rtc_GetDatetime(&Temp_date);

    //memcpy (sendData, (const *) &Temp_date,7);

    sendData[0]=Temp_date.hour;
    sendData[1]=Temp_date.minute;
    sendData[2]=Temp_date.second;

    uart_Send(sendData, 3);
}

void apptask_1s(void)
{
}

void apptask_idle(void)
{

}
