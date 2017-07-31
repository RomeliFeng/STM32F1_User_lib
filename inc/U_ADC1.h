/*
 * U_ADC1.h
 *
 *  Created on: 2016��11��23��
 *      Author: Romeli
 */

#ifndef U_ADC1_H_
#define U_ADC1_H_

#include "cmsis_device.h"

#define U_ADC_Max 4095

class U_ADC1 {
public:
	static WordtoByte_Typedef Data;

	static void Init();
	static void RegularChannelConfig(uint8_t ADC_Channel, uint8_t ADC_SampleTime);
	static void RefreshData();
	static void RefreshData(uint8_t ADC_Channel, uint8_t ADC_SampleTime);
	static void RefreshData(uint8_t ADC_Channel, uint8_t ADC_SampleTime,
			uint8_t OverLevel);
private:
	static void GPIOInit();
	static void ADCInit();
};

#endif /* U_ADC1_H_ */
