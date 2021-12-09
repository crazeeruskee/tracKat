
/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief 
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "user_hx711.h"

load_cell* new_load_cell(uint32_t lc_grams)
{
	load_cell* lc_p = (load_cell*)malloc(sizeof(load_cell) + LOAD_VALS_ARR_SIZE*sizeof(uint32_t));
	if(lc_p == NULL) return NULL;
	
	lc_p->max_load = lc_grams;
	lc_p->max_val = MAX_RAW_LOAD_VAL;
	lc_p->val_update_threshold = WEIGHT_UPDATE_THRESHOLD;	
	lc_p->calibrated = false;
	lc_p->calib_reads_left = CALIBRATION_READS;
	lc_p->zero_load = 0;
	lc_p->val_ready = false;
	lc_p->temp_load = 0;
	lc_p->load = 0;
	lc_p->new_load = 0x00ABCDEF;
	lc_p->clock_count = 0;
	lc_p->wait_count = 0;
	lc_p->max_num_reads = MAX_READS;
	lc_p->reads_count = 0;
	lc_p->max_num_vals = LOAD_VALS_ARR_SIZE;
  lc_p->num_vals = 0;
		
	return lc_p;
}

void set_calibrate_lc(load_cell *lc_p, uint32_t calib_reads)
{
	if(lc_p == NULL) return;
	
	lc_p->calibrated = false;
	lc_p->calib_reads_left = calib_reads;
}

uint32_t read_load_cell(load_cell *lc_p)
{
	return 0;
}

void free_load_cell(load_cell *lc_p)
{
	free(lc_p);
}
