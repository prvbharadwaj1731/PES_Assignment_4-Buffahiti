/*
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    PES_Assignment_4.c
 * @brief   Application entry point.
 */

#ifdef DEBUG
#define DELAY_MS 5000
#define DELAY_WARNING_MS 3000
#else
#define DELAY_MS 20000
#define DELAY_WARNING_MS 5000
#endif

//#define DEBUG_DELAY_MS 5000
//#define DEBUG_DELAY_WARNING_MS 3000
//#define PRODUCTION_DELAY_MS 20000
//#define PRODUCTION_DELAY_WARNING_MS 5000
#define TRANSITION_DELAY_MS 1000
#define LOOP_RESOLUTION 100
#define PWM_PERIOD 48000


#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "timers.h"
#include "led_control.h"
#include "pwm.h"
#include "tsi.h"
#include "state_machine.h"
#include "log.h"
/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

//typedef enum{
//   STOP_TO_GO = 1,
//   GO_TO_WARNING = 2,
//   WARNING_TO_STOP = 4
//}state;


int main(void) {

  	/* Init board hardware. */

	BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif


    Init_SysTick();
    LED_INIT();
    TSI_INIT();
    pwm_init(PWM_PERIOD);


    state current_state = STOP;
    color_code current_color;
    uint8_t transition_fraction = 1; //start off with 1 part of the transition
    uint32_t delay_counter = 0; //initialize delay value
    bool touch_event;

    LOG("Main loop starting.\n");

    while(1){

    	current_color = state_machine(current_state, transition_fraction, current_color);
    	touch_event = TSI_READ();

    	//advancing states

    	// since warning state has a different timing
    	if(delay_counter == DELAY_WARNING_MS/100)
    	{
    		if(current_state == WARNING){
    			current_state = WARNING_TO_STOP_TRANSITION;
    			LOG("transitioning from WARNING TO STOP at %d ms\n", 16*now());
    			delay_counter = 0; //reset counter
				transition_fraction = 1;
    		}
    	}

    	if(current_state == CROSSWALK) //because a blocking function takes care of the pattern
    			{
    				current_state = CROSSWALK_TO_GO_TRANSITION;
    				LOG("transitioning from CROSSWALK to GO at %d ms\n", 16*now());
    				delay_counter = 0; //reset counter
    				transition_fraction = 1;
    			}


    	// if transitions are completed
    	if(delay_counter == TRANSITION_DELAY_MS/100)
    	{
    		if(current_state == STOP_TO_GO_TRANSITION){
    			current_state = GO;
        		delay_counter = 0; //reset counter
        		transition_fraction = 1;
    		}
    		else if(current_state == GO_TO_WARNING_TRANSITION){
    			current_state = WARNING;
    			delay_counter = 0; //reset counter
				transition_fraction = 1;
    		}
    		else if(current_state == WARNING_TO_STOP_TRANSITION){
    			current_state = STOP;
    			delay_counter = 0; //reset counter
				transition_fraction = 1;
    		}
    		else if(current_state == CROSSWALK_TRANSITION){
    			current_state = CROSSWALK;
    			delay_counter = 0; //reset counter
    			transition_fraction = 1;
    		}
    		else if(current_state == CROSSWALK_TO_GO_TRANSITION){
    			current_state = GO;
    			delay_counter = 0; //reset counter
				transition_fraction = 1;
    		}
    	}

    	// timing for the traffic lights
    	if(delay_counter == DELAY_MS/100)
    	{
    		if(current_state == STOP){
    			current_state = STOP_TO_GO_TRANSITION;
    			LOG("transitioning from STOP to GO at %d ms\n", 16*now());
        		delay_counter = 0; //reset counter
        		transition_fraction = 1;
    		}

    		else if(current_state == GO){
    			current_state = GO_TO_WARNING_TRANSITION;
    			LOG("transitioning from GO to WARNING at %d ms\n", 16*now());
        		delay_counter = 0; //reset counter
        		transition_fraction = 1;
    		}


    	}


    	// if we are transitioning between colors, handle fraction count and delay separately
    	if(current_state == STOP_TO_GO_TRANSITION || GO_TO_WARNING_TRANSITION || WARNING_TO_STOP_TRANSITION || CROSSWALK_TRANSITION|| CROSSWALK_TO_GO_TRANSITION)
    	{
    		transition_fraction++;
			delay_counter++;
    	}
    	else
    		delay_counter++;



    	// if a touch event has been registered previously, we update state
    	if(touch_event)
    	{
    		state temp_state = current_state;
    		current_state = CROSSWALK_TRANSITION;
    		LOG("Button press detected at %d ms\n", 16*now());
    		LOG("transitioning from %s to CROSSWALK at %d ms\n", temp_state, 16*now());
    		delay_counter = 0;
    		transition_fraction = 1;
    	}



    	delay_ms(LOOP_RESOLUTION);
    }
    return 0;
}
