/*
 * PWM.cpp
 *
 *  Created on: 2016��4��30��
 *      Author: Romeli
 */
#include "PWM.h"
#include "diag/Trace.h"
#include "Delay.h"
#include "HardwareSerial.h"

PWMClass PWM;

void PWMClass::Init(uint16_t Period = 4095, uint16_t Duty = 0) {
	TIM2_GPIO_Config();
	TIM2_NVIC_Config();
	TIM2_Mode_Config(Period, Duty);
}

void PWMClass::SetDuty(uint16_t Duty) {
	for (uint8_t i = 1; i < 5; i++) {
		SetDuty((PWMCh) i, Duty);
	}
}

void PWMClass::SetDuty(PWMCh ch, uint16_t Duty) {
	switch (ch) {
	case PWMCh_1:
#ifdef OC1_EN
		TIM_SetCompare1(TIM2, Duty);

#endif
		break;
	case PWMCh_2:
#ifdef OC2_EN
		TIM_SetCompare2(TIM2, Duty);
#endif
		break;
	case PWMCh_3:
#ifdef OC3_EN
		TIM_SetCompare3(TIM2, Duty);
#endif
		break;
	case PWMCh_4:
#ifdef OC4_EN
		TIM_SetCompare4(TIM2, Duty);
#endif
		break;
	}
}

void PWMClass::SetPolarity(PWMPolarity polarity) {
	for (uint8_t i = 1; i < 5; i++) {
		SetPolarity((PWMCh) i, polarity);
	}
}

void PWMClass::SetPolarity(PWMCh ch, PWMPolarity polarity) {
	switch (ch) {
	case PWMCh_1:
#ifdef OC1_EN
		TIM_OC1PolarityConfig(TIM2, polarity);
#endif
		break;
	case PWMCh_2:
#ifdef OC2_EN
		TIM_OC2PolarityConfig(TIM2, polarity);
#endif
		break;
	case PWMCh_3:
#ifdef OC3_EN
		TIM_OC3PolarityConfig(TIM2, polarity);
#endif
		break;
	case PWMCh_4:
#ifdef OC4_EN
		TIM_OC4PolarityConfig(TIM2, polarity);
#endif
		break;
	}
}

void PWMClass::SetClockDiv(PWMClockDiv div) {
	TIM_SetClockDivision(TIM2, div);
}

void TIM2_GPIO_Config() {
	GPIO_InitTypeDef GPIO_InitStructure;
	//����TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//����GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//����PA0-PA3�������������������ٶ�50M��
	GPIO_InitStructure.GPIO_Pin = 0;
#ifdef OC1_EN
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_0;
#endif
#ifdef OC2_EN
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_1;
#endif
#ifdef OC3_EN
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_2;
#endif
#ifdef OC4_EN
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
#ifdef OC1_EN
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
#ifdef IT_CC1_EN
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
#endif
#endif
	//ʹ��ͨ��2
#ifdef OC2_EN
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
#ifdef IT_CC2_EN
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
#endif
#endif
	//ʹ��ͨ��3
#ifdef OC3_EN
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
#ifdef IT_CC3_EN
	TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);
#endif
#endif
	//ʹ��ͨ��4
#ifdef OC4_EN
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
#ifdef IT_CC4_EN
	TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);
#endif
#endif

#ifdef IT_UPDATE_EN
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
#endif
	//ʹ��TIM2���ؼĴ���
	TIM_ARRPreloadConfig(TIM2, ENABLE);

	//ʹ�ܶ�ʱ��2
	TIM_Cmd(TIM2, ENABLE);
}

void PWMClass::SetPrescaler(uint16_t pre) {
	TIM_PrescalerConfig(TIM2, pre, TIM_PSCReloadMode_Update);
}

void TIM2_NVIC_Config() {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;

	NVIC_Init(&NVIC_InitStructure);
}

extern "C" void TIM2_IRQHandler(void) {
	static uint64_t t = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		t = micros();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET) {
		Serial.println((long) (micros() - t));

		TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
	}
}
