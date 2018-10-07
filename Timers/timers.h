/*
 * timers.h
 *
 *  Created on: 01/02/2017
 *      Author: Enrique
 */

#ifndef TIMERS_H_
#define TIMERS_H_

extern void timers_init(void);
extern void PIT_INIT_LP(void);
extern void PWM_Init(void);


/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk)

#endif /* TIMERS_H_ */
