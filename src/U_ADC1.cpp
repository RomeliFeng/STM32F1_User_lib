/*
 * U_ADC1.cpp
 *
 *  Created on: 2016��11��23��
 *      Author: Romeli
 */

#include "U_ADC1.h"

WordtoByte_Typedef U_ADC1::Data = { 0 };

void U_ADC1::Init() {
	GPIOInit();
	ADCInit();
}

void U_ADC1::RegularChannelConfig(uint8_t ADC_Channel, uint8_t ADC_SampleTime) {
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime);
}

uint16_t U_ADC1::RefreshData() {
	ADC1->CR2 |= ((uint32_t) 0x00500000);
	while ((ADC1->SR & ADC_FLAG_EOC) == (uint8_t) RESET)
		;
	return Data.word = ADC1->DR;
}

uint16_t U_ADC1::RefreshData(uint8_t ADC_Channel, uint8_t ADC_SampleTime) {
	RegularChannelConfig(ADC_Channel, ADC_SampleTime);
	return RefreshData();
}

uint16_t U_ADC1::RefreshData(uint8_t ADC_Channel, uint8_t ADC_SampleTime,
		uint8_t OverLevel) {
	uint32_t sum = 0;
	RegularChannelConfig(ADC_Channel, ADC_SampleTime);
	for (uint16_t i = 0; i < (1 << OverLevel << OverLevel); ++i) {
		RefreshData();
		sum += Data.word;
	}
	return Data.word = sum >> OverLevel;
}

void U_ADC1::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void U_ADC1::ADCInit() {
	ADC_InitTypeDef ADC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_239Cycles5);

	ADC_Cmd(ADC1, ENABLE);

	ADC_TempSensorVrefintCmd(ENABLE);

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1))
		;

	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1))
		;
}
