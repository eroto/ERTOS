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

DebStruct_t r_Deb_Array[3] = {{PORT_B,2,500,0,{.By = 0}},{PORT_B,3,60,0,{.By = 0}},{PORT_E,20,60,0,{.By = 0}}};

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

    /*Pin Cfg RED LED*/
    io_Pin_Cfg(PORT_B, BOARD_LED_RED_GPIO_PIN, kGPIO_DigitalOutput, kPORT_PullUp);

    /*Pin Cfg GREEN LED*/
    io_Pin_Cfg(PORT_B, BOARD_LED_GREEN_GPIO_PIN, kGPIO_DigitalOutput, kPORT_PullUp);

    /*
     *LCD 1602 Configuration
     */
	io_Pin_Cfg(PORT_C, LCD_E, kGPIO_DigitalOutput, kPORT_PullDown);
    io_Pin_Cfg(PORT_A, LCD_RS, kGPIO_DigitalOutput, kPORT_PullDown);
    io_Pin_Cfg(PORT_A, LCD_RW, kGPIO_DigitalOutput, kPORT_PullDown);
#if LCD_8_DATA_LINES & LCD_4_DATA_LINES
	#error "INCORRECT LCD CONFIG, only one option is possible LCD_4_DATA_LINES 1 or LCD_8_DATA_LINES 1"
#elif  LCD_8_DATA_LINES
    io_Pin_Cfg(PORT_B, LCD_D0, kGPIO_DigitalOutput, kPORT_PullDown);
	io_Pin_Cfg(PORT_B, LCD_D1, kGPIO_DigitalOutput, kPORT_PullDown);
	io_Pin_Cfg(PORT_B, LCD_D2, kGPIO_DigitalOutput, kPORT_PullDown);
	io_Pin_Cfg(PORT_B, LCD_D3, kGPIO_DigitalOutput, kPORT_PullDown);
	io_Pin_Cfg(PORT_C, LCD_D4, kGPIO_DigitalOutput, kPORT_PullDown);
    io_Pin_Cfg(PORT_C, LCD_D5, kGPIO_DigitalOutput, kPORT_PullDown);
    io_Pin_Cfg(PORT_C, LCD_D6, kGPIO_DigitalOutput, kPORT_PullDown);
    io_Pin_Cfg(PORT_C, LCD_D7, kGPIO_DigitalOutput, kPORT_PullDown);
#elif LCD_4_DATA_LINES

	io_Pin_Cfg(PORT_C, LCD_D4, kGPIO_DigitalOutput, kPORT_PullDown);
    io_Pin_Cfg(PORT_C, LCD_D5, kGPIO_DigitalOutput, kPORT_PullDown);
    io_Pin_Cfg(PORT_C, LCD_D6, kGPIO_DigitalOutput, kPORT_PullDown);
    io_Pin_Cfg(PORT_C, LCD_D7, kGPIO_DigitalOutput, kPORT_PullDown);
#else
	#error "INCORRECT LCD CONFIG, LCD_4_DATA_LINES shall be 1 or LCD_8_DATA_LINES shall be 1"
#endif

#if (RELAY_ENABLE & LCD_4_DATA_LINES)
	io_Pin_Cfg(PORT_B, RELAY_IN1, kGPIO_DigitalOutput, kPORT_PullDown);
	io_Pin_Cfg(PORT_B, RELAY_IN2, kGPIO_DigitalOutput, kPORT_PullDown);

	io_Pin_Cfg(PORT_B, 2u, kGPIO_DigitalInput, kPORT_PullDown);/*Set Time*/
	io_Pin_Cfg(PORT_B, 3u, kGPIO_DigitalInput, kPORT_PullDown);/*Time +*/
	io_Pin_Cfg(PORT_E, 20u, kGPIO_DigitalInput, kPORT_PullDown);/*Time -*/
#elif (RELAY_ENABLE & LCD_8_DATA_LINES)
#endif

	io_Pin_Cfg(PORT_E,UART1_TX,kGPIO_DigitalOutput,kPORT_PullUp);
	io_Pin_Cfg(PORT_E,UART1_RX,kGPIO_DigitalInput,kPORT_PullUp);

    GPIO_ClearPinsOutput(GPIOB, 0x0Fu);
    GPIO_ClearPinsOutput(GPIOC, 0xF1u);
    GPIO_WritePinOutput(GPIOA, LCD_RS, 0); /*LCD_RS to 0*/
    GPIO_WritePinOutput(GPIOA, LCD_RW, 0); /*LCD_RW to 0*/
    GPIO_WritePinOutput(GPIOC, LCD_E, 0);  /*LCD_E to 0*/

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
 * Function: io_Debounce_Pin_DI_Low
 * Desc:
 * input:array of struct
		IO_PORT Deb_Port;
		uint8_t Deb_Pin;
		uint16_t Deb_Time;
		uint16_t Deb_Ctr;
		tuflag_8_Type Deb_Flags;
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void io_Debounce_Pin_DI_Low(DebStruct_t *Deb_Struct)
{
	uint8_t i = 0;
	for(i=0; i < (uint8_t)sizeof(Deb_Struct);i++)
	{
		if(Deb_Struct->DebounceReached == FALSE)
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
				Deb_Struct->DebounceReached = TRUE;
			}
		}
		else
		{
			/*Is button released*/
			if(io_Read_Pin(a_GPIO[Deb_Struct->Deb_Port], Deb_Struct->Deb_Pin) == FALSE)
			{
				Deb_Struct->ButtonReleased = TRUE;
			}
		}
		Deb_Struct++;
	}
}


/*-------------------------------------
 * Function: io_Debounce_Pin_DI_Low
 * Desc:
 * input: enum DEB_IOs I/O to check
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
uint8_t Get_and_Clear(DEB_IOs IO)
{
	uint8_t result = FALSE;

	if(r_Deb_Array[IO].DebounceReached && r_Deb_Array[IO].ButtonReleased)
	{
		r_Deb_Array[IO].Deb_Flags.By = 0;
		result = TRUE;
	}

	return result;
}
