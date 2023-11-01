#include "app_thread_1_task1.h"
#include "commons.h"
#include "mhu_control.h"
#include "services_lib_api.h"
#include "services_lib_interface.h"
#include <string.h>
#include "led0_control.h"

static void get_cmd_from_he(char *pcmd, int size);
static void send_ack_to_he(char *pmsgack);
static int parse_cmd(char *pcmd);

static void get_cmd_from_he(char *pcmd, int size)
{
	payload_t msg_from_he;

	MhuReceiveMsg();	// Get CMD from HE
	MhuGetPayload(&msg_from_he);
	printf("[M55_HP] MHU Received msg = %s\n", msg_from_he.msg);
	pcmd = memcpy(pcmd, msg_from_he.msg, size);
}

static void send_ack_to_he(char *pmsgack)
{
	MhuSendUserMsg(pmsgack);	// Send ACK to HE
}

static int parse_cmd(char *pcmd)
{
	int rc = eTaskOk;

	switch (pcmd[0])
	{
		case ('R'):
		{
			(pcmd[1]=='1')?Led0TurnON(L0RED):Led0TurnOFF(L0RED);
			break;
		}
		case ('G'):
		{
			(pcmd[1]=='1')?Led0TurnON(L0GREEN):Led0TurnOFF(L0GREEN);
			break;
		}
		case ('B'):
		{
			(pcmd[1]=='1')?Led0TurnON(L0BLUE):Led0TurnOFF(L0BLUE);
			break;
		}
		default:
			printf("[Thread_1_Task1: M55_HP] Invalid parse cmd\n");
			rc = eTaskFail;
			break;
	}

	return rc;
}

void task1_run(void)
{
	int rc = eTaskOk;
	char cmd[2] = {0};
	char msgack[8] = "NACK";

	get_cmd_from_he(cmd, 2);

	rc = parse_cmd(cmd);
	if (rc == eTaskOk)
		sprintf(msgack, "ACK");
	else
		sprintf(msgack, "NACK");

	send_ack_to_he(msgack);
}
