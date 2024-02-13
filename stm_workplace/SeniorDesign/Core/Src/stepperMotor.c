/*
 * stepperMotor.c
 *
 *  Created on: Feb 10, 2024
 *      Author: oneju
 */


// Includes
#include "main.h"


// Variables
uint8_t angle;
uint8_t direction;

// Pins
static GPIO_TypeDef * pulGroup = GPIOA;
static uint16_t pulPin = GPIO_PIN_1;

static GPIO_TypeDef * dirGroup = GPIOA;
static uint16_t dirPin = GPIO_PIN_2;

// Static Functions Declaration	----------------------------------------//



// Public Functions		------------------------------------------------//

void stepperClose(){
}

void stepperOpen(){
}

void stepperAngle(uint8_t angle){
}
