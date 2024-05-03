#include "commons.h"
#include "init_subsystem.h"
#include "services_lib_interface.h" // SERVICES_wait_ms
#include "tx_api.h"

#include "led0_control.h"
#include "mhu_control.h"

int main(void)
{
	SubsystemInit();

	// Test Code
	//--------------------------------------------------------
	Led0TurnON(L0BLUE);
	SERVICES_wait_ms(500);
	Led0TurnOFF(L0BLUE);
	//--------------------------------------------------------

	tx_kernel_enter(); 		/* Enter the ThreadX kernel. */
}
