/*
 * PWM.cpp
 *
 *  Created on: 2016年4月30日
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
	//开启TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//开启GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//配置PA0-PA3；复用推挽输出；最快速度50M；
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
	//初始化TIM
	//计数溢出数值
	TIM_TimeBaseStructure.TIM_Period = Period;
	//预分频系数：不分频
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	//时钟分频系数：不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//初始化TIM2
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	//配置模式为PWM1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//设置跳变值为占空比
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Duty;
	//设置PWM初始为高电平，跳变后为低电平
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	//默认占空比一致故不进行占空比修改
	//使能通道1
#if OC1EN
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif
	//使能通道2
#if OC2EN
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif
	//使能通道3
#if OC3EN
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif
	//使能通道4
#if OC4EN
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif

	//使能TIM2重载寄存器
	TIM_ARRPreloadConfig(TIM2, ENABLE);

	//使能定时器2
	TIM_Cmd(TIM2, ENABLE);
}
