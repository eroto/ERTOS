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
#include "io.h"
#include "LCD1602A.h"

uint8_t value = 1;
uint32_t deb = 0;


#define PTB_Dx_clear	0x0Fu
#define PTC_Dx_clear	0xF0u




void apptask_5ms(void)
{
    //GPIO_TogglePinsOutput(GPIOC, 1<<7);
	uart_main();
}




void apptask_20ms()
{


}

void apptask_100ms(void)
{
	//GPIO_TogglePinsOutput(GPIOC, 1<<3);
	RTC_SendClock();

	PublicSendData[0] = '-';

	PublicSendData[1] = io_Read_Pin(GPIOB,8);

	uart_ReqTx(PublicSendData, 2);

	Disp_Main();
}

void apptask_1s(void)
{
	//GPIO_TogglePinsOutput(GPIOC, 1<<7);
	GPIO_WritePinOutput(GPIOD, 1, FALSE);

}

void apptask_idle(void)
{

}
