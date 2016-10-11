/*
 * HardwareSerial.cpp
 *
 *  Created on: 2016年1月18日
 *      Author: Romeli
 */
#include "HardwareSerial.h"

#include <stdlib.h>
#include <stdio.h>

SerialClass Serial;

volatile static char USART1_TX_Buf[USART1_TX_Buf_Size];				//发送缓冲器
volatile static uint8_t USART1_TX_SP = 0;				//发送缓冲器指针

volatile static char USART1_RX_Buf[USART1_RX_Buf_Size];				//接收缓存器
volatile static uint8_t USART1_RX_SP = 0;				//接收缓存器指针

volatile static uint8_t USART1_Read_SP = 0;				//缓存器读取指针
volatile static uint8_t USART1_Read_Frame = 0;			//读取帧标准

#ifdef USE_DMA
volatile static char USART1_TX_Buf2[USART1_TX_Buf_Size];		//备用发送缓冲器
volatile static uint8_t USART1_TX_SP2 = 0;		//备用发送缓冲器指针
volatile static uint8_t USART1_TX_BUSY = 0;		//发送DMA器忙标志
volatile static uint8_t USART1_TX_CH = 1;//发送缓冲期通道		1:USART1_TX_Buf;2:USART1_TX_Buf2
volatile static char USART1_RX_DMA_Buf[USART1_RX_Frame_Size];		//DMA接收缓存器
#endif

void SerialClass::begin(uint32_t BaudRate) {
	/*定义初始化用结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	/*开启GPIOA和USART1的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,
			ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

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

#ifdef USE_DMA
	/*开启DMA1的时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
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

	/*开启串口DMA发送和接收*/
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
#endif

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;

	NVIC_Init(&NVIC_InitStructure);

#ifndef USE_DMA
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		// 使能接收中断
#endif
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);		// 使能帧中断

	USART_Cmd(USART1, ENABLE);
}

void SerialClass::print(long data, uint8_t base) {
	char str[20];
	byNumber(data, base, str);
	print(str);
}
void SerialClass::print(double data, uint8_t ndigit) {
	char str[20];
	byFloat(data, ndigit, str);
	print(str);
}

void SerialClass::print(char *data, uint8_t len) {
#ifdef USE_DMA
	if (USART1_TX_BUSY) {				//判断DMA是否在使用中
		switch (USART1_TX_CH) {			//判断正在使用的缓冲区
		case 1:
			while (len--) {
				USART1_TX_Buf2[USART1_TX_SP2++] = *data++;	//缓冲区1正在发送中，填充缓冲区2
			}
			break;
		case 2:
			while (len--) {
				USART1_TX_Buf[USART1_TX_SP++] = *data++;	//缓冲区2正在发送中，填充缓冲区1
			}
			break;
		default:
			break;
		}
	} else {		//DMA空闲填充缓冲区1并发送
		while (len--) {
			USART1_TX_Buf[USART1_TX_SP++] = *data++;
		}
		USART1_TX_CH = 1;
		USART1_TX_BUSY = 1;
		DMA1_Channel4->CMAR = (uint32_t) USART1_TX_Buf;		//更改DMA外设地址
		DMA1_Channel4->CNDTR = USART1_TX_SP;				//传入发送字节个数
		DMA_Cmd(DMA1_Channel4, ENABLE);
	}
#else
	while (*data != '\0') {
		write(*data++);
	}
#endif
}

void SerialClass::write(char c) {
#ifdef USE_DMA
	print(&c, 1);
#else
	USART1->DR = (c & (uint16_t) 0x01FF);			//发送一个字节并等待发送完成
	while (!(USART1->SR & USART_FLAG_TXE))
	;
#endif
}

inline char SerialClass::peek() {
	return USART1_RX_Buf[USART1_Read_SP];		//取出数据
}

char SerialClass::peekNextDigit(bool detectDecimal) {
	char data = peek();
	if (data == '-') {
		ReadSPInc();
		return data;
	} else if (data >= '0' && data <= '9') {	//is digit
		ReadSPInc();
		return data;
	} else if (data == '.' && detectDecimal) {
		ReadSPInc();
		return data;
	} else
		return -1;
}

char SerialClass::read() {
	char data = peek();
	ReadSPInc();
	return data;
}

void SerialClass::read(char *buf, uint8_t len) {
	while (len--) {
		*buf++ = read();
	}
}

long SerialClass::nextInt() {
	long data = 0;
	bool isNeg = false;
	char c;
	while (available() > 0) {
		c = peekNextDigit();
		if (c == -1) {
			break;
		} else if (c == '-') {
			if (isNeg) {
				ReadSPDec();
				break;
			}

			else {
				isNeg = true;
				continue;
			}
		}
		data = data * 10 + c - '0';
	}
	return isNeg ? -data : data;
}

double SerialClass::nextFloat() {
	double data = 0.0;
	double frac = 1.0;
	bool isNeg = false;
	bool isFra = false;
	char c;
	while (available() > 0) {
		c = peekNextDigit(true);
		if (c == -1) {
			break;
		} else if (c == '-') {
			if (isNeg) {
				ReadSPDec();
				break;
			}

			else {
				isNeg = true;
				continue;
			}
		} else if (c == '.') {
			if (isFra) {
				ReadSPDec();
				break;
			} else {
				isFra = true;
				continue;
			}
		}
		if (isFra) {
			frac *= 0.1;
		}
		data = data * 10 + c - '0';

	}
	data = isNeg ? -data : data;
	data = isFra ? data * frac : data;
	return data;
}

/*返回未读取字节个数*/
uint8_t SerialClass::available() {
	return USART1_RX_SP >= USART1_Read_SP ? USART1_RX_SP - USART1_Read_SP :
	USART1_RX_Buf_Size - USART1_Read_SP + USART1_RX_SP;
}
/*一帧数据检测标志*/
uint8_t SerialClass::checkFrame() {
	if (USART1_Read_Frame != 0) {
		USART1_Read_Frame = 0;
		return 1;
	} else
		return 0;
}

void SerialClass::flush() {
	USART1_Read_SP = USART1_RX_SP;
}

uint8_t SerialClass::getlen(char* data) {
	uint8_t len = 0;
	while (*data++ != '\0')
		len++;
	return len;
}
inline void SerialClass::ReadSPInc() {
	if (USART1_Read_SP != USART1_RX_SP)				//防止读取越界
		++USART1_Read_SP;
	if (USART1_Read_SP == USART1_RX_Buf_Size) {		//指针归零
		USART1_Read_SP = 0;
	}
}

inline void SerialClass::ReadSPDec() {
	if (USART1_Read_SP == 0) {		//指针归零
		USART1_Read_SP = USART1_RX_Buf_Size;
	}
}
/*串口缓冲器的中断程序*/
extern "C" void USART1_IRQHandler(void) {
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
		USART1_Read_Frame = 1;
		USART_ReceiveData(USART1);
#ifdef USE_DMA
		DMA_Cmd(DMA1_Channel5, DISABLE);			//接收到1帧数据，从DMA保存到缓冲区
		USART_ReceiveData(USART1);
		uint8_t len = USART1_RX_Frame_Size - DMA1_Channel5->CNDTR;
		DMA1_Channel5->CNDTR = USART1_RX_Frame_Size;
		for (uint8_t i = 0; i < len; ++i) {
			USART1_RX_Buf[USART1_RX_SP++] = USART1_RX_DMA_Buf[i];
			if (USART1_RX_SP == USART1_RX_Buf_Size) {
				USART1_RX_SP = 0;
			}
		}
		DMA_Cmd(DMA1_Channel5, ENABLE);
#endif
	}
#ifndef USE_DMA
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART1_RX_Buf[USART1_RX_SP] = USART1->DR;
		USART1_RX_SP++;
		if (USART1_RX_SP == USART1_RX_Buf_Size)
		USART1_RX_SP = 0;
	}
#endif
	if ((USART1->SR & USART_FLAG_ORE) != (uint16_t) RESET)
		USART_ReceiveData(USART1);
}

#ifdef USE_DMA
extern "C" void DMA1_Channel4_IRQHandler(void) {
	DMA_ClearFlag(DMA1_FLAG_TC4);
	DMA_Cmd(DMA1_Channel4, DISABLE);
	switch (USART1_TX_CH) {
	case 1:
		USART1_TX_SP = 0;
		if (USART1_TX_SP2 != 0) {
			USART1_TX_CH = 2;
			DMA1_Channel4->CMAR = (uint32_t) USART1_TX_Buf2;
			DMA1_Channel4->CNDTR = USART1_TX_SP2;
			DMA_Cmd(DMA1_Channel4, ENABLE);
		} else {
			USART1_TX_BUSY = 0;
		}
		break;
	case 2:
		USART1_TX_SP2 = 0;
		if (USART1_TX_SP != 0) {
			USART1_TX_CH = 1;
			DMA1_Channel4->CMAR = (uint32_t) USART1_TX_Buf;
			DMA1_Channel4->CNDTR = USART1_TX_SP;
			DMA_Cmd(DMA1_Channel4, ENABLE);
		} else {
			USART1_TX_BUSY = 0;
		}
		break;
	default:
		break;
	}
}
#endif
