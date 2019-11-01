/*
 * io.h
 *
 *  Created on: 20/05/2017
 *      Author: Enrique
 */

#ifndef IO_IO_H_
#define IO_IO_H_

#include "mytypedef.h"
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "board.h"

#define io_Read_Pin(a_PORT, a_PIN)	GPIO_ReadPinInput(a_PORT, a_PIN)

#define DEB_CFG 40


#define LCD_E		0u	/*LCD_E Pin0*/
#define LCD_RS		1u	/*LCD_RS Pin1*/
#define LCD_RW		2u	/*LCD_RW Pin2*/
#define LCD_D0		0u	/*LCD_D0 Pin 0u*/
#define LCD_D1		1u	/*LCD_D1 Pin 1u*/
#define LCD_D2		2u  /*LCD_D2 Pin 2u*/
#define LCD_D3		3u  /*LCD_D3 Pin 3u*/
#define LCD_D4		4u  /*LCD_D4 Pin 4u*/
#define LCD_D5		5u  /*LCD_D5 Pin 5u*/
#define LCD_D6		6u  /*LCD_D6 Pin 6u*/
#define LCD_D7		7u  /*LCD_D7 Pin 7u*/

#define RELAY_IN1	0u  /*RELAY_IN1 Pin 0*/
#define RELAY_IN2	1u  /*RELAY_IN2 Pin 1*/

#define UART1_TX	0u	/*UART1_TX*/
#define UART1_RX	1u	/*UART1_RX*/

/* enums and typedef*/
/*----------------------------------------------------------------------------------------------*/
typedef enum
{
    PORT_A,
    PORT_B,
    PORT_C,
    PORT_D,
    PORT_E,
    PORT_MAX_NUM
}IO_PORT;

typedef enum
{
	SET_BUTTON,
	INCREASE_BUTTON,
	DECREASE_BUTTON,
	MAX_BUTTONS
}DEB_IOs;

typedef struct
	{
	IO_PORT Deb_Port;
	uint8_t Deb_Pin;
	uint16_t Deb_Time;
	uint16_t Deb_Ctr;
	tuflag_8_Type Deb_Flags;
	}DebStruct_t;

/* Gloal Variables*/
/*----------------------------------------------------------------------------------------------*/
extern DebStruct_t r_Deb_Array[3];


/* Functions prototypes*/
/*----------------------------------------------------------------------------------------------*/
void io_init(void);
int8_t io_Pin_Cfg(IO_PORT PORT, uint8_t PIN, gpio_pin_direction_t DIR, uint16_t PullSelect);
int8_t io_Read_Pin_Cfg(IO_PORT PORT, uint8_t PIN);
extern void io_Debounce_Pin_DI(IO_PORT PORT, uint8_t PIN);
extern void io_Debounce_Pin_DI_Low(DebStruct_t *Deb_Struct);


#define GET_AND_CLEAR_SET_BUTTON()	(r_Deb_Array[0].Deb_Flags.bi.b1)
#define Deb_Clear_SET()				(r_Deb_Array[0].Deb_Flags.By = 0)

#define Deb_Get_Increase()			(r_Deb_Array[1].Deb_Flags.bi.b1)
#define Deb_Clear_Increase()		(r_Deb_Array[1].Deb_Flags.By = 0)

#define Deb_Get_Decrease()			(r_Deb_Array[2].Deb_Flags.bi.b1)
#define Deb_Clear_Decrease()		(r_Deb_Array[2].Deb_Flags.By = 0)

#define DebounceReached				Deb_Flags.bi.b0
#define ButtonReleased				Deb_Flags.bi.b1

#endif /* IO_H_ */
