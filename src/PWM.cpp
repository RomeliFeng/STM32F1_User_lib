/*
 * PWM.cpp
 *
 *  Created on: 2016年4月30日
 *      Author: Romeli
 */
#include "PWM.h"

PWMClass PWM;

void PWMClass::Init(uint16_t Period = 4095, uint16_t Duty = 0) {
	U_TIM2_GPIO_Config();
	U_TIM2_NVIC_Config();
	U_TIM2_Mode_Config(Period, Duty);
}

void PWMClass::SetDuty(uint16_t Duty) {
	for (uint8_t i = 1; i < 5; i++) {
		SetDuty((PWMCh_Typedef) i, Duty);
	}
}

void PWMClass::SetDuty(PWMCh_Typedef ch, uint16_t Duty) {
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

void PWMClass::SetPolarity(PWMPolarity_Typedef polarity) {
	for (uint8_t i = 1; i < 5; i++) {
		SetPolarity((PWMCh_Typedef) i, polarity);
	}
}

void PWMClass::SetPolarity(PWMCh_Typedef ch, PWMPolarity_Typedef polarity) {
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

void PWMClass::SetClockDiv(PWMClockDiv_Typedef div) {
	TIM_SetClockDivision(TIM2, div);
}

void PWMClass::U_TIM2_GPIO_Config() {
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

#if (OC1_MODE_PWM1 | OC2_MODE_PWM1 | OC3_MODE_PWM1 | OC4_MODE_PWM1)
	//开启GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//配置PA0-PA3；复用推挽输出；最快速度50M；
	GPIO_InitStructure.GPIO_Pin = 0;
#ifdef OC1_MODE_PWM1
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_0;
#endif
#ifdef OC2_MODE_PWM1
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_1;
#endif
#ifdef OC3_MODE_PWM1
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_2;
#endif
#ifdef OC4_MODE_PWM1
	GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_3;
#endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
}

void PWMClass::U_TIM2_Mode_Config(uint16_t Period, uint16_t Duty) {
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
#ifdef OC1_EN
#if OC1_MODE_PWM1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
#elif OC1_MODE_TIMING
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
#endif
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif
	//使能通道2
#ifdef OC2_EN
#if OC2_MODE_PWM1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
#elif OC2_MODE_TIMING
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
#endif
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif
	//使能通道3
#ifdef OC3_EN
#if OC3_MODE_PWM1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
#elif OC3_MODE_TIMING
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
#endif
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif
	//使能通道4
#ifdef OC4_EN
#if OC4_MODE_PWM1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
#elif OC4_MODE_TIMING
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
#endif
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif

	//使能TIM2重载寄存器
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	//使能定时器2
	TIM_Cmd(TIM2, ENABLE);
}

void PWMClass::SetPrescaler(uint16_t pre) {
	TIM_PrescalerConfig(TIM2, pre, TIM_PSCReloadMode_Update);
}

void PWMClass::U_TIM2_NVIC_Config() {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;

	NVIC_Init(&NVIC_InitStructure);
}

void PWMClass::SwitchInterrupt(PWMCh_Typedef ch, FunctionalState NewState) {
	switch (ch) {
	case PWMCh_1:
#ifdef OC1_EN
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		TIM_ITConfig(TIM2, TIM_IT_CC1, NewState);
#endif
		break;
	case PWMCh_2:
#ifdef OC2_EN
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		TIM_ITConfig(TIM2, TIM_IT_CC2, NewState);
#endif
		break;
	case PWMCh_3:
#ifdef OC3_EN
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
		TIM_ITConfig(TIM2, TIM_IT_CC3, NewState);
#endif
		break;
	case PWMCh_4:
#ifdef OC4_EN
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
		TIM_ITConfig(TIM2, TIM_IT_CC4, NewState);
#endif
		break;
	}
}

extern "C" void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		U_TIM2_UPADATE_ISR();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
		U_TIM2_CC1_ISR();
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET) {
		U_TIM2_CC2_ISR();
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
	}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET) {
		U_TIM2_CC3_ISR();
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
	}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET) {
		U_TIM2_CC4_ISR();
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
	}
}
//Default tim_it_cc handler
void __attribute__((weak)) U_TIM2_CC1_ISR() {
	return;
}
void __attribute__((weak)) U_TIM2_CC2_ISR() {
	return;
}
void __attribute__((weak)) U_TIM2_CC3_ISR() {
	return;
}
void __attribute__((weak)) U_TIM2_CC4_ISR() {
	return;
}
void __attribute__((weak)) U_TIM2_UPADATE_ISR() {
	return;
}

