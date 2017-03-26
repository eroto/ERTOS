/*
 * relayctrl.h
 *
 *  Created on: 25/03/2017
 *      Author: Enrique
 */

#include <stdint.h>
#include "fsl_rtc.h"
#include "rtc.h"

#ifndef RELAYCTRL_H_
#define RELAYCTRL_H_


void relayctrl_init(void);
void relayctrl_main(void);



typedef enum
{
	RELAY_CTRL_ACTIVE = 0x5A65u,
	RELAY_CTRL_INACTIVE = 0xA59Au,
	RELAY_CTRL_ERROR = 0x7D61u,
	//RELAY_CTRL_INACTIVE_WAIT = 0x829Eu
}e_RELAY_CTRL_STATES;


#define RELAYCTRL_GET_ON_HR()			13u
#define RELAYCTRL_GET_ON_MIN()			51u

#define RELAYCTRL_GET_OFF_HR()			13u
#define RELAYCTRL_GET_OFF_MIN()			52u


#endif /* RELAYCTRL_H_ */
