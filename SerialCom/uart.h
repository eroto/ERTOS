/*
 * uart.h
 *
 *  Created on: 17/10/2016
 *      Author: Enrique
 */

#ifndef SERIALCOM_UART_H_
#define SERIALCOM_UART_H_

#define UART_INIT_PATTER	((uint8_t)0xCCu)

#include <stdint.h>
#include "fsl_port.h"
#include "fsl_uart.h"
#include "mytypedef.h"


#define UART_BUFFER_MAX_SIZE	(uint8_t)50u

#define UART_INIT_PATTERN 		(uint8_t)0xccu

#define UART_srcClock_Hz		(uint32_t)24000000U

typedef enum
{
	UART_IDLE,
	UART_TX,
	UART_RX,
	UART_INIT_ERROR,
	UART_TXRX_ERROR,
	UART_MAX_STATES
}T_UART_STATES;

extern uint8_t PublicSendData[UART_BUFFER_MAX_SIZE];


void uart_main(void);
void uart_init(void);
status_t uart_ReqTx(uint8_t* lpub_data,  uint8_t lub_size);
void uart_Callback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData);


#endif /* SERIALCOM_UART_H_ */
