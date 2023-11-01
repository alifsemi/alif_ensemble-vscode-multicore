#include "init_subsystem.h"
#include "led1_control.h"
#include "uart2_control.h"
#include "mhu_control.h"
#include "error_handler.h"
#include <pinconf.h>

static void PinsConfig(void);

void SubsystemInit(void)
{
	PinsConfig();
	Led1Init();
	Uart2Init();
}

static void PinsConfig(void)
{
	int32_t rc_rx = eOk, rc_tx = eOk;
	int32_t rc_led1_r = eOk, rc_led1_g = eOk, rc_led1_b = eOk;

	rc_rx = pinconf_set(PORT_1, PIN_0, PINMUX_ALTERNATE_FUNCTION_1, PADCTRL_READ_ENABLE); // UART2_RX_A
    rc_tx = pinconf_set(PORT_1, PIN_1, PINMUX_ALTERNATE_FUNCTION_1, 0); // UART2_TX_A

    rc_led1_r = pinconf_set(PORT_6, PIN_2, PINMUX_ALTERNATE_FUNCTION_0, 0); // RED
    rc_led1_g = pinconf_set(PORT_6, PIN_4, PINMUX_ALTERNATE_FUNCTION_0, 0); // GREEN
    rc_led1_b = pinconf_set(PORT_6, PIN_6, PINMUX_ALTERNATE_FUNCTION_0, 0); // BLUE

    if ((rc_rx != eOk)
    ||  (rc_tx != eOk)
    ||  (rc_led1_r != eOk)
	||  (rc_led1_g != eOk)
	||  (rc_led1_b != eOk))
    {
    	DeviceFailure();
    }
}

