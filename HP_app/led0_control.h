#ifndef LED0_CONTROL_H_
#define LED0_CONTROL_H_

/* LED0 gpio pins */
#define GPIO12_PORT                      12  /*< Use LED0_R,LED0_B GPIO port >*/
#define GPIO7_PORT                       7   /*< Use LED0_G GPIO port >*/
#define PIN3                            (3)   /*< LED0_R gpio pin >*/
#define PIN4                            (4)   /*< LED0_G gpio pin >*/
#define PIN0                            (0)   /*< LED0_B gpio pin >*/

enum Led1Color
{
	L0RED = PIN3,
	L0GREEN = PIN4,
	L0BLUE = PIN0,
};

enum Led0ControlError
{
	eLed0Ok = 0,
	eLed0Fail = -1,
};

void Led0Init(void);
void Led0TurnON(int color);
void Led0TurnOFF(int color);
void Led0Toggle(int color);

#endif // LED0_CONTROL_H_
