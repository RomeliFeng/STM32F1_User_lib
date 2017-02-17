/*
 * U_ADC1.h
 *
 *  Created on: 2016Äê11ÔÂ23ÈÕ
 *      Author: Romeli
 */

#ifndef U_ADC1_H_
#define U_ADC1_H_

#include "cmsis_device.h"

#define U_ADC1_Ch0
//#define U_ADC1_Ch1
//#define U_ADC1_Ch2
#define U_ADC1_Ch3
#define U_ADC1_Ch4
#define U_ADC1_Ch5
#define U_ADC1_Ch6
#define U_ADC1_Ch7

extern uint16_t U_ADC1Data;

class U_ADC1Class {
public:
	U_ADC1Class() {
		GPIOInit();
		ADCModeInit();
	}
	void RefreshData();
	void RefreshData(uint8_t ADC_Channel, uint8_t ADC_SampleTime);
private:
	void GPIOInit();
	void ADCModeInit();
};

extern U_ADC1Class U_ADC1;

#endif /* U_ADC1_H_ */
