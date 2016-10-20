/*
 * io.h
 *
 *  Created on: 23/09/2016
 *      Author: Enrique
 */

#ifndef IO_H_
#define IO_H_

#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_port.h"

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
int8_t io_Pin_Cfg(IO_PORT PORT, uint8_t PIN, gpio_pin_direction_t DIR);
int8_t io_Read_Pin_Cfg(IO_PORT PORT, uint8_t PIN);





#endif /* IO_H_ */