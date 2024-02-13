/*
 * state_machine.c
 *
 *  Created on: Feb 3, 2024
 *      Author: Manny Mort Jr
 */
#include <stdio.h>
#include <stdbool.h>
#include <main.h>

enum State {
	START,
	VAC_ACHIEVMENT_TEST,
	WARM_UP,
	LOAD_TEST,
	OPERATION_TEST,
	ULTIMATE_MEASURE_TEST,
	IDLE,
	FAIL_STATE,
	STOP


} eNextState;

enum State eNextState = START;
void state_machine() {
	bool error = false;
	switch(eNextState) {
		case START:
			//print start message
			printf("Starting Pump 1 \n");
			//send starting status message
			if(!error) {
				eNextState = FAIL_STATE;
			}
			break;
			eNextState = VAC_ACHIEVMENT_TEST;
		case VAC_ACHIEVMENT_TEST:
			printf("Vacuum Achievement Pump Test in progress \n");
			/*Turn Vacuum G*/
			if(!error) {
				eNextState = FAIL_STATE;
			}
			eNextState = WARM_UP;
			break;
		case WARM_UP:
			printf("Vacuum Pump warm Test \n");
			/*Continuously read temperature*/
			if(!error) {
				eNextState = FAIL_STATE;
			}
			eNextState = LOAD_TEST;
			break;
		case LOAD_TEST:
			printf("Load being added to the system \n");
			/*Read Temperature value*/
			if(!error) {
				eNextState = FAIL_STATE;
			}
			eNextState = OPERATION_TEST;
			break;
		case OPERATION_TEST:
			printf("Measure");
			if(!error) {
				eNextState = FAIL_STATE;
			}
			eNextState = ULTIMATE_MEASURE_TEST;
			break;
		case ULTIMATE_MEASURE_TEST:

			if(!error) {
				eNextState = FAIL_STATE;
			}
			eNextState = FAIL_STATE;
			break;
		case FAIL_STATE:



			if(!error) {
				eNextState = STOP;
			}
			break;
		case IDLE:
			break;
		case STOP:
			break;
		default:
			if(!error) {
				eNextState = STOP;
			}

	}
}
