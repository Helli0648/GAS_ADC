/*
 * GAS_CAN.c
 *
 *  Created on: Sep 2, 2024
 *      Author: Me_llamo_Show
 */

#include "GAS_BuildConfig.h"
#include "stm32f3xx_hal.h"
#include "GAS_CAN.h"
#include "can.h"

CAN_FilterTypeDef canfilter1;
CAN_RxHeaderTypeDef canRxHeader;
uint8_t can1Rx0Data[8];
uint32_t TxMailBox;
uint8_t can1Tx0Data[8];

#ifdef __USE_ADC1__
CAN_TxHeaderTypeDef canTxHeader_ADC1;
CAN_ADC_t CAN_ADC1;
#endif

#ifdef __USE_ADC1__
uint32_t STM32_msgADC1_ID = 0x00334F01;
#endif

void GAS_Can_txSetting(void);
void CAN_Error_Handler(void);
void CAN_Error_LED(void);

void GAS_Can_init(void)
{
	GAS_Can_txSetting();
	if (HAL_CAN_Start(&hcan) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
		Error_Handler();
	}
}

void GAS_Can_txSetting(void)
{
#ifdef __USE_ADC1__
	canTxHeader_ADC1.ExtId = STM32_msgADC1_ID;
	canTxHeader_ADC1.IDE = CAN_ID_EXT;
	canTxHeader_ADC1.RTR = CAN_RTR_DATA;
	canTxHeader_ADC1.DLC = 8;
#endif
}

void GAS_Can_sendMessage(void)
{
#ifdef __USE_ADC1__
	stm32_msgADC1.B.IN1 = GAS_ADC_SENSOR.ADC1_IN1;
	stm32_msgADC1.B.IN2 = GAS_ADC_SENSOR.ADC1_IN2;
	stm32_msgADC1.B.IN3 = GAS_ADC_SENSOR.ADC1_IN3;
	stm32_msgADC1.B.IN4 = GAS_ADC_SENSOR.ADC1_IN4;
	TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
	HAL_CAN_AddTxMessage(&hcan, &canTxHeader_ADC1, &stm32_msgADC1.TxData[0], &TxMailBox);
	/*if(TxMailBox > 0) {
		HAL_GPIO_WritePin(GPIOB, LED02_Pin, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOB, LED02_Pin, GPIO_PIN_RESET);
	}*/
#endif
}

void CAN_Error_init(void){
    HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_EWG);
    HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_EPV);
    HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_BOF);
    HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_STF);
    HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_FOR);
    HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_ACK);
    HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_BR);
    HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_BD);
    HAL_CAN_CLEAR_FLAG(hcan, CAN_FLAG_CRC);
}

void CAN_Error_Handler(void){
	uint32_t Error = HAL_CAN_GetState(&hcan);
	int can_error;
    if (error & HAL_CAN_ERROR_EWG) {
        // Error Warning
    	can_error = 1;
    }
    else if (error & HAL_CAN_ERROR_EPV) {
        // Error Passive
    	can_error = 2;
    }
    else if (error & HAL_CAN_ERROR_BOF) {
        // Bus-Off
    	can_error = 3;
    }
    else if (error & HAL_CAN_ERROR_STF) {
        // Stuff Error
    	can_error = 4;
    }
    else if (error & HAL_CAN_ERROR_FOR) {
        // Form Error
    	can_error = 5;
    }
    else if (error & HAL_CAN_ERROR_ACK) {
        // Acknowledgment Error
    	can_error = 6;
    }
    else if (error & HAL_CAN_ERROR_BR) {
        // Bit Recessive Error
    	can_error = 7;
    }
    else if (error & HAL_CAN_ERROR_BD) {
        // Bit Dominant Error
    	can_error = 8;
    }
    else if (error & HAL_CAN_ERROR_CRC) {
        // CRC Error
    	can_error = 9;
    }
    else{
    can_error = 0}
}

void CAN_Error_LED_init(void){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
}

void CAN_Error_LED(void){
	if(can_error = 1){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	}
	else if(can_error = 2){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
	}
	else if(can_error = 3){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9 | GPIO_PIN_10, GPIO_PIN_SET);
	}
	else if(can_error = 4){
		while(1){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
		}
	}
	else if(can_error = 5){
		while(1){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		}
	}
	else if(can_error = 6){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		while(1){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		}
	}
	else if(can_error = 7){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		while(1){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
		}
	}
	else if(can_error = 8){
		while(1){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		}
	}
	else if(can_error = 9){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
	}
	else{
		while(1){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_Delay(1500);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		}
	}
}









