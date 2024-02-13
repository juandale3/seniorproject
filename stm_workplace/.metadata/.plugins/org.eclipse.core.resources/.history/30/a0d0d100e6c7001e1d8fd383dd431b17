/*
 * flow_controller.h
 *
 *  Created on: Dec 17, 2023
 *      Author: oneju
 */

#ifndef INC_FLOW_CONTROLLER_H_
#define INC_FLOW_CONTROLLER_H_

// Public Functions Declarations

void flowControllerADC(ADC_HandleTypeDef* hadc);
float adcFiltered(ADC_HandleTypeDef* hadc1);
float readFlow(float voltage);
//float setFlowRate(float inputVoltage, uint8_t targetFlowRate);
float setFlowRate(uint8_t targetFlowRate);
void flowRateMethod(uint8_t method);						// Changes Control Method: Digital = 0, Analog = 1;
void flowRateSP(uint8_t setPoint);							// If Control Method == 0, then this changes between Set Points

#endif /* INC_FLOW_CONTROLLER_H_ */
