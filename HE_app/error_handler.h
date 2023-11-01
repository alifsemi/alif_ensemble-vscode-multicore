#ifndef ERROR_HANDLER_H_
#define ERROR_HANDLER_H_

void DeviceFailure(void);
void RtosFailure(int errorcode);
void GpioDrvFailure(void);
void Uart2Failure(int errorcode);

#endif // ERROR_HANDLER_H_
