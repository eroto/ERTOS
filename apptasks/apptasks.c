/*
 * apptasks.c
 *
 *  Created on: 25/07/2016
 *      Author: uid87057
 */


#include "apptasks.h"
#include "fsl_gpio.h"
#include "uart.h"


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
    uart_Send(sendData, 15);
}

void apptask_1s(void)
{
}

void apptask_idle(void)
{

}
