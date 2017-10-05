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

#define io_Read_Pin(a_PORT, a_PIN)	GPIO_ReadPinInput(a_PORT, a_PIN)

#define DEB_CFG 40


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


#define Deb_Get_SET()				(r_Deb_Array[0].Deb_Flags.bi.b1)
#define Deb_Clear_SET()				(r_Deb_Array[0].Deb_Flags.By = 0)

#define Deb_Get_Increase()			(r_Deb_Array[1].Deb_Flags.bi.b1)
#define Deb_Clear_Increase()		(r_Deb_Array[1].Deb_Flags.By = 0)

#define Deb_Get_Decrease()			(r_Deb_Array[2].Deb_Flags.bi.b1)
#define Deb_Clear_Decrease()		(r_Deb_Array[2].Deb_Flags.By = 0)

#endif /* IO_H_ */
