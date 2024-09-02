/*
 * GAS_ADC.h
 *
 *  Created on: Aug 28, 2024
 *      Author: Me_llamo_Show
 */

#ifndef SRC_GAS_ADC_H_
#define SRC_GAS_ADC_H_

#include <stdint.h>
#include "stm32f334x8.h"
#include "stm32f3xx_hal.h"
#include "adc.h"

struct GAS_ADC{
	ADC_HandleTypeDef *hadc;
	uint32_t dataLength;
	volatile uint16_t *data[4];
};

struct GAS_ADC_SENSOR{
#ifdef __USE_ADC1__
	uint16_t ADC1_IN1;
	uint16_t ADC1_IN2;
	uint16_t ADC1_IN3;
	uint16_t ADC1_IN4;
#endif
};

void GAS_Adc_init(void);

extern struct GAS_ADC GAS_ADC_1;
extern struct GAS_ADC_SENSOR GAS_ADC_SENSOR_1;


#endif /* SRC_GAS_ADC_H_ */
