/*
 * io.c
 *
 *  Created on: 23/09/2016
 *      Author: Enrique
 */
#include "io.h"

void io_init(void)
{
    io_Pin_Cfg(PORT_C, 7, kGPIO_DigitalOutput);
    io_Pin_Cfg(PORT_C, 6, kGPIO_DigitalOutput);
    io_Pin_Cfg(PORT_C, 5, kGPIO_DigitalOutput);
}


int8_t io_Pin_Cfg(IO_PORT IO_PORT, uint8_t PIN, gpio_pin_direction_t DIR)
{

    int8_t result = 0;
	gpio_pin_config_t GPIO_Struct;
	port_pin_config_t PORT_Struct;
	clock_ip_name_t port_clk;
	GPIO_Type* GPIO;
	PORT_Type* PORT;

	switch(IO_PORT)
	{
	    case PORT_A:
	        port_clk = kCLOCK_PortA;
	        GPIO = GPIOA;
	        PORT = PORTA;
	        break;

	    case PORT_B:
	        port_clk = kCLOCK_PortB;
	        GPIO= GPIOB;
	        PORT= PORTB;
	        break;

	    case PORT_C:
	        port_clk = kCLOCK_PortC;
	        GPIO = GPIOC;
	        PORT = PORTC;
	        break;

	    case PORT_D:
	        port_clk = kCLOCK_PortD;
	        GPIO = GPIOC;
	        PORT= PORTC;
	        break;

	    case PORT_E:
	        port_clk = kCLOCK_PortE;
	        GPIO = GPIOE;
	        PORT = PORTE;
	        break;

	    default:
	        result = -1;
	}

	if(result == 0)
	{
	    /* Unlock the port clock */
        CLOCK_EnableClock(port_clk);


        // Define a digital input pin PCR configuration
        PORT_Struct.pullSelect = kPORT_PullUp;
        PORT_Struct.mux = kPORT_MuxAsGpio;
        PORT_Struct.passiveFilterEnable = kPORT_PassiveFilterDisable;
        PORT_Struct.driveStrength = kPORT_LowDriveStrength;
        PORT_Struct.slewRate = kPORT_SlowSlewRate;

        PORT_SetPinConfig(PORT, 7U, &PORT_Struct);

        GPIO_Struct.pinDirection = DIR;
        GPIO_Struct.outputLogic = DIR;
        GPIO_PinInit(GPIO,PIN,&GPIO_Struct);
	}
	else
	{/*Do nothing*/}

	return result;

}




