
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*  REVISION |   DATE      |                               |      AUTHOR      */
/*----------------------------------------------------------------------------*/
/*  1.0      | 21/08/2011  |                               | Continental      */
/* File creation. These configurations just apply on MPC5604B micro           */
/*============================================================================*/

#ifndef SCHR_CFG_H                               /* To avoid double inclusion */
#define SCHR_CFG_H

/* Includes */
/* -------- */
#include <stdint.h>

/* Enable frozeen of scheduler tick in debug mode*/
/*  1: enable tick frozzen, 
    0: disable tick frozzen */
#define FROZEEN_TICK	1 
#define PIT_CLK_SRC_HZ_HP	((uint64_t)24000000)
#define PIT_CLK_SRC_HZ_LP	((uint64_t)800000)


#endif


