/**
 ****************************************************************************************
 *
 * @file user_hx711.h
 *
 * @brief RDM6300 RFID Chip Driver file.
 *
 ****************************************************************************************
 */
 
#ifndef _USER_RFID_H_
#define _USER_RFID_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

 #include "app_api.h"
#include "rwble_config.h"
#include "app_task.h"                  // application task
#include "gapc_task.h"                 // gap functions and messages
#include "gapm_task.h"                 // gap functions and messages
#include "app.h"                       // application definitions
#include "co_error.h"                  // error code definitions
#include "systick.h"
#include "gpio.h"
#include "arch_console.h"


/*
 * DEFINES
 ****************************************************************************************
 */
 
#define BUFFER_SIZE 				14		// RFID DATA FRAME FORMAT: 1byte head (value: 2), 10byte data (2byte version + 8byte tag), 2byte checksum, 1byte tail (value: 3)
#define DATA_SIZE 					10 		// 10byte data (2byte version + 8byte tag)
#define DATA_VERSION_SIZE 	2 		// 2byte version (actual meaning of these two bytes may vary)
#define DATA_TAG_SIZE 			8			// 8byte tag
#define CHECKSUM_SIZE 			2 		// 2byte checksum

#define BAUD_RATE 9600
 
uint8_t buffer[BUFFER_SIZE]; // used to store an incoming data frame 
int buffer_index = 0;
 /*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */



/// @} APP

#endif