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
#include "fsl_gpio.h"
#include "fsl_cop.h"
#include "fsl_port.h"



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

	TestPin_Setup();
	TestPin_Cfg();

	GPIO_SetPinsOutput(GPIOC,0x80);

	Schr_Init();
}


void InitApp2(void)
{



}



void TestPin_Cfg(void)
{

	Test_PIN.pinDirection = kGPIO_DigitalOutput;
	Test_PIN.outputLogic = 1;

	GPIO_PinInit(GPIOC,7U,&Test_PIN);
}


void TestPin_Setup(void)
{
	/* Ungate the port clock */
	CLOCK_EnableClock(kCLOCK_PortC);

	// Define a digital input pin PCR configuration
	Test_PIN_Setup.pullSelect = kPORT_PullUp;
	Test_PIN_Setup.mux = kPORT_MuxAsGpio;
	Test_PIN_Setup.passiveFilterEnable = kPORT_PassiveFilterDisable;
	Test_PIN_Setup.driveStrength = kPORT_LowDriveStrength;
	Test_PIN_Setup.slewRate = kPORT_SlowSlewRate;

	PORT_SetPinConfig(PORTC, 7U, &Test_PIN_Setup);

}
