/*
 * Serial.cpp
 *
 *  Created on: 2016��1��18��
 *      Author: Romeli
 */
#include "Serial.h"
#include <stdlib.h>
#include <stdio.h>

SerialClass Serial;

volatile static char USART1_RX_Buf[256];				//���ջ�����
volatile static uint8_t USART1_RX_SP = 0;				//���ջ�����ָ��
volatile static uint8_t USART1_Read_SP = 0;				//��������ȡָ��
volatile static uint8_t USART1_Read_Available = 0;		//������δ���ֽ�
volatile static uint8_t USART1_Read_Frame = 0;			//��ȡ֡��׼

void SerialClass::begin(uint32_t BaudRate)
{
	/*�����ʼ���ýṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/*����GPIOA��USART1��ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,
			ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*����PA9Ϊ�����������*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

	/*����PA10Ϊ��������*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*���ڳ�ʼ���������ʣ�����1�����Զ����ƣ�˫��ֹͣλ��1λ���ֽڣ�8��*/
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);

	/*�����ж����ȼ�Ϊ��1  1λ��ռ 3λ��Ӧ*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;

	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		// ʹ�ܽ����ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);		// ʹ��֡�ж�
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
/*���ڻ��������жϳ���*/
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



