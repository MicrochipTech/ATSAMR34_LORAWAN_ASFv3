/**
* \file  conf_app.h
*
* \brief LORAWAN Demo Application include file
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
 

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

/****************************** INCLUDES **************************************/

/****************************** MACROS **************************************/

/* Number of software timers */
#define TOTAL_NUMBER_OF_TIMERS            (25u)

/* If enabled, app will use preprogrammed devEUI from module's NVM location */
#if (MODULE_EUI_READ == 1)
#define NVM_UID_ADDRESS   ((volatile uint16_t *)(0x0080400AU))
#endif

/*Define the Sub band of Channels to be enabled by default for the application*/
#define SUBBAND 1
#if ((SUBBAND < 1 ) || (SUBBAND > 8 ) )
#error " Invalid Value of Subband"
#endif

/* Activation method constants */
#define OVER_THE_AIR_ACTIVATION           LORAWAN_OTAA
#define ACTIVATION_BY_PERSONALIZATION     LORAWAN_ABP

/* Message Type constants */
#define UNCONFIRMED                       LORAWAN_UNCNF
#define CONFIRMED                         LORAWAN_CNF

/* Enable one of the activation methods */
#define DEMO_APP_ACTIVATION_TYPE               OVER_THE_AIR_ACTIVATION
//#define DEMO_APP_ACTIVATION_TYPE             ACTIVATION_BY_PERSONALIZATION

/* Select the Type of Transmission - Confirmed(CNF) / Unconfirmed(UNCNF) */
#define DEMO_APP_TRANSMISSION_TYPE              UNCONFIRMED
//#define DEMO_APP_TRANSMISSION_TYPE            CONFIRMED

/* FPORT Value (1-255) */
#define DEMO_APP_FPORT                           1

/* Device Class - Class of the device (CLASS_A/CLASS_C) */
#define DEMO_APP_ENDDEVICE_CLASS                 CLASS_A
//#define DEMO_APP_ENDDEVICE_CLASS               CLASS_C

#define DEMO_APP_JOIN_NONCE_TYPE                (JOIN_NONCE_INCREMENTAL)


/* ABP Join Parameters */
#define DEMO_DEVICE_ADDRESS                     0x76897689
#define DEMO_APPLICATION_SESSION_KEY            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x89}
#define DEMO_NETWORK_SESSION_KEY                {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x89, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x89, 0x76}


/* OTAA Join Parameters */

#define DEMO_DEVICE_EUI                         {0xde, 0xaf, 0xfa, 0xce, 0xde, 0xaf, 0xfa, 0xce}
#define DEMO_JOIN_EUI                    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}
#define DEMO_APPLICATION_KEY                    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}

/* Multicast Parameters */
#define DEMO_APP_MCAST_GROUPID                  0
#define DEMO_APP_MCAST_ENABLE                   true
#define DEMO_APP_MCAST_GROUP_ADDRESS            0x0037CC56
#define DEMO_APP_MCAST_APP_SESSION_KEY          {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6}
#define DEMO_APP_MCAST_NWK_SESSION_KEY          {0x3C, 0x8F, 0x26, 0x27, 0x39, 0xBF, 0xE3, 0xB7, 0xBC, 0x08, 0x26, 0x99, 0x1A, 0xD0, 0x50, 0x4D}

/* This macro defines the application's default sleep duration in milliseconds */
#define DEMO_CONF_DEFAULT_APP_SLEEP_TIME_MS     1000

/* This macro defines the application's default periodic timer duration in milliseconds */
#define DEMO_CONF_APP_PERIODIC_TIMER_PERIOD_MS			5000

/* This macro enables or disables the LED indications */
//#define DEMO_LED_STATUS

#endif /* APP_CONFIG_H_ */

