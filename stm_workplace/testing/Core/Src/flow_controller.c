/*
 * flow_controller.c
 *
 *  Created on: Dec 17, 2023
 *      Author: Juan Lopez
 */

#include "main.h"
#include "flow_controller.h"

#include <string.h>

uint32_t dacBitVal;
uint16_t adcBitVal;
float adcVolts;


// Static Functions Declaration
//static void dacSet(dac_handletypedef *dac, float volts);

// Public Functions
dac_handletypedef createDAC(DAC_HandleTypeDef *dac, uint32_t channel){
	HAL_DAC_Start(dac, channel);
	dac_handletypedef output;
	output.dac = dac;
	output.channel = channel;
	return output;
}
void dacSet(dac_handletypedef *dac, float volts){
	dacBitVal = (volts/3.3)*4095;
	HAL_DAC_SetValue(dac->dac, dac->channel, DAC_ALIGN_12B_R, dacBitVal);
}

float adcGet(ADC_HandleTypeDef *hadc1){
	HAL_ADC_Start(hadc1);
	HAL_ADC_PollForConversion(hadc1, 20);
	adcBitVal = HAL_ADC_GetValue(hadc1);
	adcVolts = (float)adcBitVal/4095 * 3.3;
	return adcVolts;
}

void printMsg(char * str, UART_HandleTypeDef *huart2){
	// sprintf(msg, "Light: %hu \r\n", lux);
	// sprintf(msg, "Volts: %.2f V\r\n", volts);
	// HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
	HAL_UART_Transmit(huart2, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}

// Static Functions

