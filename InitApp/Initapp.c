/*
 * Initapp.c
 *
 *  Created on: 22/07/2016
 *      Author: uid87057
 */


#include <stdint.h>
#include "Initapp.h"
#include "io.h"
#include "mytypedef.h"
#include "timers.h"
#include "Schr.h"

#include "fsl_cop.h"
#include "fsl_smc.h"
#include "uart.h"
#include "rtc.h"
#include "LCD1602A.h"



/*************************
*  Global Variables      *
*************************/
gpio_pin_config_t Test_PIN;
port_pin_config_t Test_PIN_Setup;


/*************************
*  Function Declaration   *
*************************/
void InitApp1(void);
void InitApp2(void);


/*************************
*  Function Definition   *
*************************/


/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void InitApp1(void)
{
	COP_Disable(SIM);
	io_init();
	timers_init();
	uart_init();
	rtc_init();
	SMC_SetPowerModeProtection(SMC,kSMC_AllowPowerModeVlp);

}


/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void InitApp2(void)
{
	relayctrl_init();
	LCD1602A_Init();
	Schr_Init();

}
