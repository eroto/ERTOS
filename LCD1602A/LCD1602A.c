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
 * Function:
 * Desc:
 * input:
 * return:
 * Note:
 *-----------------------------------*/
void Disp_FunctionSet(void)
{
	GPIO_SetPinsOutput(GPIOB, 0x08);
	GPIO_ClearPinsOutput(GPIOB, (uint8_t)~0x08u);

	GPIO_SetPinsOutput(GPIOC, 0x30);
	GPIO_ClearPinsOutput(GPIOC, (uint8_t)~0x30u);

}



/*-------------------------------------
* Function:
* Desc:
* input:
* return:
* Note:
*-----------------------------------*/
void Disp_Clear(void)
{
	GPIO_SetPinsOutput(GPIOB, 0x01);
	GPIO_ClearPinsOutput(GPIOB, (uint8_t)~0x01u);

	GPIO_ClearPinsOutput(GPIOC, 0xF0);
}



/*-------------------------------------
* Function:
* Desc:
* input:
* return:
* Note:
*-----------------------------------*/
void Disp_SendCmd(uint8_t disp_command)
{

	uint8_t cmd_L = (uint8_t) (0x0F & disp_command);
	uint8_t cmd_H = (uint8_t) (0xF0 & disp_command);

	GPIO_SetPinsOutput(GPIOB, cmd_L);
	GPIO_ClearPinsOutput(GPIOB, ~cmd_L);

	GPIO_SetPinsOutput(GPIOC, cmd_H);
	GPIO_ClearPinsOutput(GPIOC, ~cmd_H);


}

/*-------------------------------------
* Function:
* Desc:
* input:
* return:
* Note:
*-----------------------------------*/



/*-------------------------------------
* Function:
* Desc:
* input:
* return:
* Note:
*-----------------------------------*/
void Disp_Init(void)
{
	//Disp_FunctionSet();


	memset(LCD1602_buffer, 0xFF, 32);

	Disp_SendCmd(DISP_FUNC_SET | DISP_DL | DISP_N | DISP_F);
	Disp_send_enable();
	Disp_wait_us(DISP_INIT_DELAY);

	//Disp_ON();
	Disp_SendCmd(DISP_OM | DISP_ON_D /*| DISP_ON_C*/);
	Disp_send_enable();
	Disp_wait_us(DISP_INIT_DELAY);

	//Disp_Clear();
	Disp_SendCmd(DISP_CLEAR);
	Disp_send_enable();
	Disp_wait_us(DISP_INIT_DELAY);

	Disp_SendCmd(DISP_ENTRY_MODE_SET | CURSOR_INC | CURSOR_SHIFT);
	Disp_send_enable();
	Disp_wait_us(DISP_INIT_DELAY);



}


/*-------------------------------------
* Function:
* Desc:
* input:
* return:
* Note:
*-----------------------------------*/
void Disp_ON(void)
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
*-----------------------------------*/
void Disp_write_ASCII(ASCII_Char ASCII_character)
{

	uint8_t ASCII_L = (uint8_t) (0x0F & ASCII_character);
	uint8_t ASCII_H = (uint8_t) (0xF0 & ASCII_character);

	GPIO_WritePinOutput(GPIOA, 1, 1);
	/**/
	GPIO_SetPinsOutput(GPIOB, ASCII_L);
	GPIO_ClearPinsOutput(GPIOB, ~ASCII_L);

	GPIO_SetPinsOutput(GPIOC, ASCII_H);
	GPIO_ClearPinsOutput(GPIOC, ~ASCII_H);
	Disp_send_enable();
	GPIO_WritePinOutput(GPIOA, 1, 0);
}

/*-------------------------------------
* Function:
* Desc: Minimum time is 20 us
* input:
* return:
* Note:
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
* Function:
* Desc:
* input:
* return:
* Note:
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
	LCD1602_buffer[8] = ASCII_period;


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

	while(LCD1602_buffer[i] != 0xFF)
	{
		Disp_write_ASCII(LCD1602_buffer[i]);
		Disp_wait_us(20);
		i = 1 + i;
	}

	Disp_SendCmd(DISP_RETURN_HOME);
	Disp_send_enable();

//	switch(x)
//	{
//	case 0:
//
//		/*Function Set*/
//		Disp_FunctionSet();
//		x= x +1;
//		Disp_send_enable();
//		break;
//
//	case 1:
//
//		/*Display ON*/
//		Disp_ON();
//
//			x= x +1;
//
//			Disp_send_enable();
//		break;

//	case 3:
//		/*Return Home*/
//		GPIO_SetPinsOutput(GPIOB, 0x02);
//		GPIO_ClearPinsOutput(GPIOB, (uint8_t)~0x02u);
//
//		GPIO_ClearPinsOutput(GPIOC, 0xF0);
//
//		x= x +1;
//		Disp_send_enable();
//		break;


//		case 2:
//
//		/*Clear Display*/
//		Disp_Clear();
//
//		x= x +1;
//		Disp_send_enable();
//		break;
//
//
//	case 3:
//
//		/*Entry Mode Set*/
//		GPIO_SetPinsOutput(GPIOB, 0x06);
//		GPIO_ClearPinsOutput(GPIOB, (uint8_t)~0x06u);
//
//		x= 6;//x +1;
//		Disp_send_enable();
//		break;
//
//
//	case 4:
//
//		/*Write an "H"*/
//		GPIO_WritePinOutput(GPIOA, 1, 1);
//		/**/
//		GPIO_SetPinsOutput(GPIOB, 0x08);
//		GPIO_ClearPinsOutput(GPIOB, (uint8_t)~0x08u);
//
//		GPIO_SetPinsOutput(GPIOC, 0x40);
//		//GPIO_ClearPinsOutput(GPIOC, (uint8_t)~0x40u);
//		x= x +1;
//		Disp_send_enable();
//		GPIO_WritePinOutput(GPIOA, 1, 0);
//		break;
//
//	case 5:
//
//		/*Write an "o"*/
//		GPIO_WritePinOutput(GPIOA, 1, 1);
//		/**/
//		GPIO_SetPinsOutput(GPIOB, 0x0E);
//		GPIO_ClearPinsOutput(GPIOB, (uint8_t)~0x0Eu);
//
//		GPIO_SetPinsOutput(GPIOC, 0x30);
//		GPIO_ClearPinsOutput(GPIOC, (uint8_t)~0x30u);
//		x= x +1;
//		Disp_send_enable();
//		GPIO_WritePinOutput(GPIOA, 1, 0);
//		x= x +1;
//
//		break;
//
//	case 6:
//
//			//x= x +1;
//
//			break;
//
//
//	default:
//
//		break;
//
//	}

}



void Disp_send_enable(void)
{

	uint32_t i;
	GPIO_WritePinOutput(GPIOC, 0, TRUE);

	Disp_wait_us(20);

	GPIO_WritePinOutput(GPIOC, 0, FALSE);
}
