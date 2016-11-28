/*
 * PWM.h
 *	使用TIM2生成PWM 默认频率为17k 周期为4096
 *  Created on: 2016年4月30日
 *      Author: Romeli
 */

#ifndef PWM_H_
#define PWM_H_

#include <stm32f10x.h>

//#define OC1_EN
//#define OC2_EN
#define OC3_EN
#define OC4_EN
//#define IT_CC1_EN
//#define IT_CC2_EN
//#define IT_CC3_EN
//#define IT_CC4_EN
#define IT_UPDATE_EN

typedef enum _PWMCh {
	PWMCh_1, PWMCh_2, PWMCh_3, PWMCh_4,
} PWMCh;
typedef enum _PWMPolarity {
	PWMPolarity_High = TIM_OCPolarity_High, PWMPolarity_Low = TIM_OCPolarity_Low
} PWMPolarity;
typedef enum _PWMClockDiv {
	PWMClockDiv_1 = TIM_CKD_DIV1,
	PWMClockDiv_2 = TIM_CKD_DIV2,
	PWMClockDiv_4 = TIM_CKD_DIV4
} PWMClockDiv;

class PWMClass {
public:
	void Init(uint16_t Period, uint16_t Duty);
	void SetDuty(uint16_t Duty = 0);
	void SetDuty(PWMCh ch, uint16_t Duty);
	void SetPolarity(PWMPolarity polarity);
	void SetPolarity(PWMCh ch, PWMPolarity polarity);
	void SetClockDiv(PWMClockDiv div);
	void SetPrescaler(uint16_t pre);
private:
};

void TIM2_GPIO_Config();
void TIM2_Mode_Config(uint16_t Period, uint16_t Duty);
void TIM2_NVIC_Config();

extern PWMClass PWM;
#endif /* PWM_H_ */
