/*
 * Schr.c
 *
 *  Created on: 22/07/2016
 *      Author: uid87057
 */


#include <stdint.h>
#include "board.h"
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
T_TaskPowerModeType  re_curOpMode;
volatile SCHM_BOOLEAN re_mngExecflag;/* Scheduler execution flag is set in timer interrupt handler */

/* Manager/Application main function list */
const S_SCHM_MANAGER_EXEC_TYPE cps_mngTaskList[SCHD_MANAGERS_NUMBER] =
{
   /* Main function,	Startup delay(ms),   Exec. Period x 5 ms),     Job Powermode*/
	{apptask_5ms,				1,                  1,					HIGH_POWER},
	{apptask_20ms,				4,					4,					HIGH_POWER},
	{apptask_100ms,				20,					20,					HIGH_POWER},
	{apptask_500ms,				100,				100,				HIGH_POWER},
	{apptask_1s,				200,				200,				HIGH_POWER},
	{LP_apptask_1s,				10,					10,					 LOW_POWER},
  //{function_task_name,		0,					1,					HIGH_POWER},


	/* NOTE: this list shall be updated according to E_MODULES_ID_TYPE! */
};



/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void Schr_Init( void )
{
	uint32_t Sys_tick = 0;
    /* clear timer interrupt flag */
    re_mngExecflag = SCHM_FALSE;

    PIT_StartTimer(PIT, kPIT_Chnl_0);

    re_curOpMode = HIGH_POWER;


#if SCHR_PIT
    EnableIRQ(PIT_IRQn);
#elif SCHR_SYSTICK
    Sys_tick = MSEC_TO_COUNT(5, BOARD_BOOTCLOCKRUN_CORE_CLOCK);
    SysTick_Config(Sys_tick);
    EnableIRQ(SysTick_IRQn);
#endif

}


/*-------------------------------------
 * Function: func_name
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
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
                if (cps_mngTaskList[le_mngIndex].JobFunction != SCHM_NULL_PTR)
                {
                    /* decrement the local counter of modules  */
                    lasw_MngTimeCnt[le_mngIndex] -= TIMER_PERIOD;    
                    /* check whether counter is expired */
                    if (lasw_MngTimeCnt[le_mngIndex] <= 0)
                    {
                        /* set the local module counter */
                        lasw_MngTimeCnt[le_mngIndex] = (T_DelayMainType)cps_mngTaskList[le_mngIndex].ExecutionPeriod;
                        /* check whether the module starts for the current Mode */
                        if((cps_mngTaskList[le_mngIndex].JobPowerMode) == (re_curOpMode))
                        {
                            cps_mngTaskList[le_mngIndex].JobFunction();
                        }
                    }
                }
            }  
        }
        else
        {
        	if(re_curOpMode == HIGH_POWER)
        	{
        		apptask_idle();
        	}
        }
    }
    while (SCHM_TRUE);

    return;
}

#if (SCHR_PIT && SCHR_SYSTICK)
#error "Only one Scheduler Tick configuration can be active at a time"
#elif SCHR_PIT
/*-------------------------------------
 * Function: PIT_IRQHandler
 * Desc: Programable Interrup Timer
 * 		Called in interrupt context
 * 		to call the increment in
 * 		the Scheduler, Sys Tick.
 * input: void
 * return: void
 * Note: Called in PIT interrupt context
 * SRS:
 *-----------------------------------*/
void PIT_IRQHandler(void)
{
    /* Set flag so background will run */
    re_mngExecflag = SCHM_TRUE;

    PIT_ClearStatusFlags(PIT, 0, kPIT_TimerFlag);

}
#elif SCHR_SYSTICK
/*-------------------------------------
 * Function: SysTick_Handler
 * Desc: Exeption Handler Called in interrupt context
 *		Optional system timer, SysTick, is a 24-bit
 *		count-down timer. If supported by the ARM core,
 *		use this as a Real Time Operating System (RTOS)
 *		tick timer or as a simple counter.
 * input: void
 * return: void
 * Note: ARM Exeption Handler, a SysTick exception is
 * 		generated when the SysTick timer reaches zero.
 * SRS:
 *-----------------------------------*/
void SysTick_Handler(void)
{
	re_mngExecflag = SCHM_TRUE;
}

#else
#error "Scheduler Tick configuration is not valid"
#endif

