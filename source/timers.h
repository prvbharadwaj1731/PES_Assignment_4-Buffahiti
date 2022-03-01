/*
 * timers.h
 *
 *  Created on: 21-Feb-2022
 *      Author: prvbh
 */
//#define MAX_LOAD_VAL

#ifndef TIMERS_H_
#define TIMERS_H_

#endif /* TIMERS_H_ */

typedef uint32_t ticktime_t;

void Init_SysTick(void);

ticktime_t now();
ticktime_t get_timer();
void reset_timer(void);
void delay_ms(uint32_t);
