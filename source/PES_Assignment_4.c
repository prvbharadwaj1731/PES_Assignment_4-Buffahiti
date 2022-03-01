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

#define DEBUG_DELAY_MS 5000
#define DEBUG_DELAY_WARNING_MS 3000
#define PRODUCTION_DELAY_MS 20000
#define PRODUCTION_DELAY_WARNING_MS 5000


#include <stdio.h>
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
/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

typedef enum{
   STOP_TO_GO = 1,
   GO_TO_WARNING = 2,
   WARNING_TO_STOP = 4
}state;


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
    pwm_init(48000);

    state current_state = STOP_TO_GO;

    uint32_t touch_val, delay_counter;

    //declaring the color codes for the various states in decimal representation
    color_code STOP, WARNING, GO, CROSSWALK;

    STOP.red = 97;
    STOP.blue = 30;
    STOP.green = 60;

    WARNING.red = 255;
    WARNING.green = 178;
    WARNING.blue = 0;

    GO.red = 34;
    GO.green = 150;
    GO.blue = 34;

    CROSSWALK.red = 0;
    CROSSWALK.green = 16;
    CROSSWALK.blue = 48;


    SET_LED_COLOR(STOP, 48000);


    while(1){
    	 switch(current_state)
    	    {
    	    case STOP_TO_GO:
    	    	COLOR_TRANSITION(STOP,GO);
    	    	break;

    	    case GO_TO_WARNING:
    	    	COLOR_TRANSITION(GO,WARNING);
    	    	break;

    	    case WARNING_TO_STOP:
    	    	COLOR_TRANSITION(WARNING,STOP);
    	    	break;
    	    }

    	 	//Because WARNING state has a different delay requirement
    	    if(current_state & GO_TO_WARNING)
    	    	delay_counter == DEBUG_DELAY_WARNING_MS/100;
    	    else
    	    	delay_counter = DEBUG_DELAY_MS/100;

    	    //light timing delay
    	    for(int i=0;i<delay_counter;i++)
    	    {
    	    	delay_ms(100);
    	    	touch_val = TSI_READ();
    	    	if(touch_val > 50)
    	    	{
    	    		color_code start_color;
    	    		if(current_state == STOP_TO_GO)
    	    			start_color = GO;
    	    		else if(current_state == GO_TO_WARNING)
    	    			start_color = WARNING;
    	    		else if(current_state == WARNING_TO_STOP)
    	    			start_color = STOP;

    	    		CROSSWALK_MODE(start_color, CROSSWALK);
    	    		COLOR_TRANSITION(CROSSWALK,GO);
    	    		current_state = STOP_TO_GO;
    	    	}
    	    }

    	    //advancing state
    	    if(current_state == STOP_TO_GO)
    	    	current_state = GO_TO_WARNING;

    	    else if(current_state == GO_TO_WARNING)
    	    	current_state = WARNING_TO_STOP;

    	    else if(current_state == WARNING_TO_STOP)
    	    	current_state = STOP_TO_GO;
    }




    return 0 ;
}
