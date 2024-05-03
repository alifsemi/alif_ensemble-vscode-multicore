#include "commons.h"
#include "rtos_error.h"
#include "uart2_control.h"

void DeviceFailure(void)
{
	while(1) __WFI();
}

void RtosFailure(int errorcode)
{
	if (errorcode == eRtosBytePoolCreateFail)
		printf("[AzureRTOS] Could not create byte pool\n");
	if (errorcode == eRtosEventFlagsCreateFail)
		printf("[AzureRTOS] Could not create event flags\n");
	if (errorcode == eRtosByteAllocateFail)
		printf("[AzureRTOS] Could not create byte allocate\n");
	if (errorcode == eRtosThreadCreateFail)
		printf("[AzureRTOS] Could not create app_thread_1\n");
	if (errorcode == eRtosEventFlagsGetTxFail)
		printf("[AzureRTOS] Could not get UartSend Event Flag\n");
	if (errorcode == eRtosEventFlagsGetRxFail)
		printf("[AzureRTOS] Could not get UartReceive Event Flag\n");

	DeviceFailure();
}

void GpioDrvFailure(void)
{
	DeviceFailure();
}

void Uart2Failure(int errorcode)
{
	if (errorcode == eUart2InitFail)
		printf("[UART2] Init failed\n");

	if (errorcode == eUart2TxFail)
		printf("[UART2] TX failed\n");

	DeviceFailure();
}
