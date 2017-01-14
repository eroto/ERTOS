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
#include "LCD1602A.h"

void Disp_ON(void);
void Disp_send_enable(void);
void Disp_Clear(void);
void Disp_Main(void);
void Disp_FunctionSet(void);
void Disp_Init(void);



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
void Disp_ON(void)
{
	GPIO_SetPinsOutput(GPIOB, 0x0F);
	GPIO_ClearPinsOutput(GPIOC, 0xF0);
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
void Disp_Init(void)
{
	Disp_FunctionSet();

	Disp_ON();
}


/*-------------------------------------
* Function:
* Desc:
* input:
* return:
* Note:
*-----------------------------------*/
void Disp_wait_us(uint64_t usec_time)
{
	uint64_t counts = 0;

	counts = USEC_TO_COUNT(usec_time, CLK_SRC_HZ);

	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, counts);

	//PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

	PIT_StartTimer(PIT, kPIT_Chnl_0);
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


	GPIO_WritePinOutput(GPIOD, 1, FALSE);

	switch(x)
	{
	case 0:

		/*Function Set*/
		Disp_FunctionSet();
		x= x +1;
		Disp_send_enable();
		break;

	case 1:

		Disp_ON();

			x= x +1;

			Disp_send_enable();
		break;

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


		case 2:
		/*Clear Display*/
		Disp_Clear();

		x= x +1;
		Disp_send_enable();
		break;


	case 3:

		/*Entry Mode Set*/
		GPIO_SetPinsOutput(GPIOB, 0x06);
		GPIO_ClearPinsOutput(GPIOB, (uint8_t)~0x06u);

		x= x +1;
		Disp_send_enable();
		break;


	case 4:

		/*Write an "H"*/
		GPIO_WritePinOutput(GPIOA, 1, 1);
		/**/
		GPIO_SetPinsOutput(GPIOB, 0x08);
		GPIO_ClearPinsOutput(GPIOB, (uint8_t)~0x08u);

		GPIO_SetPinsOutput(GPIOC, 0x40);
		//GPIO_ClearPinsOutput(GPIOC, (uint8_t)~0x40u);
		x= x +1;
		Disp_send_enable();
		GPIO_WritePinOutput(GPIOA, 1, 0);
		break;

	case 5:

		/*Write an "o"*/
		GPIO_WritePinOutput(GPIOA, 1, 1);
		/**/
		GPIO_SetPinsOutput(GPIOB, 0x0E);
		GPIO_ClearPinsOutput(GPIOB, (uint8_t)~0x0Eu);

		GPIO_SetPinsOutput(GPIOC, 0x30);
		GPIO_ClearPinsOutput(GPIOC, (uint8_t)~0x30u);
		x= x +1;
		Disp_send_enable();
		GPIO_WritePinOutput(GPIOA, 1, 0);
		x= x +1;

		break;

	case 6:

			//x= x +1;

			break;


	default:

		break;

	}

}



void Disp_send_enable(void)
{

	uint32_t i;
	GPIO_WritePinOutput(GPIOC, 0, TRUE);

	for(i = 0 ; i <= 509; i++)
	{
		__asm("nop");
	}
	GPIO_WritePinOutput(GPIOC, 0, FALSE);
}
