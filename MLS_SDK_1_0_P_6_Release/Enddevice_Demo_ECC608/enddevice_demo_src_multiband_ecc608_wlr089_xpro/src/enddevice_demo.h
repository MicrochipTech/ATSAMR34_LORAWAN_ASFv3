/**
* \file  enddevice_demo.h
*
* \brief LORAWAN Demo Application header
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
#ifndef ENDDEVICE_DEMO_H
#define ENDDEVICE_DEMO_H

//================================== INCLUDES ==================================

//=================================== MACROS ===================================
/* Period of the restorePromptTimer - default: 1s */
#define APP_RESTORE_TIMER_PERIOD    (1000)

/* Number of attempts for resotration */
#define APP_RESTORE_ATTEMPT_COUNT   (5)

/* Status bitmask for LoRaWAN Join indicator */
#define APP_LORAWAN_NW_JOINED       (0x00000001)

#if (CERT_APP == 1)
/* Default period of the certification timer */
#define APP_CERT_TIMER_PERIOD       (5000)
#endif

#define CERT_PROTOCOL_PACKAGE_IDENTIFIER    (6)
#define CERT_PROTOCOL_PACKAGE_VERSION       (1)


bool is_certification_mode_enabled;
bool is_deact_fport224;
#if (ENABLE_PDS == 1)
#define PDS_CERT_APP_PARAM_START_INDEX	(PDS_FILE_APP_DATA1_13_IDX << 8)


#define PDS_CERT_APP_ENABLE_SIZE			(sizeof(is_certification_mode_enabled))
#define PDS_DEACTIVATE_FPORT_SIZE			(sizeof(is_deact_fport224))

#define PDS_CERT_APP_ENABLE_OFFSET		PDS_FILE_START_OFFSET
#define PDS_DEACTIVATE_FPORT_OFFSET		(PDS_CERT_APP_ENABLE_OFFSET + PDS_SIZE_OF_ITEM_HDR + PDS_DEACTIVATE_FPORT_SIZE)

#define PDS_CERT_APP_ENABLE_ADDR		((uint8_t *)&(is_certification_mode_enabled))
#define PDS_DEACTIVATE_FPORT_ADDR		((uint8_t *)&(is_deact_fport224))
typedef enum
{
	PDS_CERT_APP_ENABLE = PDS_CERT_APP_PARAM_START_INDEX,
	PDS_DEACTIVATE_FPORT,
	PDS_CERT_APP_PARAM_ITEM_ID_MAX_VALUE  /* Always add new items above this value */
}pds_cert_app_items_t;



#endif

/*
* NOTE: command length is minimum valid length,
* inclusive of its identifer
*/

/* command identifiers */
#define CPC_PACKAGE_VERSION_REQ_LEN         (1)
#define CPC_PACKAGE_VERSION_REQ             (0x00)
#define CPC_PACKAGE_VERSION_ANS             CPC_PACKAGE_VERSION_REQ

#define CPC_DUT_RESET_REQ_LEN               (1)
#define CPC_DUT_RESET_REQ                   (0x01)
#define CPC_DUT_RESET_ANS                   CPC_DUT_RESET_REQ

#define CPC_DUT_JOIN_REQ_LEN                (1)
#define CPC_DUT_JOIN_REQ                    (0x02)

#define CPC_SWITCH_CLASS_REQ_LEN            (2)
#define CPC_SWITCH_CLASS_REQ                (0x03)

#define CPC_ADR_BIT_CHANGE_REQ_LEN          (2)
#define CPC_ADR_BIT_CHANGE_REQ              (0x04)

#define CPC_REGIONAL_DUTYCYCLE_CTRL_REQ_LEN (2)
#define CPC_REGIONAL_DUTYCYCLE_CTRL_REQ     (0x05)

#define CPC_TX_PERIODICITY_CHANGE_REQ_LEN   (2)
#define CPC_TX_PERIODICITY_CHANGE_REQ       (0x06)

//** can contain 0..N variable payload
#define CPC_TX_FRAMES_CTRL_REQ_LEN          (2)
#define CPC_TX_FRAMES_CTRL_REQ              (0x07)

//** can contain 0..N variable payload
#define CPC_ECHO_INC_PAYLOAD_REQ_LEN        (1)
#define CPC_ECHO_INC_PAYLOAD_REQ            (0x08)
#define CPC_ECHO_INC_PAYLOAD_ANS            CPC_ECHO_INC_PAYLOAD_REQ

#define CPC_RX_APP_CNT_REQ_LEN              (1)
#define CPC_RX_APP_CNT_REQ                  (0x09)
#define CPC_RX_APP_CNT_ANS                  CPC_RX_APP_CNT_REQ

#define CPC_RX_APP_CNT_RESET_REQ_LEN        (1)
#define CPC_RX_APP_CNT_RESET_REQ            (0x0A)

// ones that trigger MAC commands
#define CPC_LINK_CHECK_REQ_LEN              (1)
#define CPC_LINK_CHECK_REQ                  (0x20)

#define CPC_DEVICE_TIME_REQ_LEN             (1)
#define CPC_DEVICE_TIME_REQ                 (0x21)

#define CPC_PING_SLOT_INFO_REQ_LEN          (2)
#define CPC_PING_SLOT_INFO_REQ              (0x22)

// miscellaneous
#define CPC_TX_CW_REQ_LEN                   (7)
#define CPC_TX_CW_REQ                       (0x7D)

#define CPC_DUT_FPORT224_DISABLE_REQ_LEN    (1)
#define CPC_DUT_FPORT224_DISABLE_REQ        (0x7E)

#define CPC_DUT_VERSIONS_REQ_LEN            (1)
#define CPC_DUT_VERSIONS_REQ                (0x7F)
#define CPC_DUT_VERSIONS_ANS                CPC_DUT_VERSIONS_REQ

//============================== TYPE DEFINITIONS ==============================
/* Enumerate the application task handler functions */
typedef enum _AppTaskIds_t
{
    DISPLAY_TASK_HANDLER,
    PROCESS_TASK_HANDLER,
    APP_TASKS_COUNT
}AppTaskIds_t;

/* Enumerate the application states */
typedef enum _AppTaskState_t
{
    INIT_MENU_STATE,
    MAIN_MENU_STATE,
    APP_MENU_STATE
#if (ENABLE_PDS == 1)
    ,RESTORE_PROMPT_STATE
#endif
}AppTaskState_t;

//============================ FUNCTION PROTOTYPES =============================
/*
* \brief    Function that will be called by scheduler to run application tasks
*/
SYSTEM_TaskStatus_t APP_TaskHandler(void);

/*
* \brief    Function to read a character from serial port and process it
*/
void serial_data_handler(void);

/*
* \brief    Post a task to application task handler
*
* \param1   id -    application task ID
*/
void app_post_task(AppTaskIds_t id);

/*
* \brief    Display task functions
*/
SYSTEM_TaskStatus_t display_task(void);

/*
* \brief    Process task functions
*/
SYSTEM_TaskStatus_t process_task(void);

/*
* \brief    Initializes the demo application
*/
void mote_demo_init(void);

/*
* \brief    Sends a DUT Join Req
*/
void send_join_req(ActivationType_t type);

/*
* \brief    Callback function from MAC on end of Activation procedure
*
* \param1  status    - Status of the join or activation request
*/
void demo_join_data_callback(StackRetStatus_t status);


void app_pds_fid13_CB(void);
/*
* \brief    Sends a uplink
*/
void send_data(void);

/*
* \brief    Callback function from MAC on data reception
*
* \param1   appHandle - Returns the pointer which created the send request
* \param2   data      - pointer to the callback data structure
*/
void demo_app_data_callback(void *app_handle, appCbParams_t *data);

/*
* \brief    Prints current application configuration
*/
void print_app_config(void);

/*
* \brief     Function to Initialize set default parameters
* \param1    ismBand - ISM band
* \param2    index - Index of the ISM band table
* \return    LORAWAN_SUCCESS, if successfully set all the parameters
*            LORAWAN_INVALID_PARAMETER, otherwise
*/
StackRetStatus_t mote_set_params(IsmBand_t ism_band, const uint16_t index);

/*
* \brief    Configures the activation parameters for the given type
*
* \param1   joinType - OTAA or ABP
*
* \return   'LORAWAN_SUCCESS' if no error in configuration
*           else 'error status'
*/
StackRetStatus_t set_join_params(ActivationType_t join_type);


#if (CERT_APP == 1)
/*
* \brief    Configures the certification activation parameters for the given type
*
* \param1   joinType - OTAA or ABP
*
* \return   'LORAWAN_SUCCESS' if no error in configuration
*           else 'error status'
*/
StackRetStatus_t set_certification_join_params(ActivationType_t join_type);
#endif

/*
* \brief    Configures the end-device class
*
* \param1   edClass - class of the device
*
* \return   'LORAWAN_SUCCESS' if no error in configuration
*            else 'error status'
*/
StackRetStatus_t set_device_type(EdClass_t ed_class);

/*
* \brief    Configures multicast parameters
*/
void set_mcast_params (void);

/*
* \brief    Function to print array of characters
*
* \param1  *array  - Pointer of the array to be printed
* \param2   length - Length of the array
*/
void print_array(uint8_t *array, uint8_t length);

/*
* \brief    Puts the application to sleep
*/
void app_sleep(void);

/*
* \brief    Reads the 64-bit unique ID from EDBG chip
*/

#if !defined (CRYPTO_DEV_ENABLED) && (EDBG_EUI_READ == 1)
void edbg_dev_eui_read(void);
#endif /* #if !defined (CRYPTO_DEV_ENABLED) && (MODULE_EUI_READ == 1) */

/*
* \brief    Reads the 64-bit unique ID from module's NVM location
*/
#if !defined (CRYPTO_DEV_ENABLED) && (MODULE_EUI_READ == 1)
void module_dev_eui_read(void);
#endif /* #if !defined (CRYPTO_DEV_ENABLED) && (MODULE_EUI_READ == 1) */

#endif /* #ifndef ENDDEVICE_DEMO_H */
/* eof enddevice_demo.c */
