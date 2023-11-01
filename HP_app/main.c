#include "commons.h"
#include "init_subsystem.h"
#include "tx_api.h"

#include "led0_control.h"
#include "mhu_control.h"

int main(void)
{
	SubsystemInit();

	// Test Code
	//--------------------------------------------------------
	Led0TurnON(L0BLUE);
	Led0TurnOFF(L0BLUE);
	//--------------------------------------------------------

	tx_kernel_enter(); 		/* Enter the ThreadX kernel. */
}
