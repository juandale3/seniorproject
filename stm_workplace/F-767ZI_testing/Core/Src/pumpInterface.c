/*
 * pumpInterface.c
 *
 *  Created on: Mar 27, 2024
 *      Author: oneju
 */


#include "main.h"
#include "pumpInterface.h"
#include <string.h>

#define TRANSMIT_SIZE 	16
#define STX				0x02	// Start of TX
#define ETX				0x03	// End of TX
#define CR				0x0D	// End of Frame
#define NIBBLE_MERGE(MSN,LSN)	((MSN<<4)|(LSN))

uint8_t tx_frame[TRANSMIT_SIZE];


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
    tx_frame[11] = convertToAscii(0x01);
    // End of TX
    tx_frame[12] = ETX;

    //checksum
    pumpChecksum();

    // End of Frame
    tx_frame[15] = CR;

	HAL_UART_Transmit(huart, &tx_frame[0], TRANSMIT_SIZE, HAL_MAX_DELAY);
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