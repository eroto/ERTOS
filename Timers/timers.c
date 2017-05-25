/*
 * timers.c
 *
 *  Created on: 01/02/2017
 *      Author: Enrique
 */

#include <stdint.h>
#include "mytypedef.h"
#include "Schr_cfg.h"
#include "fsl_common.h"
#include "fsl_pit.h"
#include "fsl_gpio.h"
#include "timers.h"


/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void timers_init(void)
{

	/*Timers stopped in Debug mode*/
	pit_config_t Sch_Pit_cfg;

	Sch_Pit_cfg.enableRunInDebug = FALSE;


	PIT_Init(PIT, (pit_config_t *) &Sch_Pit_cfg);

	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, MSEC_TO_COUNT(5,PIT_CLK_SRC_HZ_HP));

	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

	EnableIRQ(PIT_IRQn);

	PIT_StopTimer(PIT, kPIT_Chnl_0);

}


void PIT_INIT_LP(void)
{
	PIT_StopTimer(PIT, kPIT_Chnl_0);
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, MSEC_TO_COUNT(100,PIT_CLK_SRC_HZ_LP));
	PIT_StartTimer(PIT, kPIT_Chnl_0);
}
