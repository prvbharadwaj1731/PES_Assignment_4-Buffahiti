/*
 * state_machine.c
 *
 *  Created on: 01-Mar-2022
 *      Author: prvbh
 */

#include "MKL25Z4.h"
#include "pwm.h"
#include "state_machine.h"
#include <stdbool.h>


typedef enum{
	stop_s,
	warning_s,
	go_s,
	crosswalk_s
}state_color;

static color_code color_code_provider(uint32_t color_in)
{
    color_code stop_color, warning_color, go_color, crosswalk_color;

    /*
     * STORING COLOR CODES IN DECIMAL HERE
     * */
    stop_color.red = 97;
    stop_color.blue = 30;
    stop_color.green = 60;

    warning_color.red = 255;
    warning_color.green = 178;
    warning_color.blue = 0;

    go_color.red = 34;
    go_color.green = 150;
    go_color.blue = 34;

    crosswalk_color.red = 0;
    crosswalk_color.green = 16;
    crosswalk_color.blue = 48;


    if(color_in == stop_s)
    	return stop_color;
    else if(color_in == go_s)
    	return go_color;
    else if(color_in == warning_s)
    	return warning_color;
    else
    	return crosswalk_color;

}


/*
 * Color state machine. 9 states present, covering all possible cases
 * */

color_code state_machine(state current_state, uint8_t fraction, color_code current_color)
{
	color_code set_color, start_color, end_color;
	switch(current_state)
	{
	case STOP:
		set_color = color_code_provider(stop_s);
		current_color = set_led_color(set_color);
		break;

	case STOP_TO_GO_TRANSITION:
		start_color = color_code_provider(stop_s);
		end_color = color_code_provider(go_s);
		current_color = color_transition(current_color, end_color, fraction);
		break;

	case GO:
		set_color = color_code_provider(go_s);
		current_color = set_led_color(current_color);
		break;

	case GO_TO_WARNING_TRANSITION:
		start_color = color_code_provider(go_s);
		end_color = color_code_provider(warning_s);
		current_color = color_transition(current_color, end_color, fraction);
		break;

	case WARNING:
		set_color = color_code_provider(warning_s);
		current_color = set_led_color(current_color);
		break;

	case WARNING_TO_STOP_TRANSITION:
		start_color = color_code_provider(warning_s);
		end_color = color_code_provider(stop_s);
		current_color = color_transition(current_color, end_color, fraction);
		break;

	case CROSSWALK:
		set_color = color_code_provider(crosswalk_s);
		current_color = crosswalk_pattern(current_color); //calling blocking function
		break;

	case CROSSWALK_TO_GO_TRANSITION:
		start_color = color_code_provider(crosswalk_s);
		end_color = color_code_provider(go_s);
		current_color = color_transition(current_color, end_color, fraction);
		break;

	case CROSSWALK_TRANSITION:
		end_color = color_code_provider(crosswalk_s);
		current_color = color_transition(current_color, end_color, fraction);
		break;
	}

	return current_color;
}
