/*
 * apptasks.c
 *
 *  Created on: 25/07/2016
 *      Author: uid87057
 */


#include "apptasks.h"
#include "fsl_gpio.h"

uint8_t value = 1;

void apptask_5ms(void)
{


}

void apptask_20ms()
{
	GPIO_TogglePinsOutput(GPIOC, 0x80);
}

void apptask_100ms(void)
{
}

void apptask_1s(void)
{
}

void apptask_idle(void)
{

}
