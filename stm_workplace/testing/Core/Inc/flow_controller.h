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

// ADC_HandleTypeDef

// Public functions
dac_handletypedef createDAC(DAC_HandleTypeDef *dac, uint32_t channel);

#endif /* INC_FLOW_CONTROLLER_H_ */
