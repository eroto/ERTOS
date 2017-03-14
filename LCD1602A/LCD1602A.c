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
#include "rtc.h"
#include "uart.h"
#include "LCD1602A.h"

void Disp_ON(void);
void Disp_send_enable(void);
void Disp_Clear(void);
void Disp_Main(void);
void Disp_FunctionSet(void);


uint8_t LCD1602_buffer[32];
volatile uint8_t x = 0;



/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void Disp_FunctionSet(void)
{
	GPIO_SetPinsOutput(GPIOB, 0x08);
	GPIO_ClearPinsOutput(GPIOB, (uint8_t)~0x08u);

	GPIO_SetPinsOutput(GPIOC, 0x30);
	GPIO_ClearPinsOutput(GPIOC, (uint8_t)~0x30u);

}



/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void Disp_Clear(void)
{
	GPIO_SetPinsOutput(GPIOB, 0x01);
	GPIO_ClearPinsOutput(GPIOB, (uint8_t)~0x01u);

	GPIO_ClearPinsOutput(GPIOC, 0xF0);
}



/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void Disp_SendCmd(uint8_t disp_command)
{

	//uint8_t cmd_L = (uint8_t) (0x0F & disp_command);
	//uint8_t cmd_H = (uint8_t) (0xF0 & disp_command);
#ifndef LCD_8_DATA_LINES
	uint8_t i = 0;
#endif
	uint8_t cmd_LH[2];

#ifdef LCD_8_DATA_LINES
	cmd_LH[LOW_NIBBLE] = (uint8_t) (0x0F & disp_command); //Low

	cmd_LH[HIGH_NIBBLE] = (uint8_t) (0xF0 & disp_command); //High


	GPIO_SetPinsOutput(GPIOB, cmd_LH[LOW_NIBBLE]);
	GPIO_ClearPinsOutput(GPIOB, ~cmd_LH[LOW_NIBBLE]);

	GPIO_SetPinsOutput(GPIOC, cmd_LH[HIGH_NIBBLE]);
	GPIO_ClearPinsOutput(GPIOC, ~cmd_LH[HIGH_NIBBLE]);

	Disp_send_enable();
	Disp_wait_us(DISP_INIT_DELAY);
#endif

#ifndef LCD_8_DATA_LINES

	cmd_LH[0] = (uint8_t) ((uint8_t)(0x0F & disp_command) << 4); //Low
	cmd_LH[1] = (uint8_t) (0xF0 & disp_command); //High

	for(i = 1; (i >= LOW_NIBBLE) && (i<= HIGH_NIBBLE); i--)
	{

	GPIO_SetPinsOutput(GPIOC, cmd_LH[i]);
	GPIO_ClearPinsOutput(GPIOC, ~cmd_LH[i]);

	Disp_send_enable();
	Disp_wait_us(10);
	}

	Disp_wait_us(DISP_INIT_DELAY);

#endif



#ifdef LCD_8_DATA_LINES

#endif



}

/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void Disp_Init(void)
{
	memset(LCD1602_buffer, 0xFF, sizeof(LCD1602_buffer));

	//FunctionSet
#ifdef LCD_8_DATA_LINES
	Disp_SendCmd(DISP_FUNC_SET | DISP_DL | DISP_N | DISP_F);
#endif
#ifndef	LCD_8_DATA_LINES
	Disp_SendCmd(DISP_FUNC_SET | DISP_N | DISP_F);
#endif

	//Disp_ON();
	Disp_SendCmd(DISP_OM | DISP_ON_D /*| DISP_ON_C*/);

	//Disp_Clear();
	Disp_SendCmd(DISP_CLEAR);

	//Entry Mode Set
	Disp_SendCmd(DISP_ENTRY_MODE_SET | CURSOR_INC | CURSOR_SHIFT);

}

/*-------------------------------------
 * Function: Disp_RefreshCfg
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void Disp_RefreshCfg(void)
{
	memset(LCD1602_buffer, 0xFF, 32);

	//FunctionSet
#ifdef LCD_8_DATA_LINES
	Disp_SendCmd(DISP_FUNC_SET | DISP_DL | DISP_N | DISP_F);
#endif
#ifndef	LCD_8_DATA_LINES
	Disp_SendCmd(DISP_FUNC_SET | DISP_N | DISP_F);
#endif

	//Disp_ON();
	Disp_SendCmd(DISP_OM | DISP_ON_D /*| DISP_ON_C*/);

	//Disp_Clear();
	Disp_SendCmd(DISP_CLEAR);

	//Entry Mode Set
	Disp_SendCmd(DISP_ENTRY_MODE_SET | CURSOR_INC | CURSOR_SHIFT);

}




/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void Disp_ON(void)
{
	GPIO_SetPinsOutput(GPIOB, 0x0F);
	GPIO_ClearPinsOutput(GPIOC, 0xF0);
}


/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void Disp_write_ASCII(ASCII_Char ASCII_character)
{

#ifndef LCD_8_DATA_LINES
	uint8_t i = 0;
#endif

	ASCII_Char ASCII_LH[2];

	GPIO_WritePinOutput(GPIOA, 1, 1); /*Enable RS*/

#ifdef LCD_8_DATA_LINES
	ASCII_LH[LOW_NIBBLE] = (uint8_t) (0x0F & ASCII_character);
	ASCII_LH[HIGH_NIBBLE] = (uint8_t) (0xF0 & ASCII_character);

	GPIO_SetPinsOutput(GPIOB, ASCII_LH[LOW_NIBBLE]);
	GPIO_ClearPinsOutput(GPIOB, ~ASCII_LH[LOW_NIBBLE]);

	GPIO_SetPinsOutput(GPIOC, ASCII_LH[HIGH_NIBBLE]);
	GPIO_ClearPinsOutput(GPIOC, ~ASCII_LH[HIGH_NIBBLE]);
	Disp_send_enable();
	Disp_wait_us(10);
#endif

#ifndef LCD_8_DATA_LINES
	ASCII_LH[1] = (ASCII_Char) ((uint8_t)(0x0F & ASCII_character) << 4); //Low
	ASCII_LH[0] = (ASCII_Char) (0xF0 & ASCII_character); //High


	for(i = 0; i < 2; i++)
	{

		GPIO_SetPinsOutput(GPIOC, ASCII_LH[i]);
		GPIO_ClearPinsOutput(GPIOC, ~ASCII_LH[i]);
		Disp_send_enable();
		Disp_wait_us(10);
	}
#endif

	GPIO_WritePinOutput(GPIOA, 1, 0); /*Disable RS*/
	Disp_wait_us(DISP_DELAY);
}


/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note: Minimum time is 20 us
 * SRS:
 *-----------------------------------*/
void Disp_wait_us(uint64_t usec_time)
{
	uint64_t counts = 0;
	//PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);


	if(usec_time >= 50)
	{

		usec_time = usec_time - 21;
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
		counts = (usec_time * 2);
		while(counts)
		{
			__asm("nop");
			counts--;
			__asm("nop");
			__asm("nop");
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
void Disp_Main(void)
{

	uint8_t i = 0;
	volatile static uint16_t a = 0;
	uint8_t  hrd1;
	uint8_t  hrd2;
	uint8_t  mind1;
	uint8_t  mind2;
	uint8_t  secd1;
	uint8_t  secd2;

	//rtc_datetime_t *Date_time

	//rtc_GetDatetime(&Date_time);

	//uint8_t disp_hr = Date_time->hour;
	//uint8_t disp_min = Date_time->minute;
	//uint8_t disp_sec = Date_time->second;


	//LCD1602_buffer[8] = ASCII_0;

	hrd1 = (PublicSendData[0] / 10) + 0x30;
	hrd2 = (PublicSendData[0] % 10) + 0x30;

	mind1 = (PublicSendData[1] /10) + 0x30;
	mind2 = (PublicSendData[1] % 10)  + 0x30;

	secd1 = (PublicSendData[2] /10) + 0x30;
	secd2 = (PublicSendData[2] % 10)  + 0x30;

	LCD1602_buffer[0] = hrd1;
	LCD1602_buffer[1] = hrd2;
	LCD1602_buffer[2] = ASCII_colom;
	LCD1602_buffer[3] = mind1;
	LCD1602_buffer[4] = mind2;
	LCD1602_buffer[5] = ASCII_colom;
	LCD1602_buffer[6] = secd1;
	LCD1602_buffer[7] = secd2;
	LCD1602_buffer[8] = ASCII_rightarrow;


	if(a >= 10)
	{
		LCD1602_buffer[9] = ASCII_0;
		a = 0;
	}
	else
	{
		LCD1602_buffer[9] = a + 0x30;
	}

	a = ++a;

	i = 0;
	while(LCD1602_buffer[i] != 0xFF)
	{
		Disp_write_ASCII(LCD1602_buffer[i]);
		//Disp_wait_us(DISP_DELAY);
		i = 1 + i;
	}

	Disp_SendCmd(DISP_RETURN_HOME);

}


/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void Disp_send_enable(void)
{

	uint32_t i;
	GPIO_WritePinOutput(GPIOC, 0, TRUE);

	Disp_wait_us(ENABLE_WITH);

	GPIO_WritePinOutput(GPIOC, 0, FALSE);
}
