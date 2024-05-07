/*
 * project_calls.h
 *
 *  Created on: Feb 4, 2024
 *      Author: oneju
 */

#ifndef INC_PROJECT_CALLS_H_
#define INC_PROJECT_CALLS_H_


GPIO_TypeDef * greenLightGroup = GPIOD;
uint16_t greenLightPin = GPIO_PIN_5;

GPIO_TypeDef * redLightGroup = GPIOD;
uint16_t redLightPin = GPIO_PIN_6;
// Public Functions Declarations
//float adcFiltered(ADC_HandleTypeDef *hadc1);
void dacSet(DAC_HandleTypeDef *dac, uint32_t channel, float volts);
float adcGet(ADC_HandleTypeDef *hadc1);						// Measures the ADC, Returns Volts
void printMsg(char * str, UART_HandleTypeDef *huart2);		// Prints char[] through Uart
//void listenMsg(UART_HandleTypeDef *huart2);		// This might need to be an interrupt
void listenMsg(char* buffer,UART_HandleTypeDef *huart);
//void solenoidOpen(GPIO_TypeDef * solenoidGroup, uint16_t solenoidPin);
//void solenoidClose(GPIO_TypeDef * solenoidGroup, uint16_t solenoidPin);
void solenoidOneOpen();
void solenoidTwoOpen();


#endif /* INC_PROJECT_CALLS_H_ */
