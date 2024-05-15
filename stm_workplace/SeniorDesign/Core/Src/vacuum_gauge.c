/*
 * vacuum_gauge.c
 *
 *  Created on: Feb 4, 2024
 *      Author: oneju
 */

// Includes
#include "main.h"
#include "math.h"

//

float desiredvacuum = 0.00399966;

float pressure;
float referencePressure = pow(10, -9.301295);
float referenceVoltage = 0.774;
float scalingFactor = 0.75;

// Static Functions Declaration	----------------------------------------//



// Public Functions		------------------------------------------------//
void vacuumGaugeADC(ADC_HandleTypeDef* hadc){
	ADC_ChannelConfTypeDef sConfig = {0};

	sConfig.Channel = ADC_CHANNEL_10;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;

	if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
	{
	  Error_Handler();
	}
	return;
}

float readVacuum(float voltage){

	float exponent = (voltage * 3.198 - referenceVoltage)/scalingFactor;
	pressure = referencePressure * pow(10, exponent);

	return pressure;
}







// Static Functions		------------------------------------------------//

