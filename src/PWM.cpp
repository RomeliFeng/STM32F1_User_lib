/*
 * PWM.cpp
 *
 *  Created on: 2016��4��30��
 *      Author: Romeli
 */
#include "PWM.h"

PWMClass PWM;

void PWMClass::Init(uint16_t Period = 4095, uint16_t Duty = 0) {
	TIM2_GPIO_Config();
	TIM2_Mode_Config(Period, Duty);
}
void PWMClass::Set(uint16_t Duty) {
	for (uint8_t i = 1; i < 5; i++) {
		Set((PWMCh) i, Duty);
	}
}
void PWMClass::Set(PWMCh ch, uint16_t Duty) {
	switch (ch) {
	case PWMCh1:
#if OC1EN
		TIM_SetCompare1(TIM2, Duty);
		break;
#endif
	case PWMCh2:
#if OC2EN
		TIM_SetCompare2(TIM2, Duty);
#endif
		break;
	case PWMCh3:
#if OC3EN
		TIM_SetCompare3(TIM2, Duty);
#endif
		break;
	case PWMCh4:
#if OC4EN
		TIM_SetCompare4(TIM2, Duty);
#endif
		break;
	}
}
void TIM2_GPIO_Config() {
	GPIO_InitTypeDef GPIO_InitStructure;
	//����TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//����GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//����PA0-PA3�������������������ٶ�50M��
	GPIO_InitStructure.GPIO_Pin = 0;
#if OC1EN
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_0;
#endif
#if OC2EN
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_1;
#endif
#if OC3EN
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_2;
#endif
#if OC4EN
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_3;
#endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void TIM2_Mode_Config(uint16_t Period, uint16_t Duty) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	//��ʼ��TIM
	//���������ֵ
	TIM_TimeBaseStructure.TIM_Period = Period;
	//Ԥ��Ƶϵ��������Ƶ
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	//ʱ�ӷ�Ƶϵ��������Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//��ʼ��TIM2
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	//����ģʽΪPWM1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//��������ֵΪռ�ձ�
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Duty;
	//����PWM��ʼΪ�ߵ�ƽ�������Ϊ�͵�ƽ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	//Ĭ��ռ�ձ�һ�¹ʲ�����ռ�ձ��޸�
	//ʹ��ͨ��1
#if OC1EN
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif
	//ʹ��ͨ��2
#if OC2EN
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif
	//ʹ��ͨ��3
#if OC3EN
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif
	//ʹ��ͨ��4
#if OC4EN
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif

	//ʹ��TIM2���ؼĴ���
	TIM_ARRPreloadConfig(TIM2, ENABLE);

	//ʹ�ܶ�ʱ��2
	TIM_Cmd(TIM2, ENABLE);
}
