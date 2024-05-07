#include "led1_control.h"
#include "Driver_GPIO.h"
#include "commons.h"
#include "board.h"

/* GPIO port used for LED1_R, LED1_B & LED1_G */
extern  ARM_DRIVER_GPIO ARM_Driver_GPIO_(BOARD_LEDRGB1_R_GPIO_PORT);
extern  ARM_DRIVER_GPIO ARM_Driver_GPIO_(BOARD_LEDRGB1_G_GPIO_PORT);
extern  ARM_DRIVER_GPIO ARM_Driver_GPIO_(BOARD_LEDRGB1_B_GPIO_PORT);

static ARM_DRIVER_GPIO* getLed1GpioDrvAndPin(Led1Color color, int *pin)
{
    switch (color)
    {
    case L1RED:
        *pin = BOARD_LEDRGB1_R_PIN_NO;
        return &ARM_Driver_GPIO_(BOARD_LEDRGB1_R_GPIO_PORT);
    case L1GREEN:
        *pin = BOARD_LEDRGB1_G_PIN_NO;
        return &ARM_Driver_GPIO_(BOARD_LEDRGB1_G_GPIO_PORT);
    case L1BLUE:
        *pin = BOARD_LEDRGB1_B_PIN_NO;
        return &ARM_Driver_GPIO_(BOARD_LEDRGB1_B_GPIO_PORT);
    }

    GpioDrvFailure();
    return NULL;
}

void Led1InitColor(Led1Color color)
{
    int gpioPin = -1;
    ARM_DRIVER_GPIO *gpioDrv = getLed1GpioDrvAndPin(color, &gpioPin);
    if (gpioDrv) {
        if (gpioDrv->Initialize(gpioPin, NULL) != ARM_DRIVER_OK) {
            GpioDrvFailure();
            return;
        }
        gpioDrv->PowerControl(gpioPin, ARM_POWER_FULL);
        gpioDrv->SetDirection(gpioPin, GPIO_PIN_DIRECTION_OUTPUT);
    }
}

void Led1Init(void)
{
    Led1InitColor(L1RED);
    Led1InitColor(L1GREEN);
    Led1InitColor(L1BLUE);
}

void Led1TurnON(Led1Color color)
{
    int gpioPin = -1;
    ARM_DRIVER_GPIO *gpioDrv = getLed1GpioDrvAndPin(color, &gpioPin);
    if (gpioDrv) {
        gpioDrv->SetValue(gpioPin, GPIO_PIN_OUTPUT_STATE_HIGH);
    }
}

void Led1TurnOFF(Led1Color color)
{
    int gpioPin = -1;
    ARM_DRIVER_GPIO *gpioDrv = getLed1GpioDrvAndPin(color, &gpioPin);
    if (gpioDrv) {
        gpioDrv->SetValue(gpioPin, GPIO_PIN_OUTPUT_STATE_LOW);
    }
}

void Led1Toggle(Led1Color color)
{
    int gpioPin = -1;
    ARM_DRIVER_GPIO *gpioDrv = getLed1GpioDrvAndPin(color, &gpioPin);
    if (gpioDrv) {
        gpioDrv->SetValue(gpioPin, GPIO_PIN_OUTPUT_STATE_TOGGLE);
    }
}
