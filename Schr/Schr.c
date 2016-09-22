/*
 * Schr.c
 *
 *  Created on: 22/07/2016
 *      Author: uid87057
 */


#include <stdint.h>
#include "mytypedef.h"
#include "Schr_cfg.h"
#include "Schr.h"
#include "apptasks.h"
#include "fsl_common.h"
#include "fsl_pit.h"
#include "fsl_gpio.h"

/******************************************************************************
*   Global Variable Definitions
******************************************************************************/
T_OpModeType  ruw_curOpMode;

extern volatile SCHM_BOOLEAN re_mngExecflag;

/* Manager/Application main function list */
const S_SCHM_MANAGER_EXEC_TYPE cps_mngTaskList[SCHD_MANAGERS_NUMBER] =
{
   /* Main function,		Startup delay(ms),   Execution Period x 5 ms),          Group */
	{apptask_5ms,				1,                  1,								0xFFFF},
	{apptask_20ms,				4,					4,								0xFFFF},
	{apptask_100ms,				20,					20,								0xFFFF},
	{apptask_1s,				200,				200,							0xFFFF},
  //{function_task_name,		0,					1,								0xFFFF},


	/* NOTE: this list shall be updated according to E_MODULES_ID_TYPE! */
};

/***************************************************************************
 * Function:    schInit
 *
 * Description: Main function of Scheduler module
 *
 * Returns:
 *
 * Notes:
 *
 **************************************************************************/ 
void Schr_Init( void )
{

    /* clear timer interrupt flag */
    re_mngExecflag = SCHM_FALSE;

    /*Timers stopped un Debug mode*/
    pit_config_t Sch_Pit_cfg;

    Sch_Pit_cfg.enableRunInDebug = FALSE;

    PIT_Init(PIT, (pit_config_t *) &Sch_Pit_cfg);

    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, MSEC_TO_COUNT(5,PIT_CLK_SRC_HZ));

    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

    EnableIRQ(PIT_IRQn);

    PIT_StartTimer(PIT, kPIT_Chnl_0);

    ruw_curOpMode = 0x01;

}

/***************************************************************************
 * Function:    schExec
 *
 * Description: Main function of Scheduler module
 *
 * Returns:
 *
 * Notes:
 *
 **************************************************************************/ 
void Schr_Exec( void )
{
    E_MODULES_ID_TYPE le_mngIndex;
    T_DelayMainType lasw_MngTimeCnt[SCHD_MANAGERS_NUMBER];    

    /* Initialization of local modules counters to delay of module execution from Scheduler start */
    for (le_mngIndex = (E_MODULES_ID_TYPE)0; le_mngIndex < SCHD_MANAGERS_NUMBER; le_mngIndex++)
    {
        lasw_MngTimeCnt[le_mngIndex] = (T_DelayMainType)cps_mngTaskList[le_mngIndex].StartUpDelay;    
    }  
    
    /*Scheduler starts*/
    do
    {
        /* wait for timer interrupt */
        if (re_mngExecflag)
        {
            /* clear timer interrupt flag */
            re_mngExecflag = SCHM_FALSE;
            /* check each module from the list of modules */
            for (le_mngIndex = (E_MODULES_ID_TYPE)0; le_mngIndex < SCHD_MANAGERS_NUMBER; le_mngIndex++)
            {
                if (cps_mngTaskList[le_mngIndex].ManagerAPI != SCHM_NULL_PTR)
                {
                    /* decrement the local counter of modules  */
                    lasw_MngTimeCnt[le_mngIndex] -= TIMER_PERIOD;    
                    /* check whether counter is expired */
                    if (lasw_MngTimeCnt[le_mngIndex] <= 0)
                    {
                        /* set the local module counter */
                        lasw_MngTimeCnt[le_mngIndex] = (T_DelayMainType)cps_mngTaskList[le_mngIndex].ExecutionPeriod;
                        /* check whether the module starts for the current Mode */
                        if (0 != (cps_mngTaskList[le_mngIndex].UsedInMode & ruw_curOpMode))
                        {
                            cps_mngTaskList[le_mngIndex].ManagerAPI();    
                        }
                    }
                }
            }  
        }
        else
        {
        	apptask_idle();
        }
    }
    while (SCHM_TRUE);

    return;
}



void PIT_IRQHandler(void)
{
    /* Set flag so background will run */
    re_mngExecflag = SCHM_TRUE;

    PIT_ClearStatusFlags(PIT, 0, kPIT_TimerFlag);

}
