# VSCode Multicore Project based on Azure RTOS
A simple multicore "Hello World" example project based on Azure RTOS with M55-HP and M55-HE applications.

To build the projects for a supported board other than the DevKit, you may have to download
the Board Library submodule. From your local clone, do the following:
1. *submodule init*
2. *submodule update*
3. Update the board.h file to pick the right variant of the board. By default the template will build for gen2 DevKit.
**NOTE**: Current project does not utilize the board library. To be enabled in the future. 

## For building the application for a specific core:
1. Make sure you select the configuration by pressing F1 and typing "select a configuration" and picking the core - HE or HP
2. Press CTRL-SHIFT-B to build the porject. Default is set to build for executing from TCM
3. Press F1 and "Run Tasks" and run the "Install Debug Stubs with Security Toolkit"
5. Press F5 to start debugging

## For Multicore example build the projects for HE and HP using the step above. To debug both cores at the same time select
launch configuration "Alif Ensemble Multicore Debugging"

To Do:
1. Currently both cores have to be loaded and executed. Support for HE to boot HP to be added.
3. Add delays betweek LEDs ON and OFF to provide indication that the cores are running.
4. Remove the RTSS Secure MHU pair from the number of MHU used. Thus freeing up a pair of MHUs.

## Project Details:
**Prerequisites**

Setup the VSCode environment as outlined in [Getting Started with VSCode](https://alifsemi.com/download/AUGD0012). In addtion you need the following:

	- Gen2 DevKit
	- AlifSemi CMSIS pack v0.9.6 or above
	- AzureRTOS pack 0.3.2 or above

	- CMSIS components to be enabled in cproject file:
		- SE runtime Services:
			- core
			- Initialization helper
		- RTOS
			- Kernel
		- SOC periperhals
			- DMA
			- GPIO
			- MHU
			- PINCONF
			- USART
		- Core
		- Startup
  
1. This project is composed of 2 sub-projects,
	- HE_app and
	- HP_app

2. Demo usecase:
	- This demo envisions the scenario of a user-triggered run-time task.
	Here the user can enter commands from serial console to a HE core, simulating a wakeup use-case.
	And instruct the HP core to perform a specific task.

	- For this usecase, UART2 is connected to HE core as serial console.
	And using LED0 (R,G,B) ON/OFF control as a specific task.

	- The commands input on the serial console are simple,
		- R1 = turn ON RED led0.
		- R0 = turn OFF RED led0.
		- G1 = turn ON GREEN led0.
		- G0 = turn OFF GREEN led0.
		- B1 = turn ON BLUE led0.
		- B0 = turn OFF BLUE led0.
		
3. Source organization (APIs):
	- main.c 			(Initial entry point of the C program)
	- init_subsystem.c/.h 		(Pin-mux configuration and hardware initialization)
	- app_thread1.c/.h 		(AzureRTOS thread #1)
	- app_thread_1_task1.c/.h 	(Task #1 on HP core)
	- mhu_control.c/.h		(MHU configuration and control)
	- uart2_control.c/.h		(UART2 configuration and control)
	- led_control.c/.h		(LED 0/1 configuration and control)
	- error_handler.c/.h		(Error handling routines)
	- services_lib_interface.c	(part of include for SE Services support)

4. MHU (Message Handling Unit):
	- The HE core captures the commands entered on the serial console,
	and using the MHU module, passes these to the HP core to execute the specified task.

	- MHU module (mhu_control.c/.h) consists of following APIs,
		- payload_t : data structure containing a 64-byte buffer for message transfer.
		- service_data : pointer to the message payload that is transfered between the HE and HP cores using MHU.
		- msg_received : atomic flag on which MhuReceiveMsg() pends. This is signaled from HandleMsg(), called when MHU interrupt is triggered.

		Defined MHU pairs:

		Secure MHU-0 pair:
		- MHU0_RECV_M55_BASE: base address = 0x40080000
		- MHU0_SEND_M55_BASE: base address = 0x40090000

		Non-Secure MHU-1 pair: (EAST-WEST use-case)
		- MHU1_RECV_M55_BASE: base address = 0x400A0000
		- MHU1_SEND_M55_BASE: base address = 0x400B0000

		IRQ:
		- M55RECV_MHU0_COMB_IRQn	(41)
		- M55SEND_MHU0_COMB_IRQn 	(42)
		- M55RECV_MHU1_COMB_IRQn 	(43)
		- M55SEND_MHU1_COMB_IRQn 	(44)

		Number of MHU's used	= 4
		- 2-pair of MHU's for SESS (NORTH/SOUTH)
		- 2-pair of MHU's to send/receive messages between RTSS HE & HP. (EAST/WEST uses the Non-secure type)

		- MhuInit()		
			- Copy vector table to SRAM, (Need to check with Kimmo or Teppo why this is required) 
			  Initialize MHU driver, Setup Interrupt handlers, 
			  Setup SE Services and 
			  Register MHU channel (important):
				- It is here that the desired EAST-WEST channel assignment is registered with the SE Services.
				- without this the EAST-WEST MHU comms won't work.
		- MhuReceiveMsg()
			- pending call waiting for message to be delivered via Interrupt handler callback.

		- MhuSendMsg()			
		- MhuSendUserMsg()
			- Uses SE sevices call "SERVICES_send_msg()" for sending messages over MHU.
			- It requires "m55_comms_handle" to be returned from call to "SERVICE_regiser_channel()"
			- The "handle" is what references the correct MHU to use for commms.
		- MhuGetPayload()
			- utility function to memcopy the payload for higher-layer use.

5. NOTES:
	a. For RTOS based application, the MhuInit() needs to be called from within Thread context. 
	   If this is called from outside thread context, then the MHU does not work.
	b. The only link to figure out which MHU is being used is from the base-address being specified in "sender_base_addr_list[]" and "receiver_base_addr_list[]".
	   These arrays get passed to "MHU_driver_initialize()" via struct "s_mhu_driver_in".
	c. Removing the SESS MHU pairs and only using RTSS Non-secure MHU pair also does not work.
	   It appears that the SESS MHU pairs are required for the Non-secure RTSS MHU's to work.
	   This could be due to the reliance on SE Services library on MHUs for its operation.

6. Build and Debug instructions:	
	a. You can build each project seperately.
	b. You can launch each project seperately.
		- Each project contains its own Debug Configuration file which can be run independently.
