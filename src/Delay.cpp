/*
 * Delay.cpp
 *
 *  Created on: 2016Äê2ÔÂ6ÈÕ
 *      Author: Romeli
 */

#include "Delay.h"
#include "diag/Trace.h"

SystickClass Systick;

static volatile uint64_t nTime_now = 0;
uint64_t nTime_last;


SystickClass::SystickClass()
{
	SysTick_Init();
}

void SysTick_Init()
{
	SysTick_Config((SystemCoreClock / 1000000) - 5);	//Set SysTick timer=us
//	SysTick->CTRL&=SysTick_CTRL_ENABLE_Msk;			//Disable SysTick
}

void Delay_us(uint32_t us)
{
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;			//Ensable SysTick
	nTime_last = nTime_now;							//Record time_now
	while ((nTime_now - nTime_last) < us)
		;				//keep loop when in delay time
}

uint32_t millis()
{
	return nTime_now / 1000;
}

uint64_t micros()
{
	return nTime_now;
}

extern "C" void SysTick_Handler()
{
	nTime_now++;
}

