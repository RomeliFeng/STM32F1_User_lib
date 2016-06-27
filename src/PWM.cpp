/*
 * PWM.cpp
 *
 *  Created on: 2016��4��30��
 *      Author: Romeli
 */
#include "PWM.h"

PWM::PWM()
{

}
void PWM::init(uint16_t Period = 4095, uint16_t Duty = 0)
{
	TIM2_GPIO_Config();
	TIM2_Mode_Config(Period, Duty);
}
void PWM::set(uint16_t Duty)
{
	for(uint8_t i=1;i<5;i++)
	{
		set(i,Duty);
	}
}
void PWM::set(uint8_t ch, uint16_t Duty)
{
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	//����ģʽΪPWM1
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//	//��������ֵΪռ�ձ�
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse = Duty;
//	//����PWM��ʼΪ�ߵ�ƽ�������Ϊ�͵�ƽ
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	switch (ch)
	{
	case 1:
		TIM_SetCompare1(TIM2,Duty);
//		TIM_OC1Init(TIM2, &TIM_OCInitStructure);
		break;
	case 2:
		TIM_SetCompare2(TIM2,Duty);
//		TIM_OC2Init(TIM2, &TIM_OCInitStructure);
		break;
	case 3:
		TIM_SetCompare3(TIM2,Duty);
//		TIM_OC3Init(TIM2, &TIM_OCInitStructure);
		break;
	case 4:
		TIM_SetCompare4(TIM2,Duty);
//		TIM_OC4Init(TIM2, &TIM_OCInitStructure);
		break;

	}
}
void TIM2_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//����TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//����GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//����PA0-PA3�������������������ٶ�50M��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2
			| GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void TIM2_Mode_Config(uint16_t Period, uint16_t Duty)
{
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
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	//ʹ��ͨ��2
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	//ʹ��ͨ��3
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	//ʹ��ͨ��4
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

	//ʹ��TIM2���ؼĴ���
	TIM_ARRPreloadConfig(TIM2, ENABLE);

	//ʹ�ܶ�ʱ��2
	TIM_Cmd(TIM2, ENABLE);
}
