/*
 * pumpInterface.h
 *
 *  Created on: Mar 27, 2024
 *      Author: oneju
 */

#ifndef PUMPINTERFACE_H_
#define PUMPINTERFACE_H_

void accessPumpData(UART_HandleTypeDef* huart);
char convertToAscii(char character);
void pumpChecksum();
#endif /* PUMPINTERFACE_H_ */
