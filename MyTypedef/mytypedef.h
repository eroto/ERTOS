/*
 * mytypedef.h
 *
 *  Created on: 22/05/2017
 *      Author: Enrique
 */

#ifndef MYTYPEDEF_H_
#define MYTYPEDEF_H_

#include <stdint.h>

#define TRUE			1u
#define FALSE			0u

#define ON				1u
#define OFF				0u

typedef union
{
  struct
  {
	uint8_t b0:1;			/*!< bit: 0  flag */
    uint8_t b1:1;		/*!< bit: 1  flag */
    uint8_t b2:1;                   /*!< bit: 2  flag */
    uint8_t b3:1;                   /*!< bit: 3  flag */
    uint8_t b4:1;                   /*!< bit: 4  flag */
    uint8_t b5:1;                   /*!< bit: 5  flag */
    uint8_t b6:1;                   /*!< bit: 6  flag */
    uint8_t b7:1;                   /*!< bit: 7  flag */
  } bi;                                   /*!< Structure used for bit  access */
  uint32_t By;                            /*!< Type      used for byte access */
} tuflag_8_Type;

#endif /* MYTYPEDEF_H_ */
