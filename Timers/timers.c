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
#include "fsl_port.h"
#include "fsl_tpm.h"
#include "timers.h"



void timers_init(void);
void PIT_INIT_LP(void);
void PWM_Init(void);

/*-------------------------------------
 * Function: timers_init
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


/*-------------------------------------
 * Function: timers_init
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void PWM_Init()
{
    //bool brightnessUp = true; /* Indicates whether the LED is brighter or dimmer. */
    tpm_config_t tpmInfo;
    uint8_t updatedDutycycle = 25U;
    tpm_chnl_pwm_signal_param_t tpmParam;

    /* Configures the TPM parameters with frequency 24 kHZ. */
    tpmParam.chnlNumber = kTPM_Chnl_3;
    tpmParam.level = kTPM_HighTrue;//kTPM_LowTrue;
    tpmParam.dutyCyclePercent = updatedDutycycle;

    /* Board pin, clock, and debug console initialization. */

    PORT_SetPinMux(PORTE, 30U, kPORT_MuxAlt3);

    CLOCK_SetTpmClock(1);

    TPM_GetDefaultConfig(&tpmInfo);

    //tpmInfo.prescale  = kTPM_Prescale_Divide_1;
    //tpmInfo.enableDebugMode = TRUE;

    /* Initializes the TPM module. */
    TPM_Init(TPM0, &tpmInfo);
    TPM_SetupPwm(TPM0, &tpmParam, 1U, kTPM_CenterAlignedPwm, 4000U, TPM_SOURCE_CLOCK);
    //TPM_EnableInterrupts(TPM0,kTPM_Chnl3InterruptEnable);
    //EnableIRQ(TPM0_IRQn);
    TPM_StartTimer(TPM0, kTPM_SystemClock);

    //TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_3, kTPM_CenterAlignedPwm, updatedDutycycle);

}

void TPM0_IRQHandler(void)
{
	TPM_ClearStatusFlags(TPM0, kTPM_Chnl3Flag);
}

