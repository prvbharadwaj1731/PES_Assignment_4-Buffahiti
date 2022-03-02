/*
 * timers.c
 *
 *  Created on: 21-Feb-2022
 *      Author: prvbh
 */
#define CSR_MASK 0x7 // only sets clock source and enables interrupt
#define COUNTER_START_MASK 0x1 // sets enable bit HIGH
#define CORE_CLOCK 48000000 // 48 MHz core clock
#define CLOCK_DIVIDER 16 //Divider value for 1/16th of a second delay

#include <stdint.h>
#include <stdio.h>
#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include "assert.h"
#include "timers.h"

ticktime_t tick_count, timer_count = 0;

void Init_SysTick(void)
{
	SysTick->CTRL = 0; // Clear CSR
	SysTick->LOAD = (CORE_CLOCK/CLOCK_DIVIDER) - 1;
	NVIC_SetPriority(SysTick_IRQn, 3);
	SysTick->VAL = 0; // initializes current value register to 0
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
	uint32_t masking_state = __get_PRIMASK();
	__disable_irq();
	++tick_count;
	++timer_count;
	__set_PRIMASK(masking_state);
}

ticktime_t now()
{
	return tick_count;
}

void reset_timer(void)
{
	timer_count = 0;
}

ticktime_t get_timer()
{
	return timer_count;
}

void delay_ms(uint32_t time_in)
{
	ticktime_t temp_count;
	reset_timer();
	temp_count = get_timer();
	while(temp_count != (uint32_t)((time_in*16)/1000)) // wait for delay
	{
		temp_count = get_timer();
	}
}


