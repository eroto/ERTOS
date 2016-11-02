/*
 * uart.h
 *
 *  Created on: 17/10/2016
 *      Author: Enrique
 */

#ifndef SERIALCOM_UART_H_
#define SERIALCOM_UART_H_

#include <stdint.h>
#include "fsl_port.h"
#include "fsl_uart.h"
#include "mytypedef.h"


void uart_init(void);
void UART_UserCallback(uart_handle_t *handle, status_t status, void *userData);
void uart_Send(void);


#endif /* SERIALCOM_UART_H_ */
