#ifndef LED1_CONTROL_H_
#define LED1_CONTROL_H_

/* LED1 gpio pins */
#define GPIO6_PORT                       6   /*< Use LED1_R,LED1_B,LED1_R GPIO port >*/
#define PIN2                            (2)   /*< LED1_R gpio pin >*/
#define PIN4                            (4)   /*< LED1_G gpio pin >*/
#define PIN6                            (6)   /*< LED1_B gpio pin >*/

enum Led1Color
{
	L1RED = PIN2,
	L1GREEN = PIN4,
	L1BLUE = PIN6,
};

enum Led1ControlError
{
	eLed1Ok = 0,
	eLed1Fail = -1,
};

void Led1Init(void);
void Led1TurnON(int color);
void Led1TurnOFF(int color);
void Led1Toggle(int color);

#endif // LED1_CONTROL_H_
