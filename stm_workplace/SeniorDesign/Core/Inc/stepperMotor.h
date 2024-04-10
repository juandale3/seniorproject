/*
 * stepperMotor.h
 *
 *  Created on: Feb 10, 2024
 *      Author: oneju
 */

#ifndef INC_STEPPERMOTOR_H_
#define INC_STEPPERMOTOR_H_



#endif /* INC_STEPPERMOTOR_H_ */


// defines
//#define MAX_STEP 1600
#define MAX_STEP 800

uint16_t toggleCount;			// counts pin toggling
uint16_t currPos;
uint16_t steps;

// Pins
// Steps
GPIO_TypeDef * pulGroup = GPIOC;
uint16_t pulPin = GPIO_PIN_2;

//Direction
GPIO_TypeDef * dirGroup = GPIOF;
uint16_t dirPin = GPIO_PIN_4;

//void stepperClose();
//void stepperOpen();
//void stepperAngle(uint8_t angle);

void stepperClose(){
	steps = MAX_STEP;
}

void stepperOpen(){
	steps = 0;
}

void stepperStep(uint16_t num){
	if(num > MAX_STEP){
		steps = MAX_STEP;
	}else{
	steps = num;
	}
}

void recalibrate(){
	currPos = MAX_STEP;
	stepperOpen();
}
