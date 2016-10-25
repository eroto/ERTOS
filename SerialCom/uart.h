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


uint8_t sendData[15];

void uart_init(void);
void uart_Callback(uart_handle_t *handle, status_t status, void *userData);
status_t uart_Send(uint8_t *lpub_data, const uint8_t lub_size);


#endif /* SERIALCOM_UART_H_ */
