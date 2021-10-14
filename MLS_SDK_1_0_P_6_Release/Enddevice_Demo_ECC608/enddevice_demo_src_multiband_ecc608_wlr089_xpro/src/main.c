/**
* \file  main.c
*
* \brief LORAWAN Demo Application Main file
*
*
* Copyright (c) 2020 Microchip Technology Inc. and its subsidiaries.
*
* \asf_license_start
*
* \page License
*
* Subject to your compliance with these terms, you may use Microchip
* software and any derivatives exclusively with Microchip products.
* It is your responsibility to comply with third party license terms applicable
* to your use of third party software (including open source software) that
* may accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
* INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
* AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
* LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
* LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
* SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
* POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
* ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
* RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*
* \asf_license_stop
*
*/
/*
* Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
*/

//================================== INCLUDES ==================================
#include "asf.h"
#include "delay.h"
#include "sys.h"
#include "system_init.h"
#include "system_task_manager.h"
#include "radio_driver_hal.h"
#include "lorawan.h"
#include "sw_timer.h"
#if defined(CONF_PMM_ENABLE)
#include "sleep_timer.h"
#endif
#include "pds_interface.h"
#include "sal.h"
#include "sio2host.h"
#include "enddevice_demo.h"

//=================================== MACROS ===================================

//============================== TYPE DEFINITIONS ==============================

//========================= STATIC FUNCTION PROTOTYPES =========================
/* Initialize all the required hardware and software modules */
static void driverInit(void);

/* Print the cause for the latest reset in console */
static void printResetCause(void);

//============================== GLOBAL VARIABLES ==============================

//================================== EXTERNS ===================================

//============================ FUNCTION DEFINITIONS ============================
static void driverInit(void)
{
	SalStatus_t salStatus = SAL_SUCCESS;

    /* Initialize the Radio Hardware */
    HAL_RadioInit();

    /* Initialize the Software Timer Module */
    SystemTimerInit();

#ifdef CONF_PMM_ENABLE
    /* Initialize the Sleep Timer Module */
    SleepTimerInit();
#endif

#if (ENABLE_PDS == 1)
    /* PDS Module Init */
    PDS_Init();
#endif

	/* Initializes the Security modules */
	salStatus = SAL_Init();

	if (SAL_SUCCESS != salStatus)
	{
		printf("Initialization of Security module (SAL) failed\r\n");

		/* Stop Further execution */
		while(1);
	}

    /* Start application prints in new line */
    printf("\r\n");
}
//------------------------------------------------------------------------------

static void printResetCause(void)
{
    enum system_reset_cause rcause = system_get_reset_cause();

    printf("\r\nLast reset cause: ");

    if(rcause & (1 << 6))
    {
        printf("System Reset Request\r\n");
    }
    else if(rcause & (1 << 5))
    {
        printf("Watchdog Reset\r\n");
    }
    else if(rcause & (1 << 4))
    {
        printf("External Reset\r\n");
    }
    else if(rcause & (1 << 2))
    {
        printf("Brown Out 33 Detector Reset\r\n");
    }
    else if(rcause & (1 << 1))
    {
        printf("Brown Out 12 Detector Reset\r\n");
    }
    else if(rcause & (1 << 0))
    {
        printf("Power-On Reset\r\n");
    }
}
//------------------------------------------------------------------------------

/* Main function of Enddevice demo application */
int main(void)
{
    /* System Initialization */
    system_init();

    /* Initialize the delay driver */
    delay_init();

    /* Initialize the board target resources */
    board_init();

    /* Enable interrupts */
    INTERRUPT_GlobalInterruptEnable();

	/* Initialize the Serial Interface */
	sio2host_init();
    delay_ms(1000);

    /* Display the latest reset cause */
    printResetCause();

    /* Initialize Hardware and Software Modules */
	driverInit();

    /* Initialize demo application */
    mote_demo_init();

    /* Kick-start application tasks */
    Stack_Init();
    
    while(1)
    {
        /* Read a character from serial port */
		serial_data_handler();

        /* Run all the posted tasks */
        SYSTEM_RunTasks();
    }
}
//------------------------------------------------------------------------------

//eof main.c
