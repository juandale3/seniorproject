/*
 * pumpInterface.c
 *
 *  Created on: Mar 27, 2024
 *      Author: oneju
 */


#include "main.h"
#include "pumpInterface.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TRANSMIT_SIZE 	16
#define RECEIVE_SIZE	14
#define END_MESSAGE_SIZE 8
#define SENSOR_MAX		22		// Max num of sensory data
#define STX				0x02	// Start of TX
#define ETX				0x03	// End of TX
#define CR				0x0D	// End of Frame
#define NIBBLE_MERGE(MSN,LSN)	((MSN<<4)|(LSN))

uint8_t tx_frame[TRANSMIT_SIZE];
uint8_t rx_frame[RECEIVE_SIZE];
float analogData[SENSOR_MAX];
uint8_t frameTotal;

void accessPumpData(UART_HandleTypeDef* huart){
	 // Initialize tx_frame with Start of TX (STX)
	tx_frame[0] = STX;

    // TEXT
	// Analog data read command is M20
    memcpy(&tx_frame[1], "M20", 3);	// Assign 'M', '2', '0' at indices 1, 2, 3 using memcpy
    // Analog data acquisition bits
    tx_frame[4] = '0';
    tx_frame[5] = '0';
    tx_frame[6] = '0';
    tx_frame[7] = '0';
    tx_frame[8] = '0';
    tx_frame[9] = '0';
    //tx_frame[10] = '9';
    //tx_frame[11] = convertToAscii(0x0E);
    tx_frame[10] = '0';
    tx_frame[11] = convertToAscii(0x03);
    // End of TX
    tx_frame[12] = ETX;

    //checksum
    pumpChecksum();

    // End of Frame
    tx_frame[15] = CR;

    frameTotal = 2;
    uint8_t count = 0;
	HAL_UART_Transmit(huart, &tx_frame[0], TRANSMIT_SIZE, HAL_MAX_DELAY);
	while(count < frameTotal){
		HAL_UART_Receive(huart, &rx_frame[0], RECEIVE_SIZE, HAL_MAX_DELAY);
		interpreter(rx_frame);
		count++;
	}
	HAL_UART_Receive(huart, &rx_frame[0], END_MESSAGE_SIZE, HAL_MAX_DELAY);
	return;
}

char convertToAscii(char character){
	if((uint8_t) character <= 0x0F){
		return	character + 0x30;
	}else{
		return	character;
	}
}

void pumpChecksum(){
	uint16_t sum = 0;
	for(int i = 0; i <= 12; i++){
		sum +=tx_frame[i];
	}
	tx_frame[13] = convertToAscii((sum>>4)&0x000F);
	tx_frame[14] = convertToAscii(sum&0x000F);

	return;
}
uint8_t interpreter(uint8_t * rx_frame){
	// uint8_t endOfMessage = 0;	// 1 = end of message
	uint8_t ReadCode;

	char ReadStr[3];
	char analogDataStr[8];
	strncpy(ReadStr,(char *)&rx_frame[1], 2);
	ReadStr[2] = '\0';

	ReadCode = atoi(ReadStr);
	strncpy(analogDataStr, (char *)&rx_frame[3],7);
	analogDataStr[7] = '\0';

	if(ReadCode > 22 || ReadCode < 0){
		return 1;
	}

	sscanf(analogDataStr, "%f", &analogData[ReadCode]);

//	switch(analogReadCode){
//	case(0):	// Total Running Time in Hr
////			strncpy(analogDataStr, (char *)&rx_frame[3],7);
////			analogDataStr[7] = '\0';
//			sscanf(analogDataStr, "%f", &runTime);
//			break;
//	case(1):	// BP power in KW
////			strncpy(analogDataStr, (char *)&rx_frame[3],7);
////			analogDataStr[7] = '\0';
//			sscanf(analogDataStr, "%f", &bpPower);
//			break;
//	case(2):	// MP power in KW
//			sscanf(analogDataStr, "%f", &mpPower);
//			break;
//	case(3):	// BP motor speed
//			sscanf(analogDataStr, "%f", &mpPower);
//			break;
//	case(4):	// MP motor speed
//					sscanf(analogDataStr, "%f", &mpPower);
//			break;
//	case(5):	// BP current
//					sscanf(analogDataStr, "%f", &mpPower);
//			break;
//	case(6):	// MP current
//					sscanf(analogDataStr, "%f", &mpPower);
//			break;
//	case(7):	// BP casing temp.
//					sscanf(analogDataStr, "%f", &mpPower);
//			break;
//	case(8):	// MP casing temp.
//					sscanf(analogDataStr, "%f", &mpPower);
//			break;
//	case(11):	// Cooling water flow
//					sscanf(analogDataStr, "%f", &mpPower);
//			break;
//	case(12):	// Pump N2 flow
//					sscanf(analogDataStr, "%f", &mpPower);
//			break;
//	case(14):	// Back pressure 1
//			break;
//	case(15):	// Heater1
//			break;
//	case(16):	// Heater2
//			break;
//	case(17):	// Heater3
//			break;
//	case(18):	// Heater4
//			break;
//	case(19):	// Vacuum pressure
//			break;
//	case(20):	// Cooler 1
//			break;
//	case(21):	// Cooler 2
//			break;
//	case(22):	// Cooler 3
//			break;
//	default:	// An Error or End of Message
//		return 1;
//	}
	return 0;

}