/*
 * pwm.c
 *
 *  Created on: 27-Feb-2022
 *      Author: prvbh
 */

#define MAX_COLOR_CODE 255
#define TIMER_RESOLUTION 63
#define COLOR_CHANGE_PERIOD_MS 0.0625 //(1/16th of a second)
#define CROSSWALK_ONTIME_MS 750
#define CROSSWALK_OFFTIME_MS 250
#define CROSSWALK_PERIOD_S 10
#define TPM_PERIOD_TICKS 48000
#define TRANSITION_RESOLUTION_POW 3


#include <stdint.h>
#include <stdio.h>
#include "MKL25Z4.h"
#include "timers.h"
#include "core_cm0plus.h"
#include "pwm.h"


void pwm_init(uint16_t period)
{
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;
	//configure TPM to use 48 MHz clock
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);

	//load counter and mod
	TPM0->MOD = period -1;
	TPM2->MOD = period -1;

	TPM0->SC = TPM_SC_PS(0);
	TPM2->SC = TPM_SC_PS(0);
	//Continue operation in debug mode
	TPM0->CONF |= TPM_CONF_DBGMODE(3);
	TPM2->CONF |= TPM_CONF_DBGMODE(3);

	//Use channel 1 in edge-aligned low-true mode
	TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

	//set initial duty cycle to 0
	TPM0->CONTROLS[1].CnV = 0;
	TPM2->CONTROLS[1].CnV = 0;
	TPM2->CONTROLS[0].CnV = 0;
	//TPM0->CONTROLS[1].CnV = 0;

	//Start timer
	TPM0->SC |= TPM_SC_CMOD(1);
	TPM2->SC |= TPM_SC_CMOD(1);
}

color_code set_led_color(color_code color_in)
{
	TPM0->CONTROLS[1].CnV = (color_in.blue*TPM_PERIOD_TICKS)/MAX_COLOR_CODE;
	TPM2->CONTROLS[1].CnV = (color_in.green*TPM_PERIOD_TICKS)/MAX_COLOR_CODE;
	TPM2->CONTROLS[0].CnV = (color_in.red*TPM_PERIOD_TICKS)/MAX_COLOR_CODE;

	return color_in;
}

static void CLEAR_LED(void)
{
	TPM0->CONTROLS[1].CnV = 0;
	TPM2->CONTROLS[1].CnV = 0;
	TPM2->CONTROLS[0].CnV = 0;
}


color_code color_transition(color_code start_color, color_code end_color, uint8_t fraction)
{
	color_code temp_color;
	temp_color.red = ((end_color.red - start_color.red)*(fraction)>>TRANSITION_RESOLUTION_POW) + start_color.red;
	temp_color.green = ((end_color.green - start_color.green)*(fraction)>>TRANSITION_RESOLUTION_POW) + start_color.green;
	temp_color.blue = ((end_color.blue - start_color.blue)*(fraction)>>TRANSITION_RESOLUTION_POW) + start_color.blue;
	set_led_color(temp_color);
	return temp_color;
}


color_code crosswalk_pattern(color_code color_in)
{
	for(int i=0;i<CROSSWALK_PERIOD_S;i++) //hardcoded delay for crosswalk
	{
		color_code temp_color;
		temp_color = set_led_color(color_in);
		delay_ms(750); //ON time
		CLEAR_LED();
		delay_ms(250); //OFF time
	}
	return color_in;
}

