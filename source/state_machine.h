/*
 * state_machine.h
 *
 *  Created on: 01-Mar-2022
 *      Author: prvbh
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_



#endif /* STATE_MACHINE_H_ */

typedef enum{
	STOP,
	STOP_TO_GO_TRANSITION,
	GO,
	GO_TO_WARNING_TRANSITION,
	WARNING,
	WARNING_TO_STOP_TRANSITION,
	CROSSWALK_TRANSITION,
	CROSSWALK,
	CROSSWALK_TO_GO_TRANSITION
}state;

color_code state_machine(state, uint8_t, color_code);
