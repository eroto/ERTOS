/*
 * uart.c
 *
 *  Created on: 17/10/2016
 *      Author: Enrique
 */



#include <stdint.h>
#include "uart.h"

uart_config_t user_config;
uart_handle_t g_uartHandle;
uint8_t sendData[] = {'T', 'e', ' ', 'A', 'm', 'o', ' ', 'P','a','u','l','a'};
uart_transfer_t sendXfer;

/*-------------------------------------
* Function: uart_init
* Desc: This function is called after
* reset, and initialize the UART to
* default values 115200bps, no parity
* 8bits x char, one stop bit Tx enable
* Rx enable
* input:
* return:
* Note:
*-----------------------------------*/
void uart_init(void)
{
    uint8_t ch;
    UART_GetDefaultConfig(&user_config);
    user_config.baudRate_Bps = 115200U;
    user_config.enableTx = true;
    user_config.enableRx = true;


    UART_Init(UART1,&user_config,24000000U);

    /* Initialize LPUART0 pins below */
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortE);
    /* Affects PORTE_PCR1 register */
    PORT_SetPinMux(PORTE, 0u, kPORT_MuxAlt3);
    /* Affects PORTE_PCR2 register */
    PORT_SetPinMux(PORTE, 1u, kPORT_MuxAlt3);

    // Prepare to send.
    sendXfer.data = sendData;
    sendXfer.dataSize = sizeof(sendData)/sizeof(sendData[0]);

    UART_TransferCreateHandle(UART1, &g_uartHandle, UART_UserCallback, NULL);
}

void uart_Send(void)
{
    UART_TransferSendNonBlocking(UART1,&g_uartHandle, &sendXfer);
}

/*-------------------------------------
* Function:
* Desc:
* input:
* return:
* Note:
*-----------------------------------*/
void UART_UserCallback(uart_handle_t *handle, status_t status, void *userData)
{
    userData = userData;
//    if (kStatus_UART_TxIdle == status)
//    {
//        txFinished = true;
//    }
//    if (kStatus_UART_RxIdle == status)
//    {
//        rxFinished = true;
//    }
}


/*-------------------------------------
* Function:
* Desc:
* input:
* return:
* Note:
*-----------------------------------*/
//void UART1_IRQHandler(void)
//{
//    static volatile UartIRQ = 0;
//    UartIRQ = UartIRQ++;
//    UART_ClearStatusFlags(UART1, kUART_TransmissionCompleteFlag);
//}
