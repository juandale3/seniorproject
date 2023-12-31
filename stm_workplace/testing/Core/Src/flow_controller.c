/*
 * flow_controller.c
 *
 *  Created on: Dec 17, 2023
 *      Author: Juan Lopez
 */

#include "main.h"
#include "flow_controller.h"

uint32_t dacBitVal;

// Static Functions Declaration
static void dacSet(dac_handletypedef *dac, float volts);

// Public Functions
dac_handletypedef createDAC(DAC_HandleTypeDef *dac, uint32_t channel){
	dac_handletypedef output;
	output.dac = dac;
	output.channel = channel;
	return output;
}

// Static Functions
static void dacSet(dac_handletypedef *dac, float volts){
	dacBitVal = (volts/3.3)*4095;
	HAL_DAC_SetValue(dac->dac, dac->channel, DAC_ALIGN_12B_R, dacBitVal);
}
