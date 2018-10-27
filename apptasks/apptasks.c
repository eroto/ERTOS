/*
 * apptasks.c
 *
 *  Created on: 25/07/2016
 *      Author: uid87057
 */

#include <string.h>
#include "fsl_gpio.h"
#include "fsl_smc.h"
#include "fsl_pit.h"
#include "uart.h"
#include "rtc.h"
#include "io.h"
#include "LCD1602A.h"
#include "relayctrl.h"
#include "Schr.h"
#include "pm.h"
#include "timers.h"
#include "apptasks.h"

uint8_t value = 1;
uint32_t deb = 0;
uint8_t LowPowerMode_Ctr = 0;



#define PTB_Dx_clear	0x0Fu
#define PTC_Dx_clear	0xF0u



/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void apptask_5ms(void)
{
    //GPIO_TogglePinsOutput(GPIOC, 1<<7);
	uart_main();

	//io_Debounce_Pin_DI(PORT_B, 2);

	io_Debounce_Pin_DI_Low(r_Deb_Array);
	//io_Debounce_Pin_DI_Low(r_Deb_Array[1]);
	//io_Debounce_Pin_DI_Low(r_Deb_Array[2]);

}



/*-------------------------------------
 * Function: apptask_20ms
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void apptask_20ms()
{
	LCD1602A_Disp_Cfg_Refres();

}

/*-------------------------------------
 * Function: apptask_100ms
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void apptask_100ms(void)
{
	//GPIO_TogglePinsOutput(GPIOC, 1<<3);
	//RTC_SendClock();


	//PublicSendData[0] = '-';

	//PublicSendData[1] = io_Read_Pin(GPIOB,8);

	//uart_ReqTx(PublicSendData, 2);

	//Disp_Main();
}


/*-------------------------------------
 * Function: apptask_500ms
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void apptask_500ms(void)
{


	/*Send the RTC over UAT*/
	RTC_SendClock();

	LCD1602A_Disp_Main();
}

/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void apptask_1s(void)
{
	//GPIO_WritePinOutput(GPIOA, 1, 0); /*Enable RS*/
	GPIO_TogglePinsOutput(GPIOB,1);


	relayctrl_main();

	LowPowerMode_Ctr++;

	//if(LowPowerMode_Ctr >= 21)
	if(0)
	{
		/*STOP PIT*/
		PIT_StopTimer(PIT, kPIT_Chnl_0);

		/*Limit  BusClock to 800 KHz*/
		/*Limit  CoreClock to 4 MHz*/
		PwrMode_to_H2L();

		/*Reconfigure PIT to use 800 KHz freq.*/
		PIT_INIT_LP();

		SMC_SetPowerModeVlpr(SMC);

		while(kSMC_PowerStateVlpr != SMC_GetPowerModeState(SMC))
		{

			//SMC_SetPowerModeVlpw(SMC);
		}
		re_curOpMode = LOW_POWER;
	}

}


/*-------------------------------------
 * Function: LP_apptask_1s
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void LP_apptask_1s(void)
{
	/*Toggle Blue LED*/
	GPIO_TogglePinsOutput(GPIOD, 2);

}


/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void apptask_idle(void)
{

}
