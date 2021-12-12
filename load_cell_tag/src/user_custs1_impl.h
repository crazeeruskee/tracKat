/**
 ****************************************************************************************
 *
 * @file user_custs1_impl.h
 *
 * @brief Peripheral project Custom1 Server implementation header file.
 *
 * Copyright (C) 2015-2019 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef _USER_CUSTS1_IMPL_H_
#define _USER_CUSTS1_IMPL_H_

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
 * DEFINES
 ****************************************************************************************
 */

enum
{
    CUSTS1_CP_ADC_VAL1_DISABLE = 0,
    CUSTS1_CP_ADC_VAL1_ENABLE,
};

enum
{
    CUSTS1_LED_OFF = 0,
    CUSTS1_LED_ON,
};

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "gapc_task.h"                 // gap functions and messages
#include "gapm_task.h"                 // gap functions and messages
#include "custs1_task.h"
#include "user_hx711.h"

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void user_svc1_read_lc_val_handler(load_cell *lc_p,
																	ke_msg_id_t const msgid,
																	struct custs1_value_req_ind const *param,
																	ke_task_id_t const dest_id,
																	ke_task_id_t const src_id);
															
void user_svc1_read_lc_base_val_handler(load_cell *lc_p,
																	ke_msg_id_t const msgid,
																	struct custs1_value_req_ind const *param,
																	ke_task_id_t const dest_id,
																	ke_task_id_t const src_id);
																	
void user_svc1_read_lc_max_val_handler(load_cell *lc_p,
																	ke_msg_id_t const msgid,
																	struct custs1_value_req_ind const *param,
																	ke_task_id_t const dest_id,
																	ke_task_id_t const src_id);
																				
void user_svc1_read_lc_num_vals_handler(load_cell *lc_p,
																	ke_msg_id_t const msgid,
																	struct custs1_value_req_ind const *param,
																	ke_task_id_t const dest_id,
																	ke_task_id_t const src_id);
																				
void user_svc1_read_lc_ts_handler(load_cell *lc_p,
																	ke_msg_id_t const msgid,
																	struct custs1_value_req_ind const *param,
																	ke_task_id_t const dest_id,
																	ke_task_id_t const src_id);

/// @} APP

#endif // _USER_CUSTS1_IMPL_H_
