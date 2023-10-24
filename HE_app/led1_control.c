#include "led1_control.h"
#include "Driver_GPIO.h"
#include "commons.h"

/* GPIO port used for LED1_R, LED1_B & LED1_G */
extern  ARM_DRIVER_GPIO ARM_Driver_GPIO_(GPIO6_PORT);
ARM_DRIVER_GPIO *gpioDrv6 = &ARM_Driver_GPIO_(GPIO6_PORT);


void Led1Init(void)
{
	int rc_r = eLed1Ok, rc_g = eLed1Ok, rc_b = eLed1Ok;

	rc_r = gpioDrv6->Initialize(PIN2, NULL);	// R
	rc_g = gpioDrv6->Initialize(PIN4, NULL);	// G
	rc_b = gpioDrv6->Initialize(PIN6, NULL);	// B

	if ((rc_r != eLed1Ok)
	|| (rc_g != eLed1Ok)
	|| (rc_b != eLed1Ok))
	{
		GpioDrvFailure();
	}
    gpioDrv6->PowerControl(PIN2, ARM_POWER_FULL);
    gpioDrv6->PowerControl(PIN4, ARM_POWER_FULL);
    gpioDrv6->PowerControl(PIN6, ARM_POWER_FULL);

    gpioDrv6->SetDirection(PIN2, GPIO_PIN_DIRECTION_OUTPUT);
    gpioDrv6->SetDirection(PIN4, GPIO_PIN_DIRECTION_OUTPUT);
    gpioDrv6->SetDirection(PIN6, GPIO_PIN_DIRECTION_OUTPUT);
}

void Led1TurnON(int color)
{
	gpioDrv6->SetValue(color, GPIO_PIN_OUTPUT_STATE_HIGH);
}

void Led1TurnOFF(int color)
{
	gpioDrv6->SetValue(color, GPIO_PIN_OUTPUT_STATE_LOW);
}

void Led1Toggle(int color)
{
	gpioDrv6->SetValue(color, GPIO_PIN_OUTPUT_STATE_TOGGLE);
}
