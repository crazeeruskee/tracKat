/**
 ****************************************************************************************
 *
 * @file main.c
 *
 * @brief SysTick example
 *
 * Copyright (C) 2012-2019 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */
#include <stdio.h>
#include "arch_system.h"
#include "user_periph_setup.h"
#include "systick.h"
#include "gpio.h"

#define SYSTICK_PERIOD_US   30     // period for systick timer in us, so 1000000ticks = 1second
#define SYSTICK_EXCEPTION   1      // generate systick exceptions

#define LOAD_THRESHOLD 			500
#define MAX_READS 					300

int clock_count = 0;
int num_reads = 0;
int temp_load = 0;
int load = 0;

/**
 ****************************************************************************************
 * @brief SysTick Test routine
 ****************************************************************************************
 */
void systick_test(void);

/**
 ****************************************************************************************
 * @brief Main routine of the SysTick example
 ****************************************************************************************
 */
int main (void)
{
    system_init();
    systick_test();
    while(1);
}

/**
 ****************************************************************************************
 * @brief SysTick ISR routine
 ****************************************************************************************
 */
static void systick_isr(void)
{	
	//	if(num_reads < MAX_READS){
			if (clock_count < 48) {
				if(clock_count % 2 == 0){
					GPIO_SetInactive(LC_CLK_PORT,LC_CLK_PIN);
				} else{
					GPIO_SetActive(LC_CLK_PORT,LC_CLK_PIN);
					temp_load = (temp_load << 1) | GPIO_GetPinStatus(LC_DO_PORT, LC_DO_PIN);
				}
				clock_count++;
			} else {
				load = temp_load;
				temp_load = 0;
				clock_count = 0;
			//	num_reads++;
			}	
}


void systick_test(void)
{
    systick_register_callback(systick_isr);
    // Systick will be initialized to use a reference clock frequency of 1 MHz
    systick_start(SYSTICK_PERIOD_US, SYSTICK_EXCEPTION);
}
