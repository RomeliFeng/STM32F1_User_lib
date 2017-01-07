/*
 * U_ADC1.cpp
 *
 *  Created on: 2016Äê11ÔÂ23ÈÕ
 *      Author: Romeli
 */

#include "U_ADC1.h"

U_ADC1Class U_ADC1;

uint16_t U_ADC1Data = 0;

void U_ADC1Class::RefreshData(uint8_t ADC_Channel, uint8_t ADC_SampleTime) {
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
		;
	U_ADC1Data = ADC1->DR;
}

void U_ADC1Class::ADCModeInit() {
	ADC_InitTypeDef ADC_InitStructure;

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_1Cycles5);

	ADC_Cmd(ADC1, ENABLE);

	ADC_TempSensorVrefintCmd(ENABLE);

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1))
		;

	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1))
		;

//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void U_ADC1Class::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = 0;
#ifdef U_ADC1_Ch0
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_0;
#endif
#ifdef U_ADC1_Ch1
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_1;
#endif
#ifdef U_ADC1_Ch2
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_2;
#endif
#ifdef U_ADC1_Ch3
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_3;
#endif
#ifdef U_ADC1_Ch4
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_4;
#endif
#ifdef U_ADC1_Ch5
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_5;
#endif
#ifdef U_ADC1_Ch6
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_6;
#endif
#ifdef U_ADC1_Ch7
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_7;
#endif

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
