/*
 * PWM.h
 *	ʹ��TIM2����PWM Ĭ��Ƶ��Ϊ17k ����Ϊ4096
 *  Created on: 2016��4��30��
 *      Author: Romeli
 */

#ifndef PWM_H_
#define PWM_H_

#include <stm32f10x.h>

class PWM
{
public:
	PWM();
	static void init(uint16_t Period, uint16_t Duty);
	static void set(uint16_t Duty = 0);
	static void set(uint8_t ch, uint16_t Duty);
private:
};

void TIM2_GPIO_Config();
void TIM2_Mode_Config(uint16_t Period, uint16_t Duty);

#endif /* PWM_H_ */
