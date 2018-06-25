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
uart_transfer_t receiveXfer;
uint8_t PublicSendData[UART_BUFFER_MAX_SIZE];
uint8_t UartReceiveData[1];

static T_UART_STATES uart_State = UART_IDLE;
static uint8_t UART_init_pattern = 0x00;
static uint8_t last_index = 0;
static uint8_t uart_TX_TimeOut_ctr = 0;
static uint8_t txFinished = FALSE;
static uint8_t rxFinished = FALSE;
static uint8_t DataInQueu = FALSE;

status_t RxStat = kStatus_Fail;


T_UART_STATES uart_IDLE(void);
status_t uart_Send(void);
status_t uart_Receive(void);
void uart_RXINV(UART_Type *base, uint8_t invert);

/*-------------------------------------
 * Function: uart_IDLE
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
T_UART_STATES uart_IDLE(void)
{
	T_UART_STATES NextSate = UART_INIT_ERROR;
	status_t sendReq_satus = kStatus_Fail;


	if (UART_init_pattern != UART_INIT_PATTER)
	{
		NextSate = UART_INIT_ERROR;
	}
//	else if(DataInQueu == TRUE)
//	{
//		sendReq_satus = uart_Send();
//		if(sendReq_satus == kStatus_Success)
//		{
//			DataInQueu = FALSE;
//			NextSate = UART_TX;
//			txFinished = FALSE;
//			last_index = 0;
//		}
//		else
//		{
//			if(uart_TX_TimeOut_ctr >= 3)
//			{
//				NextSate = UART_TXRX_ERROR;
//				uart_TX_TimeOut_ctr = 0;
//			}
//			else
//			{
//				NextSate = UART_IDLE;
//			}
//			uart_TX_TimeOut_ctr ++;
//		}
//	}
	else//if(rxFinished == TRUE)
	{
		//uart_Receive();
		//rxFinished = FALSE;
		//NextSate = UART_RX;
	}

	//else
	{
		NextSate = UART_IDLE;
	}

	//NextSate = UART_IDLE;
	//uart_Receive();

	return NextSate;
}

/*-------------------------------------
 * Function: uart_main
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void uart_main(void)
{
	T_UART_STATES local_uart_state = UART_TXRX_ERROR;
	status_t sendReq_satus = kStatus_Fail;

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
		sendReq_satus = uart_Receive();
		if(sendReq_satus == kStatus_Success)
		{
			uart_State = UART_IDLE;
			rxFinished = FALSE;
			last_index = 0;
		}
		else
		{
			uart_State = UART_TXRX_ERROR;
		}
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
 *
 * Desc: This function is called after
 * 		reset, and initialize the UART to
 * 		default values 115200bps, no parity
 * 		8bits x char, one stop bit Tx enable
 * 		Rx enable
 *
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void uart_init(void)
{
    UART_GetDefaultConfig(&user_config);
    user_config.baudRate_Bps = 115200U;
    user_config.enableTx = FALSE;
    user_config.enableRx = FALSE;
    user_config.stopBitCount = kUART_OneStopBit;
    user_config.parityMode = kUART_ParityDisabled;

    /* Initialize LPUART0 pins below */
    /* Affects PORTE_PCR1 register UART1_TX*/
    PORT_SetPinMux(PORTE, 0u, kPORT_MuxAlt3);

    /* Affects PORTE_PCR2 register UART1_RX*/
    PORT_SetPinMux(PORTE, 1u, kPORT_MuxAlt3);

    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortE);

    if(UART_Init(UART1,&user_config,UART_srcClock_Hz) == kStatus_Success)
    {
		UART_TransferCreateHandle(UART1, &g_uartHandle, (uart_transfer_callback_t) uart_Callback, NULL);

		UART_ClearStatusFlags(UART1, kUART_RxDataRegFullFlag|kUART_TransmissionCompleteFlag|kUART_TxDataRegEmptyFlag);

		//uart_RXINV(UART1, TRUE);

		//RxStat = uart_Receive();

		//UART_EnableInterrupts(UART1, kUART_RxDataRegFullInterruptEnable|kUART_RxActiveEdgeInterruptEnable);

		//EnableIRQ(UART1_IRQn);


		uart_State = UART_IDLE;

		UART_init_pattern = UART_INIT_PATTERN;


		UART1->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);

    }
    else
    {
    	uart_State = UART_INIT_ERROR;
    }


}


/*-------------------------------------
 * Function: UART_RXINV
 *
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void uart_RXINV(UART_Type *base, uint8_t invert)
{
	uint8_t reg = base->S2;

	if(invert >= TRUE)
	{
		reg |= 0x10u;
	}
	else
	{
		reg &= 0xEFu;
	}
	base->S2 = reg;
}


/*-------------------------------------
 * Function: uart_Send
 * Desc: This function is called to
 *		 start a UART Tx request using UART1.
 * 		UART Handle shall be available/created
 * 		before using this interface
 *
 * input:	lpub_data
 * 			lub_size
 *
 * return:	1 kStatus_Fail
 * 			0 kStatus_Success
 * Note:
 * SRS:
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
 * Function: uart_Receive
 * Desc: This function is called to
 *		 Rx UART Datat using UART1.
 * 		UART Handle shall be available/created
 * 		before using this interface
 *
 * input:	lpub_data
 * 			lub_size
 *
 * return:	1 kStatus_Fail
 * 			0 kStatus_Success
 * Note:
 * SRS:
 *-----------------------------------*/
status_t uart_Receive(void)
{
	status_t result = kStatus_InvalidArgument;

	//receiveXfer.data = UartReceiveData;
	//receiveXfer.dataSize = 1u;

	//if(RxStat != kStatus_UART_RxBusy || rxFinished == TRUE)
	//{
		RxStat = UART_TransferReceiveNonBlocking(UART1, &g_uartHandle, &receiveXfer, &receiveXfer.dataSize);
	//}

	return RxStat;
}


/*-------------------------------------
 * Function: uart_ReqTx
 *
 * Desc: This function is called to
 * 		 Queue a UART Tx operation by the application.
 * 		 UART Handle shall be available/created before
 * 		 using this interface
 *
 * input:
 *
 * return:   1 kStatus_Fail
*            0 kStatus_Success
 * Note:
 * SRS:
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
			else{/*Error Data buffer length is incorrect*/}
		}
		else
		{/*Error Data buffer is empty*/}
	}
	else
	{/*Error UART is not initialised*/}
	return result;
}

/*-------------------------------------
 * Function: uart_Callback
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
void uart_Callback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_UART_TxIdle == status)
    {
        txFinished = true;
    }

    else if (kStatus_UART_RxIdle == status)
    {
        rxFinished = true;
    }

//    else if(kStatus_UART_RxRingBufferOverrun == status)
//    {
//    	rxFinished = 5U;
//    	UART_ClearStatusFlags(UART1, kUART_RxOverrunFlag);
//    }
//
//    else if(kStatus_UART_RxHardwareOverrun == status)
//    {
//    	rxFinished = true;
//    }

    else if(kStatus_UART_FramingError == status)
    {
    	UART_ClearStatusFlags(UART1, kUART_FramingErrorFlag);
    	rxFinished = 0x58;
    }

    //UART_ClearStatusFlags(UART1, kUART_TransmissionCompleteFlag|kUART_TxDataRegEmptyFlag|kUART_RxDataRegFullFlag|kUART_FramingErrorFlag);
    //UART_ClearStatusFlags(UART1, );
}


/*-------------------------------------
 * Function: UART1_IRQHandler
 * Desc:
 * input:
 * return:
 * Note:
 * SRS:
 *-----------------------------------*/
//void UART1_IRQHandler(void)
//{
//    static volatile UartIRQ = 0;
//
//    if(kUART_RxDataRegFullFlag & UART_GetStatusFlags(UART1))
//	{
//    	UartIRQ = UartIRQ++;
//	}
//    else if(kUART_RxActiveEdgeFlag & UART_GetStatusFlags(UART1))
//    {
//    	UartIRQ = 0x55u;
//    }
//
//    UART_ClearStatusFlags(UART1, kUART_TransmissionCompleteFlag | kUART_RxDataRegFullFlag | kUART_RxActiveEdgeFlag);
//}
