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


    /*Reset*/
    io_Pin_Cfg(PORT_B, 8, kGPIO_DigitalInput, kPORT_PullDown);

    /*Pin Cfg Blue LED*/
    io_Pin_Cfg(PORT_D, 1, kGPIO_DigitalOutput, kPORT_PullUp);

    /*Pin Cfg for LCD 1602*/
	io_Pin_Cfg(PORT_B, 0, kGPIO_DigitalOutput, kPORT_PullUp);
	io_Pin_Cfg(PORT_B, 1, kGPIO_DigitalOutput, kPORT_PullUp);
	io_Pin_Cfg(PORT_B, 2, kGPIO_DigitalOutput, kPORT_PullUp);
	io_Pin_Cfg(PORT_B, 3, kGPIO_DigitalOutput, kPORT_PullUp);

	io_Pin_Cfg(PORT_C, 0, kGPIO_DigitalOutput, kPORT_PullUp);
	io_Pin_Cfg(PORT_C, 4, kGPIO_DigitalOutput, kPORT_PullUp);
    io_Pin_Cfg(PORT_C, 5, kGPIO_DigitalOutput, kPORT_PullUp);
    io_Pin_Cfg(PORT_C, 6, kGPIO_DigitalOutput, kPORT_PullUp);
    io_Pin_Cfg(PORT_C, 7, kGPIO_DigitalOutput, kPORT_PullUp);

    /*LCD RS*/
    io_Pin_Cfg(PORT_A, 1, kGPIO_DigitalOutput, kPORT_PullDown);

    GPIO_ClearPinsOutput(GPIOB, 0x0Fu);
    GPIO_ClearPinsOutput(GPIOC, 0xF1u);
    GPIO_WritePinOutput(GPIOA, 1, 0);
    //GPIO_WritePinOutput(GPIOC, 0, 0);
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
void io_Pin_Init(void)
{

}

/*-----------------------------------*/
/* Function:                         */
/* input:                            */
/* return:                           */
/* Note:                             */
/*-----------------------------------*/
int8_t io_Pin_Cfg(IO_PORT IO_PORT, uint8_t PIN, gpio_pin_direction_t DIR, uint16_t PullSelect)
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
        PORT_Struct.pullSelect = PullSelect;
        PORT_Struct.mux = kPORT_MuxAsGpio;
        PORT_Struct.passiveFilterEnable = kPORT_PassiveFilterDisable;
        PORT_Struct.driveStrength = kPORT_LowDriveStrength;
        PORT_Struct.slewRate = kPORT_SlowSlewRate;

        CLOCK_EnableClock(port_clk);

        PORT_SetPinMux(a_PORT[IO_PORT], PIN, kPORT_MuxAsGpio);

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

/*-----------------------------------*/
/* Function:                         */
/* input:                            */
/* return:                           */
/* Note:                             */
/*-----------------------------------*/




