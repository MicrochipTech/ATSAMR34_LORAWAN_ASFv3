/**
* \file  enddevice_demo.c
*
* \brief LORAWAN Demo Application file
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

/**
* \mainpage
* \section preface Preface
* The EndDevice_Demo_application available in Atmel Studio,
* is used to send the temperature sensor data through the
* LoRaWAN network to the network server.
* <P>• This example provides option to user to configure regional band in run time.</P>
* <P>• Using this example application, LoRaWAN Functionalities such as Joining, Sending data
* and changing end device class is demonstrated.</P>
* <P>• This example showcases sleep functionality of LoRaWAN Stack and the Hardware.</P>
* <P>• This example demonstrates storing stack parameters in NVM using PDS. </P>
*/

//================================== INCLUDES ==================================
#include <stdint.h>
#include <stdlib.h>

#include "asf.h"
#include "delay.h"
#include "atomic.h"

#include "system_task_manager.h"
#include "pds_interface.h"
#include "radio_driver_hal.h"
#include "radio_interface.h"
#include "lorawan.h"

#include "sio2host.h"
#include "resources.h"
#include "sw_timer.h"
#include "LED.h"
#include "pmm.h"

#if (EDBG_EUI_READ == 1)
#include "edbg_eui.h"
#endif

#if (CERT_APP == 1)
#include "conf_certification.h"
#endif

#include "conf_app.h"
#include "conf_pmm.h"
#include "conf_sio2host.h"
#include "conf_sal.h"

#ifdef CRYPTO_DEV_ENABLED
#include "cryptoauthlib.h"
#endif /* #ifdef CRYPTO_DEV_ENABLED */

#include "enddevice_demo.h"

//============================== TYPE DEFINITIONS ==============================
/* Enumerate the possible choices in init menu */
typedef enum _InitMenuChoice {
    INIT_MENU_DUMMY,
    INIT_MENU_DEMO_APP
#if(CERT_APP == 1)
    ,INIT_MENU_CERT_MODE
#endif
} InitMenuChoice_t;

/* Enumerate the possible choices in main menu */
typedef enum _MainMenuChoice {
    MAIN_MENU_DUMMY,
#if(EU_BAND == 1)
    MAIN_MENU_EU868,
#endif
#if(NA_BAND == 1)
    MAIN_MENU_NA915,
#endif
#if(AU_BAND == 1)
    MAIN_MENU_AU915,
#endif
#if(AS_BAND == 1)
    MAIN_MENU_AS923,
#endif
#if(JPN_BAND == 1)
    MAIN_MENU_JPN923,
#endif
#if(KR_BAND == 1)
    MAIN_MENU_KR920,
#endif
#if(IND_BAND == 1)
    MAIN_MENU_IND865,
#endif
#if(ENABLE_PDS == 1)
    MAIN_MENU_CLEAR_PDS,
#endif
    MAIN_MENU_RESET_BOARD
} MainMenuChoice_t;

/* Enumerate the possible choices in app menu */
typedef enum _AppMenuChoice {
    APP_MENU_DUMMY,
    APP_MENU_SEND_JOIN_REQ,
    APP_MENU_SEND_DATA,
	APP_MENU_START_PERIODIC_DATA,
	APP_MENU_STOP_PERIODIC_DATA,
#if defined(CONF_PMM_ENABLE)
    APP_MENU_SLEEP,
#endif
    APP_MENU_MAIN_MENU
} AppMenuChoice_t;

//================================= CONSTANTS ==================================
/* First menu */
const char * init_menu[] = {
    "Demo application",
#if(CERT_APP == 1)
    "Enable/disable Certification mode",
#endif
    '\0'
};

/* Main Menu */
const char * main_menu[] = {
#if(EU_BAND == 1)
    "EU868",
#endif
#if(NA_BAND == 1)
    "NA915",
#endif
#if(AU_BAND == 1)
    "AU915",
#endif
#if(AS_BAND == 1)
    "AS923",
#endif
#if(JPN_BAND == 1)
    "JPN923",
#endif
#if(KR_BAND == 1)
    "KR920",
#endif
#if(IND_BAND == 1)
    "IND865",
#endif
#if(ENABLE_PDS == 1)
    "Clear PDS",
#endif
    "Reset Board",
    '\0'
};

/* App Menu */
const char * app_menu[] = {
    "Send Join Request",
    "Send Data",
	"Start Periodic Data",
	"Stop Periodic Data",
#if defined(CONF_PMM_ENABLE)
    "Sleep",
#endif
    "Main Menu",
    '\0'
};

/* Band names */
const char* band_strings[] = {
    "Dummy",
#if(EU_BAND == 1)
    "EU868",
#endif
#if(NA_BAND == 1)
    "NA915",
#endif
#if(AU_BAND == 1)
    "AU915",
#endif
#if(AS_BAND == 1)
    "AS923",
#endif
#if(JPN_BAND == 1)
    "JPN923",
#endif
#if(KR_BAND == 1)
    "KR920",
#endif
#if(IND_BAND == 1)
    "IND865"
#endif
};

/* Band enumeration mapping table */
const uint8_t band_table[] = {
    0xFF,
#if(EU_BAND == 1)
    ISM_EU868,
#endif
#if(NA_BAND == 1)
    ISM_NA915,
#endif
#if(AU_BAND == 1)
    ISM_AU915,
#endif
#if(AS_BAND == 1)
    ISM_THAI923,
#endif
#if(JPN_BAND == 1)
    ISM_JPN923,
#endif
#if(KR_BAND == 1)
    ISM_KR920,
#endif
#if(IND_BAND == 1)
    ISM_IND865,
#endif
    0xFF,
    0xFF
};

/* Stack status message strings */
const char * lw_status_str[] = {
    "RADIO_SUCCESS",
    "RADIO_NO_DATA",
    "RADIO_DATA_SIZE",
    "RADIO_INVALID_REQ",
    "RADIO_BUSY",
    "RADIO_OUT_OF_RANGE",
    "RADIO_UNSUPPORTED_ATTR",
    "RADIO_CHANNEL_BUSY",
    "SUCCESS",
    "NWK_NOT_JOINED",
    "INVALID_PARAMETER",
    "KEYS_NOT_INITIALIZED",
    "SILENT_IMMEDIATELY_ACTIVE",
    "FCNTR_ERROR_REJOIN_NEEDED",
    "INVALID_BUFFER_LENGTH",
    "MAC_PAUSED",
    "NO_CHANNELS_FOUND",
    "BUSY",
    "NO_ACK",
    "NWK_JOIN_IN_PROGRESS",
    "RESOURCE_UNAVAILABLE",
    "INVALID_REQUEST",
    "UNSUPPORTED_BAND",
    "FCNTR_ERROR",
    "MIC_ERROR",
    "INVALID_MTYPE",
    "MCAST_HDR_INVALID",
    "TX_TIMEOUT",
    "RADIO_TX_TIMEOUT",
    "MAX_MCAST_GROUP_REACHED",
    "INVALID_PACKET",
    "RXPKT_ENCRYPTION_FAILED",
    "TXPKT_ENCRYPTION_FAILED",
    "SKEY_DERIVATION_FAILED",
    "MIC_CALCULATION_FAILED",
    "SKEY_READ_FAILED",
    "JOIN_NONCE_ERROR"
};

//============================== GLOBAL VARIABLES ==============================
/* Lorawan Send Request buffer */
LorawanSendReq_t app_lorawan_send_req;
char app_msg_buffer[25];
uint8_t app_msg_buffer_length;
float app_temperature_celsius;
float app_temperature_fahrenheit;
uint8_t led_timer;
uint8_t app_timer;
uint32_t app_timer_period = DEMO_CONF_APP_PERIODIC_TIMER_PERIOD_MS;
bool periodic_uplink_mode_enabled = false;

/* Variables for serial port processing */
bool start_receiving = false;
uint8_t serial_buffer = 0;

/* Task flag for application task handlers */
volatile uint8_t app_task_flags = 0x00u;
volatile AppTaskState_t app_task_state = INIT_MENU_STATE;

#if (CERT_APP == 1)
/* Certification mode status */


/* Timer for periodic uplinks needed for certification */
uint8_t cert_timer;

/* Period for the certTimer */
uint32_t cert_timer_period = APP_CERT_TIMER_PERIOD;

/* Certification app payload buffer */
uint8_t cert_msg_buffer[255];

/* Certification app payload length */
uint8_t cert_msg_buffer_length;

/* Downlink counter */
uint16_t cert_rx_app_cnt = 0;

/* Join type */
ActivationType_t cert_join_type;

/* Send request buffer */
LorawanSendReq_t cert_lorawan_send_req;

/* Whether certification data to send in CNF or UNCNF? */
TransmissionType_t cert_confirmed = CERT_APP_TRANSMISSION_TYPE;

/* Certification test port */
uint8_t cert_fport = CERT_APP_FPORT;

/* Is data received on Test FPort? */
bool cert_fport_received = false;

/* Timer used for continuous wave transmission period */
uint8_t cw_tx_timer;

/* Certification OTAA activation parameters */
uint8_t cert_dev_eui[8] = CERT_DEVICE_EUI;
uint8_t cert_join_eui[8] = CERT_APPLICATION_EUI;
uint8_t cert_app_key[LORAWAN_SESSIONKEY_LENGTH] = CERT_APPLICATION_KEY;

/* Certification ABP activation parameters */
uint32_t cert_dev_addr = CERT_DEVICE_ADDRESS;
uint8_t cert_apps_key[LORAWAN_SESSIONKEY_LENGTH] = CERT_APPLICATION_SESSION_KEY;
uint8_t cert_nwks_key[LORAWAN_SESSIONKEY_LENGTH] = CERT_NETWORK_SESSION_KEY;

/* Is data receive in downlink? */
bool is_pkt_received = false;
#endif /* #if (CERT_APP == 1) */

/* Flag for Deactivate FPORT 224 message */
bool is_deact_fport224 = false;
/* Timer to elapse timeout in Reset Stack after request from CertApp */
uint8_t NvicSystemResetTimer;
/* Timer interval for NVICSystemResetTimer */
#define APP_NVIC_SYSTEM_RESET_TMR_PERIOD    1000

#if (ENABLE_PDS == 1)
/* Timer to elapse timeout in restore prompt */
uint8_t restore_prompt_timer;
/* Stores the character to interrupt restore timer */
uint8_t restore_prompt_stop_key;
/* Restoration attempt count */
volatile uint8_t restore_attempt_count = APP_RESTORE_ATTEMPT_COUNT;
#endif /* #if (ENABLE_PDS == 1) */

/* Status indicator for ED whether joined or not */
bool joined = false;

/* Current ISM band used by APP */
IsmBand_t current_ism_band;

#ifdef DEMO_LED_STATUS
/* LED status */
uint8_t led_off = LOFF;
uint8_t led_on = LON;
uint8_t led_toggle = LTOGGLE;
#endif

/*ABP Join Parameters */
uint32_t demo_dev_addr = DEMO_DEVICE_ADDRESS;
uint8_t demo_nwks_key[LORAWAN_SESSIONKEY_LENGTH] = DEMO_NETWORK_SESSION_KEY;
uint8_t demo_apps_key[LORAWAN_SESSIONKEY_LENGTH] = DEMO_APPLICATION_SESSION_KEY;

#ifndef CRYPTO_DEV_ENABLED
/* OTAA join parameters */
uint8_t demo_dev_eui[8] = DEMO_DEVICE_EUI;
uint8_t demo_join_eui[8] = DEMO_JOIN_EUI;
uint8_t demo_app_key[LORAWAN_SESSIONKEY_LENGTH] = DEMO_APPLICATION_KEY;
#endif

/* Muticast Parameters */
bool demo_mcast_enable = DEMO_APP_MCAST_ENABLE;
uint32_t demo_mcast_dev_addr = DEMO_APP_MCAST_GROUP_ADDRESS;
uint8_t demo_mcast_nwks_key[LORAWAN_SESSIONKEY_LENGTH] = DEMO_APP_MCAST_NWK_SESSION_KEY;
uint8_t demo_mcast_apps_key[LORAWAN_SESSIONKEY_LENGTH] = DEMO_APP_MCAST_APP_SESSION_KEY;
uint8_t demo_mcast_group_id = DEMO_APP_MCAST_GROUPID;
#if (ENABLE_PDS==1)
const ItemMap_t pds_cert_app_item_list[] = {
	DECLARE_ITEM(PDS_CERT_APP_ENABLE_ADDR,
	PDS_FILE_APP_DATA1_13_IDX,
	(uint8_t)PDS_CERT_APP_ENABLE,
	PDS_CERT_APP_ENABLE_SIZE,
	PDS_CERT_APP_ENABLE_OFFSET),
	DECLARE_ITEM(PDS_DEACTIVATE_FPORT_ADDR,
	PDS_FILE_APP_DATA1_13_IDX,
	(uint8_t)PDS_DEACTIVATE_FPORT,
	PDS_DEACTIVATE_FPORT_SIZE,
	PDS_DEACTIVATE_FPORT_OFFSET)	
};
#endif

//========================= STATIC FUNCTION PROTOTYPES =========================
PdsStatus_t app_data_pds_reg(void);
static void app_run_task(AppTaskIds_t task, AppTaskState_t state);

static void process_init_menu(void);
static void process_main_menu(void);
static void process_app_menu(void);
#if (ENABLE_PDS == 1)
static void process_restore_prompt(void);
#endif

#ifdef DEMO_LED_STATUS
static void led_timer_callback(void);
#endif
static void app_timer_callback(void);

static SYSTEM_TaskStatus_t (*app_task_handlers[APP_TASKS_COUNT])(void) = {
    /* In the order of descending priority */
    display_task,
    process_task
};
/* Callback for the NvicSystemResetTimer */
static void nvic_system_reset_timer_callback(void);

#if (ENABLE_PDS == 1)
/* Callback for the restorePromptTimer */
static void restore_prompt_timer_callback(void);
#endif

#ifdef CONF_PMM_ENABLE
static void app_resources_uninit(void);
static void app_resources_init(void);
static void app_pmm_wakeup_callback(uint32_t slept_duration);
#endif

#if (CERT_APP == 1)
static void cert_timer_callback(void);
static void cert_exec_protocol_cmd(uint8_t *buffer, uint8_t buflen);
static bool cert_is_protocol_cmd_valid(uint8_t *buffer, uint8_t len);
static void cert_cw_wave_tx_callback(void);
#endif

static inline void print_stack_status(StackRetStatus_t status);

#ifdef CRYPTO_DEV_ENABLED
static void print_ecc_info(void);
#endif

//============================ FUNCTION DEFINITIONS ============================
SYSTEM_TaskStatus_t APP_TaskHandler(void) {
    if (app_task_flags) {
        for (uint16_t task_id = 0; task_id < APP_TASKS_COUNT; task_id++) {
            if ((1 << task_id) & (app_task_flags)) {
                ATOMIC_SECTION_ENTER
                app_task_flags &= ~(1 << task_id);
                ATOMIC_SECTION_EXIT

                app_task_handlers[task_id]();

                if (app_task_flags) {
                    SYSTEM_PostTask(APP_TASK_ID);
                }

                break;
            }
        }
    }

    return SYSTEM_TASK_SUCCESS;
}
//------------------------------------------------------------------------------

void serial_data_handler(void) {
    int rx_int;
    char rx_char;

    /* check the global variable condition if it is ok to receive now? */
    if (start_receiving) {
        rx_int = sio2host_getchar_nowait();
        if (-1 != rx_int) {
            rx_char = (char)rx_int;
            switch(rx_char) {
                case '\b':
                case '\r':
                case '\n':
                case '\t':
                    ; /* ignore these inputs */
                break;
                default:
                {
                    start_receiving = false;

                    /* set the received character in buffer */
                    serial_buffer = (uint8_t)rx_char;
                    serial_buffer -= (uint8_t)'0';
                    app_run_task(PROCESS_TASK_HANDLER, app_task_state);
                }
            }
        }
    }
}
//------------------------------------------------------------------------------

void app_post_task(AppTaskIds_t id) {
    ATOMIC_SECTION_ENTER
    app_task_flags |= (1 << id);
    ATOMIC_SECTION_EXIT
    /* Also post a APP task to the system */
    SYSTEM_PostTask(APP_TASK_ID);
}
//------------------------------------------------------------------------------

#if (ENABLE_PDS == 1)
PdsStatus_t app_data_pds_reg(void)
{
	PdsStatus_t pds_result = PDS_OK;
	PdsFileMarks_t app_filemarks;
	PdsOperations_t cert_app_pds_ops[PDS_CERT_APP_PARAM_ITEM_ID_MAX_VALUE];
	app_filemarks.fileMarkListAddr = cert_app_pds_ops;
	app_filemarks.numItems = (uint8_t)(PDS_CERT_APP_PARAM_ITEM_ID_MAX_VALUE & 0x00FF);
	app_filemarks.itemListAddr =(ItemMap_t *)&pds_cert_app_item_list;
	app_filemarks.fIDcb = app_pds_fid13_CB;
	PDS_RegFile(PDS_FILE_APP_DATA1_13_IDX,app_filemarks);
	if(PDS_OK != pds_result)
	{
		printf("PDS register file failed = 0x%02X", pds_result);
	}
	return pds_result;

}
#endif /* #if (ENABLE_PDS == 1) */

//------------------------------------------------------------------------------
void app_pds_fid13_CB(void)
{
	
}

//------------------------------------------------------------------------------
SYSTEM_TaskStatus_t display_task(void) {
    bool ask_choice = true;
#ifdef DEMO_LED_STATUS
    set_LED_data(LED_AMBER, &led_off);
    set_LED_data(LED_GREEN, &led_off);
#endif
    printf("\r\n\r\n");
    switch(app_task_state) {
        case INIT_MENU_STATE:
        {
            uint8_t i = 0;
            while ('\0' != init_menu[i]) {
                printf("%2d. %s\r\n", (i + 1), init_menu[i]);
                i++;
            }
        }
        break;

        case MAIN_MENU_STATE:
        {
            uint8_t i = 0;
            while ('\0' != main_menu[i]) {
                printf("%2d. %s\r\n", (i + 1), main_menu[i]);
                i++;
            }
        }
        break;

        case APP_MENU_STATE:
        {
            uint8_t i = 0;
            while ('\0' != app_menu[i]) {
                printf("%2d. %s\r\n", (i + 1), app_menu[i]);
                i++;
            }
        }
        break;
#if (ENABLE_PDS == 1)
        case RESTORE_PROMPT_STATE:
        {
            printf("ED will restore the previous configuration!\r\n");
#ifdef DEMO_LED_STATUS
            set_LED_data(LED_AMBER, &led_off);
            set_LED_data(LED_GREEN, &led_off);
#endif
            app_run_task(PROCESS_TASK_HANDLER, RESTORE_PROMPT_STATE);
            ask_choice = false;
        }
        break;
#endif
        default:
        {
            printf("Warn - %s:%d - Invalid appTaskState\r\n",
                __FUNCTION__, __LINE__);
        }
        break;
    }

    if (ask_choice) {
        printf("\r\nEnter your choice:\r\n");
        start_receiving = true;
    }

    return LORAWAN_SUCCESS;
}
//------------------------------------------------------------------------------

SYSTEM_TaskStatus_t process_task(void)
{
    printf("\r\n\r\n");
    switch(app_task_state) {
        case INIT_MENU_STATE:
        {
            process_init_menu();
        }
        break;

        case MAIN_MENU_STATE:
        {
            process_main_menu();
        }
        break;

        case APP_MENU_STATE:
        {
            process_app_menu();
        }
        break;
#if (ENABLE_PDS == 1)
        case RESTORE_PROMPT_STATE:
        {
            process_restore_prompt();
        }
        break;
#endif
        default:
        {
            printf("Warn - %s:%d - Invalid appTaskState\r\n",
                __FUNCTION__, __LINE__);
        }
        break;
    }
    return LORAWAN_SUCCESS;
}
//------------------------------------------------------------------------------

void send_join_req(ActivationType_t type) {
    StackRetStatus_t status;
#if (CERT_APP == 1)
    if (is_certification_mode_enabled) {
        StackRetStatus_t stat = set_certification_join_params(type);
        if (LORAWAN_SUCCESS != stat) {
            printf("Certification Join parameters initialization failed\r\n");
        }
    }
    else
#endif
    {
        StackRetStatus_t stat = set_join_params(type);
        if (LORAWAN_SUCCESS != stat) {
            printf("\nJoin parameters initialization failed\r\n");
        }
    }
    printf("\r\n%s Join Request In Progress...\r\n", (LORAWAN_OTAA == type) ? "OTAA" : "ABP");
    status = LORAWAN_Join(type);
    
#if (CERT_APP == 1)
    cert_join_type = type;
#endif
#ifdef DEMO_LED_STATUS
    set_LED_data(LED_GREEN, &led_off);

    if (!is_certification_mode_enabled) {
        if (LORAWAN_SUCCESS == status) {
            if (SwTimerIsRunning(led_timer)) {
                SwTimerStop(led_timer);
            }
            SwTimerStart(led_timer, MS_TO_US(100), SW_TIMEOUT_RELATIVE, led_timer_callback, NULL);
            set_LED_data(LED_GREEN, &led_on);
        }  
    }      
#endif
    (void)status;
}
//------------------------------------------------------------------------------

void demo_join_data_callback(StackRetStatus_t status)
{
#ifdef DEMO_LED_STATUS
    if (SwTimerIsRunning(led_timer)) {
        SwTimerStop(led_timer);
    }
    
    set_LED_data(LED_GREEN, &led_off);
    set_LED_data(LED_AMBER, &led_off);
#endif
	printf("Join response\r\n");
    
    if (LORAWAN_RADIO_BUSY == status) {
        printf("OTAA Join Response NOT received\r\n");
    }
    
    if (LORAWAN_SUCCESS != status) {
        printf("Join failed\r\n");
#ifdef DEMO_LED_STATUS
        set_LED_data(LED_AMBER, &led_on);
#endif
#if (CERT_APP == 1)
        if (is_certification_mode_enabled && (LORAWAN_OTAA == cert_join_type)) {
            printf("\r\nOTAA Join Request...\r\n");
            status = LORAWAN_Join(LORAWAN_OTAA);
            print_stack_status(status);
            return;
        }
#endif
    }
    else { /* on successful join */                
        bool true_bool = true;
        uint32_t new_dev_addr;
        /* Get the new device address */
        LORAWAN_GetAttr(DEV_ADDR, NULL, &new_dev_addr);
        LORAWAN_SetAttr(ADR, &true_bool);
        printf("Join success\r\n");
        printf("New device address : 0x%08x\r\n", (unsigned int)new_dev_addr);
        /* Check for address conflict with multicast group addr */
        if (DEMO_APP_MCAST_GROUP_ADDRESS == new_dev_addr) {
            printf("Warn - %s:%d - Address conflict b/w DevAddr and McGroupAddr\r\n",
                __FUNCTION__, __LINE__);
        }
		print_stack_status(status); 
#if (ENABLE_PDS == 1)
        PDS_StoreAll();
#endif

#if (CERT_APP == 1)
	if (is_certification_mode_enabled && (LORAWAN_OTAA == cert_join_type)) {
		/* Start periodic data uplinks */
		send_data();
		return;
	}
#endif
       
        print_app_config();
#ifdef DEMO_LED_STATUS
        set_LED_data(LED_GREEN, &led_on);
#endif

    }
#ifdef DEMO_LED_STATUS
    if (LORAWAN_SUCCESS != status) {
        set_LED_data(LED_AMBER, &led_on);
    }
#endif
    app_run_task(DISPLAY_TASK_HANDLER, APP_MENU_STATE);
}
//------------------------------------------------------------------------------

void send_data(void)
{
    uint8_t datarate;
	static uint8_t min_dr_flag = 0;
    LorawanSendReq_t *req;
    LorawanStatus_t lw_status;
    StackRetStatus_t ret_status;
    uint32_t fcnt_up;
    uint16_t available_size;
    LORAWAN_GetAttr(LORAWAN_STATUS, NULL, &lw_status);
    
    if (false == lw_status.networkJoined) {
        printf("Warn - Cannot uplink, device not joined\r\n");
        if (SwTimerIsRunning(app_timer)) {
            SwTimerStop(app_timer);
        }
        app_run_task(DISPLAY_TASK_HANDLER, APP_MENU_STATE);
        return;
    }

#if (CERT_APP == 1)
    if (is_certification_mode_enabled) {
        is_pkt_received = false;
        
        /* Restart certification period timer */
        if (SwTimerIsRunning(cert_timer)) {
            SwTimerStop(cert_timer);
        }
        SwTimerStart(cert_timer,
            MS_TO_US(cert_timer_period),
            SW_TIMEOUT_RELATIVE,
            cert_timer_callback,
            NULL
        );
        
        if (CERT_APP_FPORT == cert_fport) {
            cert_msg_buffer[0] = (uint8_t)(cert_rx_app_cnt >> 8);
            cert_msg_buffer[1] = (uint8_t) cert_rx_app_cnt;
            cert_msg_buffer_length = 2;
        } else if (TEST_PORT_NB != cert_fport) {
            memset(cert_msg_buffer, 0x00, sizeof(cert_msg_buffer));
            cert_msg_buffer_length = 0;
        }
        
        cert_lorawan_send_req.buffer = cert_msg_buffer;
        cert_lorawan_send_req.bufferLength = cert_msg_buffer_length;
        cert_lorawan_send_req.confirmed = cert_confirmed;
        cert_lorawan_send_req.port = cert_fport;
        req = &cert_lorawan_send_req;
    }
    else
    {
#endif
        /* Read temperature sensor value */
        get_resource_data(TEMP_SENSOR, (uint8_t *) &app_temperature_celsius);
        /* T(°F) = T(°C) × 9/5 + 32 */
        app_temperature_fahrenheit = (((app_temperature_celsius * 9.0)/5.0) + 32.0);
        printf("Temperature: ");
        snprintf(app_msg_buffer, sizeof(app_msg_buffer), "%.1f°C %.1f°F", app_temperature_celsius, app_temperature_fahrenheit);
        app_msg_buffer_length = strlen(app_msg_buffer);
        printf("%.1f°C/%.1f°F\r\n", app_temperature_celsius, app_temperature_fahrenheit);

        app_lorawan_send_req.buffer = app_msg_buffer;
        app_lorawan_send_req.bufferLength = app_msg_buffer_length;
        app_lorawan_send_req.confirmed = DEMO_APP_TRANSMISSION_TYPE;
        app_lorawan_send_req.port = DEMO_APP_FPORT;
        req = &app_lorawan_send_req;        
#if (CERT_APP == 1)
    }    
#endif
	
	// For EU and NA lowest dr is 0
	if (min_dr_flag){
		uint8_t dr = 0;
		LORAWAN_SetAttr(CURRENT_DATARATE,&dr);
		min_dr_flag = 0;
	}
	
	LORAWAN_GetAttr(UPLINK_COUNTER, NULL, &fcnt_up);
	LORAWAN_GetAttr(CURRENT_DATARATE, NULL, &datarate);
	LORAWAN_GetAttr(NEXT_PAYLOAD_SIZE, NULL, &available_size);
	
    ret_status = LORAWAN_Send(req);
    
    if (ret_status == LORAWAN_SUCCESS) {
               
        printf("\r\n=== Uplink =============================\r\n");
        printf("DR     : %d\r\n", datarate);
        printf("Type   : %sCnf\r\n", req->confirmed ? "" : "Un");
        printf("FPort  : %d\r\n", req->port);
		 if (req->port == TEST_PORT_NB)
		 {
			 printf(" (reply for TEST port command)");
		 }
        printf("FCntUp : %d\r\n", (unsigned int)fcnt_up);
#if (CERT_APP == 1)
        if (is_certification_mode_enabled) {
            printf("SzAvl  : %d\r\n", available_size);
            printf("DatLen : %d\r\n", req->bufferLength);
        }
#endif
        printf("Data   : ");
	    uint8_t *msg_starting =  req->buffer;
		//if the msg is DutVersionsAns,then send the next uplink in the lowest dr of specific band	
	    if (*msg_starting==127){	
		    min_dr_flag = 1;
	    }
        if (req->buffer && (req->bufferLength)) {
            print_array(req->buffer, req->bufferLength);
        } else {
            printf(" *** E M P T Y ***\r\n");
        }  
    
#ifdef DEMO_LED_STATUS
        SwTimerStart(led_timer, MS_TO_US(100), SW_TIMEOUT_RELATIVE, led_timer_callback, NULL);
        set_LED_data(LED_GREEN, &led_on);
#endif
    } else {		
		print_stack_status(ret_status);
		app_run_task(DISPLAY_TASK_HANDLER, APP_MENU_STATE);
	}
}
//------------------------------------------------------------------------------

void demo_app_data_callback(void *app_handle, appCbParams_t *data)
{
    uint32_t fcnt_down;
    EdClass_t ed_class;

    LORAWAN_GetAttr(EDCLASS, NULL, &ed_class);
    switch(data->evt) {
        default:
            /* keep compiler happy */
        break;
        
        case LORAWAN_EVT_TRANSACTION_COMPLETE:
        {
#ifdef DEMO_LED_STATUS
            if (SwTimerIsRunning(led_timer)) {
                SwTimerStop(led_timer);
            }
#endif
			 
#if (CERT_APP == 1)
            if (is_certification_mode_enabled) { 
				printf("\r\n--- Transaction Complete ---------------\r\n");
                if  (LORAWAN_SUCCESS == data->param.transCmpl.status) {
                    if ((false == is_pkt_received)  && (cert_lorawan_send_req.confirmed)){
                        /* CNF uplink may have received a ACK in empty frame */
                        cert_rx_app_cnt++;
                    }
					
					if (false == is_pkt_received)
					{
						cert_fport = CERT_APP_FPORT;
					}
                }
            }
            else
#endif
            {
				if (LORAWAN_SUCCESS == data->param.transCmpl.status)
				{
					printf("Transmission Success\r\n");
				}
				else
				{
					printf("\r\n--- Transaction Ended ------------------\r\n");

					if (LORAWAN_NO_CHANNELS_FOUND == data->param.transCmpl.status)
					{
						uint32_t remtime;
						LORAWAN_GetAttr(PENDING_DUTY_CYCLE_TIME, NULL, &remtime);
						printf("\r\nPending duty cycle time: %dms\r\n",
						(unsigned int) remtime);
					}
				}
				print_stack_status(data->param.transCmpl.status); 
                app_run_task(DISPLAY_TASK_HANDLER, APP_MENU_STATE);
            }
#ifdef DEMO_LED_STATUS
            set_LED_data(LED_GREEN, &led_off);
            if (LORAWAN_SUCCESS != data->param.transCmpl.status) {
                set_LED_data(LED_AMBER, &led_on);
            }
#endif
        }
        break;
        
        case LORAWAN_EVT_RX_DATA_AVAILABLE:
        {
#if (CERT_APP == 1)
            if (is_certification_mode_enabled) {
                is_pkt_received = true;
                cert_rx_app_cnt++;
            }
#endif
            LORAWAN_GetAttr(DOWNLINK_COUNTER, NULL, &fcnt_down);
            printf("\r\n*** Received DL Data ***\r\n");
            printf("FPort  : %d\r\n", data->param.rxData.pData[0]);
            printf("FCntDn : %d\r\n", (unsigned int)fcnt_down);            
            if ((data->param.rxData.dataLength > 0) && (data->param.rxData.pData)) {
                printf("Data   : ");
                print_array(data->param.rxData.pData + 1, data->param.rxData.dataLength - 1);
            }
            
#if (CERT_APP == 1)
            if (TEST_PORT_NB == data->param.rxData.pData[0]) {
                if (cert_is_protocol_cmd_valid(data->param.rxData.pData + 1, data->param.rxData.dataLength - 1)) {
                    cert_exec_protocol_cmd(data->param.rxData.pData + 1, data->param.rxData.dataLength - 1);
                }
            }
#endif /* #if (CERT_APP == 1) */
            if (
#if (CERT_APP == 1)
                (false == is_certification_mode_enabled) &&
#endif
                (CLASS_C == ed_class)
               ) {
                app_run_task(DISPLAY_TASK_HANDLER, APP_MENU_STATE);
            }
        }
        break;
    }
}
//------------------------------------------------------------------------------

void mote_demo_init(void)
{
    
#if !defined(CRYPTO_DEV_ENABLED) && (EDBG_EUI_READ == 1)
    edbg_dev_eui_read();
#endif

#if !defined(CRYPTO_DEV_ENABLED) && (MODULE_EUI_READ == 1)
	module_dev_eui_read();
#endif

#if (CERT_APP == 1) && (ENABLE_PDS==1)
	app_data_pds_reg();
#endif

#ifdef CRYPTO_DEV_ENABLED
	print_ecc_info();
#endif

    start_receiving = false;
#ifdef DEMO_LED_STATUS
    if (LORAWAN_SUCCESS != SwTimerCreate(&led_timer)) {
        printf("Error - %s:%d - Failed to create ledTimer, app stopped\r\n",
            __FUNCTION__, __LINE__);
        
        while(1);
    }
#endif
	if (LORAWAN_SUCCESS != SwTimerCreate(&app_timer)) {
		printf("Error - %s:%d - Failed to create appTimer, app stopped\r\n",
		__FUNCTION__, __LINE__);
		
		while(1);
	}

#if (ENABLE_PDS == 1)
    if (LORAWAN_SUCCESS != SwTimerCreate(&restore_prompt_timer)) {
        printf("Error - %s:%d - Failed to create restorePromptTimer, app stopped\r\n",
            __FUNCTION__, __LINE__);
        while(1);
    }
#endif
	
#if (CERT_APP == 1)
    cert_rx_app_cnt = 0;
    
    if (LORAWAN_SUCCESS != SwTimerCreate(&cert_timer)) {
        printf("Error - %s:%d - Failed to create certTimer, app stopped\r\n",
            __FUNCTION__, __LINE__);
        while(1);
    }

	if (LORAWAN_SUCCESS != SwTimerCreate(&NvicSystemResetTimer)) {
		printf("Error - %s:%d - Failed to create NvicSystemResetTimer, app stopped\r\n",
		__FUNCTION__, __LINE__);
		while(1);
	}
	
        
    if (LORAWAN_SUCCESS != SwTimerCreate(&cw_tx_timer)) {
        printf("Error - %s:%d - Failed to create cwTimer, app stopped\r\n",
        __FUNCTION__, __LINE__);
        while(1);
    }
#endif

    resource_init();
	
	printf("\r\n");
	printf("Microchip LoRaWAN Stack - %s\r\n\r\n", STACK_VER);
    LORAWAN_Init(demo_app_data_callback, demo_join_data_callback);
    
    printf("Init - Successful\r\n");
    
#if (ENABLE_PDS == 1)
    if (PDS_IsRestorable()) {
        static uint8_t previous_band = 0xFF;
        uint8_t previous_choice = 0xFF;
        PDS_RestoreAll();
        LORAWAN_GetAttr(ISMBAND,NULL, &previous_band);
        for (uint32_t i = 0; i < sizeof(band_table) - 1; i++) {
            if(band_table[i] == previous_band) {
                previous_choice = i;
                break;
            }
        }

        restore_attempt_count = APP_RESTORE_ATTEMPT_COUNT;

        printf("Last configured regional band %s\r\n", band_strings[previous_choice]);
        printf("Press a key ([A-Z;0-9]) to change band\r\n");
        printf("Continuing to %s in ", band_strings[previous_choice]);

        SwTimerStart(restore_prompt_timer,
            MS_TO_US(APP_RESTORE_TIMER_PERIOD),
            SW_TIMEOUT_RELATIVE,
            restore_prompt_timer_callback,
            NULL
        );
    }
    else
#endif
    {
        app_run_task(DISPLAY_TASK_HANDLER, INIT_MENU_STATE);
    }
}
//------------------------------------------------------------------------------

void print_app_config(void)
{
    EdClass_t ed_class;
    printf("\r\n====== Application Configuration =======\r\n");
    LORAWAN_GetAttr(EDCLASS, NULL, &ed_class);
    printf("DevType : Class ");
    if (ed_class == CLASS_A) {
        printf("A\r\n");
    } else if (ed_class == CLASS_C) {
        printf("C\r\n");
    }
    /* ------------------------------------ */
    
    printf("ActivationType : ");
    if(DEMO_APP_ACTIVATION_TYPE == OVER_THE_AIR_ACTIVATION) {
        printf("OTAA\r\n");
    } else if(DEMO_APP_ACTIVATION_TYPE == ACTIVATION_BY_PERSONALIZATION) {
        printf("ABP\r\n");
    }
    /* ------------------------------------ */

    printf("Transmission Type : ");
    if(DEMO_APP_TRANSMISSION_TYPE == CONFIRMED) {
        printf("CONFIRMED\r\n");
    } else if(DEMO_APP_TRANSMISSION_TYPE == UNCONFIRMED) {
        printf("UNCONFIRMED\r\n");
    }
    /* ------------------------------------ */

    printf("FPort : %d\r\n", DEMO_APP_FPORT);
    printf("\r\n========================================\r\n");
}
//------------------------------------------------------------------------------

StackRetStatus_t mote_set_params(IsmBand_t ism_band, const uint16_t index) {
    StackRetStatus_t status;
    bool join_backoff_enable = false;
    LORAWAN_Reset(ism_band);
#if (NA_BAND == 1 || AU_BAND == 1)
#if (RANDOM_NW_ACQ == 0)
    if ((ism_band == ISM_NA915) || (ism_band == ISM_AU915)) {
        #define MAX_NA_CHANNELS 72
        #define MAX_SUBBAND_CHANNELS 8

        ChannelParameters_t ch_params;
        uint8_t min_allowed_125kHz_ch, max_allowed_125kHz_ch, allowed_500kHz_ch;
        min_allowed_125kHz_ch = (SUBBAND-1)*MAX_SUBBAND_CHANNELS;
        max_allowed_125kHz_ch = ((SUBBAND-1)*MAX_SUBBAND_CHANNELS) + 7 ;
        allowed_500kHz_ch = SUBBAND+63;

        for (ch_params.channelId = 0; ch_params.channelId < MAX_NA_CHANNELS; ch_params.channelId++) {
            if((ch_params.channelId >= min_allowed_125kHz_ch) && (ch_params.channelId <= max_allowed_125kHz_ch)) {
                ch_params.channelAttr.status = true;
            } else if(ch_params.channelId == allowed_500kHz_ch) {
                ch_params.channelAttr.status = true;
            } else {
                ch_params.channelAttr.status = false;
            }
            LORAWAN_SetAttr(CH_PARAM_STATUS, &ch_params);
        }
    }
#endif
#endif
    /*Disabled Join backoff in Demo application
	Needs to be enabled in Production Environment Ref Section */
    LORAWAN_SetAttr(JOIN_BACKOFF_ENABLE, &join_backoff_enable);

#ifdef CRYPTO_DEV_ENABLED
	bool crypto_dev_enabled = true;
	LORAWAN_SetAttr(CRYPTODEVICE_ENABLED, &crypto_dev_enabled);
#endif

#if (CERT_APP == 1)
    if (is_certification_mode_enabled) {
        /* Initialize the join parameters for  */
        status = set_certification_join_params(CERT_APP_ACTIVATION_TYPE);
        if (LORAWAN_SUCCESS != status) {
            printf("\nCertification Join parameters initialization failed\n\r");
            return status;
        }    
    }
    else
#endif
    {
        /* Initialize the join parameters for Demo application */
        status = set_join_params(DEMO_APP_ACTIVATION_TYPE);
        if (LORAWAN_SUCCESS != status) {
            printf("\nJoin parameters initialization failed\n\r");
            return status;
        }    
    }

    /* Set the device type */
	 if (is_certification_mode_enabled)
	{
		status = set_device_type(CERT_APP_ENDDEVICE_CLASS);
	}
	else
	{
		status = set_device_type(DEMO_APP_ENDDEVICE_CLASS);
	}

    if (LORAWAN_SUCCESS != status) {
        printf("\nUnsupported Device Type\n\r");
        return status;
    }

    /* Send Join request for Demo application */
	if (is_certification_mode_enabled) {
		status = LORAWAN_Join(CERT_APP_ACTIVATION_TYPE);
	}
	else
	{
		status = LORAWAN_Join(DEMO_APP_ACTIVATION_TYPE);
	}

    if (LORAWAN_SUCCESS == status && index < sizeof(band_table)) {
        printf("\nJoin Request Sent for %s\n\r",band_strings[index]);
    } else {
        print_stack_status(status);
		app_run_task(PROCESS_TASK_HANDLER, APP_MENU_STATE);
    }

    return status;
}
//------------------------------------------------------------------------------

StackRetStatus_t set_join_params(ActivationType_t join_type) {
    StackRetStatus_t status = LORAWAN_SUCCESS;
	
    printf("\r\n========= Demo Join Parameters =========\r\n");
    
    JoinNonceType_t jn_type = DEMO_APP_JOIN_NONCE_TYPE;
    LORAWAN_SetAttr(JOIN_NONCE_TYPE, &jn_type);

    if(ACTIVATION_BY_PERSONALIZATION == join_type) {
        status = LORAWAN_SetAttr (DEV_ADDR, &demo_dev_addr);
        if (LORAWAN_SUCCESS == status) {
            status = LORAWAN_SetAttr (APPS_KEY, demo_apps_key);
        }

        if (LORAWAN_SUCCESS == status) {
            printf("AppSessionKey : ");
            print_array((uint8_t *)&demo_apps_key, sizeof(demo_apps_key));
            status = LORAWAN_SetAttr (NWKS_KEY, demo_nwks_key);
        }

        if (LORAWAN_SUCCESS == status) {
            printf("NwkSessionKey : ");
            print_array((uint8_t *)&demo_nwks_key, sizeof(demo_nwks_key));
        }

    } else {
#ifndef CRYPTO_DEV_ENABLED
        status = LORAWAN_SetAttr (DEV_EUI, demo_dev_eui);
        if (LORAWAN_SUCCESS == status)
        {
            printf("DevEUI  : ");
            print_array((uint8_t *)&demo_dev_eui, sizeof(demo_dev_eui));
            status = LORAWAN_SetAttr (JOIN_EUI, demo_join_eui);
        }

        if (LORAWAN_SUCCESS == status)
        {
            printf("JoinEUI : ");
            print_array((uint8_t *)&demo_join_eui, sizeof(demo_join_eui));
			status = LORAWAN_SetAttr (APP_KEY, demo_app_key);          
        }
        if (LORAWAN_SUCCESS == status)
        {
            printf("AppKey  : ");
            print_array((uint8_t *)&demo_app_key, sizeof(demo_app_key));
        }
#endif
    }
    return status;
}
//------------------------------------------------------------------------------

#if (CERT_APP == 1)
StackRetStatus_t set_certification_join_params(ActivationType_t join_type) {
    StackRetStatus_t status = LORAWAN_SUCCESS;
    uint8_t datarate;
#ifdef CRYPTO_DEV_ENABLED
    bool crypto_dev_enabled = true;
    status = LORAWAN_SetAttr(CRYPTODEVICE_ENABLED, &crypto_dev_enabled );
#endif
    printf("\r\n==== Certification Join Parameters =====\r\n");
    if(band_table[current_ism_band] == ISM_THAI923 || band_table[current_ism_band] == ISM_JPN923) {
        datarate = DR2;
        } else {
        datarate = DR0;
    }
    status = LORAWAN_SetAttr (CURRENT_DATARATE, &datarate);
    if(LORAWAN_ABP == join_type) {
        if (LORAWAN_SUCCESS == status) {
            status = LORAWAN_SetAttr (DEV_ADDR, &cert_dev_addr);
        }  
        if (LORAWAN_SUCCESS == status) {
            printf("DevAddr : 0x%lx\r\n", cert_dev_addr);
            status = LORAWAN_SetAttr (APPS_KEY, cert_apps_key);
        }
        if (LORAWAN_SUCCESS == status) {
            printf("AppSKey : ");
            print_array((uint8_t *)&cert_apps_key, sizeof(cert_apps_key));
            status = LORAWAN_SetAttr(NWKS_KEY, cert_nwks_key);
        }
        if (LORAWAN_SUCCESS == status) {
            printf("NwkSKey : ");
            print_array((uint8_t *)&cert_nwks_key, sizeof(cert_nwks_key));
        }
    } else {
        if (LORAWAN_SUCCESS == status) {
            status = LORAWAN_SetAttr (DEV_EUI, cert_dev_eui);
        }
        if (LORAWAN_SUCCESS == status) {
            printf("DevEUI  : ");
            print_array((uint8_t *)&cert_dev_eui, sizeof(cert_dev_eui));
            status = LORAWAN_SetAttr (JOIN_EUI, cert_join_eui);
        }
        if (LORAWAN_SUCCESS == status) {
            printf("JoinEUI : ");
            print_array((uint8_t *)&cert_join_eui, sizeof(cert_join_eui));
            status = LORAWAN_SetAttr (APP_KEY, cert_app_key);
        }
        if (LORAWAN_SUCCESS == status) {
            printf("AppKey  : ");
            print_array((uint8_t *)&cert_app_key, sizeof(cert_app_key));
        }
    }
    return status;
}
#endif
//------------------------------------------------------------------------------

StackRetStatus_t set_device_type(EdClass_t ed_class) {
    StackRetStatus_t status = LORAWAN_SUCCESS;
    status = LORAWAN_SetAttr(EDCLASS, &ed_class);
    if((LORAWAN_SUCCESS == status) && ((CLASS_C | CLASS_B) & ed_class) && \
        (true == DEMO_APP_MCAST_ENABLE)) {
        set_mcast_params();
    }

    return status;
}
//------------------------------------------------------------------------------

void set_mcast_params (void) {
    StackRetStatus_t status;
    LorawanMcastDevAddr_t mcast_dev_addr;
    LorawanMcastAppSkey_t mcast_apps_key;
    LorawanMcastNwkSkey_t mcast_nwks_key;
	LorawanMcastDlFreqeuncy_t mcast_dl_freq;
	LorawanMcastDatarate_t mcast_datarate;
    LorawanMcastStatus_t  mcast_group_status;
	ReceiveWindow2Params_t rx2_window_params;
	
    printf("\r\n========= Multicast Parameters =========\r\n");
    
    mcast_dev_addr.groupId = demo_mcast_group_id;
    mcast_apps_key.groupId  = demo_mcast_group_id;
    mcast_nwks_key.groupId  = demo_mcast_group_id;
	mcast_dl_freq.groupId   = demo_mcast_group_id;
	mcast_datarate.groupId = demo_mcast_group_id;
    mcast_group_status.groupId   = demo_mcast_group_id;
	
    memcpy(&(mcast_apps_key.mcastAppSKey), &demo_mcast_apps_key,LORAWAN_SESSIONKEY_LENGTH);
    mcast_dev_addr.mcast_dev_addr = demo_mcast_dev_addr;
    memcpy(&(mcast_nwks_key.mcastNwkSKey), &demo_mcast_nwks_key,LORAWAN_SESSIONKEY_LENGTH);
    memcpy(&(mcast_group_status.status),&demo_mcast_enable,sizeof(demo_mcast_enable));
	LORAWAN_GetAttr(RX2_WINDOW_PARAMS ,NULL, &rx2_window_params);
	mcast_datarate.datarate = rx2_window_params.dataRate;
	mcast_dl_freq.dlFrequency = rx2_window_params.frequency;
	
    
    status = LORAWAN_SetAttr(MCAST_APPS_KEY, &mcast_apps_key);
    if (status == LORAWAN_SUCCESS) {
	    printf("McastAppSessionKey : ");
	    print_array((uint8_t *)&(mcast_apps_key.mcastAppSKey), LORAWAN_SESSIONKEY_LENGTH);
	    status = LORAWAN_SetAttr(MCAST_NWKS_KEY, &mcast_nwks_key);
    }

    if(status == LORAWAN_SUCCESS) {
	    printf("McastNwkSessionKey : ");
	    print_array((uint8_t *)&(mcast_nwks_key.mcastNwkSKey), LORAWAN_SESSIONKEY_LENGTH);
	    status = LORAWAN_SetAttr(MCAST_GROUP_ADDR, &mcast_dev_addr);
    }
    if (status == LORAWAN_SUCCESS) {
	    printf("McastGroupAddr : 0x%lx\r\n", mcast_dev_addr.mcast_dev_addr);
	    status = LORAWAN_SetAttr(MCAST_ENABLE, &mcast_group_status);
    }
	if (status == LORAWAN_SUCCESS) {
	  status = LORAWAN_SetMulticastParam(MCAST_DATARATE , &mcast_datarate);
	}
	if (status == LORAWAN_SUCCESS) {
	   status = LORAWAN_SetMulticastParam(MCAST_FREQUENCY , &mcast_dl_freq);
	} else {
	    printf("McastGroupAddrStatus : Failed\r\n");
    }
	
    if (status == LORAWAN_SUCCESS) {
	    printf("MulticastStatus : Enabled\r\n");
    } else {
	    printf("MulticastStatus : Failed\r\n");
    }
	
	 printf("========================================\r\n");

}
//------------------------------------------------------------------------------

void print_array(uint8_t *array, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++) {
        printf("%02X", *array);
        array++;
    }
    printf("\r\n");
}
//------------------------------------------------------------------------------

static inline void print_stack_status(StackRetStatus_t status)
{
    printf("Status : LORAWAN_%s\r\n", lw_status_str[status]);
}
//------------------------------------------------------------------------------

#ifdef CONF_PMM_ENABLE
void app_sleep(void)
{
	static bool device_resets_for_wakeup = false;
	PMM_SleepReq_t pmm_sleep_req;
	/* Put the application to sleep */
	pmm_sleep_req.sleepTimeMs = DEMO_CONF_DEFAULT_APP_SLEEP_TIME_MS;
	pmm_sleep_req.pmmWakeupCallback = app_pmm_wakeup_callback;
	pmm_sleep_req.sleep_mode = CONF_PMM_SLEEPMODE_WHEN_IDLE;
	if (CONF_PMM_SLEEPMODE_WHEN_IDLE == SLEEP_MODE_STANDBY) {
    	device_resets_for_wakeup = false;
	}
	if (true == LORAWAN_ReadyToSleep(device_resets_for_wakeup)) {
    	app_resources_uninit();
    	if (PMM_SLEEP_REQ_DENIED == PMM_Sleep(&pmm_sleep_req)) {
        	app_resources_init();
        	app_run_task(DISPLAY_TASK_HANDLER, APP_MENU_STATE);
        	printf("Device cannot sleep now\r\n");
    	}
	} else {
    	app_run_task(DISPLAY_TASK_HANDLER, APP_MENU_STATE);
    	printf("Device cannot sleep now\r\n");
	}
}
#endif
//------------------------------------------------------------------------------

#if !defined (CRYPTO_DEV_ENABLED) && (EDBG_EUI_READ == 1)
void edbg_dev_eui_read(void) {
	uint8_t invalid_edbg_dev_eui[8];
	uint8_t edbg_dev_eui[8];
	edbg_eui_read_eui64((uint8_t *)&edbg_dev_eui);
	memset(&invalid_edbg_dev_eui, 0xFF, sizeof(invalid_edbg_dev_eui));
	/* If EDBG doesnot have DEV EUI, the read value will be of all 0xFF, 
	   Set devEUI in conf_app.h in that case */
	if(0 != memcmp(&edbg_dev_eui, &invalid_edbg_dev_eui, sizeof(demo_dev_eui))) {
		/* Set EUI addr in EDBG if there */
		memcpy(demo_dev_eui, edbg_dev_eui, sizeof(demo_dev_eui));
	}
}
#endif
//------------------------------------------------------------------------------

#if !defined (CRYPTO_DEV_ENABLED) && (MODULE_EUI_READ == 1)
void module_dev_eui_read(void) 
{
	uint8_t i = 0, j = 0;
	uint8_t invalidMODULEDevEui[8];
	uint8_t moduleDevEUI[8];
	for (i = 0; i < 8; i += 2, j++)
	{
		moduleDevEUI[i] = (NVM_UID_ADDRESS[j] & 0xFF);
		moduleDevEUI[i + 1] = (NVM_UID_ADDRESS[j] >> 8);
	}
	memset(&invalidMODULEDevEui, 0xFF, sizeof(invalidMODULEDevEui));
	/* If Module doesnot have DEV EUI, the read value will be of all 0xFF,
	Set devEUI in conf_app.h in that case */
	if(0 != memcmp(&moduleDevEUI, &invalidMODULEDevEui, sizeof(demo_dev_eui)))
	{
		/* Set EUI addr in Module if there */
		memcpy(demo_dev_eui, moduleDevEUI, sizeof(demo_dev_eui));
	}
}
#endif

//------------------------------------------------------------------------------

static void app_run_task(AppTaskIds_t task, AppTaskState_t state) {
    app_task_state = state;
    app_post_task(task);
}
//------------------------------------------------------------------------------

static void process_init_menu(void) {
    switch(serial_buffer) {
        default:
        {
            printf("Choice not valid\r\n");
            app_run_task(DISPLAY_TASK_HANDLER, INIT_MENU_STATE);
        }
        break;

        case INIT_MENU_DEMO_APP:
            app_run_task(DISPLAY_TASK_HANDLER, MAIN_MENU_STATE);
        break;

#if (CERT_APP == 1)
        case INIT_MENU_CERT_MODE:
        {
            bool true_bool = true;
			is_certification_mode_enabled = true;
			is_deact_fport224 = false;
            printf("Certification mode (Fport#224): %sABLED\r\n",
                is_certification_mode_enabled ? "EN" : "DIS");            
            LORAWAN_SetAttr(TEST_MODE_ENABLE, &true_bool);
            LORAWAN_SetAttr(DEV_EUI, cert_dev_eui);
            LORAWAN_SetAttr(JOIN_EUI, cert_join_eui);
            LORAWAN_SetAttr(APP_KEY, cert_app_key);
            LORAWAN_SetAttr(ADR, &true_bool);
#if (ENABLE_PDS == 1)
			PDS_STORE(PDS_CERT_APP_ENABLE);
#endif			
            app_run_task(DISPLAY_TASK_HANDLER, INIT_MENU_STATE);
        }
#endif
    }
}
//------------------------------------------------------------------------------

static void process_main_menu(void)
{
    bool set_band = true;
    switch(serial_buffer) {
        default:
        {
            set_band = false;
            printf("Choice not valid\r\n");
            app_run_task(DISPLAY_TASK_HANDLER, MAIN_MENU_STATE);
        }
        break;

        case MAIN_MENU_RESET_BOARD:
            NVIC_SystemReset();
        break;
        
#if (ENABLE_PDS == 1)
        case MAIN_MENU_CLEAR_PDS:
        {
            set_band = false;
            PDS_DeleteAll();
            printf("PDS is now cleared, device at factory new state\r\n");
            app_run_task(DISPLAY_TASK_HANDLER, MAIN_MENU_STATE);
        }
        break;
#endif

#if (EU_BAND == 1)
        case MAIN_MENU_EU868:
            current_ism_band = ISM_EU868;
        break;
#endif
#if (NA_BAND == 1)
        case MAIN_MENU_NA915:
            current_ism_band = ISM_NA915;
        break;
#endif
#if (AU_BAND == 1)
        case MAIN_MENU_AU915:
            current_ism_band = ISM_AU915;
        break;
#endif
#if (AS_BAND == 1)
        case MAIN_MENU_AS923:
            current_ism_band = ISM_THAI923;
        break;
#endif
#if (JPN_BAND == 1)
        case MAIN_MENU_JPN923:
            current_ism_band = ISM_JPN923;
        break;
#endif
#if (KR_BAND == 1)
        case MAIN_MENU_KR920:
            current_ism_band = ISM_KR920;
        break;
#endif
#if (IND_BAND == 1)
        case MAIN_MENU_IND865:
            current_ism_band = ISM_IND865;
        break;
#endif
    }

    if (set_band) {
        LORAWAN_Reset(current_ism_band);
        mote_set_params(current_ism_band, serial_buffer);
    }
}
//------------------------------------------------------------------------------

static void process_app_menu(void) {
	
	StackRetStatus_t retStatus;
    switch(serial_buffer) {
        default:
        {
            printf("Choice not valid\r\n");
            app_run_task(DISPLAY_TASK_HANDLER, APP_MENU_STATE);
        }
        break;

        case APP_MENU_SEND_JOIN_REQ:
        {
            ActivationType_t type = DEMO_APP_ACTIVATION_TYPE;
#if (CERT_APP == 1)
            if (is_certification_mode_enabled) {
                type = CERT_APP_ACTIVATION_TYPE;
            }
#endif
            send_join_req(type);
        }
        break;

        case APP_MENU_SEND_DATA:
        {
            send_data();
        }
        break;
		
		case APP_MENU_START_PERIODIC_DATA:
		{
            LorawanStatus_t lw_status;
            LORAWAN_GetAttr(LORAWAN_STATUS, NULL, &lw_status);
            if (!lw_status.networkJoined) {
                printf("Warn - cannot start periodic uplink, not joined\r\n");
                app_run_task(DISPLAY_TASK_HANDLER, APP_MENU_STATE);
                return;
            }                
			if (!periodic_uplink_mode_enabled)
			{
				periodic_uplink_mode_enabled = true;
				SwTimerStop(app_timer);
				send_data();
				retStatus = SwTimerStart(app_timer, MS_TO_US(app_timer_period), SW_TIMEOUT_RELATIVE, app_timer_callback, NULL);
				if (retStatus != LORAWAN_SUCCESS)
				{
					printf("Failed to start periodic timer %d\r\n", retStatus);
				}
			}	
			else
			{
				printf("Periodic uplink already started\r\n");
				app_run_task(DISPLAY_TASK_HANDLER, APP_MENU_STATE);
			}			
		}
		break;
		
		case APP_MENU_STOP_PERIODIC_DATA:
		{
			periodic_uplink_mode_enabled = false;
            if (SwTimerIsRunning(app_timer))
			    SwTimerStop(app_timer);            
			app_run_task(DISPLAY_TASK_HANDLER, APP_MENU_STATE);
		}
		break;

        
        case APP_MENU_MAIN_MENU:
        {
            app_run_task(DISPLAY_TASK_HANDLER, MAIN_MENU_STATE);
        }
        break;

#ifdef CONF_PMM_ENABLE
        case APP_MENU_SLEEP:
        {
            app_sleep();
        }
        break;
#endif /* #ifdef CONF_PMM_ENABLE */
    }
}
//------------------------------------------------------------------------------

#if (ENABLE_PDS == 1)
static void process_restore_prompt(void) {
	StackRetStatus_t status = LORAWAN_SUCCESS;
	uint8_t previous_band = 0xff;
	uint8_t choice = 0xff;
	bool join_backoff_enable = false;
	bool test_enable = true;
	PDS_RestoreAll();
	LORAWAN_GetAttr(ISMBAND,NULL, &previous_band);
    
	for (uint32_t i = 0; i < sizeof(band_table)-1; i++) {
		if(band_table[i] == previous_band) {
			choice = i;
			break;
		}
	}
	if ((choice > 0) && (choice < (sizeof(band_table) - 1)))	{
		status = LORAWAN_Reset(band_table[choice]);
	}
	
	 /*Disabled Join backoff in Demo application
	Needs to be enabled in Production Environment Ref Section */
    LORAWAN_SetAttr(JOIN_BACKOFF_ENABLE,&join_backoff_enable);
	
	if ((status == LORAWAN_SUCCESS) && (choice < (sizeof(band_table) - 1))) {
		uint32_t join_status = 0;
		PDS_RestoreAll();
		LORAWAN_GetAttr(LORAWAN_STATUS,NULL, &join_status);
		printf("Successfully restored settings from PDS\r\n" );
        
		if(join_status & APP_LORAWAN_NW_JOINED) {
			joined = true;
			printf("Device has joined already\r\n");
		} else {
			joined = false;
			printf("Device has not joined previously\r\n");
#ifdef DEMO_LED_STATUS
			set_LED_data(LED_AMBER, &led_on);
#endif
		}
		printf("Band : %s\r\n", band_strings[choice]);

		print_app_config();
		
		if(is_certification_mode_enabled == true)
		{
			LORAWAN_SetAttr(TEST_MODE_ENABLE, &test_enable);
			if(CERT_APP_ACTIVATION_TYPE == ACTIVATION_BY_PERSONALIZATION)
			{
				send_data();
			}
			else
			{   
				send_join_req(LORAWAN_OTAA);
			}
			
		}
		else
		{
			app_run_task(DISPLAY_TASK_HANDLER, APP_MENU_STATE);
		}
	} else {
		printf("Failed to restore settings from PDS\r\n");
		app_run_task(DISPLAY_TASK_HANDLER, APP_MENU_STATE);
	}
}
#endif
//------------------------------------------------------------------------------

#if (ENABLE_PDS == 1)
static void restore_prompt_timer_callback(void) {
    printf("%d..", restore_attempt_count);
    restore_attempt_count--;
    
    restore_prompt_stop_key = 0;
    sio2host_rx(&restore_prompt_stop_key, 1);
    
    if (restore_prompt_stop_key) {
        /* User has pressed a key; start fresh */
        printf("\r\n");
        app_run_task(DISPLAY_TASK_HANDLER, INIT_MENU_STATE);
    } else if (restore_attempt_count) {
        /* User has not pressed a key YET and attempts remaining */
        SwTimerStart(restore_prompt_timer,
            MS_TO_US(APP_RESTORE_TIMER_PERIOD),
            SW_TIMEOUT_RELATIVE,
            restore_prompt_timer_callback,
            NULL
        );
    } else {
        /* User has not pressed a key YET and attempts are over */
        printf("\r\nNo key pressed\r\n");
		app_run_task(DISPLAY_TASK_HANDLER, RESTORE_PROMPT_STATE);
    }
}
#endif

static void nvic_system_reset_timer_callback(void) {
	/* In this Timer Callback function, system reset is called */
	NVIC_SystemReset();
}


//------------------------------------------------------------------------------

#ifdef CONF_PMM_ENABLE
static void app_resources_uninit(void) {
    /* Disable USART TX and RX Pins */
    struct port_config pin_conf;
    port_get_config_defaults(&pin_conf);
    pin_conf.powersave  = true;
#ifdef HOST_SERCOM_PAD0_PIN
    port_pin_set_config(HOST_SERCOM_PAD0_PIN, &pin_conf);
#endif
#ifdef HOST_SERCOM_PAD1_PIN
    port_pin_set_config(HOST_SERCOM_PAD1_PIN, &pin_conf);
#endif
    /* Disable UART module */
    sio2host_deinit();
    /* Disable Transceiver SPI Module */
    HAL_RadioDeInit();
}
#endif
//------------------------------------------------------------------------------

#ifdef CONF_PMM_ENABLE
static void app_resources_init(void) {
    HAL_Radio_resources_init();
    sio2host_init(); 
}
#endif
//------------------------------------------------------------------------------

#ifdef CONF_PMM_ENABLE
static void app_pmm_wakeup_callback(uint32_t slept_duration) {
    app_resources_init();
    printf("Sleep done: %ld ms\r\n", slept_duration);
    app_run_task(DISPLAY_TASK_HANDLER, APP_MENU_STATE);
}
#endif
//------------------------------------------------------------------------------

#if (CERT_APP == 1)
static void cert_timer_callback(void) {
    SwTimerStart(cert_timer,
        MS_TO_US(cert_timer_period),
        SW_TIMEOUT_RELATIVE,
        cert_timer_callback,
        NULL
    );
    send_data();
}
#endif
//------------------------------------------------------------------------------

#if (CERT_APP == 1)
static void cert_exec_protocol_cmd(uint8_t *buffer, uint8_t buflen) {
    StackRetStatus_t status;
    memset(cert_msg_buffer, 0x00, sizeof(cert_msg_buffer));
    printf("\r\nCertification command received\r\n");
    switch (buffer[0]) {
        case CPC_PACKAGE_VERSION_REQ:
        {
            cert_msg_buffer[0] = CPC_PACKAGE_VERSION_ANS;
            cert_msg_buffer[1] = CERT_PROTOCOL_PACKAGE_IDENTIFIER;
            cert_msg_buffer[2] = CERT_PROTOCOL_PACKAGE_VERSION;
            cert_msg_buffer_length = 3;
            cert_fport = TEST_PORT_NB;
        }
        break;
        
        case CPC_DUT_RESET_REQ:
        {
		//All channels must be enabled after the reset	
		ChannelParameters_t ch_params;
        for (ch_params.channelId = 0; ch_params.channelId <=15; ch_params.channelId++) {
				ch_params.channelAttr.status = true;  				
            LORAWAN_SetAttr(CH_PARAM_STATUS, &ch_params);
		}
            bool new_adr;
			uint8_t dr = 0;
			LORAWAN_GetAttr(ADR, NULL, &new_adr); 
			if (new_adr == 1) {
				LORAWAN_SetAttr(CURRENT_DATARATE,&dr);           
			}
			delay_ms(1000);	
			SwTimerStart(NvicSystemResetTimer,
			MS_TO_US(APP_NVIC_SYSTEM_RESET_TMR_PERIOD),
			SW_TIMEOUT_RELATIVE,
			nvic_system_reset_timer_callback,
			NULL
			);			
        }
        break;
        
        case CPC_DUT_JOIN_REQ:
        {
            bool true_bool = true;
#ifdef CRYPTO_DEV_ENABLED
            LORAWAN_SetAttr(CRYPTODEVICE_ENABLED, &true_bool);
#endif /* #ifdef CRYPTO_DEV_ENABLED */
            if (SwTimerIsRunning(cert_timer)) {
                SwTimerStop(cert_timer);
            }            
#if (ENABLE_PDS == 1)
            PDS_RestoreAll();
#endif
            LORAWAN_SetAttr(TEST_MODE_ENABLE, &true_bool);
            LORAWAN_SetAttr(DEV_EUI, cert_dev_eui);
            LORAWAN_SetAttr(JOIN_EUI, cert_join_eui);
            LORAWAN_SetAttr(APP_KEY, cert_app_key);
            LORAWAN_SetAttr(ADR, &true_bool);
            
            send_join_req(LORAWAN_OTAA);
        }
        break;
        
        case CPC_SWITCH_CLASS_REQ:
        {
            EdClass_t target_class = (1 << buffer[1]);
            printf("\nSwitch to class %c:%sOK\n\r",
                (1 == target_class) ? 'A' : (2 == target_class) ? 'B' : 'C',
                (LORAWAN_SUCCESS == LORAWAN_SetAttr(EDCLASS, &target_class)) ?
                " " : "NOT");
        }
        break;
        
        case CPC_ADR_BIT_CHANGE_REQ:
        {
            bool new_adr;
            LORAWAN_SetAttr(ADR, &buffer[1]);
            LORAWAN_GetAttr(ADR, NULL, &new_adr);
        }
        break;
        
        case CPC_REGIONAL_DUTYCYCLE_CTRL_REQ:
        {
            bool new_regdcycle;
            LORAWAN_SetAttr(REGIONAL_DUTY_CYCLE, &buffer[1]);
            LORAWAN_GetAttr(REGIONAL_DUTY_CYCLE, NULL, &new_regdcycle);
        }
        break;
        
        case CPC_TX_PERIODICITY_CHANGE_REQ:
        {
            uint16_t periodicity[11] = {5, 5, 10, 20, 30,
                40, 50, 60, 120, 240, 480};
            if (10 >= buffer[1]) {
                cert_timer_period = 1000 * periodicity[buffer[1]];
                if (SwTimerIsRunning(cert_timer)) {
                    SwTimerStop(cert_timer);
                }
                SwTimerStart(cert_timer,
                    MS_TO_US(cert_timer_period),
                    SW_TIMEOUT_RELATIVE,
                    cert_timer_callback,
                    NULL
                );
            }
        }
        break;
        
        case CPC_TX_FRAMES_CTRL_REQ:
        {
            switch (buffer[1]) {
                case 0:
                {
                    cert_confirmed = cert_confirmed;
                }
                break;
                case 1:
                {
                    cert_confirmed = (bool)LORAWAN_UNCNF;
                }
                break;
                case 2:
                {
                    cert_confirmed = (bool)LORAWAN_CNF;
                }
                break;
                default:
                {
                    // keep compiler happy
                }
                break;
            }
        }
        break;
        
        case CPC_ECHO_INC_PAYLOAD_REQ:
        {
            LORAWAN_GetAttr(NEXT_PAYLOAD_SIZE, NULL, &cert_msg_buffer_length);
            cert_msg_buffer_length = (cert_msg_buffer_length <= buflen) ? cert_msg_buffer_length : buflen;
            cert_msg_buffer[0] = CPC_ECHO_INC_PAYLOAD_ANS;
            for (uint8_t i = 1; i < cert_msg_buffer_length; i++) {
                cert_msg_buffer[i] = (1 + buffer[i]) % UINT8_MAX;
            }
			if(false == is_deact_fport224)
			{
				cert_fport = TEST_PORT_NB;
			}
			else
			{
				cert_fport = CERT_APP_FPORT;
			}
         }
        break;
        
        case CPC_RX_APP_CNT_REQ:
        {
            cert_msg_buffer[0] = CPC_RX_APP_CNT_ANS;
            cert_msg_buffer[1] = (uint8_t) (cert_rx_app_cnt);
            cert_msg_buffer[2] = (uint8_t) (cert_rx_app_cnt >> 8);
            cert_msg_buffer_length = 3;
            cert_fport = TEST_PORT_NB;
        }
        break;
        
        case CPC_RX_APP_CNT_RESET_REQ:
        {
            cert_rx_app_cnt = 0;
        }
        break;
        
        case CPC_LINK_CHECK_REQ:
        {	
			LORAWAN_SetAttr(SEND_LINK_CHECK_CMD, NULL);									
        }
        break;
        
        case CPC_DEVICE_TIME_REQ:
        {			
			LORAWAN_SetAttr(SEND_DEVICE_TIME_CMD, NULL);
        }
        break;
        
        case CPC_PING_SLOT_INFO_REQ:
        {
        }
        break;
        
        case CPC_TX_CW_REQ:
        {
            uint8_t txpower = 0;
            uint16_t timeout = 0;
            uint32_t freq = 0;

            timeout = buffer[2];
            timeout |= ((uint16_t)buffer[1]) << 8;
            
            freq = buffer[5];
            freq |= ((uint32_t)buffer[4]) << 8;
            freq |= ((uint32_t)buffer[3]) << 16;
            freq *= 100U;
            
            txpower = buffer[6];
            
            RADIO_SetAttr(CHANNEL_FREQUENCY, (void *)&freq);
            RADIO_SetAttr(OUTPUT_POWER, (void *)&txpower);
            
            if (LORAWAN_SUCCESS == SwTimerStart(cw_tx_timer,
                    MS_TO_US(timeout * 1000U), SW_TIMEOUT_RELATIVE,
                    cert_cw_wave_tx_callback, NULL)) {
                RADIO_TransmitCW();
            }                
        }
        break;
        
        case CPC_DUT_FPORT224_DISABLE_REQ:
        {	
			bool disable = false;
			is_deact_fport224 = true;
#if (ENABLE_PDS == 1)
			PDS_STORE(PDS_DEACTIVATE_FPORT);
#endif
			LORAWAN_SetAttr(TEST_MODE_ENABLE,&disable);
			delay_ms(1000);			
			SwTimerStart(NvicSystemResetTimer,
				MS_TO_US(APP_NVIC_SYSTEM_RESET_TMR_PERIOD),
				SW_TIMEOUT_RELATIVE,
				nvic_system_reset_timer_callback,
				NULL
			);

		}
        break;
        
        case CPC_DUT_VERSIONS_REQ:
        {
            cert_msg_buffer[0] = CPC_DUT_VERSIONS_ANS;
            // FwVersion
            cert_msg_buffer[1] = 1; // 1
            cert_msg_buffer[2] = 0; // 0
            cert_msg_buffer[3] = 4; // P_4
            // LrwanVersion
            cert_msg_buffer[4] = 1;
            cert_msg_buffer[5] = 0;
            cert_msg_buffer[6] = 4;
            // LrwanRpVersion
            cert_msg_buffer[7] = 1;
            cert_msg_buffer[8] = 0;
            cert_msg_buffer[9] = 4;
            
            cert_msg_buffer_length = 10;
            cert_fport = TEST_PORT_NB;
        }
        break;
    }
    (void)status;
}
#endif /* #if (CERT_APP == 1) */
//------------------------------------------------------------------------------

#if (CERT_APP == 1)
bool cert_is_protocol_cmd_valid(uint8_t *buffer, uint8_t len) {
    bool valid = false;
    switch (*buffer) {
        case CPC_TX_FRAMES_CTRL_REQ:
        case CPC_ECHO_INC_PAYLOAD_REQ:
        {
            valid = (len >= 1);
        }
        break;
        
        case CPC_PACKAGE_VERSION_REQ:
        case CPC_DUT_RESET_REQ:
        case CPC_DUT_JOIN_REQ:
        case CPC_RX_APP_CNT_REQ:
        case CPC_RX_APP_CNT_RESET_REQ:
        case CPC_LINK_CHECK_REQ:
        case CPC_DEVICE_TIME_REQ:
        case CPC_DUT_FPORT224_DISABLE_REQ:
        case CPC_DUT_VERSIONS_REQ:
        {
            valid = (1 == len);
        }
        break;
        
        case CPC_SWITCH_CLASS_REQ:
        case CPC_ADR_BIT_CHANGE_REQ:
        case CPC_REGIONAL_DUTYCYCLE_CTRL_REQ:
        case CPC_TX_PERIODICITY_CHANGE_REQ:
        case CPC_PING_SLOT_INFO_REQ:
        {
            valid = (2 == len);
        }
        break;
        
        case CPC_TX_CW_REQ:
        {
            valid = (7 == len);
        }
        break;
    }
    
    if (!valid) {
        printf("\r\nCertification command received with invalid payload\r\n");
    }
    return valid;
}
#endif /* #if (CERT_APP == 1) */
//------------------------------------------------------------------------------

#if (CERT_APP == 1)
static void cert_cw_wave_tx_callback(void) {
    RADIO_StopCW();
}
#endif /* #if (CERT_APP == 1) */
//------------------------------------------------------------------------------
#ifdef DEMO_LED_STATUS
static void led_timer_callback(void) {
    SwTimerStart(led_timer, MS_TO_US(100), SW_TIMEOUT_RELATIVE, led_timer_callback, NULL);
    set_LED_data(LED_GREEN, &led_toggle);
}
#endif
//------------------------------------------------------------------------------
static void app_timer_callback(void)
{
	if (periodic_uplink_mode_enabled)
	{	
		send_data();		
		if (SwTimerStart(app_timer, MS_TO_US(app_timer_period), SW_TIMEOUT_RELATIVE, app_timer_callback, NULL) != LORAWAN_SUCCESS)
		{
			printf("Failed to restart periodic timer\r\n");
		}
	}	
}

//------------------------------------------------------------------------------
#ifdef CRYPTO_DEV_ENABLED
static void print_ecc_info(void)
{
	ATCA_STATUS  status ;
	uint8_t    sn[9] ;			// ECC608A serial number (9 Bytes)
	uint8_t    info[2] ;
	uint8_t    tkm_info[10] ;
	int      slot = 10 ;
	int      offset = 70 ;
	uint8_t appEUI[8] ;
	uint8_t devEUIascii[16] ;
	uint8_t devEUIdecoded[8] ;	// hex.
	size_t bin_size = sizeof(devEUIdecoded) ;
	
	(void) status;

	// read the serial number
	status = atcab_read_serial_number(sn) ;
	printf("\r\n--------------------------------\r\n") ;

	// read the SE_INFO
	status = atcab_read_bytes_zone(ATCA_ZONE_DATA, slot, offset, info, sizeof(info)) ;
	
	// Read the CustomDevEUI
	status = atcab_read_bytes_zone(ATCA_ZONE_DATA, DEV_EUI_SLOT, 0, devEUIascii, 16) ;
	atcab_hex2bin((char*)devEUIascii, strlen((char*)devEUIascii), devEUIdecoded, &bin_size) ;

	// Print DevEUI
	printf("DEV EUI:  ") ;
	#if (SERIAL_NUM_AS_DEV_EUI == 1)
	print_array(sn, sizeof(sn)-1) ;
	#else
	print_array(devEUIdecoded, sizeof(devEUIdecoded)) ;
	#endif
	
	// Read the AppEUI
	status = atcab_read_bytes_zone(ATCA_ZONE_DATA, APP_EUI_SLOT, 0, appEUI, 8) ;
	printf("APP EUI:  ") ;
	print_array(appEUI, sizeof(appEUI)) ;
	
	// assemble full TKM_INFO
	memcpy(tkm_info, info, 2) ;
	memcpy(&tkm_info[2], sn, 8) ;
	// tkm_info[] now contains the assembled tkm_info data
	printf("TKM INFO: ") ;
	print_array(tkm_info, sizeof(tkm_info)) ;
	printf("--------------------------------\r\n") ;
}
#endif
//------------------------------------------------------------------------------

/* eof enddevice_demo.c */
