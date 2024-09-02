/*
 * GAS_ADC.c
 *
 *  Created on: Aug 28, 2024
 *      Author: Me_llamo_Show
 */

/*incluudes_start*/
#include "GAS_ADC.h"
#include <stdlib.h>
#include <stdio.h>
/*includes_end*/



struct GAS_ADC GAS_ADC_1;
struct GAS_ADC_SENSOR GAS_ADC_SENSOR_1;

static void GAS_Adc_initErrorTrap(void);
static void GAS_Adc_startAdc(struct GAS_ADC *ADC);
static int Error_sum(int);

void GAS_ADC_init(void){
#ifdef __USE_ADC1__

		struct GAS_ADC *adc1 = &GAS_ADC_1;
		adc1->hadc = &hadc1;
		adc1->dataLength = ((adc1->hadc->Instance->SQR1)>>20U);
		adc1->data = (uint32_t*)malloc(adc1->dataLength*sizeof(uint32_t));

	    adc1->hadc->DMA_Handle->Init.Mode = DMA_CIRCULAR;
	    HAL_DMA_Init(adc1->hadc->DMA_Handle);

		GAS_Adc_startAdc(adc1);
#endif
}

static void GAS_Adc_startAdc(struct GAS_ADC *ADC){
	if(HAL_ADC_Start_DMA(ADC->hadc, ADC->data, ADC->dataLength) != HAL_OK)
	{
		GAS_Adc_initErrorTrap();
	}
}


static void GAS_Adc_initErrorTrap(void){
	__disable_irq();
	while(1);
}

void GAS_Adc_getValue(void){
#ifdef __USE_ADC1__
	GAS_ADC_SENSOR_1.ADC1_IN1 = GAS_ADC_1.data[0];
	GAS_ADC_SENSOR_1.ADC1_IN2 = GAS_ADC_1.data[1];
	GAS_ADC_SENSOR_1.ADC1_IN3 = GAS_ADC_1.data[2];
	GAS_ADC_SENSOR_1.ADC1_IN4 = GAS_ADC_1.data[3];
#endif
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        for (int i = 0; i < 4; i++)
        {
        	int ADC1_Errornum[4];
            if (GAS_ADC_1.data[i] < 0 || GAS_ADC_1.data[i] > 4095) // 12비트 ADC의 경우 0~4095 범위
            {
            	ADC1_Errornum[i] = 1;
            }
            if (i == 4 && ADC1_Errornum[0] == 1){
            	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
            }
            if (i == 4 && ADC1_Errornum[1] == 1){
            	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
            }
            if (i == 4 && ADC1_Errornum[2] == 1){
            	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
            }
            if (i == 4 && ADC1_Errornum[3] == 1){
            	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
            }
            if (i == 4 && ADC1_Errornum[0] != 1){
            	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
            }
            if (i == 4 && ADC1_Errornum[1] != 1){
            	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
            }
            if (i == 4 && ADC1_Errornum[2] != 1){
            	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
            }
            if (i == 4 && ADC1_Errornum[3] != 1){
            	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
            }

        }
    }
}
