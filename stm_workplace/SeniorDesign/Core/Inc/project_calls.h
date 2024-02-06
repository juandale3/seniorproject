/*
 * project_calls.h
 *
 *  Created on: Feb 4, 2024
 *      Author: oneju
 */

#ifndef INC_PROJECT_CALLS_H_
#define INC_PROJECT_CALLS_H_



// Public Functions Declarations
//float adcFiltered(ADC_HandleTypeDef *hadc1);
void dacSet(DAC_HandleTypeDef *dac, uint32_t channel, float volts);
float adcGet(ADC_HandleTypeDef *hadc1);						// Measures the ADC, Returns Volts
void printMsg(char * str, UART_HandleTypeDef *huart2);		// Prints char[] through Uart


#endif /* INC_PROJECT_CALLS_H_ */
