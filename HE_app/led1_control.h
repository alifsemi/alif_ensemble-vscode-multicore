#ifndef LED1_CONTROL_H_
#define LED1_CONTROL_H_

typedef enum
{
    L1RED = 0,
    L1GREEN,
    L1BLUE,
} Led1Color;

void Led1Init(void);
void Led1TurnON(Led1Color color);
void Led1TurnOFF(Led1Color color);
void Led1Toggle(Led1Color color);

#endif // LED1_CONTROL_H_
