/*
 * uart.c
 *
 *  Created on: 17/10/2016
 *      Author: Enrique
 */



#include <stdint.h>
#include "mytypedef.h"
#include "uart.h"

uart_config_t user_config;
uart_handle_t g_uartHandle;
uint8_t sendData[15] = {'T', 'e', ' ', 'A', 'm', 'o', ' ', 'P','a','u','l','a',0x55,0x55,0x55};
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
    user_config.stopBitCount = 1;


    UART_Init(UART1,&user_config,24000000U);

    /* Initialize LPUART0 pins below */
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortE);
    /* Affects PORTE_PCR1 register */
    PORT_SetPinMux(PORTE, 0u, kPORT_MuxAlt3);
    /* Affects PORTE_PCR2 register */
    PORT_SetPinMux(PORTE, 1u, kPORT_MuxAlt3);

    UART_TransferCreateHandle(UART1, &g_uartHandle, (uart_transfer_callback_t) uart_Callback, NULL);
}

/*-------------------------------------
* Function: uart_init
* Desc: This function is called to
* start a UART Tx request using UART1.
* UART Handle shall be available/created
* before using this interface
* input:    lpub_data
*           lub_size
*
* return:   1 kStatus_Fail
*           0 kStatus_Success
* Note:
*-----------------------------------*/
status_t uart_Send(uint8_t *lpub_data, const uint8_t lub_size)
{
    status_t result = kStatus_Fail;

    if ((lpub_data != NULL) && (lub_size != 0))
    {
        // Prepare to send.
        sendXfer.data = lpub_data;//sendData;
        sendXfer.dataSize = lub_size;//sizeof(sendData)/sizeof(sendData[0]);
        result = UART_TransferSendNonBlocking(UART1,&g_uartHandle, &sendXfer);
    }

    return result;
}

/*-------------------------------------
* Function:
* Desc:
* input:
* return:
* Note:
*-----------------------------------*/
void uart_Callback(uart_handle_t *handle, status_t status, void *userData)
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
