/*
 * flow_controller.h
 *
 *  Created on: Dec 17, 2023
 *      Author: oneju
 */

#ifndef INC_FLOW_CONTROLLER_H_
#define INC_FLOW_CONTROLLER_H_

// DAC_HandleTypeDef
typedef struct{
	DAC_HandleTypeDef *dac;
	uint32_t channel;
}dac_handletypedef;


// Public functions
dac_handletypedef createDAC(DAC_HandleTypeDef *dac, uint32_t channel);

void dacSet(dac_handletypedef *dac, float volts);
float adcGet(ADC_HandleTypeDef *hadc1);
void printMsg(char * str, UART_HandleTypeDef *huart2);

#endif /* INC_FLOW_CONTROLLER_H_ */
