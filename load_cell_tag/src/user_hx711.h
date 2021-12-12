/**
 ****************************************************************************************
 *
 * @file user_hx711.h
 *
 * @brief HX711 Chip Driver file.
 *
 ****************************************************************************************
 */
 
#ifndef _USER_HX711_H_
#define _USER_HX711_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwble_config.h"
#include "app_task.h"                  // application task
#include "gapc_task.h"                 // gap functions and messages
#include "gapm_task.h"                 // gap functions and messages
#include "app.h"                       // application definitions
#include "co_error.h"                  // error code definitions
#include "systick.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define PREP_READ_TICKS						20
#define MAX_READS 								1
#define WAIT_TICKS								2000
#define CALIBRATION_READS					20

#define MAX_RAW_LOAD_VAL					16277215 //HX711's 24 Bit ADC (2^24 - 1) 
#define WEIGHT_UPDATE_THRESHOLD 	5000 /*50000*/    //Minimum difference between load values to add value to weights array

#define LOAD_VALS_ARR_SIZE    		50			 //Currently arbitrary but should be based on sampling number vs. memory availability


/// Load Cell structure with all relevant configuration values
typedef struct Load_Cell
{
		///Maximum rated load (in grams) of load cell in use
		uint32_t max_load;
		///Maximum value of load (raw value; for 24 bit hx711 adc, this is (2^24 - 1) = 16777215)
		uint32_t max_val;
		///Minimum difference between load values to add value to weights array
		uint32_t val_update_threshold;
	
		/// Is unit calibrated?
		bool calibrated;
		/// Calibration Reads Left Unitl Unit is calibrated
		uint32_t calib_reads_left;
		/// Tared zero raw value
		uint32_t zero_load;
		/// New value ready to be read lock
		bool val_ready;
		/// Variable used to store raw load value as it is being buffered in
		uint32_t temp_load;
		/// Current raw load value reported by HX711
		uint32_t load;
		/// Latest fully set valid new raw load value
		uint32_t new_load;
		
		/// Value reading counter
		uint32_t clock_count;
		/// Time waiting counter
		uint32_t wait_count;
		/// Number of full value reads before settling on new valid value
		uint32_t max_num_reads;
		/// Counter for number of full value reads before settling on new valid value
		uint32_t reads_count;

		/// Maximum value of Load Values Array	
		uint32_t max_num_vals;
    /// Current length of Load Values Array
    uint32_t num_vals;
    /// Raw Load Values Array
    uint32_t load_array[LOAD_VALS_ARR_SIZE];
	
} load_cell;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
load_cell *new_load_cell(uint32_t lc_grams);

void set_calibrate_lc(load_cell *lc_p, uint32_t calib_reads);

uint32_t read_load_cell(load_cell *lc_p);

void free_load_cell(load_cell *lc_p);
/// @} APP

#endif
