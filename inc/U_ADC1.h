/*
 * U_ADC1.h
 *
 *  Created on: 2016��11��23��
 *      Author: Romeli
 */

#ifndef U_ADC1_H_
#define U_ADC1_H_

#include "cmsis_device.h"

extern uint16_t U_ADC1Data;

class U_ADC1Class {
public:
	U_ADC1Class() {
		ADCInit();
	}
	void RegularChannelConfig(uint8_t ADC_Channel, uint8_t ADC_SampleTime);
	void RefreshData();
	void RefreshData(uint8_t ADC_Channel, uint8_t ADC_SampleTime);
private:
	void ADCInit();
};

extern U_ADC1Class U_ADC1;

#endif /* U_ADC1_H_ */
