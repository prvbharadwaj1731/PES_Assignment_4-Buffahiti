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

color_code set_led_color(color_code);

color_code color_transition(color_code, color_code, uint8_t);

color_code crosswalk_pattern(color_code);

//void CROSSWALK_MODE(color_code, color_code);
