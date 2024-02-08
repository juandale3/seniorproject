/*
 * flow_controller.c
 *
 *  Created on: Dec 17, 2023
 *      Author: Juan Lopez
 */

#include "main.h"
#include "flow_controller.h"

#include <string.h>	// used for UART printing
#include <math.h> 	// used for rounding

// Private variables	------------------------------------------------//

// Analog Variables
uint32_t dacBitVal;	// DAC outputs voltage = 12-bit value
uint16_t adcBitVal; // ADC returns voltage = 12-bit value
float adcVolts = 0;		// adcBitVal converted into volts
const uint8_t ADCSAMPLESIZE = 5;
float instFlow = 0;


// Pins
// PA0, input pin DI1
GPIO_InitTypeDef FlowMethodPin = {GPIO_PIN_1, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0};
GPIO_TypeDef * FlowMethodGroup = GPIOA;
// PA1, input pin DI2
GPIO_InitTypeDef ContactDI2Pin = {GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0};
GPIO_TypeDef * ContactDI2Group = GPIOA;
// PA2, input pin DI3
GPIO_InitTypeDef ContactDI3Pin = {GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0};
GPIO_TypeDef * ContactDI3Group = GPIOA;


// Static Functions Declaration	----------------------------------------//


// Public Functions		------------------------------------------------//
dac_handletypedef createDAC(DAC_HandleTypeDef *dac, uint32_t channel)
/*
 * Creates user-defined data type to be able to control the DAC
 * Pass the DAC_HandleTypeDef and channel as args
 */
{
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


// testing this
/*
void dacSet(DAC_HandleTypeDef *dac, uint32_t channel, float volts){
	HAL_DAC_Start(dac, channel);
	dacBitVal = (volts/3.3)*4095;
	HAL_DAC_SetValue(dac, channel, DAC_ALIGN_12B_R, dacBitVal);
}*/


float adcGet(ADC_HandleTypeDef *hadc1){
	HAL_ADC_Start(hadc1);
	HAL_ADC_PollForConversion(hadc1, 100);
	adcBitVal = HAL_ADC_GetValue(hadc1);
	adcVolts = (float)adcBitVal/4095 * 3.3;
	return adcVolts;
}

float adcFiltered(ADC_HandleTypeDef *hadc1){
	float arr[ADCSAMPLESIZE];
	float temp;
	int i, j,k;

	// take 5 samples
	for(i = 0; i < ADCSAMPLESIZE; i++){
		arr[i] = adcGet(hadc1);
		HAL_Delay(1);
	}

	// insertion sort
	for(j = 1; j < ADCSAMPLESIZE; j++){
		temp = arr[j];
		k = j-1;

		while(k>=0 && arr[k] > temp){
			arr[k+1] = arr[k];
			k = k -1;
		}
		arr[k+1] = temp;
	}

	// take the medium
	//adcVolts =roundf(arr[2] *100.0)/100;
	adcVolts = arr[2];
	return adcVolts;
}

void printMsg(char * str, UART_HandleTypeDef *huart2){
	// sprintf(msg, "Volts: %.2f V\r\n", volts);
	// HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
	HAL_UART_Transmit(huart2, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}

float readFlow(ADC_HandleTypeDef *hadc1)
{
	// 5-0V 10 L/min=250 mV
	// voltage divider  R2=10k, R1=5.1k
	// .75 = 0, 0.88 = 10}
	adcFiltered(hadc1);
	// instFlow = adcVolts * (10.0+5.1)/10.0 / 0.025;	// 0.025V = 1 L/min
	float prev = instFlow;
	instFlow = (adcVolts - 0.57)/  0.013;
	if(fabs(instFlow - prev) < 0.7){
		instFlow = prev;
	}
	instFlow = roundf(instFlow);
	return instFlow;
}

void flowRateMethod(uint8_t method){

	// This is PA_0
	// 0 = "Tri-State"
	// 1 = "Gnd"
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	if(method == 0){
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	} else{
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	}
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	// This must change


	/*
	// This needs to be tested
	if(method == 0){
		FlowMethodPin.Mode = GPIO_MODE_INPUT;
	} else{
		FlowMethodPin.Mode = GPIO_MODE_OUTPUT_PP;
	}
	HAL_GPIO_Init(FlowMethodGroup, &FlowMethodPin);
*/
}

void flowRateSP(uint8_t setPoint){
	// test this function
	switch(setPoint){
	case(0):
			ContactDI2Pin.Mode = GPIO_MODE_INPUT;
			ContactDI3Pin.Mode = GPIO_MODE_INPUT;
			HAL_GPIO_Init(ContactDI2Group, &ContactDI2Pin);
			HAL_GPIO_Init(ContactDI3Group, &ContactDI3Pin);
			break;
	case(1):
			ContactDI2Pin.Mode = GPIO_MODE_OUTPUT_PP;;
			ContactDI3Pin.Mode = GPIO_MODE_INPUT;
			HAL_GPIO_Init(ContactDI2Group, &ContactDI2Pin);
			HAL_GPIO_Init(ContactDI3Group, &ContactDI3Pin);
			break;
	case(2):
			ContactDI2Pin.Mode = GPIO_MODE_INPUT;
			ContactDI3Pin.Mode = GPIO_MODE_OUTPUT_PP;;
			HAL_GPIO_Init(ContactDI2Group, &ContactDI2Pin);
			HAL_GPIO_Init(ContactDI3Group, &ContactDI3Pin);
			break;
	case(3):
			ContactDI2Pin.Mode = GPIO_MODE_OUTPUT_PP;
			ContactDI3Pin.Mode = GPIO_MODE_OUTPUT_PP;
			HAL_GPIO_Init(ContactDI2Group, &ContactDI2Pin);
			HAL_GPIO_Init(ContactDI3Group, &ContactDI3Pin);
			break;
	default:
			ContactDI2Pin.Mode = GPIO_MODE_INPUT;
			ContactDI3Pin.Mode = GPIO_MODE_INPUT;
			HAL_GPIO_Init(ContactDI2Group, &ContactDI2Pin);
			HAL_GPIO_Init(ContactDI3Group, &ContactDI3Pin);
			break;
	}
}


// Static Functions		------------------------------------------------//
