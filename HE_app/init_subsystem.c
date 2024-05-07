#include "init_subsystem.h"
#include "led1_control.h"
#include "uart2_control.h"
#include "mhu_control.h"
#include "error_handler.h"
#include "board.h"

static void PinsConfig(void);

void SubsystemInit(void)
{
	BOARD_Pinmux_Init();
	Led1Init();
	Uart2Init();
}
