/*
 * Serial.cpp
 *
 *  Created on: 2016年1月18日
 *      Author: Romeli
 */
#include "Serial.h"
#include <stdlib.h>
#include <stdio.h>

SerialClass Serial;

volatile static char USART1_RX_Buf[256];				//接收缓存器
volatile static uint8_t USART1_RX_SP = 0;				//接收缓存器指针
volatile static uint8_t USART1_Read_SP = 0;				//缓存器读取指针
volatile static uint8_t USART1_Read_Available = 0;		//缓冲器未读字节
volatile static uint8_t USART1_Read_Frame = 0;			//读取帧标准

void SerialClass::begin(uint32_t BaudRate)
{
	/*定义初始化用结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/*开启GPIOA和USART1的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,
			ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*设置PA9为复用推挽输出*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

	/*设置PA10为浮动输入*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*串口初始化，波特率：参数1；无自动控制；双向；停止位：1位；字节：8；*/
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);

	/*配置中断优先级为组1  1位抢占 3位响应*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;

	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		// 使能接收中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);		// 使能帧中断
}

void SerialClass::print(char *data)
{
	while (*data != '\0')
	{
		write(*data);
		data++;
	}
}
void SerialClass::print(long data)
{
	char str[20];
	sprintf(str, "%ld", data);
	print(str);
}
void SerialClass::print(double data, uint8_t ndigit)
{
	char str[20];
	char format[6] = "%.0lf";
	format[2] = 0x30 + ndigit;
	sprintf(str, format, data);
	print(str);
}

void SerialClass::write(char c)
{
	USART1->DR = (c & (uint16_t) 0x01FF);
	while (!(USART1->SR & USART_FLAG_TXE))
		;
}

uint8_t SerialClass::available()
{
	return USART1_Read_Available;
}

uint8_t SerialClass::checkFrame()
{
	if (USART1_Read_Frame != 0)
	{
		USART1_Read_Frame = 0;
		return 1;
	}
	else
		return 0;
}

char SerialClass::read()
{
	USART1_Read_Available--;
	return USART1_RX_Buf[USART1_Read_SP++];
}

void SerialClass::read(char* buf, uint8_t len)
{
	while (len--)
	{
		*++buf = read();
	}
}
/*串口缓冲器的中断程序*/
extern "C" void USART1_IRQHandler(void)
{
	if ((USART1->SR & USART_IT_IDLE) != (uint16_t) RESET)
	{
		USART1_Read_Frame = 1;
	}
	if ((USART1->SR & USART_FLAG_ORE) != (uint16_t) RESET)
		USART_ReceiveData(USART1);
	if ((USART1->SR & USART_IT_RXNE) != (uint16_t) RESET)
	{
		USART1_RX_Buf[USART1_RX_SP] = USART1->DR;
		USART1_RX_SP++;
		USART1_Read_Available++;
	}
}



