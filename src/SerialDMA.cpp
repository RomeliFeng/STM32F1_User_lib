/*
 * Serial.cpp
 *
 *  Created on: 2016年1月18日
 *      Author: Romeli
 */
#include "SerialDMA.h"

#include <stdlib.h>
#include <stdio.h>

SerialClass Serial;

#define USART1_RX_Frame_Size 128
#define USART1_TX_Buf_Size 128
#define USART1_RX_Buf_Size 128

volatile static char USART1_TX_Buf[USART1_TX_Buf_Size];				//发送缓冲器
volatile static uint8_t USART1_TX_SP = 0;				//发送缓冲器指针
volatile static char USART1_TX_Buf2[USART1_TX_Buf_Size];		//备用发送缓冲器
volatile static uint8_t USART1_TX_SP2 = 0;				//备用发送缓冲器指针
volatile static uint8_t USART1_TX_BUSY = 0;				//发送DMA器忙标志
volatile static uint8_t USART1_TX_CH = 1;//发送缓冲期通道		1:USART1_TX_Buf;2:USART1_TX_Buf2

volatile static char USART1_RX_DMA_Buf[USART1_RX_Frame_Size];	//DMA接收缓存器
volatile static char USART1_RX_Buf[USART1_RX_Buf_Size];				//接收缓存器
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
	DMA_InitTypeDef DMA_InitStructure;

	/*开启GPIOA和USART1的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,
			ENABLE);
	/*开启DMA1的时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/*DMA1 USART1 发送中断*/
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/*DMA1 USART1 接收中断*/
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	DMA_DeInit(DMA1_Channel4);
	/*设置外设地址和内存地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) USART1_TX_Buf;
	/*设置传输方向*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 10;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
//	DMA_Cmd(DMA1_Channel4, ENABLE);

	DMA_DeInit(DMA1_Channel5);
	/*设置外设地址和内存地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) USART1_RX_DMA_Buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = USART1_RX_Frame_Size;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel5, ENABLE);

	/*设置PA9为复用推挽输出*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*设置PA10为浮动输入*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
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

	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	USART_Cmd(USART1, ENABLE);

//	/*配置中断优先级为组1  1位抢占 3位响应*/
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;

	NVIC_Init(&NVIC_InitStructure);
//
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		// 使能接收中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);		// 使能帧中断
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

void SerialClass::print(char *data, uint8_t len)
{
	if (USART1_TX_BUSY)
	{
		switch (USART1_TX_CH)
		{
		case 1:
			while (len--)
			{
				USART1_TX_Buf2[USART1_TX_SP2++] = *data++;
			}
			break;
		case 2:
			while (len--)
			{
				USART1_TX_Buf[USART1_TX_SP++] = *data++;
			}
			break;
		default:
			break;
		}
	}
	else
	{
		while (len--)
		{
			USART1_TX_Buf[USART1_TX_SP++] = *data++;
		}
		USART1_TX_CH = 1;
		USART1_TX_BUSY = 1;
		DMA1_Channel4->CMAR = (uint32_t) USART1_TX_Buf;
		DMA_SetCurrDataCounter(DMA1_Channel4, USART1_TX_SP);
		DMA_Cmd(DMA1_Channel4, ENABLE);
	}
}

void SerialClass::write(char c)
{
	USART1->DR = (c & (uint16_t) 0x01FF);
	while (!(USART1->SR & USART_FLAG_TXE))
		;
}

char SerialClass::read()
{
	uint8_t data = USART1_RX_Buf[USART1_Read_SP++];
	if (USART1_Read_SP == USART1_RX_Buf_Size)
	{
		USART1_Read_SP = 0;
	}
	USART1_Read_Available--;
	return data;
}

void SerialClass::read(char *buf, uint8_t len)
{
	while (len--)
	{
		*++buf = read();
	}
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

uint8_t SerialClass::getlen(char* data)
{
	uint8_t len = 0;
	while (*data++ != '\0')
		len++;
	return len;
}

/*串口缓冲器的中断程序*/
extern "C" void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(DMA1_Channel5, DISABLE);
		USART_ReceiveData(USART1);
		uint8_t len = USART1_RX_Frame_Size - DMA1_Channel5->CNDTR;
		DMA1_Channel5->CNDTR = USART1_RX_Frame_Size;
		USART1_Read_Available += len;
		if (USART1_Read_Available >= USART1_RX_Buf_Size)
		{
			USART1_Read_Available -= USART1_RX_Buf_Size;
		}
		for (uint8_t i = 0; i < len; ++i)
		{
			USART1_RX_Buf[USART1_RX_SP++] = USART1_RX_DMA_Buf[i];
			if (USART1_RX_SP == USART1_RX_Buf_Size)
			{
				USART1_RX_SP = 0;
			}
		}
		USART1_Read_Frame = 1;
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
}

extern "C" void DMA1_Channel4_IRQHandler(void)
{
	DMA_ClearFlag(DMA1_FLAG_TC4);
	DMA_Cmd(DMA1_Channel4, DISABLE);
	switch (USART1_TX_CH)
	{
	case 1:
		USART1_TX_SP = 0;
		if (USART1_TX_SP2 != 0)
		{
			USART1_TX_CH = 2;
			DMA1_Channel4->CMAR = (uint32_t) USART1_TX_Buf2;
			DMA_SetCurrDataCounter(DMA1_Channel4, USART1_TX_SP2);
			DMA_Cmd(DMA1_Channel4, ENABLE);
		}
		else
		{
			USART1_TX_BUSY = 0;
		}
		break;
	case 2:
		USART1_TX_SP2 = 0;
		if (USART1_TX_SP != 0)
		{
			USART1_TX_CH = 1;
			DMA1_Channel4->CMAR = (uint32_t) USART1_TX_Buf;
			DMA_SetCurrDataCounter(DMA1_Channel4, USART1_TX_SP);
			DMA_Cmd(DMA1_Channel4, ENABLE);
		}
		else
		{
			USART1_TX_BUSY = 0;
		}
		break;
	default:
		break;
	}
}
