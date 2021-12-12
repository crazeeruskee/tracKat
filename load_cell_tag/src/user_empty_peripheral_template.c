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
#include "systick.h"
#include "user_hx711.h"

#define SYSTICK_PERIOD_US   			20       // period for systick timer in us, so 1000000ticks = 1second
#define SYSTICK_EXCEPTION   			1        // generate systick exceptions
#define USEC_DIVISOR							50000

#define WEIGHT_LED_THRESHOLD 	    1750000  //16777100 //3500000

int new_load = 0;

bool systick_initialized = false;
load_cell* load_cell_2kg_p;

//=====OLD===============

int clock_count = 0;
int prep_count = 0;
int calibration_count = 0;
int wait_count = 0;
int num_reads = 0;
int temp_load = 0;
//uint32_t* load_p;
int load = 0;
int new_load;

int load_array[50];
uint32_t timestamp_array[50];

int baseline = 3158440; //0x003031A8
bool baseline_set = false;
int num_vals = 0;
bool val_ready = false;
bool btn_released = true;

uint32_t usec_update_counter = 0;
uint32_t sec_timestamp_counter = 0;
uint32_t sec_timestamp = 0;

static void systick_isr(void)
{
		if(prep_count < PREP_READ_TICKS){
			GPIO_SetInactive(LC_CLK_PORT,LC_CLK_PIN);
			prep_count++;
		} else{
			usec_update_counter += SYSTICK_PERIOD_US;
			if(usec_update_counter >= USEC_DIVISOR) { // 50000*20usec = 1 second
				sec_timestamp_counter++;
				usec_update_counter = 0;
			}
			if(sec_timestamp_counter >= 1000000000) sec_timestamp_counter = 0;
			
			if(!btn_released && !GPIO_GetPinStatus(BTN_PORT, BTN_PIN)){
				btn_released = true;
			}
				
			if(btn_released && GPIO_GetPinStatus(BTN_PORT, BTN_PIN)){
				new_load = load;
				sec_timestamp = sec_timestamp_counter;
				//sec_timestamp_counter = 0;
				num_vals++;
				btn_released = false;
			}
		
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

							if(num_vals >= 48){
								//clear_array((uint32_t *)load_array, LOAD_VALS_ARR_SIZE);
								num_vals = 0;
							}

							if((abs((uint32_t)load - (uint32_t)load_array[num_vals] /*(uint32_t)new_load*/)) > WEIGHT_UPDATE_THRESHOLD){
								if(!baseline_set){
									baseline = load;
									baseline_set = true;
								} else {
									new_load = load;
									sec_timestamp = sec_timestamp_counter;
								//	sec_timestamp_counter = 0;
								
									load_array[num_vals + 1] = load;
									timestamp_array[num_vals + 1] = sec_timestamp;
								
									num_vals++;
								}
							}

							//app_lc_val_handler(&load);
							temp_load = 0;
							clock_count = 0;
						//	if(/*(uint32_t)(*load_p)*/ load > WEIGHT_LED_THRESHOLD) control_LED(true);
							//else control_LED(false);
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
	}
}

//========================

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
*/
void control_LED(bool state)
{
	#if !defined(CUSTOM_TAG)
    if(state == true){
        GPIO_SetActive(LED_PORT,LED_PIN);
				#if defined(CFG_PRINTF_UART2)
					arch_puts("Turning the LED on!\n\r");
				#endif
		}
    else{
        GPIO_SetInactive(LED_PORT,LED_PIN);
				#if defined(CFG_PRINTF_UART2)
					arch_puts("Turning the LED off!\n\r");
				#endif
		}
	#endif	
}

void clear_array(uint32_t* array, uint32_t size){
		for(int i = 0; i < size; i++) array[i] = 0;
}

void my_timer_cb()
{
	#if !defined(CUSTOM_TAG)
		#if defined(CFG_PRINTF_UART2)
			arch_puts("Turning the LED off after 10 seconds\n\r");
		#endif
		control_LED(false);
	#endif
}

/**
 ****************************************************************************************
 * @brief SysTick ISR routine
 ****************************************************************************************
 */
//uint32_t load_array_test[100];

static void systick_isr_new(void)
{
			/*if(prep_count < PREP_READ_TICKS){
			GPIO_SetInactive(LC_CLK_PORT,LC_CLK_PIN);
			prep_count++;
		} else{	*/

		if(!(load_cell_2kg_p->val_ready) && !GPIO_GetPinStatus(LC_DO_PORT, LC_DO_PIN)) load_cell_2kg_p->val_ready = true;
		if(load_cell_2kg_p->val_ready){
	
			if(!(load_cell_2kg_p->calibrated)){
				
			}
		
			if(load_cell_2kg_p->reads_count < load_cell_2kg_p->max_num_reads){
				if (load_cell_2kg_p->clock_count < 50) {
					if(load_cell_2kg_p->clock_count % 2 == 0){
						GPIO_SetActive(LC_CLK_PORT,LC_CLK_PIN);
						if(load_cell_2kg_p->clock_count < 47) load_cell_2kg_p->temp_load = (load_cell_2kg_p->temp_load) << 1;
					} else{
						GPIO_SetInactive(LC_CLK_PORT,LC_CLK_PIN);
						if(load_cell_2kg_p->clock_count < 49) (load_cell_2kg_p->temp_load) += GPIO_GetPinStatus(LC_DO_PORT, LC_DO_PIN);
					}
					(load_cell_2kg_p->clock_count)++;
				} else {
					load_cell_2kg_p->load = load_cell_2kg_p->temp_load;
					
					if(load_cell_2kg_p->num_vals >= load_cell_2kg_p->max_num_vals){
//						clear_array(load_cell_2kg_p->load_array, load_cell_2kg_p->max_num_vals);
						load_cell_2kg_p->num_vals = 0;
					}
					
					if((abs((load_cell_2kg_p->load) - load_cell_2kg_p->load_array[load_cell_2kg_p->num_vals])) > load_cell_2kg_p->val_update_threshold){
						load_cell_2kg_p->new_load = load_cell_2kg_p->load;
	//					load_cell_2kg_p->load_array[load_cell_2kg_p->num_vals + 1] = load_cell_2kg_p->load;
						(load_cell_2kg_p->num_vals)++;
					}

//					if((abs((load_cell_2kg_p->load) - load_array_test[load_cell_2kg_p->num_vals])) > load_cell_2kg_p->val_update_threshold){
//						load_cell_2kg_p->new_load = load_cell_2kg_p->load;
//						load_array_test[load_cell_2kg_p->num_vals + 1] = load_cell_2kg_p->load;
//						(load_cell_2kg_p->num_vals)++;
//					}
					
					load_cell_2kg_p->temp_load = 0;
					load_cell_2kg_p->clock_count = 0;
					#if !defined(CUSTOM_TAG)
						if(load_cell_2kg_p->new_load > WEIGHT_LED_THRESHOLD) control_LED(true);
						else control_LED(false);
					#endif
					(load_cell_2kg_p->reads_count)++;
				}	
			} else {
				if(load_cell_2kg_p->wait_count < WAIT_TICKS){
					GPIO_SetInactive(LC_CLK_PORT,LC_CLK_PIN);
					(load_cell_2kg_p->wait_count)++;
				} else {
					load_cell_2kg_p->val_ready = false;
					load_cell_2kg_p->wait_count = 0;
					load_cell_2kg_p->reads_count = 0;
					//load_cell_2kg_p->prep_count = 0;
				}
			}
		}
//	}
}
//int count = 0;
//static void systick_isr(void)
//{
//	count++;
//	if(count >= 1000000) count = 0;
//}

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
		if(load_cell_2kg_p == NULL){
			load_cell_2kg_p = new_load_cell(2000);
			set_calibrate_lc(load_cell_2kg_p, CALIBRATION_READS);
		}
		
		if(!systick_initialized){
			systick_register_callback(systick_isr);
			systick_start(SYSTICK_PERIOD_US, SYSTICK_EXCEPTION);
			systick_initialized = true;
		}
		

    default_app_on_connection(connection_idx, param);
}

void user_on_disconnect( struct gapc_disconnect_ind const *param )
{
		#if !defined(CUSTOM_TAG)
			control_LED(false);
		#endif 
		//free(load_cell_2kg_p);
	//	systick_stop();
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
									user_svc1_read_lc_val_handler(load_cell_2kg_p, msgid, msg_param, dest_id, src_id);
                } break;
								
								case SVC1_IDX_LC_BASE_VAL:
                {
									user_svc1_read_lc_base_val_handler(load_cell_2kg_p, msgid, msg_param, dest_id, src_id);
                } break;
								
								case SVC1_IDX_LC_NUM_VALS_VAL:
                {
									user_svc1_read_lc_num_vals_handler(load_cell_2kg_p, msgid, msg_param, dest_id, src_id);
                } break;
								
								case SVC1_IDX_LC_TS_VAL:
                {
									user_svc1_read_lc_ts_handler(load_cell_2kg_p, msgid, msg_param, dest_id, src_id);
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

