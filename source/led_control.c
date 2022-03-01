/*
 * led_control.c
 *
 *  Created on: 23-Feb-2022
 *      Author: prvbh
 */

#define RED_LED_PIN 18
#define GREEN_LED_PIN 19
#define BLUE_LED_PIN 1

#define RED_MASK (1 << RED_LED_PIN)
#define GREEN_MASK (1 << GREEN_LED_PIN)
#define BLUE_MASK (1 << BLUE_LED_PIN)


#include "MKL25Z4.h"

void LED_INIT(void)
{
	//enabling clocks to port B and D
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;


	PORTB->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_PIN] |= PORT_PCR_MUX(3);

	PORTB->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(3);

	PORTD->PCR[BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_PIN] |= PORT_PCR_MUX(4);
}




