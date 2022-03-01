/*
 * pwm.c
 *
 *  Created on: 27-Feb-2022
 *      Author: prvbh
 */

#define MAX_COLOR_CODE 255
#define TIMER_RESOLUTION 63
#define COLOR_CHANGE_PERIOD_MS 0.0625 //(1/16th of a second)
#define test 1/COLOR_CHANGE_PERIOD_MS
#define CROSSWALK_ONTIME_MS 750
#define CROSSWALK_OFFTIME_MS 250
#define CROSSWALK_PERIOD_S 10

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

void SET_LED_COLOR(color_code color_in, uint16_t period)
{
	TPM0->CONTROLS[1].CnV = (uint32_t)((color_in.blue*period)/MAX_COLOR_CODE);
	TPM2->CONTROLS[1].CnV = (uint32_t)((color_in.green*period)/MAX_COLOR_CODE);
	TPM2->CONTROLS[0].CnV = (uint32_t)((color_in.red*period)/MAX_COLOR_CODE);
}

static void CLEAR_LED(void)
{
	TPM0->CONTROLS[1].CnV = 0;
	TPM2->CONTROLS[1].CnV = 0;
	TPM2->CONTROLS[0].CnV = 0;
}

void COLOR_TRANSITION(color_code start_color, color_code end_color)
{
	color_code temp_color;
	for(int j=0;j<16;j++)
	{
		temp_color.red = (end_color.red - start_color.red)*(j+1)*0.125 + start_color.red;
		temp_color.green = (end_color.green - start_color.green)*(j+1)*0.125 + start_color.green;
		temp_color.blue = (end_color.blue - start_color.blue)*(j+1)*0.125 + start_color.blue;
		SET_LED_COLOR(temp_color, 48000);
		delay_ms(TIMER_RESOLUTION);
	}
}

//color_code COLOR_TRANSITION(color_code start_color, color_code end_color, uint32_t scaling)
//{
//	color_code temp_color;
//	temp_color.red = (end_color.red - start_color.red)*(scaling+1)*0.125 + start_color.red;
//	temp_color.green = (end_color.green - start_color.green)*(scaling+1)*0.125 + start_color.green;
//	temp_color.blue = (end_color.blue - start_color.blue)*(scaling+1)*0.125 + start_color.blue;
//	SET_LED_COLOR(temp_color, 48000);
//	//delay_ms(TIMER_RESOLUTION);
//
//	return temp_color;
//}

void CROSSWALK_MODE(color_code start_color, color_code end_color)
{
	COLOR_TRANSITION(start_color, end_color);
	for(int i=0;i<CROSSWALK_PERIOD_S;i++)
	{
		delay_ms(CROSSWALK_ONTIME_MS);
		CLEAR_LED();
		delay_ms(CROSSWALK_OFFTIME_MS);
		SET_LED_COLOR(end_color, 48000);
	}
}

