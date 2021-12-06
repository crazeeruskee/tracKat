/**
 ****************************************************************************************
 *
 * @file user_empty_peripheral_template.c
 *
 * @brief Empty peripheral template project source code.
 *
 * Copyright (C) 2012-2019 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */
#include "rwip_config.h"             // SW configuration


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_api.h"
#include "user_empty_peripheral_template.h"
#include "gpio.h"
#include "user_periph_setup.h"
#include "arch_console.h"
#include "user_custs1_impl.h"
#include "user_custs1_def.h"
#include "co_bt.h"

#define SYSTICK_PERIOD_US   			20       // period for systick timer in us, so 1000000ticks = 1second
#define SYSTICK_EXCEPTION   			1        // generate systick exceptions

#define LOAD_THRESHOLD 						500
#define PREP_READ_TICKS						20
#define MAX_READS 								1
#define WAIT_TICKS								2000
			 
#define WEIGHT_LED_THRESHOLD 	    1750000  //16777100 //3500000
#define WEIGHT_UPDATE_THRESHOLD 	8192   //Minimum difference between load values to add value to weights array, experimentally found lower 12 bits fluctuate too much with same weights 

int clock_count = 0;
int prep_count = 0;
int wait_count = 0;
int num_reads = 0;
int temp_load = 0;
//uint32_t* load_p;
int load = 0;
int new_load = 0;
int load_array[LOAD_VALS_ARR_SIZE];
int num_vals = 0;
bool val_ready = false;


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
*/
void control_LED(bool state)
{
    if(state == true){
        GPIO_SetActive(LED_PORT,LED_PIN);
				//arch_puts("Turning the LED on!\n\r");
		}
    else{
        GPIO_SetInactive(LED_PORT,LED_PIN);
				//arch_puts("Turning the LED off!\n\r");
		}
}

void clear_array(uint32_t* array, uint32_t size){
		for(int i = 0; i < size; i++) array[i] = 1;
}

void my_timer_cb()
{
		//arch_puts("Turning the LED off after 10 seconds\n\r");
		control_LED(false);
}

/**
 ****************************************************************************************
 * @brief SysTick ISR routine
 ****************************************************************************************
 */
static void systick_isr(void)
{
		/*if(prep_count < PREP_READ_TICKS){
			GPIO_SetInactive(LC_CLK_PORT,LC_CLK_PIN);
			prep_count++;
		} else{	*/
	
		if(!val_ready && !GPIO_GetPinStatus(LC_DO_PORT, LC_DO_PIN)) val_ready = true;
		if(val_ready){
			if(num_reads < MAX_READS){
				if (clock_count < 50) {
					if(clock_count % 2 == 0){
						GPIO_SetActive(LC_CLK_PORT,LC_CLK_PIN);
						if(clock_count < 47) temp_load = temp_load << 1;
					} else{
						GPIO_SetInactive(LC_CLK_PORT,LC_CLK_PIN);
						if(clock_count < 49) temp_load += GPIO_GetPinStatus(LC_DO_PORT, LC_DO_PIN);
					}
					clock_count++;
				} else {
					//(*load_p) = (uint32_t)temp_load;
					load = temp_load;//0xDEADBEEF;//
					
					if(num_vals >= 99){
						clear_array((uint32_t *)load_array, LOAD_VALS_ARR_SIZE);
						num_vals = 0;
					}
					
					if((abs((uint32_t)load - (uint32_t)(load_array[num_vals]))) > WEIGHT_UPDATE_THRESHOLD){
						load_array[num_vals + 1] = load;
						new_load = load;
						num_vals++;
					}
					
					//app_lc_val_handler(&load);
					temp_load = 0;
					clock_count = 0;
					if(/*(uint32_t)(*load_p)*/ load > WEIGHT_LED_THRESHOLD) control_LED(true);
					else control_LED(false);
					num_reads++;
				}	
			} else {
				if(wait_count < WAIT_TICKS){
					GPIO_SetInactive(LC_CLK_PORT,LC_CLK_PIN);
					wait_count++;
				} else {
					val_ready = false;
					wait_count = 0;
					num_reads = 0;
					prep_count = 0;
				}
			}
		}
//	}
}

/*
void systick_test(void)
{
    systick_register_callback(systick_isr);
    // Systick will be initialized to use a reference clock frequency of 1 MHz
    systick_start(SYSTICK_PERIOD_US, SYSTICK_EXCEPTION);
}
*/

void user_on_connection(uint8_t connection_idx, struct gapc_connection_req_ind const *param)
{
		//app_easy_timer(1000, my_timer_cb);
		//if(GPIO_GetPinStatus(BTN_PORT, BTN_PIN)){
			GPIO_SetInactive(LC_CLK_PORT,LC_CLK_PIN);
			systick_register_callback(systick_isr);
			systick_start(SYSTICK_PERIOD_US, SYSTICK_EXCEPTION);
	//	}
		//load_p = malloc(sizeof(uint32_t));
    default_app_on_connection(connection_idx, param);
}

void user_on_disconnect( struct gapc_disconnect_ind const *param )
{
		control_LED(false);
		//free(load_p);
		default_app_on_disconnect(param);
}

void user_get_bdaddr(struct bd_addr* address){
		struct bd_addr tempAddr = {0xEE, 0xFF, 0xC0, 0xEE, 0xFF, 0xC0};
		memcpy(address, &tempAddr, 6);
}

void user_advertise_operation(void){
	  // Copy the current default advertising data specified in user_config.h
    struct gapm_start_advertise_cmd* adv_cmd; // Retained version
    struct gapm_start_advertise_cmd* cmd;     // Version we send
    cmd = app_advertise_start_msg_create();   // Create an empty advertise start command

    // Get the advertising properties from the app_easy function
    adv_cmd = app_easy_gap_undirected_advertise_get_active();
    // Make a copy of the advertising properties
    memcpy(cmd, adv_cmd, sizeof(struct gapm_start_advertise_cmd));

    // Specify the advertise data (we are just changing the BD name)
    // Adv. data segment with length of 0x05, type is BD name and name is a Smiling Cat emoji
    uint8_t adv_data[] = {0x05, GAP_AD_TYPE_COMPLETE_NAME, 0xF0, 0x9F, 0x98, 0xBA};
    cmd->info.host.adv_data_len = sizeof(adv_data);
    memcpy(&cmd->info.host.adv_data, &adv_data, sizeof(adv_data));

    // Send message to GAP task
    ke_msg_send(cmd);

    // We are now connectable
    ke_state_set(TASK_APP, APP_CONNECTABLE);
		
		// Copy the BD name from the advertising data (ignore the size and the type fields)
		device_info.dev_name.length =  sizeof(adv_data) - 2;
		memcpy(device_info.dev_name.name , &adv_data[2], device_info.dev_name.length);
}


void user_catch_rest_hndl(ke_msg_id_t const msgid,
                          void const *param,
                          ke_task_id_t const dest_id,
                          ke_task_id_t const src_id)
{
    switch(msgid)
    {
				/*case CUSTS1_VAL_SET_REQ:
				{
					struct custs1_val_write_ind const *msg_param = (struct custs1_val_write_ind const *)(param);
					
					switch(msg_param->handle)
					{
						case SVC1_IDX_LC_VAL_NTF_CFG:
							user_svc1_lc_val_rd_handler(msgid, msg_param, dest_id, src_id);
							break;
						
						case SVC1_IDX_LC_VAL_VAL:
							user_svc1_lc_val_rd_handler(msgid, msg_param, dest_id, src_id);
							break;
					}
					
				}*/
			
        case CUSTS1_VAL_WRITE_IND:
        {
            struct custs1_val_write_ind const *msg_param = (struct custs1_val_write_ind const *)(param);

            switch (msg_param->handle)
            {
							
						/*	 case SVC1_IDX_LC_VAL_NTF_CFG:
                    user_svc1_lc_val_wr_handler(msgid, msg_param, dest_id, src_id);
                    break;
							*/
                case SVC1_IDX_CONTROL_POINT_VAL:
                    user_svc1_ctrl_wr_ind_handler(msgid, msg_param, dest_id, src_id);
                    break;

                case SVC1_IDX_LED_STATE_VAL:
                    user_svc1_led_wr_ind_handler(msgid, msg_param, dest_id, src_id);
                    break;

                case SVC1_IDX_ADC_VAL_1_NTF_CFG:
                    user_svc1_adc_val_1_cfg_ind_handler(msgid, msg_param, dest_id, src_id);
                    break;

                case SVC1_IDX_BUTTON_STATE_NTF_CFG:
                    user_svc1_button_cfg_ind_handler(msgid, msg_param, dest_id, src_id);
                    break;

                case SVC1_IDX_INDICATEABLE_IND_CFG:
                    user_svc1_long_val_cfg_ind_handler(msgid, msg_param, dest_id, src_id);
                    break;

                case SVC1_IDX_LONG_VALUE_NTF_CFG:
                    user_svc1_long_val_cfg_ind_handler(msgid, msg_param, dest_id, src_id);
                    break;

                case SVC1_IDX_LONG_VALUE_VAL:
                    user_svc1_long_val_wr_ind_handler(msgid, msg_param, dest_id, src_id);
                    break;

                default:
                    break;
            }
        } break;

        case CUSTS1_VAL_NTF_CFM:
        {
            struct custs1_val_ntf_cfm const *msg_param = (struct custs1_val_ntf_cfm const *)(param);

            switch (msg_param->handle)
            {
                case SVC1_IDX_ADC_VAL_1_VAL:
                    break;

                case SVC1_IDX_BUTTON_STATE_VAL:
                    break;

                case SVC1_IDX_LONG_VALUE_VAL:
                    break;

                default:
                    break;
            }
        } break;

        case CUSTS1_VAL_IND_CFM:
        {
            struct custs1_val_ind_cfm const *msg_param = (struct custs1_val_ind_cfm const *)(param);

            switch (msg_param->handle)
            {
                case SVC1_IDX_INDICATEABLE_VAL:
                    break;

                default:
                    break;
             }
        } break;

        case CUSTS1_ATT_INFO_REQ:
        {
            struct custs1_att_info_req const *msg_param = (struct custs1_att_info_req const *)param;

            switch (msg_param->att_idx)
            {
/*                case SVC1_IDX_LONG_VALUE_VAL:
                    user_svc1_long_val_att_info_req_handler(msgid, msg_param, dest_id, src_id);
                    break;
*/
                default:
                    user_svc1_rest_att_info_req_handler(msgid, msg_param, dest_id, src_id);
                    break;
							
             }
        } break;

        case GAPC_PARAM_UPDATED_IND:
        {
            // Cast the "param" pointer to the appropriate message structure
            struct gapc_param_updated_ind const *msg_param = (struct gapc_param_updated_ind const *)(param);

            // Check if updated Conn Params filled to preferred ones
            if ((msg_param->con_interval >= user_connection_param_conf.intv_min) &&
                (msg_param->con_interval <= user_connection_param_conf.intv_max) &&
                (msg_param->con_latency == user_connection_param_conf.latency) &&
                (msg_param->sup_to == user_connection_param_conf.time_out))
            {
            }
        } break;

        case CUSTS1_VALUE_REQ_IND:
        {
            struct custs1_value_req_ind const *msg_param = (struct custs1_value_req_ind const *) param;

            switch (msg_param->att_idx)
            {

								case SVC1_IDX_LC_VAL_VAL:
                {
                    user_svc1_read_lc_val_handler(msgid, msg_param, dest_id, src_id);
                } break;
								
                case SVC3_IDX_READ_4_VAL:
                {
                    user_svc3_read_non_db_val_handler(msgid, msg_param, dest_id, src_id);
                } break;

                default:
                {
                    // Send Error message
                    struct custs1_value_req_rsp *rsp = KE_MSG_ALLOC(CUSTS1_VALUE_REQ_RSP,
                                                                    src_id,
                                                                    dest_id,
                                                                    custs1_value_req_rsp);

                    // Provide the connection index.
                    rsp->conidx  = app_env[msg_param->conidx].conidx;
                    // Provide the attribute index.
                    rsp->att_idx = msg_param->att_idx;
                    // Force current length to zero.
                    rsp->length = 0;
                    // Set Error status
                    rsp->status  = ATT_ERR_APP_ERROR;
                    // Send message
                    ke_msg_send(rsp);
                } break;
             }
        } break;

        default:
            break;
    }
}

/// @} APP

