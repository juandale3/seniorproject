/*
 * flow_controller.h
 *
 *  Created on: Dec 17, 2023
 *      Author: oneju
 */

#ifndef INC_FLOW_CONTROLLER_H_
#define INC_FLOW_CONTROLLER_H_

// Public functions
void dacSet(DAC_HandleTypeDef *dac, uint32_t channel, float volts);
float adcGet(ADC_HandleTypeDef *hadc1);						// Measures the ADC, Returns Volts
float adcFiltered(ADC_HandleTypeDef *hadc1);
void printMsg(char * str, UART_HandleTypeDef *huart2);		// Prints char[] through Uart
float readFlow(ADC_HandleTypeDef *hadc1);					// Reads the flow rate and returns L/min
void flowRateMethod(uint8_t method);						// Changes Control Method: Digital = 0, Analog = 1;
void flowRateSP(uint8_t setPoint);							// If Control Method == 0, then this changes between Set Points

#endif /* INC_FLOW_CONTROLLER_H_ */
