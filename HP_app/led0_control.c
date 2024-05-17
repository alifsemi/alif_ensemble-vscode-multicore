#include "led0_control.h"
#include "Driver_GPIO.h"
#include "commons.h"
#include "board.h"

/* GPIO port used for LED0_R, LED0_B & LED0_G */
extern  ARM_DRIVER_GPIO ARM_Driver_GPIO_(BOARD_LEDRGB0_R_GPIO_PORT);
extern  ARM_DRIVER_GPIO ARM_Driver_GPIO_(BOARD_LEDRGB0_G_GPIO_PORT);
extern  ARM_DRIVER_GPIO ARM_Driver_GPIO_(BOARD_LEDRGB0_B_GPIO_PORT);

static ARM_DRIVER_GPIO* getLed0GpioDrvAndPin(Led0Color color, int *pin)
{
    switch (color)
    {
    case L0RED:
        *pin = BOARD_LEDRGB0_R_PIN_NO;
        return &ARM_Driver_GPIO_(BOARD_LEDRGB0_R_GPIO_PORT);
    case L0GREEN:
        *pin = BOARD_LEDRGB0_G_PIN_NO;
        return &ARM_Driver_GPIO_(BOARD_LEDRGB0_G_GPIO_PORT);
    case L0BLUE:
        *pin = BOARD_LEDRGB0_B_PIN_NO;
        return &ARM_Driver_GPIO_(BOARD_LEDRGB0_B_GPIO_PORT);
    }

    GpioDrvFailure();
    return NULL;
}

void Led0InitColor(Led0Color color)
{
    int gpioPin = -1;
    ARM_DRIVER_GPIO *gpioDrv = getLed0GpioDrvAndPin(color, &gpioPin);
    if (gpioDrv) {
        if (gpioDrv->Initialize(gpioPin, NULL) != ARM_DRIVER_OK) {
            GpioDrvFailure();
            return;
        }
        gpioDrv->PowerControl(gpioPin, ARM_POWER_FULL);
        gpioDrv->SetDirection(gpioPin, GPIO_PIN_DIRECTION_OUTPUT);
    }
}

void Led0Init(void)
{
    Led0InitColor(L0RED);
    Led0InitColor(L0GREEN);
    Led0InitColor(L0BLUE);
}

void Led0TurnON(Led0Color color)
{
    int gpioPin = -1;
    ARM_DRIVER_GPIO *gpioDrv = getLed0GpioDrvAndPin(color, &gpioPin);
    if (gpioDrv) {
        gpioDrv->SetValue(gpioPin, GPIO_PIN_OUTPUT_STATE_HIGH);
    }
}

void Led0TurnOFF(Led0Color color)
{
    int gpioPin = -1;
    ARM_DRIVER_GPIO *gpioDrv = getLed0GpioDrvAndPin(color, &gpioPin);
    if (gpioDrv) {
        gpioDrv->SetValue(gpioPin, GPIO_PIN_OUTPUT_STATE_LOW);
    }
}

void Led0Toggle(Led0Color color)
{
    int gpioPin = -1;
    ARM_DRIVER_GPIO *gpioDrv = getLed0GpioDrvAndPin(color, &gpioPin);
    if (gpioDrv) {
        gpioDrv->SetValue(gpioPin, GPIO_PIN_OUTPUT_STATE_TOGGLE);
    }
}
