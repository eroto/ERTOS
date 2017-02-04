/*
 * LCD1602A.c
 *
 *  Created on: 13/01/2017
 *      Author: Enrique
 */

#include <stdint.h>
#include "MKL25Z4.h"
#include "io.h"
#include "mytypedef.h"
#include "fsl_pit.h"
#include "rtc.h"
#include "LCD1602A.h"

void Disp_ON(void);
void Disp_send_enable(void);
void Disp_Clear(void);
void Disp_Main(void);
void Disp_FunctionSet(void);





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

	Disp_SendCmd(DISP_FUNC_SET);
	Disp_send_enable();
	Disp_wait_us(DISP_INIT_DELAY);

	//Disp_ON();
	Disp_SendCmd(DISP_OM);
	Disp_send_enable();
	Disp_wait_us(DISP_INIT_DELAY);

	//Disp_Clear();
	Disp_SendCmd(DISP_CLEAR);
	Disp_send_enable();
	Disp_wait_us(DISP_INIT_DELAY);

	Disp_SendCmd(DISP_ENTRY_MODE_SET);
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

	//rtc_datetime_t *Date_time

	//rtc_GetDatetime(&Date_time);

	//uint8_t disp_hr = Date_time->hour;
	//uint8_t disp_min = Date_time->minute;
	//uint8_t disp_sec = Date_time->second;


	Disp_write_ASCII(ASCII_P);
	Disp_wait_us(20);
	Disp_write_ASCII(ASCII_a);
	Disp_wait_us(20);
	Disp_write_ASCII(ASCII_u);
	Disp_wait_us(20);
	Disp_write_ASCII(ASCII_l);
	Disp_wait_us(20);
	Disp_write_ASCII(ASCII_a);
	Disp_wait_us(20);
	Disp_write_ASCII(ASCII_pound);
	Disp_wait_us(20);
	Disp_write_ASCII(ASCII_E);
	Disp_wait_us(20);
	Disp_write_ASCII(ASCII_n);
	Disp_wait_us(20);
	Disp_write_ASCII(ASCII_r);
	Disp_wait_us(20);
	Disp_write_ASCII(ASCII_i);
	Disp_wait_us(20);
	Disp_write_ASCII(ASCII_q);
	Disp_wait_us(20);
	Disp_write_ASCII(ASCII_u);
	Disp_wait_us(20);
	Disp_write_ASCII(ASCII_e);
	Disp_wait_us(20);
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
