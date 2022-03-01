/*
 * tsi.c
 *
 *  Created on: 28-Feb-2022
 *      Author: prvbh
 */

#define TSI0_GENCS_EOSF 2 // scan complete bit. gets set once scan is complete
#define TSI0_DATA_START 22
#define TOUCH_OFFSET 560

#include "MKL25Z4.h"
#include <stdint.h>

void TSI_INIT(void)
{
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;
//	TSI0->GENCS |= (1 << TSI0_GENCS_ENABLE);
//	TSI0->GENCS |= TSI_GENCS_NSCN(31u);

	TSI0->GENCS = TSI_GENCS_MODE(0u) | //operating in non-noise mode
								TSI_GENCS_REFCHRG(0u) | //reference oscillator charge and discharge value 500nA
								TSI_GENCS_DVOLT(0u) | //oscillator voltage rails set to default
								TSI_GENCS_EXTCHRG(0u) | //electrode oscillator charge and discharge value 500nA
								TSI_GENCS_PS(0u) |  // frequency clcok divided by one
								TSI_GENCS_NSCN(31u) | //scanning the electrode 32 times
								TSI_GENCS_TSIEN_MASK | //enabling the TSI module
								TSI_GENCS_EOSF_MASK; // writing one to clear the end of scan flag
}

uint32_t TSI_READ(void)
{
	uint32_t capacitive_reading = 0;
	TSI0->DATA = TSI_DATA_TSICH(10u); //use channel 10
	TSI0->DATA |= (1 << TSI0_DATA_START);
	while(!(TSI0->GENCS & TSI_GENCS_EOSF_MASK))
	{

	}

	capacitive_reading = TSI0->DATA & 0xFFFF; // storing last 16 bits
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK; // clear EOSF bit by writing 1 to it
	return capacitive_reading - TOUCH_OFFSET;
}
