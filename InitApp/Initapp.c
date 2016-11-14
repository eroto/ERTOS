/*
 * Initapp.c
 *
 *  Created on: 22/07/2016
 *      Author: uid87057
 */

#include <stdint.h>
#include "mytypedef.h"
#include "Initapp.h"
#include "Schr.h"
#include "io.h"
#include "fsl_cop.h"
#include "uart.h"
#include "rtc.h"



/*************************
*  Global Variables      *
*************************/
gpio_pin_config_t Test_PIN;
port_pin_config_t Test_PIN_Setup;


/*************************
*  Function Declaration   *
*************************/
void InitApp1(void);
void Initapp2(void);




/*************************
*  Function Definition   *
*************************/


void InitApp1(void)
{

	COP_Disable(SIM);
	io_init();
	Schr_Init();
	uart_init();
	rtc_init();

}


void InitApp2(void)
{



}
