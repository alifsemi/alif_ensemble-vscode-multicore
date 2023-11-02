#include "app_thread_1.h"
#include "tx_api.h"
#include "commons.h"
#include "init_subsystem.h"
#include "mhu_control.h"
#include "services_lib_api.h"
#include "services_lib_interface.h"

#include <ctype.h>
#include <stdio.h>

#include "uart2_control.h"
#define CONSOLE_RX_BUFF_SIZE		(32)

/* Define the ThreadX object control blocks...  */
#define APP_STACK_SIZE             	(1024)
#define APP_BYTE_POOL_SIZE          (9120)


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
	payload_t msg_from_hp;
	char selected_led = 0; // 'G', 'R' or 'B', 0 means no LED selected
	char selected_state = 0; // '0' = OFF, '1' = ON, 0 means no state selected

	const char thread_string[] = "[M55_HE] East<->West MHU demo. RGB LED0 control.\n";
	const char select_led_str[] = "Select a LED by entering G, R or B\n";
	const char select_state_str[] = "Select LED state, 0 = OFF, 1 = ON\n";

	char console_string[CONSOLE_RX_BUFF_SIZE] = {0};

	MhuInit();

	Uart2Send(thread_string, sizeof(thread_string));

	while(1)
	{
		if (!selected_led) {
			Uart2Send(select_led_str, strlen(select_led_str));
			Uart2Receive(&selected_led, 1);
			selected_led = toupper(selected_led);
			if (selected_led != 'G' && selected_led != 'R' && selected_led != 'B') {
				/* User gave invalid LED identifier*/
				selected_led = 0;
			}
		} else {
			Uart2Send(select_state_str, strlen(select_state_str));
			Uart2Receive(&selected_state, 1);
			if (selected_state != '0' && selected_state != '1') {
				/* User gave invalid LED state */
				selected_state = 0;
			}
		}

		/* User has selected a LED and state, so let's send it to M55 HP */
		if (selected_led && selected_state) {
			sprintf(console_string, "%c%c", selected_led, selected_state);
			selected_led = 0;
			selected_state = 0;
			printf("[UART2] %s\n", console_string);

			MhuSendUserMsg(console_string);

			MhuReceiveMsg();	// Get ACK from HP
			MhuGetPayload(&msg_from_hp);
			printf("[M55_HE] MHU received msg = %s\n", msg_from_hp.msg);

			printf("[AzureRTOS M55_HE] Spinning in Thread_1\n");
			SERVICES_wait_ms(0x1000000);	// HE runs at 160MHz compared to HP at 400MHz
			__WFI();
		}
	}
}
