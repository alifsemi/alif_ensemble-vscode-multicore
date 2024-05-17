#ifndef RTOS_ERROR_1_H_
#define RTOS_ERROR_1_H_

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

#endif
