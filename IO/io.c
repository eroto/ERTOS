/*
 * io.c
 *
 *  Created on: 20/05/2017
 *      Author: Enrique
 */
#include "io.h"
#include "LCD1602A.h"
#include "mytypedef.h"

/*Global Variables*/
/*----------------------------------------------------------------------------------------------*/
GPIO_Type* a_GPIO[PORT_MAX_NUM] = GPIO_BASE_PTRS;
PORT_Type* a_PORT[PORT_MAX_NUM] = PORT_BASE_PTRS;
clock_ip_name_t a_PortClk[PORT_MAX_NUM] = {kCLOCK_PortA, kCLOCK_PortB, kCLOCK_PortC, kCLOCK_PortD, kCLOCK_PortE};
DebStruct_t r_Deb_Array[3] = {{PORT_B,2,1000,0,{.By = 0}},{PORT_B,3,250,0,{.By = 0}},{PORT_E,20,250,0,{.By = 0}}};

/*Function prototype*/
/*----------------------------------------------------------------------------------------------*/
clock_ip_name_t io_Get_PortClk(IO_PORT PORT);
void io_Debounce_Pin_DI(IO_PORT PORT, uint8_t PIN);
void io_Debounce_Pin_DI_Low(DebStruct_t *Deb_Struct);

/*Functions*/
/*----------------------------------------------------------------------------------------------*/

/*-----------------------------------*/
/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void io_init(void)
{


    /*Reset*/
    io_Pin_Cfg(PORT_B, 8, kGPIO_DigitalInput, kPORT_PullDown);

    /*Pin Cfg Blue LED*/
    io_Pin_Cfg(PORT_D, 1, kGPIO_DigitalOutput, kPORT_PullUp);

    /*Pin Cfg for LCD 1602*/
#ifdef LCD_8_DATA_LINES
    io_Pin_Cfg(PORT_B, 0, kGPIO_DigitalOutput, kPORT_PullDown);/*LCD 1602 D0*/
	io_Pin_Cfg(PORT_B, 1, kGPIO_DigitalOutput, kPORT_PullDown);/*LCD 1602 D1*/
	io_Pin_Cfg(PORT_B, 2, kGPIO_DigitalOutput, kPORT_PullDown);/*LCD 1602 D2*/
	io_Pin_Cfg(PORT_B, 3, kGPIO_DigitalOutput, kPORT_PullDown);/*LCD 1602 D3*/
#else
	io_Pin_Cfg(PORT_B, 0, kGPIO_DigitalOutput, kPORT_PullDown);/*Relay IN1*/
	io_Pin_Cfg(PORT_B, 1, kGPIO_DigitalOutput, kPORT_PullDown);/*Relay IN2*/

	io_Pin_Cfg(PORT_B, 2, kGPIO_DigitalInput, kPORT_PullDown);/*Set Time*/
	io_Pin_Cfg(PORT_B, 3, kGPIO_DigitalInput, kPORT_PullDown);/*Time +*/
	io_Pin_Cfg(PORT_E, 20, kGPIO_DigitalInput, kPORT_PullDown);/*Time -*/
#endif

	io_Pin_Cfg(PORT_C, 0, kGPIO_DigitalOutput, kPORT_PullDown); /*LCD 1602 E*/

	io_Pin_Cfg(PORT_C, 4, kGPIO_DigitalOutput, kPORT_PullDown); /*LCD 1602 D4*/
    io_Pin_Cfg(PORT_C, 5, kGPIO_DigitalOutput, kPORT_PullDown); /*LCD 1602 D5*/
    io_Pin_Cfg(PORT_C, 6, kGPIO_DigitalOutput, kPORT_PullDown); /*LCD 1602 D6*/
    io_Pin_Cfg(PORT_C, 7, kGPIO_DigitalOutput, kPORT_PullDisable); /*LCD 1602 D7*/

    io_Pin_Cfg(PORT_A, 1, kGPIO_DigitalOutput, kPORT_PullDown);/*LCD RS*/
    io_Pin_Cfg(PORT_A, 2, kGPIO_DigitalOutput, kPORT_PullDown);/*LCD R/W*/

    GPIO_ClearPinsOutput(GPIOB, 0x0Fu);
    GPIO_ClearPinsOutput(GPIOC, 0xF1u);
    GPIO_WritePinOutput(GPIOA, 1, 0); /*LCD RS to 0*/
    GPIO_WritePinOutput(GPIOA, 2, 0); /*LCD RW to 0*/
    //GPIO_WritePinOutput(GPIOC, 0, 0);
}


/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
clock_ip_name_t io_Get_PortClk(IO_PORT PORT)
{
    clock_ip_name_t result = kCLOCK_IpInvalid;

    if (PORT < PORT_MAX_NUM)
    {
        result = a_PortClk[PORT];
    }

    return result;
}

/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void io_Pin_Init(void)
{

}

/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
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

		CLOCK_EnableClock(port_clk);

		if(DIR == kGPIO_DigitalOutput)
		{
			// Define a digital output pin PCR configuration
			PORT_Struct.pullSelect = kPORT_PullDisable;
			PORT_Struct.mux = kPORT_MuxAsGpio;
			//PORT_Struct.passiveFilterEnable = kPORT_PassiveFilterDisable;
			//PORT_Struct.driveStrength = kPORT_LowDriveStrength;
			//PORT_Struct.slewRate = kPORT_FastSlewRate;

			PORT_SetPinMux(a_PORT[IO_PORT], PIN, kPORT_MuxAsGpio);

			PORT_SetPinConfig(a_PORT[IO_PORT], PIN, &PORT_Struct);

			GPIO_Struct.pinDirection = kGPIO_DigitalOutput;
			GPIO_Struct.outputLogic = 1;
			GPIO_PinInit(a_GPIO[IO_PORT],PIN,&GPIO_Struct);
		}
		else if (DIR == kGPIO_DigitalInput)
		{
			// Define a digital input pin PCR configuration
			PORT_Struct.pullSelect = kPORT_PullDisable;
			PORT_Struct.mux = kPORT_MuxAsGpio;
			//PORT_Struct.passiveFilterEnable = kPORT_PassiveFilterDisable;
			//PORT_Struct.driveStrength = kPORT_LowDriveStrength;
			//PORT_Struct.slewRate = kPORT_FastSlewRate;

			PORT_SetPinMux(a_PORT[IO_PORT], PIN, kPORT_MuxAsGpio);

			PORT_SetPinConfig(a_PORT[IO_PORT], PIN, &PORT_Struct);

			GPIO_Struct.pinDirection = kGPIO_DigitalInput;
			GPIO_Struct.outputLogic = 0;
			GPIO_PinInit(a_GPIO[IO_PORT],PIN,&GPIO_Struct);
		}
		else
		{
			result = -1;
		}
    }

	return result;
}

/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
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

/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void io_Debounce_Pin_DI(IO_PORT PORT, uint8_t PIN)
{
	static volatile uint8_t Deb_Ctr = 0;

	if(io_Read_Pin(a_GPIO[PORT], PIN) == TRUE)
	{
		Deb_Ctr ++;
	}
	else
	{
		Deb_Ctr = 0;
	}

	if(Deb_Ctr >= DEB_CFG)
	{
		Deb_Ctr = 0;
		SetMenu++;
		if(SetMenu == INVALID_MENU)
		{
			SetMenu = SHOW_TIME;
		}
	}
}


/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void io_Debounce_Pin_DI_Low(DebStruct_t *Deb_Struct)
{
	if(Deb_Struct->Deb_Flags.bi.b0 == FALSE)
	{
		if(io_Read_Pin(a_GPIO[Deb_Struct->Deb_Port], Deb_Struct->Deb_Pin) == TRUE)
		{
			Deb_Struct->Deb_Ctr = 1 + Deb_Struct->Deb_Ctr;
		}
		else
		{
			Deb_Struct->Deb_Ctr = 0;
		}

		if(Deb_Struct->Deb_Ctr >= (Deb_Struct->Deb_Time/5u))
		{
			Deb_Struct->Deb_Ctr = 0;
			Deb_Struct->Deb_Flags.bi.b0 = TRUE;
		}
	}
	else
	{
		/*Is button released*/
		if(io_Read_Pin(a_GPIO[Deb_Struct->Deb_Port], Deb_Struct->Deb_Pin) == FALSE)
		{
			Deb_Struct->Deb_Flags.bi.b1 = TRUE;
		}
	}
}







