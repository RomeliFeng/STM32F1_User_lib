/*
 * HardwareSerial.cpp
 *
 *  Created on: 2016��1��18��
 *      Author: Romeli
 */
#include "HardwareSerial.h"

#include <stdlib.h>
#include <stdio.h>

SerialClass Serial;

volatile static char USART1_TX_Buf[USART1_TX_Buf_Size];				//���ͻ�����
volatile static uint8_t USART1_TX_SP = 0;				//���ͻ�����ָ��

volatile static char USART1_RX_Buf[USART1_RX_Buf_Size];				//���ջ�����
volatile static uint8_t USART1_RX_SP = 0;				//���ջ�����ָ��

volatile static uint8_t USART1_Read_SP = 0;				//��������ȡָ��
volatile static uint8_t USART1_Read_Frame = 0;			//��ȡ֡��׼

#ifdef USE_DMA
volatile static char USART1_TX_Buf2[USART1_TX_Buf_Size];		//���÷��ͻ�����
volatile static uint8_t USART1_TX_SP2 = 0;		//���÷��ͻ�����ָ��
volatile static uint8_t USART1_TX_BUSY = 0;		//����DMA��æ��־
volatile static uint8_t USART1_TX_CH = 1;//���ͻ�����ͨ��		1:USART1_TX_Buf;2:USART1_TX_Buf2
volatile static char USART1_RX_DMA_Buf[USART1_RX_Frame_Size];		//DMA���ջ�����
#endif

void SerialClass::begin(uint32_t BaudRate) {
	/*�����ʼ���ýṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	/*����GPIOA��USART1��ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,
			ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/*����PA9Ϊ�����������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*����PA10Ϊ��������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
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

#ifdef USE_DMA
	/*����DMA1��ʱ��*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/*DMA1 USART1 �����ж�*/
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/*DMA1 USART1 �����ж�*/
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	DMA_DeInit(DMA1_Channel4);
	/*���������ַ���ڴ��ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) USART1_TX_Buf;
	/*���ô��䷽��*/
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
	/*���������ַ���ڴ��ַ*/
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

	/*��������DMA���ͺͽ���*/
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
#endif

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;

	NVIC_Init(&NVIC_InitStructure);

#ifndef USE_DMA
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		// ʹ�ܽ����ж�
#endif
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);		// ʹ��֡�ж�

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
	if (USART1_TX_BUSY) {				//�ж�DMA�Ƿ���ʹ����
		switch (USART1_TX_CH) {			//�ж�����ʹ�õĻ�����
		case 1:
			while (len--) {
				USART1_TX_Buf2[USART1_TX_SP2++] = *data++;	//������1���ڷ����У���仺����2
			}
			break;
		case 2:
			while (len--) {
				USART1_TX_Buf[USART1_TX_SP++] = *data++;	//������2���ڷ����У���仺����1
			}
			break;
		default:
			break;
		}
	} else {		//DMA������仺����1������
		while (len--) {
			USART1_TX_Buf[USART1_TX_SP++] = *data++;
		}
		USART1_TX_CH = 1;
		USART1_TX_BUSY = 1;
		DMA1_Channel4->CMAR = (uint32_t) USART1_TX_Buf;		//����DMA�����ַ
		DMA1_Channel4->CNDTR = USART1_TX_SP;				//���뷢���ֽڸ���
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
	USART1->DR = (c & (uint16_t) 0x01FF);			//����һ���ֽڲ��ȴ��������
	while (!(USART1->SR & USART_FLAG_TXE))
	;
#endif
}

inline char SerialClass::peek() {
	return USART1_RX_Buf[USART1_Read_SP];		//ȡ������
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

/*����δ��ȡ�ֽڸ���*/
uint8_t SerialClass::available() {
	return USART1_RX_SP >= USART1_Read_SP ? USART1_RX_SP - USART1_Read_SP :
	USART1_RX_Buf_Size - USART1_Read_SP + USART1_RX_SP;
}
/*һ֡���ݼ���־*/
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
	if (USART1_Read_SP != USART1_RX_SP)				//��ֹ��ȡԽ��
		++USART1_Read_SP;
	if (USART1_Read_SP == USART1_RX_Buf_Size) {		//ָ�����
		USART1_Read_SP = 0;
	}
}

inline void SerialClass::ReadSPDec() {
	if (USART1_Read_SP == 0) {		//ָ�����
		USART1_Read_SP = USART1_RX_Buf_Size;
	}
}
/*���ڻ��������жϳ���*/
extern "C" void USART1_IRQHandler(void) {
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
		USART1_Read_Frame = 1;
		USART_ReceiveData(USART1);
#ifdef USE_DMA
		DMA_Cmd(DMA1_Channel5, DISABLE);			//���յ�1֡���ݣ���DMA���浽������
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
