#ifndef LED0_CONTROL_H_
#define LED0_CONTROL_H_

typedef enum
{
    L0RED = 0,
    L0GREEN,
    L0BLUE,
} Led0Color;

void Led0Init(void);
void Led0TurnON(Led0Color color);
void Led0TurnOFF(Led0Color color);
void Led0Toggle(Led0Color color);

#endif // LED0_CONTROL_H_
