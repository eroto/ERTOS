/*
 * uart.c
 *
 *  Created on: 17/10/2016
 *      Author: Enrique
 */



#include <stdint.h>
#include <string.h>
#include "mytypedef.h"
#include "uart.h"

uart_config_t user_config;
uart_handle_t g_uartHandle;
uint8_t UartSendBuffer[UART_BUFFER_MAX_SIZE];
uart_transfer_t sendXfer;

static T_UART_STATES uart_State = UART_IDLE;
static uint8_t UART_init_pattern = 0x00;
static uint8_t last_index = 0;
static uint8_t uart_TX_TimeOut_ctr = 0;
static uint8_t txFinished = FALSE;
static uint8_t rxFinished = FALSE;
static uint8_t DataInQueu = FALSE;


T_UART_STATES uart_IDLE(void);
status_t uart_Send(void);

/*-------------------------------------
* Function: uart_IDLE
* Desc: T
* input:
* return:
* Note:
*-----------------------------------*/
T_UART_STATES uart_IDLE(void)
{
	T_UART_STATES NextSate = UART_INIT_ERROR;
	status_t sendReq_satus = kStatus_Fail;

	if (UART_init_pattern != UART_INIT_PATTER)
	{
		NextSate = UART_INIT_ERROR;
	}
	else if(DataInQueu == TRUE)
	{
		sendReq_satus = uart_Send();
		if(sendReq_satus == kStatus_Success)
		{
			DataInQueu = FALSE;
			NextSate = UART_TX;
			txFinished = FALSE;
			last_index = 0;
		}
		else
		{
			if(uart_TX_TimeOut_ctr >= 3)
			{
				NextSate = UART_TXRX_ERROR;
				uart_TX_TimeOut_ctr = 0;
			}
			else
			{
				NextSate = UART_IDLE;
			}
			uart_TX_TimeOut_ctr ++;
		}
	}
	else
	{
		NextSate = UART_IDLE;
	}
	return NextSate;
}

/*-------------------------------------
* Function: uart_main
* Desc: T
* input:
* return:
* Note:
*-----------------------------------*/
void uart_main(void)
{
	T_UART_STATES local_uart_state = UART_TXRX_ERROR;

	switch(uart_State)
	{

	case UART_IDLE:

		local_uart_state = uart_IDLE();

		break;

	case UART_TX:
		if(txFinished == TRUE)
		{
			local_uart_state = UART_IDLE;
		}
		else
		{
			if(uart_TX_TimeOut_ctr >= 2)
			{
				local_uart_state = UART_TXRX_ERROR;
				uart_TX_TimeOut_ctr = 0;
			}
			else
			{
				local_uart_state = UART_TX;
			}

			uart_TX_TimeOut_ctr++;
		}

		break;

	case UART_RX:
		break;

	case UART_INIT_ERROR:
		break;

	case UART_TXRX_ERROR:
		break;

	default:
		break;
	}

	uart_State = local_uart_state;
}


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

    uart_State = UART_IDLE;

    UART_init_pattern = UART_INIT_PATTERN;
}

/*-------------------------------------
* Function: uart_Send
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
status_t uart_Send(void)
{
	status_t result = kStatus_Fail;
	// Prepare to send.
	sendXfer.data = UartSendBuffer;//sendData;
	sendXfer.dataSize = last_index;//sizeof(sendData)/sizeof(sendData[0]);
	result = UART_TransferSendNonBlocking(UART1,&g_uartHandle, &sendXfer);

	return result;
}

/*-------------------------------------
* Function: uart_Tx_Request
* Desc: This function is called to
* Queue a UART Tx operation by the application.
* UART Handle shall be available/created
* before using this interface
* input:    lpub_data
*           lub_size
*
* return:   1 kStatus_Fail
*           0 kStatus_Success
* Note:
*-----------------------------------*/
status_t uart_ReqTx(uint8_t* lpub_data, uint8_t lub_size)
{
	status_t result = kStatus_Fail;


	if(UART_init_pattern == UART_INIT_PATTERN)
	{
		if(lpub_data != NULL )
		{
			if((lub_size > 0) && (lub_size <= 49))
			{
				//Copy Data to UART Buffer
				memcpy(&UartSendBuffer[last_index], lpub_data, lub_size);
				last_index = last_index + lub_size;

				DataInQueu = TRUE;

				result = kStatus_Success;
			}
			else{}
		}
		else
		{}
	}
	return result;
}

/*-------------------------------------
* Function: uart_Callback
* Desc:
* input:
* return:
* Note:
*-----------------------------------*/

void uart_Callback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_UART_TxIdle == status)
    {
        txFinished = true;
    }

    if (kStatus_UART_RxIdle == status)
    {
        rxFinished = true;
    }

    UART_ClearStatusFlags(UART1, kUART_TransmissionCompleteFlag);
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
