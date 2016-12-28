/*
 * PWM.h
 *	使用TIM2生成PWM 默认频率为17k 周期为4096
 *  Created on: 2016年4月30日
 *      Author: Romeli
 */

#ifndef PWM_H_
#define PWM_H_

#include <stm32f10x.h>

#define OC1_EN
#ifdef OC1_EN
//OC模式定义
#define OC1_MODE_PWM1 1
//#define OC1_MODE_TIMING 1
//CC比较中断
//#define IT_CC1_EN
#endif

#define OC2_EN
#ifdef OC2_EN
//OC模式定义
#define OC2_MODE_PWM1 1
//#define OC2_MODE_TIMING 1
//CC比较中断
//#define IT_CC2_EN
#endif

//#define OC3_EN
#ifdef OC3_EN
//OC模式定义
#define OC3_MODE_PWM1 1
//#define OC3_MODE_TIMING 1
//CC比较中断
//#define IT_CC3_EN
#endif

//#define OC4_EN
#ifdef OC4_EN
//OC模式定义
#define OC4_MODE_PWM1 1
//#define OC4_MODE_TIMING 1
//CC比较中断
//#define IT_CC4_EN
#endif

#define IT_UPDATE_EN 1

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

void U_TIM2_GPIO_Config();
void U_TIM2_Mode_Config(uint16_t Period, uint16_t Duty);
void U_TIM2_NVIC_Config();

extern void U_TIM2_CC1_ISR();
extern void U_TIM2_CC2_ISR();
extern void U_TIM2_CC3_ISR();
extern void U_TIM2_CC4_ISR();
extern void U_TIM2_UPADATE_ISR();

extern PWMClass PWM;
#endif /* PWM_H_ */
