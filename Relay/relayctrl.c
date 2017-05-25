/*
 * relayctrl.c
 *
 *  Created on: 25/03/2017
 *      Author: Enrique
 */
#include <stdint.h>
#include <string.h>
#include "MKL25Z4.h"
#include "io.h"
#include "mytypedef.h"
#include "relayctrl.h"



void relayctrl_init(void);
void relayctrl_main(void);

e_RELAY_CTRL_STATES relay_ctrl_state;
static rtc_datetime_t relayTemp_date;

uint16_t relaytctrl_GetTime(void);



/*-------------------------------------
 * Function: relayctrl_init
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void relayctrl_init(void)
{
	relay_ctrl_state = RELAY_CTRL_INACTIVE;
}


/*-------------------------------------
 * Function: relayctrl_main
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void relayctrl_main(void)
{

	e_RELAY_CTRL_STATES next_state = RELAY_CTRL_INACTIVE;
	uint8_t relayctrl_on_hr = 0;
	uint8_t relayctrl_on_min = 0;
	uint8_t relayctrl_off_hr = 0;
	uint8_t relayctrl_off_min = 0;
	uint16_t loc_Datetime = 0;



	relayctrl_on_hr = RELAYCTRL_GET_ON_HR();
	relayctrl_on_min = RELAYCTRL_GET_ON_MIN();

	relayctrl_off_hr = RELAYCTRL_GET_OFF_HR();
	relayctrl_off_min = RELAYCTRL_GET_OFF_MIN();

	/*Get Time*/
	loc_Datetime = relaytctrl_GetTime();


	switch(relay_ctrl_state)
	{
	case RELAY_CTRL_INACTIVE:

		/*Is it Time to Turn ON ?*/
		if((relayctrl_on_hr == (uint8_t)(loc_Datetime >> 8)) && (relayctrl_on_min == (uint8_t)loc_Datetime))
		{
			next_state = RELAY_CTRL_ACTIVE;

			/*TODO A Diagnostic will be implemented later to know if it's OK to activate*/
			/* TODO Activate Output here*/
		}
		else
		{
			next_state = RELAY_CTRL_INACTIVE;
		}
		break;

	case RELAY_CTRL_ACTIVE:

		/*Is it Time to Turn OFF ?*/
		if((relayctrl_off_hr == (uint8_t)(loc_Datetime >> 8)) && (relayctrl_off_min == (uint8_t)loc_Datetime))
		{
			next_state = RELAY_CTRL_INACTIVE;
			/*TODO A Diagnostic will be implemented later to know if it's OK to deactivate*/
			/* TODO Deactivate Output here*/
		}
		else
		{
			next_state = RELAY_CTRL_ACTIVE;
		}
		break;

	case RELAY_CTRL_ERROR:
	default:
		/*TODO Take a safe state and wait until error/failure is gone or a Timeout*/

		break;
	}

	relay_ctrl_state = next_state;

}

/*-------------------------------------
 * Function: relaytctrl_GetTime
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
uint16_t relaytctrl_GetTime(void)
{
	uint16_t hr_min = 0;

	rtc_GetDatetime(&relayTemp_date);

	hr_min = (uint16_t)(((uint16_t)relayTemp_date.hour) << 8u);
	hr_min |= relayTemp_date.minute;

	return (hr_min);
}



