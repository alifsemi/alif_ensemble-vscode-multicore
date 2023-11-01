#ifndef APP_THREAD_1_H_
#define APP_THREAD_1_H_

#include "tx_api.h"

/* Define the ThreadX object control blocks...  */
#define TX_EVENT_REQUESTED_FLAGS_TO_SET        	(0x01)

enum AzureRtosError
{
	eRtosOk = 0,
	eRtosBytePoolCreateFail = -1,
	eRtosEventFlagsCreateFail = -2,
	eRtosByteAllocateFail = -3,
	eRtosThreadCreateFail = -4,
	eRtosEventFlagsGetTxFail = -5,
	eRtosEventFlagsGetRxFail = -6,
};

void app_thread_1_entry(ULONG thread_input);

#endif // APP_THREAD_1_H_
