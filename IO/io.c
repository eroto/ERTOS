/*
 * io.c
 *
 *  Created on: 23/09/2016
 *      Author: Enrique
 */
#include "io.h"

/*Global Variables*/
/*----------------------------------------------------------------------------------------------*/
GPIO_Type* a_GPIO[PORT_MAX_NUM] = GPIO_BASE_PTRS;
PORT_Type* a_PORT[PORT_MAX_NUM] = PORT_BASE_PTRS;
clock_ip_name_t a_PortClk[PORT_MAX_NUM] = {kCLOCK_PortA, kCLOCK_PortB, kCLOCK_PortC, kCLOCK_PortD, kCLOCK_PortE};


/*Function prototype*/
/*----------------------------------------------------------------------------------------------*/
clock_ip_name_t io_Get_PortClk(IO_PORT PORT);

/*Functions*/
/*----------------------------------------------------------------------------------------------*/

/*-----------------------------------*/
/* Function:                         */
/* input:                            */
/* return:                           */
/* Note:                             */
/*-----------------------------------*/
void io_init(void)
{
    io_Pin_Cfg(PORT_C, 0, kGPIO_DigitalOutput);
    io_Pin_Cfg(PORT_C, 7, kGPIO_DigitalOutput);
    io_Pin_Cfg(PORT_C, 3, kGPIO_DigitalOutput);
}


/*-----------------------------------*/
/* Function:                         */
/* input:                            */
/* return:                           */
/* Note:                             */
/*-----------------------------------*/
clock_ip_name_t io_Get_PortClk(IO_PORT PORT)
{
    clock_ip_name_t result = kCLOCK_IpInvalid;

    if (PORT < PORT_MAX_NUM)
    {
        result = a_PortClk[PORT];
    }

    return result;
}



/*-----------------------------------*/
/* Function:                         */
/* input:                            */
/* return:                           */
/* Note:                             */
/*-----------------------------------*/
int8_t io_Pin_Cfg(IO_PORT IO_PORT, uint8_t PIN, gpio_pin_direction_t DIR)
{

    int8_t result = 0;
    clock_ip_name_t port_clk = kCLOCK_IpInvalid;
	gpio_pin_config_t GPIO_Struct;
	port_pin_config_t PORT_Struct;

	port_clk = io_Get_PortClk(IO_PORT);

	if(port_clk == kCLOCK_IpInvalid)
	{
	    result = -1;
	}
	else
	{
        // Define a digital input pin PCR configuration
        PORT_Struct.pullSelect = kPORT_PullUp;
        PORT_Struct.mux = kPORT_MuxAsGpio;
        PORT_Struct.passiveFilterEnable = kPORT_PassiveFilterDisable;
        PORT_Struct.driveStrength = kPORT_LowDriveStrength;
        PORT_Struct.slewRate = kPORT_SlowSlewRate;

        CLOCK_EnableClock(port_clk);
        PORT_SetPinConfig(a_PORT[IO_PORT], PIN, &PORT_Struct);

        GPIO_Struct.pinDirection = DIR;
        GPIO_Struct.outputLogic = DIR;
        GPIO_PinInit(a_GPIO[IO_PORT],PIN,&GPIO_Struct);
    }

	return result;
}

/*-----------------------------------*/
/* Function:                         */
/* input:                            */
/* return:                           */
/* Note:                             */
/*-----------------------------------*/
int8_t io_Read_Pin_Cfg(IO_PORT PORT, uint8_t PIN)
{
    uint8_t PinDir = 0;

    if(PORT >= PORT_MAX_NUM)
    {
        PinDir = -1;
    }
    else
    {
        PinDir = (a_GPIO[PORT]->PDDR & (1 << PIN));
    }

    return PinDir;
}




