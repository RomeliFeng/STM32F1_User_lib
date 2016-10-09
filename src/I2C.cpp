/*
 * I2C.cpp
 *
 *  Created on: 2016年2月7日
 *      Author: Romeli
 */
#include "I2C.h"

/* I2C START mask */
#define CR1_START_Set           ((uint16_t)0x0100)
#define CR1_START_Reset         ((uint16_t)0xFEFF)
/* I2C STOP mask */
#define CR1_STOP_Set            ((uint16_t)0x0200)
#define CR1_STOP_Reset          ((uint16_t)0xFDFF)
/* I2C ACK mask */
#define CR1_ACK_Set             ((uint16_t)0x0400)
#define CR1_ACK_Reset           ((uint16_t)0xFBFF)

#define I2C1_TX_Buf_Size 16

I2CClass I2C;

union _I2C_Event_Flag {
	uint32_t All;
	uint16_t Word[2];
} I2C_Event_Flag;

volatile static uint8_t I2C1_Busy = 0;
volatile static uint8_t I2C1_Direction;
volatile static uint8_t I2C1_Sub_Add_Flag = 1;

volatile static uint8_t I2C1_Device_Add = 0;
volatile static uint8_t I2C1_Tx_Buf[I2C1_TX_Buf_Size];
volatile static uint8_t I2C1_Tx_Index = 0;
volatile static uint8_t I2C1_Tx_Size = 0;
volatile static uint8_t* I2C1_Rx_Buf;
volatile static uint8_t I2C1_Rx_Index = 0;
volatile static uint8_t I2C1_Rx_Size = 0;

void I2CClass::Init(uint32_t Speed) {
	I2C_InitTypeDef I2C_InitStructure;
	NVIC_InitTypeDef NVIC_InitTypeStructure;

	I2C_GPIO_Init();

	I2C_DeInit(I2C1);

	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = Speed;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = SlaveAddress;

	I2C_Init(I2C1, &I2C_InitStructure);

	I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE);
	I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE);
//	I2C_ITConfig(I2C1, I2C_IT_ERR, ENABLE);
	//	I2C_Cmd(I2C1, ENABLE);
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);

	NVIC_InitTypeStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
	NVIC_InitTypeStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitTypeStructure.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&NVIC_InitTypeStructure);
}
void I2C_GPIO_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  //！！！需要打开I2C时钟才可设置引脚模式

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void I2CClass::Send(uint8_t D_Add, uint8_t W_Add, uint8_t* dataBuf,
		uint8_t size) {
	while (I2C1_Busy)
		;
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		;
	I2C1_Sub_Add_Flag = 1;
	I2C1_Busy = 1;  //设置I2C忙标识
	I2C1_Direction = I2C_Direction_Transmitter;
	I2C1_Device_Add = D_Add;

	I2C1_Tx_Index = 0;
	I2C1_Tx_Size = size + 1;
	I2C1_Tx_Buf[0] = W_Add;

	for (uint8_t i = 0; i < size; ++i) {
		I2C1_Tx_Buf[i + 1] = *dataBuf + i;
	}
	I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE);
	I2C1->CR1 |= I2C_CR1_START;  //发送开始时序
}

void I2CClass::Send(uint8_t D_Add, uint8_t W_Add, uint8_t data) {
	Send(D_Add, W_Add, &data, 1);
}

void I2CClass::Receive(uint8_t D_Add, uint8_t R_Add, uint8_t *dataBuf,
		uint8_t size) {
	while (I2C1_Busy)
		;
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		;
	I2C1_Sub_Add_Flag = 1;
	I2C1_Busy = 1;  //设置I2C忙标识
	I2C1_Direction = I2C_Direction_Receiver;
	I2C1_Device_Add = D_Add;

	I2C1_Tx_Index = 0;
	I2C1_Tx_Size = 1;
	I2C1_Tx_Buf[0] = R_Add;

	I2C1_Rx_Index = 0;
	I2C1_Rx_Size = size;
	I2C1_Rx_Buf = dataBuf;

	I2C1->CR1 |= CR1_ACK_Set;  //打开自动应答
	I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE);
	I2C1->CR1 |= I2C_CR1_START;  //发送开始时序

	while (I2C1_Busy)
		;
}

uint8_t I2CClass::Receive(uint8_t D_Add, uint8_t R_Add) {
	uint8_t data;
	Receive(D_Add, R_Add, &data, 1);
	while (I2C1_Busy != 0)
		;
	return data;
}

extern "C" void I2C1_EV_IRQHandler(void) {
	uint32_t Event = I2C_GetLastEvent(I2C1);

	switch (Event) {
	case I2C_EVENT_MASTER_MODE_SELECT:		//I2C开始事件
		if (I2C1_Tx_Index == 0 && I2C1_Sub_Add_Flag)
			I2C_Send7bitAddress(I2C1, I2C1_Device_Add,
			I2C_Direction_Transmitter);
		else
			I2C_Send7bitAddress(I2C1, I2C1_Device_Add, I2C1_Direction);
		break;
	case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
		I2C_SendData(I2C1, I2C1_Tx_Buf[I2C1_Tx_Index++]);
		break;
	case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:
		if (I2C1_Rx_Size == 1) {
			I2C1->CR1 &= CR1_ACK_Reset;		//关闭自动应答
			I2C1->CR1 |= CR1_STOP_Set;		//发送结束时序
		}
		break;
	case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
		if (I2C1_Direction == I2C_Direction_Receiver) {
			I2C1->CR1 |= I2C_CR1_START;		//发送开始时序
		} else {
			if (I2C1_Tx_Index < I2C1_Tx_Size)
				I2C_SendData(I2C1, I2C1_Tx_Buf[I2C1_Tx_Index++]);
			else {
				I2C_ITConfig(I2C1, I2C_IT_EVT, DISABLE);
				I2C1->CR1 |= CR1_STOP_Set;		//发送结束时序
				I2C1_Busy = 0;
			}
		}
		break;
	case I2C_EVENT_MASTER_BYTE_RECEIVED:
		I2C1_Rx_Buf[I2C1_Rx_Index++] = I2C1->DR;		//读取数据
		if (I2C1_Rx_Index == I2C1_Rx_Size - 1) {
			I2C1->CR1 &= CR1_ACK_Reset;		//关闭自动应答
			I2C1->CR1 |= CR1_STOP_Set;		//发送结束时序
		}
		if (I2C1_Rx_Index == I2C1_Rx_Size) {
			I2C_ITConfig(I2C1, I2C_IT_EVT, DISABLE);
			I2C1_Busy = 0;
		}
		break;
	default:
		break;
	}
}
