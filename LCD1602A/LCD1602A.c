/*
 * LCD1602A.c
 *
 *  Created on: 13/01/2017
 *      Author: Enrique
 */

#include <stdint.h>
#include <string.h>
#include "MKL25Z4.h"
#include "io.h"
#include "mytypedef.h"
#include "fsl_pit.h"
#include "fsl_tpm.h"
#include "rtc.h"
#include "uart.h"
#include "LCD1602A.h"

/*Local Functions*/
void LCD1602A_Disp_ON(void);
void LCD1602A_Disp_send_enable(void);
void LCD1602A_Disp_FunctionSet(void);
void LCD1602A_Disp_Read_BF(void);
void LCD1602A_Disp_SendCmd(uint8_t disp_command);
void LCD1602A_Disp_wait_us(uint64_t usec_time);
void LCD1602A_Disp_Menues(void);
void LCD1602A_Disp_write_ASCII(uint8_t ASCII_character);
void LCD1602A_Print(uint8_t const *buffer);
uint8_t LCD1602A_dig2ascii(uint8_t digit,uint8_t *dest);
uint16_t Button_IncDec_Mgr(uint16_t value);


/*******************************************
 * V A R I A B L E S
 *******************************************/
uint8_t LCD1602_buffer[32];
volatile uint8_t x = 0;
uint8_t Disp_Init_Complete = 0;
uint8_t refresh_ctr = 0;

menu_index_type SetMenu = SHOW_TIME;
menu_index_type MenuName = SHOW_TIME;

ASCII_Char SetTime_Menu[5] =  {ASCII_T,ASCII_i,ASCII_m,ASCII_e,ASCII_colom};
ASCII_Char SetDate_Menu[5] = {ASCII_D,ASCII_a,ASCII_t,ASCII_e,ASCII_colom};

uint8_t Disp_On_Var = 0;
uint8_t Disp_FuncSet_Var = 0;
uint8_t Disp_EntryMode_Var = 0;

uint8_t Dutycycle = 25;

/*-------------------------------------
 * Function: Disp_Init
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void LCD1602A_Disp_Cfg_Refres(void)
{


	if(Disp_Init_Complete == FALSE)
	{

		memset(LCD1602_buffer, 0xFFu, sizeof(LCD1602_buffer));


		Disp_On_Var = DISP_ON_D;
		Disp_EntryMode_Var = CURSOR_INC;

		//FunctionSet
#if LCD_8_DATA_LINES
		Disp_FuncSet_Var = DISP_DL | DISP_N | DISP_F;
		LCD1602A_Disp_SendCmd(DISP_FUNC_SET | Disp_FuncSet_Var);
#endif
#if	LCD_4_DATA_LINES
		Disp_FuncSet_Var = DISP_N | DISP_F;
		LCD1602A_Disp_SendCmd(DISP_FUNC_SET | Disp_FuncSet_Var);
#endif

		//Entry Mode Set
		LCD1602A_Disp_SendCmd(DISP_ENTRY_MODE_SET | Disp_EntryMode_Var);

		//Disp_ON();
		LCD1602A_Disp_SendCmd(DISP_ON | Disp_On_Var);

		/*Display Clear*/
		LCD1602A_Disp_SendCmd(DISP_CLEAR);

		LCD1602A_Disp_SendCmd(DISP_RETURN_HOME);

		Disp_Init_Complete = TRUE;
	}
}


/*-------------------------------------
 * Function: LCD1602A_Init
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void LCD1602A_Init(void)
{
	memset(LCD1602_buffer, 0xFFu, sizeof(LCD1602_buffer));
	Disp_Init_Complete = FALSE;
}


/*-------------------------------------
 * Function: Disp_FunctionSet
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void LCD1602A_Disp_FunctionSet(void)
{
	GPIO_SetPinsOutput(GPIOB, 0x08);
	GPIO_ClearPinsOutput(GPIOB, (uint8_t)~0x08u);

	GPIO_SetPinsOutput(GPIOC, 0x30);
	GPIO_ClearPinsOutput(GPIOC, (uint8_t)~0x30u);

}



/*-------------------------------------
 * Function: Disp_Read_BF
 * Desc: Reads Busy gflag D7
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void LCD1602A_Disp_Read_BF(void)
{
	volatile uint8_t BF = 1;
	volatile uint8_t LCDBusy = 1;
	uint8_t i = 0;
	uint16_t to = 0;

	GPIO_WritePinOutput(GPIOC, 7, 0); /*Write D7 to 0 before reading*/

	/*Change D7 to Input*/
	io_Pin_Cfg(PORT_C, 7, kGPIO_DigitalInput, kPORT_PullDisable); /*LCD 1602 D7*/

	/*LCD RS to 0*/
	GPIO_WritePinOutput(GPIOA, 1, 0);

#if	LCD_8_DATA_LINES
	/*Read BF while is 1*/
	 do
	 {
		 /*Send Enable*/
		GPIO_WritePinOutput(GPIOC, LCD_E, TRUE);/*Send Enable HIGH*/
		LCD1602A_Disp_wait_us(10);
		GPIO_WritePinOutput(GPIOC, LCD_E, FALSE);/*Send Enable LOW*/

		 BF =  GPIO_ReadPinInput(GPIOC, 7); /*Read Busy Flag D7*/
		 LCDBusy = BF;

		 to = 1 + to;
		 if(to > 10u)
		 {
			 LCDBusy = 0u;
		 }
	 }while(LCDBusy);
#endif

#if	LCD_4_DATA_LINES
	/*Read BF while is 1*/
	 do
	 {
		 /*Check BF*/
		for(i = 0; i < 2; i++)
		{
			/*set LCD RW to 1*/
			GPIO_WritePinOutput(GPIOA, LCD_RW, 1);
			LCD1602A_Disp_wait_us(10);

			 if(i == 0)
			 {
				GPIO_WritePinOutput(GPIOC, LCD_E, TRUE);/*Send Enable HIGH*/
				LCD1602A_Disp_wait_us(10);
				GPIO_WritePinOutput(GPIOC, LCD_E, FALSE);/*Send Enable LOW*/
				 /*Read BusyFlag BF*/
				 BF = GPIO_ReadPinInput(GPIOC, 7); /*Read Busy Flag D7*/
				 if (BF == 0)
				 {
					 LCDBusy = FALSE;
				 }
				 else{BF == 1;}
			 }
			 else
			 {
				GPIO_WritePinOutput(GPIOC, LCD_E, TRUE);/*Send Enable HIGH*/
				LCD1602A_Disp_wait_us(20);
				GPIO_WritePinOutput(GPIOC, LCD_E, FALSE);/*Send Enable LOW*/
			 }
			 to = 1 + to;
		}

		if(to >= 10u)
		 {
			 BF = 0u;
			 LCDBusy = FALSE;
		 }

	 }while(LCDBusy);

	 /*LCD RW to 0*/
	GPIO_WritePinOutput(GPIOA, LCD_RW, 0);

#endif
	/*Change D7 to Output*/
	io_Pin_Cfg(PORT_C, 7, kGPIO_DigitalOutput, kPORT_PullDisable); /*LCD 1602 D7*/
}


/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void LCD1602A_Disp_SendCmd(uint8_t disp_command)
{

#if LCD_4_DATA_LINES
	uint8_t i = 0;
#endif
	uint8_t cmd_LH[2] = {0,0}; /*Command in 4-bit  Low and High init*/

	GPIO_WritePinOutput(GPIOA, LCD_RS, 0); /*Enable RS*/
	GPIO_WritePinOutput(GPIOA, LCD_RW, 0); /*LCD RW to 0*/

#if LCD_8_DATA_LINES
	cmd_LH[LOW_NIBBLE] = (uint8_t) (0x0F & disp_command); //Low
	cmd_LH[HIGH_NIBBLE] = (uint8_t) (0xF0 & disp_command); //High


	GPIO_SetPinsOutput(GPIOB, cmd_LH[LOW_NIBBLE]);
	GPIO_ClearPinsOutput(GPIOB, ~cmd_LH[LOW_NIBBLE]);

	GPIO_SetPinsOutput(GPIOC, cmd_LH[HIGH_NIBBLE]);
	GPIO_ClearPinsOutput(GPIOC, ~cmd_LH[HIGH_NIBBLE]);

	Disp_send_enable();
#endif

#if LCD_4_DATA_LINES

	/*Instruction Register (IR) process*/
	/*High nibble of PORTC is used bits 4-7*/
	/*In 4 bits mode the high nibble is sent first IR7-IR4 then low nibble IR3-IR0*/
	cmd_LH[0] = (uint8_t) ((uint8_t)(0xF0u & disp_command)); //High
	cmd_LH[1] = (uint8_t) ((uint8_t)(0x0Fu & disp_command) << 4); //Low

	for(i = 0; i < 2u; i++)
	{
		GPIO_SetPinsOutput(GPIOC, cmd_LH[i]);
		GPIO_ClearPinsOutput(GPIOC, ~cmd_LH[i]);
		LCD1602A_Disp_send_enable();
	}
#endif
	/* Read Busy Flag*/
	LCD1602A_Disp_Read_BF();

}

/*-------------------------------------
 * Function: Disp_RefreshCfg
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void LCD1602A_Disp_RefreshCfg(void)
{
	memset(LCD1602_buffer, 0xFFu, sizeof(LCD1602_buffer));

	LCD1602_buffer[16] = ASCII_Money;
	LCD1602_buffer[31] = ASCII_x;

	//FunctionSet
#if LCD_8_DATA_LINES
	LCD1602A_Disp_SendCmd(DISP_FUNC_SET | Disp_FuncSet_Var);
#endif
#if	LCD_4_DATA_LINES
	LCD1602A_Disp_SendCmd(DISP_FUNC_SET | Disp_FuncSet_Var);
#endif

	//Disp_ON();
	LCD1602A_Disp_SendCmd(DISP_ON | Disp_On_Var);

	LCD1602A_Disp_SendCmd(DISP_CLEAR);

	//Entry Mode Set
	LCD1602A_Disp_SendCmd(DISP_ENTRY_MODE_SET | Disp_EntryMode_Var);

}




/*-------------------------------------
 * Function: Disp_ON
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void LCD1602A_Disp_ON(void)
{
	GPIO_SetPinsOutput(GPIOB, 0x0F);
	GPIO_ClearPinsOutput(GPIOC, 0xF0);
}


/*-------------------------------------
 * Function: Disp_write_ASCII
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void LCD1602A_Disp_write_ASCII(uint8_t ASCII_character)
{

#if LCD_4_DATA_LINES
	uint8_t i = 0;
#endif

	ASCII_Char ASCII_LH[2];

	GPIO_WritePinOutput(GPIOA, LCD_RS, 1); /*Enable RS*/

/*8 Data lines code*/
#if LCD_8_DATA_LINES
	ASCII_LH[LOW_NIBBLE] = (uint8_t) (0x0F & ASCII_character);
	ASCII_LH[HIGH_NIBBLE] = (uint8_t) (0xF0 & ASCII_character);

	GPIO_SetPinsOutput(GPIOB, ASCII_LH[LOW_NIBBLE]);
	GPIO_ClearPinsOutput(GPIOB, ~ASCII_LH[LOW_NIBBLE]);

	GPIO_SetPinsOutput(GPIOC, ASCII_LH[HIGH_NIBBLE]);
	GPIO_ClearPinsOutput(GPIOC, ~ASCII_LH[HIGH_NIBBLE]);
	Disp_send_enable();
	//Disp_wait_us(10);
#endif

/*4 Data lines code*/
#if LCD_4_DATA_LINES
	ASCII_LH[1] = (ASCII_Char) ((uint8_t)(0x0F & ASCII_character) << 4); //Low
	ASCII_LH[0] = (ASCII_Char) (0xF0 & ASCII_character); //High


	for(i = 0; i < 2; i++)
	{
		GPIO_SetPinsOutput(GPIOC, ASCII_LH[i]);
		GPIO_ClearPinsOutput(GPIOC, ~ASCII_LH[i]);
		LCD1602A_Disp_send_enable();
	}
#endif

	GPIO_WritePinOutput(GPIOA, LCD_RS, 0); /*Disable RS*/

	LCD1602A_Disp_Read_BF();
}


/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note: Minimum time is 20 us
 * SRS:
 *-----------------------------------*/
void LCD1602A_Disp_wait_us(uint64_t usec_time)
{
	uint64_t counts = 0;
	//PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);


	if(usec_time >= 50)
	{

		usec_time = usec_time - 25;
		PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(usec_time, CLK_SRC_HZ));

		PIT_StartTimer(PIT, kPIT_Chnl_1);

		while(!PIT_GetStatusFlags(PIT, kPIT_Chnl_1))
		{
			__asm("nop");
		}
		PIT_StopTimer(PIT, kPIT_Chnl_1);
		PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);
	}
	else
	{
		usec_time = usec_time - 2;
		counts = (usec_time * 2);
		while(counts)
		{
			__asm("nop");
			counts--;
		}

	}

}


/*-------------------------------------
 * Function: Disp_Main
 * Desc: Disp main function called every 500 ms
 * input:
 * return: void
 * Note:
 * SRS:
 *-----------------------------------*/
void LCD1602A_Disp_Main(void)
{

	//uint8_t i = 0;
	volatile static uint16_t a = 0;
	//menu_index_type l_menu = SHOW_TIME;

	//refresh_ctr ++;
	if(refresh_ctr >= 11)
	{
		LCD1602A_Disp_RefreshCfg();
		refresh_ctr = 0;
	}
	//l_menu = SetMenu;


	LCD1602A_Disp_Menues();

	//LCD1602A_Print(LCD1602_buffer);


/*	if(i>8)
	{
		i = 0;
		LCD1602A_Disp_SendCmd(DISP_RETURN_HOME);
	}*/

	//LCD1602A_Disp_SendCmd(DISP_RETURN_HOME);

}

/*-------------------------------------
 * Function: Disp_CfgBlink
 * Desc: Config the display to
 * 		 blink the display cursor
 * 		 not showing the cursor
 * input: State OFF:0 ON:not 0
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void LCD1602A_Disp_CfgBlink(uint8_t State)
{
	//LCD1602A_Disp_SendCmd(DISP_CLEAR);
	if(State >= ON )
	{
		LCD1602A_Disp_SendCmd(DISP_ON | DISP_ON_D /*|DISP_ON_C*/ | DISP_ON_B);
	}
	else
	{
		LCD1602A_Disp_SendCmd(DISP_ON | DISP_ON_D);
	}
}

/*-------------------------------------
 * Function: LCD1602A_Print
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void LCD1602A_Print(uint8_t const *buffer)
{
	uint8_t i = 0;
	while(buffer[i] != 0xFF)
		{
			if(i == 16u)
			{
				LCD1602A_Disp_SendCmd(DISP_DDRAM_ADDRESS|0x40u);
			}

			LCD1602A_Disp_write_ASCII(buffer[i]);
			i++;
		}
}



/*-------------------------------------
 * Function: Disp_Menues
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void LCD1602A_Disp_Menues(void)
{

	uint8_t  hrd1;
	uint8_t  hrd2;
	uint8_t  mind1;
	uint8_t  mind2;
	uint8_t  secd1;
	uint8_t  secd2;
	uint8_t trigger = 0;
	static uint8_t MenuShow = 0;
	static uint16_t value;

	char TimeMsg[5]={"Time:"};


	switch (MenuName)
	{
	case SHOW_TIME:

		memcpy(LCD1602_buffer, TimeMsg, strlen(TimeMsg)+1);
		LCD1602A_dig2ascii(PublicSendData[0],&LCD1602_buffer[5]); /*hr*/
		LCD1602_buffer[7] = ':';
		LCD1602A_dig2ascii(PublicSendData[1],&LCD1602_buffer[8]); /*min*/
		LCD1602_buffer[10] = ':';
		LCD1602A_dig2ascii(PublicSendData[2],&LCD1602_buffer[11]); /*sec*/
		LCD1602_buffer[13] = ASCII_space;

		LCD1602A_Print(LCD1602_buffer);
		LCD1602A_Disp_SendCmd(DISP_RETURN_HOME);

		break;

	case SET_TIME_CFG:
		LCD1602A_Disp_CfgBlink(ON);
		memcpy(LCD1602_buffer, SetTime_Menu, sizeof(SetTime_Menu));
		LCD1602_buffer[5] = ASCII_0;
		LCD1602_buffer[6] = ASCII_0;
		LCD1602_buffer[7] = ASCII_colom;
		LCD1602_buffer[8] = ASCII_0;
		LCD1602_buffer[9] = ASCII_0;
		LCD1602A_Print(LCD1602_buffer);

		memcpy((void *) &value, (const void *) &date, sizeof(date));

		MenuName = SET_TIME_HR;
		break;

	case SET_TIME_HR:
		LCD1602A_Disp_SendCmd(DISP_SET_DDRAM_ADDRESS|0x05);
		memset(LCD1602_buffer, 0xFFu, sizeof(LCD1602_buffer));

		value = date.hour;

		value = Button_IncDec_Mgr(value);


		if(value >=10)
		{
			if(value <=23)
			{
			LCD1602_buffer[0] = (value/10)+ASCII_0;
			LCD1602_buffer[1] = ASCII_0+(value%10);
			}
			else
			{
				value = 0;
				LCD1602_buffer[0] = ASCII_0;
				LCD1602_buffer[1] = ASCII_0;
			}
		}
		else
		{
			LCD1602_buffer[0] = ASCII_0;
			LCD1602_buffer[1] = ASCII_0+value;
		}

		LCD1602A_Print(LCD1602_buffer);

		date.hour = (uint8_t)value;

		rtc_SetDatetime(&date);

		trigger = 0;
		break;

	case SET_TIME_MIN:
		LCD1602A_Disp_SendCmd(DISP_SET_DDRAM_ADDRESS|0x08);
		memset(LCD1602_buffer, 0xFFu, sizeof(LCD1602_buffer));

		value = date.minute;

		value = Button_IncDec_Mgr(value);

		if(value >=10)
		{
			if(value <=59)
			{
			LCD1602_buffer[0] = (value/10)+ASCII_0;
			LCD1602_buffer[1] = ASCII_0+(value%10);
			}
			else
			{
				value = 0;
				LCD1602_buffer[0] = ASCII_0;
				LCD1602_buffer[1] = ASCII_0;
			}
		}
		else
		{
			LCD1602_buffer[0] = ASCII_0;
			LCD1602_buffer[1] = ASCII_0+value;
		}

		LCD1602A_Print(LCD1602_buffer);

		date.minute = (uint8_t)value;

		rtc_SetDatetime(&date);

		break;
			//}while(trigger);


	case SET_DATE_CFG:
		LCD1602A_Disp_SendCmd(DISP_SET_DDRAM_ADDRESS|0x00);
		Disp_On_Var = DISP_ON_D | DISP_ON_B;
		LCD1602A_Disp_SendCmd(DISP_ON | Disp_On_Var);

		memcpy(LCD1602_buffer, SetDate_Menu,sizeof(SetDate_Menu));
		LCD1602_buffer[5] = ASCII_D;
		LCD1602_buffer[6] = ASCII_D;
		LCD1602_buffer[7] = ASCII_slash;
		LCD1602_buffer[8] = ASCII_M;
		LCD1602_buffer[9] = ASCII_M;
		LCD1602_buffer[10] = ASCII_slash;
		LCD1602_buffer[11] = ASCII_A;
		LCD1602_buffer[12] = ASCII_A;

		value = 1;
		LCD1602A_Print(LCD1602_buffer);
		MenuName = SET_DATE_DAY;
		break;

	case SET_DATE_DAY:
		LCD1602A_Disp_SendCmd(DISP_SET_DDRAM_ADDRESS|0x05);
		memset(LCD1602_buffer, 0xFFu, sizeof(LCD1602_buffer));

		value = date.day;

		value = Button_IncDec_Mgr(value);

		if(value >=10)
		{
			if(value <=31)
			{
			LCD1602_buffer[0] = (value/10)+ASCII_0;
			LCD1602_buffer[1] = ASCII_0+(value%10);
			}
			else
			{
				value = 1;
				LCD1602_buffer[0] = ASCII_0;
				LCD1602_buffer[1] = ASCII_1;
			}
		}
		else
		{
			LCD1602_buffer[0] = ASCII_0;
			LCD1602_buffer[1] = ASCII_0+value;
		}

		LCD1602A_Print(LCD1602_buffer);
		date.day = (uint8_t)value;
		rtc_SetDatetime(&date);
		break;

	case SET_DATE_MONTH:
		LCD1602A_Disp_SendCmd(DISP_SET_DDRAM_ADDRESS|0x08);
		memset(LCD1602_buffer, 0xFFu, sizeof(LCD1602_buffer));

		value = date.month;

		value = Button_IncDec_Mgr(value);

		if(value >=10)
		{
			if(value <=12)
			{
			LCD1602_buffer[0] = (value/10)+ASCII_0;
			LCD1602_buffer[1] = ASCII_0+(value%10);
			}
			else
			{
				value = 1;
				LCD1602_buffer[0] = ASCII_0;
				LCD1602_buffer[1] = ASCII_1;
			}
		}
		else
		{
			LCD1602_buffer[0] = ASCII_0;
			LCD1602_buffer[1] = ASCII_0+value;
		}

		LCD1602A_Print(LCD1602_buffer);
		date.month = (uint8_t)value;
		rtc_SetDatetime(&date);
		break;

	case SET_DATE_YEAR:
		LCD1602A_Disp_SendCmd(DISP_SET_DDRAM_ADDRESS|0x0Bu);
		memset(LCD1602_buffer, 0xFFu, sizeof(LCD1602_buffer));

		value = date.year;

		value = Button_IncDec_Mgr(value);

		if(value >=2000u)
		{
			LCD1602_buffer[0] = (value/1000)+ASCII_0;
			LCD1602_buffer[1] = ((value%1000)/100)+ASCII_0;
			LCD1602_buffer[2] = (((value%1000)%100)/10)+ASCII_0;
			LCD1602_buffer[3] = (((value%1000)%100)%10)+ASCII_0;
		}
		else
		{
			value = 2000u;
			LCD1602_buffer[0] = ASCII_2;
			LCD1602_buffer[1] = ASCII_0;
			LCD1602_buffer[2] = ASCII_0;
			LCD1602_buffer[3] = ASCII_0;
		}

		LCD1602A_Print(LCD1602_buffer);

		date.year = value;
		rtc_SetDatetime(&date);
		break;

	case SET_COMPLETE:
		MenuName = SHOW_TIME;
		LCD1602A_Disp_CfgBlink(OFF);
		LCD1602A_Disp_SendCmd(DISP_CLEAR);
		rtc_Start();
		break;

	default:
		break;
	}

	if(Get_and_Clear(SET_BUTTON))
	{
		MenuName++;

		if(MenuName == SET_DATE_YEAR)
		{value = 2000u;}

		if (MenuName >= INVALID_MENU)
		{
			MenuName = SHOW_TIME;
		}
	}

	if(Get_and_Clear(INCREASE_BUTTON))
	{
		Dutycycle = Dutycycle + 2;
		TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_3, kTPM_CenterAlignedPwm, Dutycycle);
	}

	if(Get_and_Clear(DECREASE_BUTTON))
	{
		Dutycycle = Dutycycle - 2;
		TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_3, kTPM_CenterAlignedPwm, Dutycycle);
	}

}

/*-------------------------------------
 * Function: Disp_send_enable
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void LCD1602A_Disp_send_enable(void)
{

	GPIO_WritePinOutput(GPIOC, LCD_E, TRUE);

	LCD1602A_Disp_wait_us(ENABLE_WITH);

	GPIO_WritePinOutput(GPIOC,LCD_E, FALSE);
}


/*-------------------------------------
 * Function: LCD1602A_dig2ascii
 * Desc: function that receives a digit (0 to 99)
 * 			and returns it ascci value in a arraay of 2
 * 			e.g. 19 -> '1','9'
 * 			e.g. 8 -> '0','8'
 *
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
uint8_t LCD1602A_dig2ascii(uint8_t digit,uint8_t *dest)
{
	uint8_t ret = 1; /*error*/

	if((digit >= 10u) && (digit <= 99u))
	{
		dest[0] = (digit / 10u) + 0x30u;
		dest[1] = (digit % 10u) + 0x30u;
		ret = 0;
	}
	else if(digit <= 9u)
	{
		dest[0] = 0x30u;
		dest[1] = digit + 0x30u;
		ret = 0;
	}
	else
	{
		/*error the function converts only digits from 0 to 99*/
		ret = 1;
	}

	return ret;
}

/*-------------------------------------
 * Function: Button_IncDec_Mgr
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
uint16_t Button_IncDec_Mgr(uint16_t value)
{
	if(Get_and_Clear(INCREASE_BUTTON) == TRUE)
	{
		value++;
	}
	else if(Get_and_Clear(DECREASE_BUTTON) == TRUE)
	{
		value--;
	}
	else
	{/*Do nothing*/}

	return value;
}
