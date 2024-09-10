/*
 * GAS_CAN.h
 *
 *  Created on: Sep 5, 2024
 *      Author: Me_llamo_Show
 */

#ifndef SRC_GAS_CAN_H_
#define SRC_GAS_CAN_H_

#include "GAS_BuildConfig.h"
#include "stm32f3xx_hal.h"
#include "can.h"

extern CAN_FilterTypeDef canfilter1;
extern CAN_RxHeaderTypeDef canRxHeader;
extern uint8_t can1Rx0Data[8];
extern uint32_t TxMailBox;
extern uint8_t can1Tx0Data[8];
/*
extern uint32_t STM32_msgADC1_IN1_ID;
extern uint32_t STM32_msgADC1_IN2_ID;
extern uint32_t STM32_msgADC1_IN3_ID;
extern uint32_t STM32_msgADC1_IN4_ID;
*/
typedef union{
	uint8_t TxData[8];
	struct{
		uint16_t IN1;
		uint16_t IN2;
		uint16_t IN3;
		uint16_t IN4;
	}__attribute__((aligned(1),packed)) ADC_IN;
}CAN_ADC_t;

#ifdef __USE_ADC1__
extern CAN_ADC_t CAN_ADC1;
#endif

extern uint32_t STM32_msgADC1_ID;


extern void GAS_Can_init(void);
extern void GAS_Can_sendMessage();
#endif /* SRC_GETANDSEND_H_ */

