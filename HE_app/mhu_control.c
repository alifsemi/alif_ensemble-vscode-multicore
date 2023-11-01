#include <RTE_Components.h>
#include CMSIS_device_header

#include "mhu_control.h"
#include "M55_HE.h"
#include "mhu.h"
#include "mhu_driver.h"
#include "services_lib_api.h"
#include "services_lib_interface.h"

#include <stdio.h>
#include <string.h>
#include <stdatomic.h>
#include <inttypes.h>
#include <stdarg.h>
#include <time.h>


// On M55 HE, these are communication HE -> HP
// On M55 HP, these are communication HP -> HE

#define SE_MHU0_RECV_BASE      	(0x40040000)
#define SE_MHU0_SEND_BASE      	(0x40050000)
#define SE_MHU1_RECV_BASE      	(0x40060000)
#define SE_MHU1_SEND_BASE      	(0x40070000)
#define MHU0_RECV_M55_BASE     	(0x40080000)	// Secure MHU-0
#define MHU0_SEND_M55_BASE     	(0x40090000)
#define MHU1_RECV_M55_BASE     	(0x400A0000)	// Non-Secure MHU-1
#define MHU1_SEND_M55_BASE     	(0x400B0000)


#define MHU_SESS_S_RX_IRQ  	(37)
#define MHU_SESS_S_TX_IRQ  	(38)
#define MHU_SESS_NS_RX_IRQ 	(39)
#define MHU_SESS_NS_TX_IRQ 	(40)
#define M55RECV_MHU0_COMB_IRQn	(41)
#define M55SEND_MHU0_COMB_IRQn 	(42)
#define M55RECV_MHU1_COMB_IRQn 	(43)
#define M55SEND_MHU1_COMB_IRQn 	(44)

#define MHU_M55_SE_MHU0        	(0)
#define MHU_M55_SE_MHU1        	(1)
#define MHU_M55_M55_MHU0       	(2)
#define MHU_M55_M55_MHU1       	(3)

static payload_t 	received_payload;	// memcopied from HandleMsg()
static atomic_bool 	msg_received;
static mhu_driver_in_t 	s_mhu_driver_in;
static mhu_driver_out_t s_mhu_driver_out;

uint32_t services_handle  = 0xffffffff;
uint32_t m55_comms_handle = 0xffffffff;

uint32_t sender_base_addr_list[NUM_MHU] 	 = {SE_MHU0_SEND_BASE, SE_MHU1_SEND_BASE,
						    MHU0_SEND_M55_BASE, MHU1_SEND_M55_BASE};
uint32_t receiver_base_addr_list[NUM_MHU] = {SE_MHU0_RECV_BASE, SE_MHU1_RECV_BASE,
						    MHU0_RECV_M55_BASE, MHU1_RECV_M55_BASE};

static void Mhu0SenderIrqHandler();
static void Mhu0ReceiverIrqHandler();
static void Mhu1SenderIrqHandler();
static void Mhu1ReceiverIrqHandler();

static void SetupIrq(int irq_num, uint32_t irq_handler);
void RxMsgCallback(uint32_t rec_id, uint32_t ch_num, uint32_t service_data);
void HandleMsg(uint32_t rec_id, uint32_t ch_num, uint32_t service_data);
void CopyVtorTableToRam();
static void SetupMhu(void);

static void Mhu0SenderIrqHandlerSE()
{
    s_mhu_driver_out.sender_irq_handler(MHU_M55_SE_MHU0);
}

static void Mhu0ReceiverIrqHandlerSE()
{
    s_mhu_driver_out.receiver_irq_handler(MHU_M55_SE_MHU0);
}

static void Mhu1SenderIrqHandlerSE()
{
    s_mhu_driver_out.sender_irq_handler(MHU_M55_SE_MHU1);
}

static void Mhu1ReceiverIrqHandlerSE()
{
    s_mhu_driver_out.receiver_irq_handler(MHU_M55_SE_MHU1);
}

static void Mhu0SenderIrqHandler()
{
    s_mhu_driver_out.sender_irq_handler(MHU_M55_M55_MHU0);
}

static void Mhu0ReceiverIrqHandler()
{
    s_mhu_driver_out.receiver_irq_handler(MHU_M55_M55_MHU0);
}

static void Mhu1SenderIrqHandler()
{
    s_mhu_driver_out.sender_irq_handler(MHU_M55_M55_MHU1);
}

static void Mhu1ReceiverIrqHandler()
{
    s_mhu_driver_out.receiver_irq_handler(MHU_M55_M55_MHU1);
}

static void SetupIrq(int irq_num, uint32_t irq_handler)
{
    NVIC_DisableIRQ(irq_num);
    NVIC_ClearPendingIRQ(irq_num);
    NVIC_SetVector(irq_num, irq_handler);
    NVIC_SetPriority(irq_num, 2);
    NVIC_EnableIRQ(irq_num);
}

void RxMsgCallback(uint32_t rec_id, uint32_t ch_num, uint32_t service_data)
{
    switch (rec_id)
    {
		case MHU_M55_SE_MHU0:
		case MHU_M55_SE_MHU1:
			SERVICES_rx_msg_callback(rec_id, ch_num, service_data);
			break;

        case MHU_M55_M55_MHU0:
        case MHU_M55_M55_MHU1:
            HandleMsg(rec_id, ch_num, service_data);
            break;

        default:
            printf("RxMsgCallback: Invalid receiver_id = %d\n", rec_id);
    }
}

void HandleMsg(uint32_t rec_id, uint32_t ch_num, uint32_t service_data)
{
	payload_t* payload = (payload_t *)GlobalToLocal((uint32_t)service_data);

	SCB_InvalidateDCache_by_Addr(payload, sizeof(payload_t));	// ensure the data is read from memory
    memcpy(&received_payload, payload, sizeof(payload_t));
    __DMB();

    atomic_store(&msg_received, true);
}

// VTOR must be copied into RAM to enable dynamic setting of interrupt handler
// This can be removed once build time vector table is fixed to include MHU interrupts
void CopyVtorTableToRam()
{
    extern const VECTOR_TABLE_Type __VECTOR_TABLE[496];
    static VECTOR_TABLE_Type MyVectorTable[496] __attribute__((aligned (2048)));

    for(int i = 0; i < 496; i++)
    {
        MyVectorTable[i] = __VECTOR_TABLE[i];
    }
    // Set the new vector table into use.
    SCB->VTOR = (uint32_t)(&MyVectorTable[0]);
    __DSB();
}

static void SetupMhu(void)
{
    s_mhu_driver_in.sender_base_address_list = sender_base_addr_list;
    s_mhu_driver_in.receiver_base_address_list = receiver_base_addr_list;
    s_mhu_driver_in.mhu_count = NUM_MHU;
    s_mhu_driver_in.send_msg_acked_callback = SERVICES_send_msg_acked_callback;
    s_mhu_driver_in.rx_msg_callback = RxMsgCallback;

    MHU_driver_initialize(&s_mhu_driver_in, &s_mhu_driver_out);

    SetupIrq(MHU_SESS_S_TX_IRQ, 	 (uint32_t)&Mhu0SenderIrqHandlerSE);
    SetupIrq(MHU_SESS_S_RX_IRQ, 	 (uint32_t)&Mhu0ReceiverIrqHandlerSE);
    SetupIrq(MHU_SESS_NS_TX_IRQ, 	 (uint32_t)&Mhu1SenderIrqHandlerSE);
    SetupIrq(MHU_SESS_NS_RX_IRQ, 	 (uint32_t)&Mhu1ReceiverIrqHandlerSE);

    SetupIrq(M55SEND_MHU0_COMB_IRQn, (uint32_t)&Mhu0SenderIrqHandler);
    SetupIrq(M55RECV_MHU0_COMB_IRQn, (uint32_t)&Mhu0ReceiverIrqHandler);
    SetupIrq(M55SEND_MHU1_COMB_IRQn, (uint32_t)&Mhu1SenderIrqHandler);
    SetupIrq(M55RECV_MHU1_COMB_IRQn, (uint32_t)&Mhu1ReceiverIrqHandler);
}

void MhuInit(void)
{
	CopyVtorTableToRam();
	SetupMhu();

    SERVICES_Setup(s_mhu_driver_out.send_message, MAXIMUM_TIMEOUT);
    m55_comms_handle = SERVICES_register_channel(MHU_M55_M55_MHU1, 0);
    SERVICES_wait_ms(0x2000000);
}

void MhuReceiveMsg(void)
{
    atomic_init(&msg_received, false);
    while(!atomic_load(&msg_received)); // msg in 'received_payload' variable.
    atomic_store(&msg_received, false);	// msg_received = false;
}

void MhuSendMsg(void)
{
	char msg_string[] = "R1 From M55_HE: Set LED0";
	uint32_t rc = SERVICE_SUCCESS;
	payload_t payload;

	sprintf(payload.msg, "%s", msg_string);
    rc = SERVICES_send_msg(m55_comms_handle, (uint32_t)LocalToGlobal(&payload));
}

// Todo: Add parameter for msg_string.
void MhuSendUserMsg(char *pusermsg)
{
	uint32_t rc = SERVICE_SUCCESS;
	payload_t payload;

	sprintf(payload.msg, "%s", pusermsg);
    rc = SERVICES_send_msg(m55_comms_handle, (uint32_t)LocalToGlobal(&payload));
}

void MhuGetPayload(payload_t *ppayload)
{
	memcpy(ppayload, &received_payload, sizeof(payload_t));
}

