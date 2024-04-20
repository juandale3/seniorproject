/*
 * pumpInterface.h
 *
 *  Created on: Mar 27, 2024
 *      Author: oneju
 */

#ifndef PUMPINTERFACE_H_
#define PUMPINTERFACE_H_

enum SensorCode{
    RUNTIME,
    BPPOWER,
    MPPOWER,
    BPMOTORSPEED,
    MPMOTORSPEED,
    BPCURRENT,
    MPCURRENT,
    BPCASTINGTEMP,
    MPCASTINGTEMP,			// 9&10 RESERVED
    COOLINGWATERFLOW = 11,
    PUMPN2FLOW,				// 13 = RESERVED
    BACKPRESSURE1 = 14,
    HEATER1,
    HEATER2,
    HEATER3,
    HEATER4,
    VACUUMPRESSURE,
    COOLER1,
    COOLER2,
    COOLER3
};

void accessPumpData(UART_HandleTypeDef* huart);
char convertToAscii(char character);
void pumpChecksum();
uint8_t interpreter(uint8_t * rx_frame);
#endif /* PUMPINTERFACE_H_ */
