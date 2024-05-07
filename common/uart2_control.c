#include "uart2_control.h"
#include "Driver_USART.h"
#include "rtos_error.h"
#include "error_handler.h"
#include "tx_api.h"

extern TX_EVENT_FLAGS_GROUP event_flags_uart;

extern ARM_DRIVER_USART ARM_Driver_USART_(UART);			  /* UART drv */
static ARM_DRIVER_USART *USARTdrv = &ARM_Driver_USART_(UART); /* UART drv inst*/

static void uart2_callback(unsigned int event);

void Uart2Init(void)
{
	int rc_init = eUart2Ok;

	rc_init = USARTdrv->Initialize((void *)uart2_callback);	/* Initialize UART driver */
	if(rc_init != eUart2Ok)
	{
		Uart2Failure(eUart2InitFail);
	}

	USARTdrv->PowerControl(ARM_POWER_FULL);	/* Power up UART peripheral */

	/* Configure UART to 115200 Bits/sec */
	USARTdrv->Control(ARM_USART_MODE_ASYNCHRONOUS |
					  ARM_USART_DATA_BITS_8 |
					  ARM_USART_PARITY_NONE |
					  ARM_USART_STOP_BITS_1 |
					  ARM_USART_FLOW_CONTROL_NONE,
					  UART2_BAUD_RATE);

	/* Enable Receiver and Transmitter lines */
	USARTdrv->Control(ARM_USART_CONTROL_TX, 1);
	USARTdrv->Control(ARM_USART_CONTROL_RX, 1);
}

void Uart2Send(const char* ptxt, const unsigned int length)
{
	int rc_send = eUart2Ok;
	int rc_rtos_flags = TX_SUCCESS;
	ULONG events = 0;

	rc_send = USARTdrv->Send(ptxt, length);
	if (rc_send != eUart2Ok)
	{
		Uart2Failure(eUart2TxFail);
	}

	rc_rtos_flags = tx_event_flags_get(&event_flags_uart, TX_EVENT_REQUESTED_FLAGS_TO_SET, TX_OR_CLEAR, &events, TX_WAIT_FOREVER);
	if (rc_rtos_flags != TX_SUCCESS)
	{
		RtosFailure(eRtosEventFlagsGetTxFail);
	}
}

void Uart2Receive(char* ptxt, const unsigned int length)
{
	int rc_receive = eUart2Ok;
	int rc_rtos_flags = TX_SUCCESS;
	ULONG events = 0;

	rc_receive = USARTdrv->Receive(ptxt, length);
	if (rc_receive != eUart2Ok)
	{
		Uart2Failure(eUart2RxFail);
	}

	rc_rtos_flags = tx_event_flags_get(&event_flags_uart, TX_EVENT_REQUESTED_FLAGS_TO_SET, TX_OR_CLEAR, &events, TX_WAIT_FOREVER);
	if (rc_rtos_flags != TX_SUCCESS)
	{
		RtosFailure(eRtosEventFlagsGetRxFail);
	}

	Uart2Send(ptxt, length);
}

static void uart2_callback(unsigned int event)
{
	if (event & ARM_USART_EVENT_SEND_COMPLETE)
	{
		/* Send Success: Wake-up Thread. */
		tx_event_flags_set(&event_flags_uart, TX_EVENT_REQUESTED_FLAGS_TO_SET, TX_OR);
	}

	if (event & ARM_USART_EVENT_RECEIVE_COMPLETE)
	{
		/* Receive Success: Wake-up Thread. */
		tx_event_flags_set(&event_flags_uart, TX_EVENT_REQUESTED_FLAGS_TO_SET, TX_OR);
	}

	if (event & ARM_USART_EVENT_RX_TIMEOUT)
	{
		/* Receive Success: Wake-up Thread. */
		tx_event_flags_set(&event_flags_uart, TX_EVENT_REQUESTED_FLAGS_TO_SET, TX_OR);
	}
}

