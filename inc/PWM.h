/*
 * PWM.h
 *	使用TIM2生成PWM 默认频率为17k 周期为4096
 *  Created on: 2016年4月30日
 *      Author: Romeli
 */

#ifndef PWM_H_
#define PWM_H_

#include <stm32f10x.h>

#define OC1EN 1
#define OC2EN 0
#define OC3EN 0
#define OC4EN 0

class PWMClass
{
public:
	void init(uint16_t Period, uint16_t Duty);
	void set(uint16_t Duty = 0);
	void set(uint8_t ch, uint16_t Duty);
private:
};

void TIM2_GPIO_Config();
void TIM2_Mode_Config(uint16_t Period, uint16_t Duty);

extern PWMClass PWM;
#endif /* PWM_H_ */
