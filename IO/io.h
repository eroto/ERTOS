/*
 * io.h
 *
 *  Created on: 20/05/2017
 *      Author: Enrique
 */

#ifndef IO_IO_H_
#define IO_IO_H_

#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_port.h"



#define io_Read_Pin(a_PORT, a_PIN)	GPIO_ReadPinInput(a_PORT, a_PIN)

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

/* Functions prototypes*/
/*----------------------------------------------------------------------------------------------*/
void io_init(void);
int8_t io_Pin_Cfg(IO_PORT PORT, uint8_t PIN, gpio_pin_direction_t DIR, uint16_t PullSelect);
int8_t io_Read_Pin_Cfg(IO_PORT PORT, uint8_t PIN);




#endif /* IO_H_ */
