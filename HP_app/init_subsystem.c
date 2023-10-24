#include "init_subsystem.h"
#include "led0_control.h"
#include "mhu_control.h"
#include "error_handler.h"
#include <pinconf.h>

static void PinsConfig(void);

void SubsystemInit(void)
{
	PinsConfig();
	Led0Init();
}

static void PinsConfig(void)
{
	int32_t rc_led0_r = eOk, rc_led0_g = eOk, rc_led0_b = eOk;

    rc_led0_r = pinconf_set(PORT_12, PIN_3, PINMUX_ALTERNATE_FUNCTION_0, 0); // RED
    rc_led0_g = pinconf_set(PORT_7, PIN_4, PINMUX_ALTERNATE_FUNCTION_0, 0); // GREEN
    rc_led0_b = pinconf_set(PORT_7, PIN_0, PINMUX_ALTERNATE_FUNCTION_0, 0); // BLUE

	if ((rc_led0_r != eOk)
	||  (rc_led0_g != eOk)
	||  (rc_led0_b != eOk))
    {
    	DeviceFailure();
    }
}

