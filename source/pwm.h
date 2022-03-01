/*
 * pwm.h
 *
 *  Created on: 27-Feb-2022
 *      Author: prvbh
 */

#ifndef PWM_H_
#define PWM_H_



#endif /* PWM_H_ */

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}color_code;

void pwm_init(uint16_t);

void SET_LED_COLOR(color_code, uint16_t);

void COLOR_TRANSITION(color_code, color_code);

void CROSSWALK_MODE(color_code, color_code);
