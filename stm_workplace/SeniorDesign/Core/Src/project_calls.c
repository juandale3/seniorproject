/*
 * project_calls.c
 *
 *  Created on: Feb 4, 2024
 *      Author: oneju
 */


// Includes
#include "main.h"
#include <string.h>	// used for UART printing
#include <math.h> 	// used for rounding

// Private variables	------------------------------------------------//
uint32_t dacBitVal;	// DAC outputs voltage = 12-bit value
uint16_t adcBitVal; // ADC returns voltage = 12-bit value
float adcVolts = 0;		// adcBitVal converted into volts
//const uint8_t ADCSAMPLESIZE = 5;

// Analog Variables



// Static Functions Declaration	----------------------------------------//



// Public Functions		------------------------------------------------//
void dacSet(DAC_HandleTypeDef *dac, uint32_t channel, float volts){
	HAL_DAC_Start(dac, channel);
	dacBitVal = (volts/3.3)*4095;
	HAL_DAC_SetValue(dac, channel, DAC_ALIGN_12B_R, dacBitVal);
}

float adcGet(ADC_HandleTypeDef *hadc1){
	HAL_ADC_Start(hadc1);
	HAL_ADC_PollForConversion(hadc1, 1000);
	adcBitVal = HAL_ADC_GetValue(hadc1);
	adcVolts = (float)adcBitVal/4095 * 3.3;
	HAL_ADC_Stop(hadc1);
	return adcVolts;
}

//float adcFiltered(ADC_HandleTypeDef *hadc1){
//	float arr[ADCSAMPLESIZE];
//	float temp;
//	int i, j,k;
//
//	// take 5 samples
//	for(i = 0; i < ADCSAMPLESIZE; i++){
//		arr[i] = adcGet(hadc1);
//		HAL_Delay(1);
//	}
//
//	// insertion sort
//	for(j = 1; j < ADCSAMPLESIZE; j++){
//		temp = arr[j];
//		k = j-1;
//
//		while(k>=0 && arr[k] > temp){
//			arr[k+1] = arr[k];
//			k = k -1;
//		}
//		arr[k+1] = temp;
//	}
//
//	// take the medium
//	//adcVolts =roundf(arr[2] *100.0)/100;
//	adcVolts = arr[2];
//	return adcVolts;
//}

void printMsg(char * str, UART_HandleTypeDef *huart2){
	// sprintf(msg, "Volts: %.2f V\r\n", volts);
	// HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
	HAL_UART_Transmit(huart2, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}
// Static Functions		------------------------------------------------//