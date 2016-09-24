/*
 * io.c
 *
 *  Created on: 23/09/2016
 *      Author: Enrique
 */
#include "io.h"


void io_Pin_Cfg(GPIO_Type GPIO, PORT_Type PORT, uint8_t PIN, gpio_pin_direction_t DIR)
{

	gpio_pin_config_t GPIO_Struct;
	port_pin_config_t PORT_Struct;


	GPIO_Struct.pinDirection = DIR;
	GPIO_Struct.outputLogic = DIR;



	GPIO_PinInit(GPIO,PIN,&PinStruct);
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


