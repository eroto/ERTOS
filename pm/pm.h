/*
 * pm.h
 *
 *  Created on: 24/05/2017
 *      Author: Enrique
 */

#ifndef PM_H_
#define PM_H_

#include "fsl_common.h"


/* Power mode definition used in application. */
typedef enum _app_power_mode
{
    kAPP_PowerModeMin = 0,
    kAPP_PowerModeRun,  /*!< Run mode. All Kinetis chips. */
    kAPP_PowerModeWait, /*!< Wait mode. All Kinetis chips. */
    kAPP_PowerModeStop, /*!< Stop mode. All Kinetis chips. */
    kAPP_PowerModeVlpr, /*!< Very low power run mode. All Kinetis chips. */
    kAPP_PowerModeVlpw, /*!< Very low power wait mode. All Kinetis chips. */
    kAPP_PowerModeVlps, /*!< Very low power stop mode. All Kinetis chips. */
    kAPP_PowerModeLls, /*!< Low leakage stop mode. All Kinetis chips. */
    kAPP_PowerModeVlls0, /*!< Very low leakage stop 0 mode. Chip-specific. */
    kAPP_PowerModeVlls1, /*!< Very low leakage stop 1 mode. All Kinetis chips. */
    kAPP_PowerModeVlls3, /*!< Very low leakage stop 3 mode. All Kinetis chips. */
    kAPP_PowerModeMax
} app_power_mode_t;




typedef struct power_user_config
{
    app_power_mode_t mode; /*!< Power mode. */
    bool enablePorDetectInVlls0; /*!< true - Power on reset detection circuit is enabled in Very low leakage stop 0
                                    mode, false - Power on reset detection circuit is disabled. */
} power_user_config_t;



/* callback data type which is used for power manager user callback */
typedef struct
{
    uint32_t beforeNotificationCounter; /*!< Callback before notification counter */
    uint32_t afterNotificationCounter;  /*!< Callback after notification counter */
    smc_power_state_t originPowerState; /*!< Origin power state before switch */
} user_callback_data_t;


typedef enum _app_wakeup_source
{
    kAPP_WakeupSourceLptmr, /*!< Wakeup by LPTMR.        */
    kAPP_WakeupSourcePin    /*!< Wakeup by external pin. */
} app_wakeup_source_t;


/**************************************/
/*         Function prototypes        */
/**************************************/
extern void pm_SetClockRun_2_Vlpr(void);

extern void pm_SetClockVlpr(void);







#endif /* PM_H_ */


