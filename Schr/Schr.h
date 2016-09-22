/*
 * Schr.h
 *
 *  Created on: 22/07/2016
 *      Author: uid87057
 */

#ifndef SCHR_H_
#define SCHR_H_

#include <stdint.h>
#include "schm_trm_isr.h"

/******************************************************************************
*   Type Definitions
******************************************************************************/
/* operation mode type */
typedef uint16_t T_OpModeType;

/* task delay type */
typedef uint16_t T_TaskDelayType;

/* delay manager type */
typedef uint16_t T_DelayMainType;


typedef enum
{
    SCHM_FALSE = 0,
    SCHM_TRUE  = 1
}SCHM_BOOLEAN;

/******************************************************************************
*   Macro Definitions
******************************************************************************/
/* Timer Period in ms */
#define TIMER_PERIOD      (uint16_t)1

/* Time execution period */
#define ITBM_schGetModuleTimePeriod(ModuleID) cps_mngTaskList[ModuleID].ExecutionPeriod

/* null constant if not App or Init routine */
#define SCHM_NULL_PTR (T_SCHM_MANAGER_API_TYPE)0

/*****************************************************************************
 *  Local Macro Definitions
 *****************************************************************************/

/******************************************************************************
*   Type Definitions
******************************************************************************/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* + Manager/Application ID */

typedef enum {
    /* Add here the IDs Tasks, order them same to Manager/Aplication main function list */
    TASK_5MS,
    TASK_20MS,
    TASK_100MS,
    TASK_1S,
    SCHD_MANAGERS_NUMBER/* Number of Managers/Applications */
} E_MODULES_ID_TYPE;

/*----------------------------------------------------------------------------------------------*/



/*Type definition for pointer to main functions of the managers*/
typedef void (*T_SCHM_MANAGER_API_TYPE)(void);

/*Type definition for main functions of the managers and their execution period*/
typedef struct{
    /* the main function */
    T_SCHM_MANAGER_API_TYPE  ManagerAPI;
    /* delay of submodule execution from Scheduler start in ms */
    const T_TaskDelayType StartUpDelay;
    /* module is executed each 'ExecutionPeriod' time in ms */
    const T_TaskDelayType ExecutionPeriod;

    T_OpModeType UsedInMode;

} S_SCHM_MANAGER_EXEC_TYPE;

/******************************************************************************
*   External Variables
******************************************************************************/
/* Managers/Application main function List */
extern const S_SCHM_MANAGER_EXEC_TYPE cps_mngTaskList[SCHD_MANAGERS_NUMBER];

/******************************************************************************
*   Global Function Prototypes  
******************************************************************************/
void Schr_Init( void );
void Schr_Exec( void );



#endif /* SCHR_H_ */
