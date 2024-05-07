#ifndef MHU_CONTROL_H_
#define MHU_CONTROL_H_

#define NUM_MHU                (4)
#define MAXIMUM_TIMEOUT        (0x01000000)

typedef struct payload
{
    char msg[64];
} payload_t;

void MhuInit(void);
void MhuReceiveMsg(void);	// msg in 'received_payload' variable.
void MhuSendMsg(void);
void MhuSendUserMsg(char *pusermsg);
void MhuGetPayload(payload_t *ppayload);

#endif // MHU_CONTROL_H_
