#include "led0_control.h"
#include "Driver_GPIO.h"
#include "commons.h"

/* GPIO port used for LED0_R & LED0_B */
extern  ARM_DRIVER_GPIO ARM_Driver_GPIO_(GPIO12_PORT);
ARM_DRIVER_GPIO *gpioDrv12 = &ARM_Driver_GPIO_(GPIO12_PORT);

/* GPIO port used for LED0_G */
extern  ARM_DRIVER_GPIO ARM_Driver_GPIO_(GPIO7_PORT);
ARM_DRIVER_GPIO *gpioDrv7 = &ARM_Driver_GPIO_(GPIO7_PORT);


void Led0Init(void)
{
	int rc_r = eLed0Ok, rc_g = eLed0Ok, rc_b = eLed0Ok;

	rc_r = gpioDrv12->Initialize(PIN3, NULL);	// R
	rc_g = gpioDrv7->Initialize(PIN4, NULL);	// G
	rc_b = gpioDrv12->Initialize(PIN0, NULL);	// B

	if ((rc_r != eLed0Ok)
	|| (rc_g != eLed0Ok)
	|| (rc_b != eLed0Ok))
	{
		GpioDrvFailure();
	}

    gpioDrv12->PowerControl(PIN3, ARM_POWER_FULL);
    gpioDrv7->PowerControl(PIN4, ARM_POWER_FULL);
    gpioDrv12->PowerControl(PIN0, ARM_POWER_FULL);

    gpioDrv12->SetDirection(PIN3, GPIO_PIN_DIRECTION_OUTPUT);
    gpioDrv7->SetDirection(PIN4, GPIO_PIN_DIRECTION_OUTPUT);
    gpioDrv12->SetDirection(PIN0, GPIO_PIN_DIRECTION_OUTPUT);
}

void Led0TurnON(int color)
{
	if (color == L0GREEN)
	{
		gpioDrv7->SetValue(color, GPIO_PIN_OUTPUT_STATE_HIGH);
	}
	else
	{
		gpioDrv12->SetValue(color, GPIO_PIN_OUTPUT_STATE_HIGH);
	}
}

void Led0TurnOFF(int color)
{
	if (color == L0GREEN)
	{
		gpioDrv7->SetValue(color, GPIO_PIN_OUTPUT_STATE_LOW);
	}
	else
	{
		gpioDrv12->SetValue(color, GPIO_PIN_OUTPUT_STATE_LOW);
	}
}

void Led0Toggle(int color)
{
	if (color == L0GREEN)
	{
		gpioDrv7->SetValue(color, GPIO_PIN_OUTPUT_STATE_TOGGLE);
	}
	else
	{
		gpioDrv12->SetValue(color, GPIO_PIN_OUTPUT_STATE_TOGGLE);
	}
}
