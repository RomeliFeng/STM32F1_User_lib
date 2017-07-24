/*
 * U_DAC.cpp
 *
 *  Created on: 2017年7月20日
 *      Author: Romeli
 */

#include "U_DAC.h"

void U_DAC::Init() {
	GPIOInit();
	DACInit();
}

void U_DAC::RefreshData(float vol) {
	uint16_t data;

	data = (uint16_t) ((vol / 3.3) * 4096);
	RefreshData(data);
}

void U_DAC::RefreshData(uint16_t data) {
	DAC_SetChannel1Data(DAC_Align_12b_R, data & 0xfff);
	DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
}

void U_DAC::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void U_DAC::DACInit() {
	DAC_InitTypeDef DAC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	DAC_Cmd(DAC_Channel_1, ENABLE);
}
