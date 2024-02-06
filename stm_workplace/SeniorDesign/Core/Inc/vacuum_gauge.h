/*
 * vacuum_gauge.h
 *
 *  Created on: Feb 4, 2024
 *      Author: oneju
 */

#ifndef INC_VACUUM_GAUGE_H_
#define INC_VACUUM_GAUGE_H_


// Public Functions Declarations

void vacuumGaugeADC(ADC_HandleTypeDef* hadc);
float readVacuum(float voltage);

#endif /* INC_VACUUM_GAUGE_H_ */
