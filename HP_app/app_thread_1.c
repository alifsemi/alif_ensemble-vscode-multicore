#include "app_thread_1.h"
#include "tx_api.h"
#include "commons.h"
#include "init_subsystem.h"
#include "mhu_control.h"
#include "services_lib_api.h"
#include "services_lib_interface.h"
#include "app_thread_1_task1.h"

/* Define the ThreadX object control blocks...  */
#define APP_STACK_SIZE 			(1024)
#define APP_BYTE_POOL_SIZE      (9120)


TX_EVENT_FLAGS_GROUP event_flags_uart;

static UCHAR 		memory_area[APP_BYTE_POOL_SIZE];
static TX_THREAD 	app_thread_1;
static TX_BYTE_POOL byte_pool_0;

void tx_application_define(void *first_unused_memory)
{
	INT	 status 	= TX_SUCCESS;
	CHAR *pointer	= TX_NULL;

	/* Create a byte memory pool from which to allocate the thread stacks.  */
	status = tx_byte_pool_create(&byte_pool_0, "byte pool 0", memory_area, APP_BYTE_POOL_SIZE);
	if (status != TX_SUCCESS)
	{
		RtosFailure(eRtosBytePoolCreateFail);
	}

	/* Put system definition stuff in here, e.g. thread creates and other assorted create information.  */
	/* Create the event flags group used by UART thread */
	status = tx_event_flags_create(&event_flags_uart, "event flags UART");
	if (status != TX_SUCCESS)
	{
		RtosFailure(eRtosEventFlagsCreateFail);
	}

	/* Allocate the stack for thread.  */
	status = tx_byte_allocate(&byte_pool_0, (VOID **) &pointer, APP_STACK_SIZE, TX_NO_WAIT);
	if (status != TX_SUCCESS)
	{
		RtosFailure(eRtosByteAllocateFail);
	}

	/* Create the main thread.  */
	status = tx_thread_create(&app_thread_1, "app_thread_1",
								app_thread_1_entry,
								0, pointer, APP_STACK_SIZE, 1, 1,
								TX_NO_TIME_SLICE,
								TX_AUTO_START);
	if (status != TX_SUCCESS)
	{
		RtosFailure(eRtosThreadCreateFail);
	}
}

void app_thread_1_entry(ULONG thread_input)
{
	MhuInit();

	while(1)
	{
		task1_run();

		printf("[AzureRTOS M55_HP] Spinning in Thread_1\n");
		SERVICES_wait_ms(0x3000000);	// HP runs at 400Mhz, hence requires longer delay.
		__WFI();
	}
}
