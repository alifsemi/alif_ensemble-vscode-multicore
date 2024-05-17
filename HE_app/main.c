#include "commons.h"
#include "init_subsystem.h"
#include "services_lib_interface.h" // SERVICES_wait_ms
#include "tx_api.h"

#include "led1_control.h"
#include "mhu_control.h"

int main(void)
{
	SubsystemInit();

	// Test Code
	//--------------------------------------------------------
	Led1TurnON(L1RED);
	SERVICES_wait_ms(500);
	Led1TurnOFF(L1RED);
	//--------------------------------------------------------

	tx_kernel_enter(); 		/* Enter the ThreadX kernel. */
}
