/**
 ****************************************************************************************
 *
 * @file user_custs1_impl.c
 *
 * @brief Peripheral project Custom1 Server implementation source code.
 *
 * Copyright (C) 2015-2019 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "gpio.h"
#include "app_api.h"
#include "app.h"
#include "prf_utils.h"
#include "custs1.h"
#include "custs1_task.h"
#include "user_custs1_def.h"
#include "user_custs1_impl.h"
#include "user_empty_peripheral_template.h"
#include "user_periph_setup.h"
#include "gpio.h"

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

ke_msg_id_t timer_used      __SECTION_ZERO("retention_mem_area0"); //@RETENTION MEMORY
uint16_t indication_counter __SECTION_ZERO("retention_mem_area0"); //@RETENTION MEMORY
uint16_t non_db_val_counter __SECTION_ZERO("retention_mem_area0"); //@RETENTION MEMORY

/*
extern int load;
extern int load_array[100];
*/

extern int new_load;
extern int baseline;
extern int num_vals;
extern uint32_t sec_timestamp;
extern int load_array[50];
extern uint32_t timestamp_array[50];



//extern uint32_t load_array_test[100];

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void user_svc1_read_lc_val_handler(load_cell *lc_p,
																	 ke_msg_id_t const msgid,
                                   struct custs1_value_req_ind const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
	
    struct custs1_value_req_rsp *rsp = KE_MSG_ALLOC_DYN(CUSTS1_VALUE_REQ_RSP,
                                                        prf_get_task_from_id(TASK_ID_CUSTS1),
                                                        TASK_APP,
                                                        custs1_value_req_rsp,
																											  /*(lc_p->num_vals)*/ /*num_vals **/ DEF_SVC1_LC_VAL_CHAR_LEN);
		
		//(*lc_p).load_array[nv] = lc_p->new_load;
		//load_array_test[lc_p->num_vals] = lc_p->new_load;
		//(lc_p->num_vals)++;
		//(lc_p->new_load)++;

    // Provide the connection index.
    rsp->conidx  = app_env[param->conidx].conidx;
    // Provide the attribute index.
    rsp->att_idx = param->att_idx;
    // Force current length to zero.
    rsp->length  = /*(lc_p->num_vals) */ /*num_vals **/ DEF_SVC1_LC_VAL_CHAR_LEN;
    // Provide the ATT error code.
    rsp->status  = ATT_ERR_NO_ERROR;
    // Copy value
   // memcpy(&rsp->value, &(lc_p->load_array), /*DEF_SVC1_LC_VAL_CHAR_LEN*/rsp->length);
   // memcpy(&rsp->value, &(load_array), /*rsp->length*/ num_vals * DEF_SVC1_LC_VAL_CHAR_LEN);
    memcpy(&rsp->value, &(load_array[num_vals]), /*rsp->length*/ num_vals * DEF_SVC1_LC_VAL_CHAR_LEN);
   // memcpy(&rsp->value, &(lc_p->new_load), DEF_SVC1_LC_VAL_CHAR_LEN/*rsp->length*/);

		//memcpy(&rsp->value, &new_load, DEF_SVC1_LC_VAL_CHAR_LEN/*rsp->length*/);

   // memcpy(&(rsp->value), (void *)(&(load_array[num_vals])), num_vals * DEF_SVC1_LC_VAL_CHAR_LEN/*rsp->length*/);
    // Send message
    ke_msg_send(rsp);
}

void user_svc1_read_lc_base_val_handler(load_cell *lc_p,
																			ke_msg_id_t const msgid,
																			struct custs1_value_req_ind const *param,
																	ke_task_id_t const dest_id,
																	ke_task_id_t const src_id)
{
	
    struct custs1_value_req_rsp *rsp = KE_MSG_ALLOC_DYN(CUSTS1_VALUE_REQ_RSP,
                                                        prf_get_task_from_id(TASK_ID_CUSTS1),
                                                        TASK_APP,
                                                        custs1_value_req_rsp,
																											  /*num_vals **/ DEF_SVC1_LC_BASE_CHAR_LEN);
		
    // Provide the connection index.
    rsp->conidx  = app_env[param->conidx].conidx;
    // Provide the attribute index.
    rsp->att_idx = param->att_idx;
    // Force current length to zero.
    rsp->length  = /*num_vals **/ DEF_SVC1_LC_BASE_CHAR_LEN;
    // Provide the ATT error code.
    rsp->status  = ATT_ERR_NO_ERROR;
    // Copy value

		// memcpy(&rsp->value, &(lc_p->sec_timestamp), DEF_SVC1_LC_VAL_CHAR_LEN/*rsp->length*/);
		//memcpy(&rsp->value, &sec_timestamp, DEF_SVC1_LC_TS_CHAR_LEN/*rsp->length*/);
		memcpy(&rsp->value, &(baseline), DEF_SVC1_LC_BASE_CHAR_LEN/*rsp->length*/);

    // Send message
    ke_msg_send(rsp);
}


void user_svc1_read_lc_num_vals_handler(load_cell *lc_p,
																			ke_msg_id_t const msgid,
																			struct custs1_value_req_ind const *param,
																			ke_task_id_t const dest_id,
																			ke_task_id_t const src_id)
{
	
    struct custs1_value_req_rsp *rsp = KE_MSG_ALLOC_DYN(CUSTS1_VALUE_REQ_RSP,
                                                        prf_get_task_from_id(TASK_ID_CUSTS1),
                                                        TASK_APP,
                                                        custs1_value_req_rsp,
																											  DEF_SVC1_LC_NUM_VALS_CHAR_LEN);
		
    // Provide the connection index.
    rsp->conidx  = app_env[param->conidx].conidx;
    // Provide the attribute index.
    rsp->att_idx = param->att_idx;
    // Force current length to zero.
    rsp->length  = DEF_SVC1_LC_NUM_VALS_CHAR_LEN;
    // Provide the ATT error code.
    rsp->status  = ATT_ERR_NO_ERROR;
    // Copy value

		// memcpy(&rsp->value, &(lc_p->new_load), DEF_SVC1_LC_VAL_CHAR_LEN/*rsp->length*/);

		memcpy(&rsp->value, &num_vals, DEF_SVC1_LC_NUM_VALS_CHAR_LEN/*rsp->length*/);

    // Send message
    ke_msg_send(rsp);
}

void user_svc1_read_lc_ts_handler(load_cell *lc_p,
																	ke_msg_id_t const msgid,
																	struct custs1_value_req_ind const *param,
																	ke_task_id_t const dest_id,
																	ke_task_id_t const src_id)
{
	
    struct custs1_value_req_rsp *rsp = KE_MSG_ALLOC_DYN(CUSTS1_VALUE_REQ_RSP,
                                                        prf_get_task_from_id(TASK_ID_CUSTS1),
                                                        TASK_APP,
                                                        custs1_value_req_rsp,
																											  /*num_vals **/ DEF_SVC1_LC_TS_CHAR_LEN);
		
    // Provide the connection index.
    rsp->conidx  = app_env[param->conidx].conidx;
    // Provide the attribute index.
    rsp->att_idx = param->att_idx;
    // Force current length to zero.
    rsp->length  = /*num_vals **/ DEF_SVC1_LC_TS_CHAR_LEN;
    // Provide the ATT error code.
    rsp->status  = ATT_ERR_NO_ERROR;
    // Copy value

		// memcpy(&rsp->value, &(lc_p->sec_timestamp), DEF_SVC1_LC_VAL_CHAR_LEN/*rsp->length*/);
		//memcpy(&rsp->value, &sec_timestamp, DEF_SVC1_LC_TS_CHAR_LEN/*rsp->length*/);
		memcpy(&rsp->value, &(timestamp_array[num_vals]), /*num_vals * */ DEF_SVC1_LC_TS_CHAR_LEN/*rsp->length*/);

    // Send message
    ke_msg_send(rsp);
}
