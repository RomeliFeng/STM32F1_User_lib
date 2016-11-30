/*
 * U_ADC1.h
 *
 *  Created on: 2016Äê11ÔÂ23ÈÕ
 *      Author: Romeli
 */

#ifndef U_ADC1_H_
#define U_ADC1_H_

#include "stm32f10x.h"

class U_ADC1Class {
public:
	void Init();
};

void U_ADC1_GPIO_Init();

extern U_ADC1Class U_ADC1;

#endif /* U_ADC1_H_ */
