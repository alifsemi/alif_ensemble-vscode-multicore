#ifndef UART2_CONTROL_H_
#define UART2_CONTROL_H_

#define UART      			2		/* UART Driver instance (UART0-UART7) */
#define UART2_BAUD_RATE 	(115200)

enum Uart2ControlError
{
	eUart2Ok = 0,
	eUart2InitFail = -1,
	eUart2TxFail = -2,
	eUart2RxFail = -3,
};

void Uart2Init(void);
void Uart2Send(const char* ptxt, const unsigned int length);
void Uart2Receive(const char* ptxt, const unsigned int length);

#endif // UART2_CONTROL_H_
