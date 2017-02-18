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
//#define OC1_MODE_PWM1 1
#define OC1_MODE_TIMING 1
//CC比较中断
#define IT_CC1_EN
#endif

#define OC2_EN
#ifdef OC2_EN
//OC模式定义
#define OC2_MODE_PWM1 1
//#define OC2_MODE_TIMING 1
//CC比较中断
//#define IT_CC2_EN
#endif

#define OC3_EN
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

//#define IT_UPDATE_EN 1

typedef enum _PWMCh_Typedef {
	PWMCh_1, PWMCh_2, PWMCh_3, PWMCh_4,
} PWMCh_Typedef;
typedef enum _PWMPolarity_Typedef {
	PWMPolarity_High = TIM_OCPolarity_High, PWMPolarity_Low = TIM_OCPolarity_Low
} PWMPolarity_Typedef;
typedef enum _PWMClockDiv_Typedef {
	PWMClockDiv_1 = TIM_CKD_DIV1,
	PWMClockDiv_2 = TIM_CKD_DIV2,
	PWMClockDiv_4 = TIM_CKD_DIV4
} PWMClockDiv_Typedef;

class PWMClass {
public:
	void Init(uint16_t Period, uint16_t Duty);
	void SetDuty(uint16_t Duty = 0);
	void SetDuty(PWMCh_Typedef ch, uint16_t Duty);
	void SetPolarity(PWMPolarity_Typedef polarity);
	void SetPolarity(PWMCh_Typedef ch, PWMPolarity_Typedef polarity);
	void SetClockDiv(PWMClockDiv_Typedef div);
	void SetPrescaler(uint16_t pre);
	void SwitchInterrupt(PWMCh_Typedef ch, FunctionalState NewState);
private:
	void GPIOInit();
	void TIMInit(uint16_t Period, uint16_t Duty);
	void NVICInit();
};

extern void U_TIM2_CC1_ISR();
extern void U_TIM2_CC2_ISR();
extern void U_TIM2_CC3_ISR();
extern void U_TIM2_CC4_ISR();
extern void U_TIM2_UPADATE_ISR();

extern PWMClass PWM;
#endif /* PWM_H_ */
