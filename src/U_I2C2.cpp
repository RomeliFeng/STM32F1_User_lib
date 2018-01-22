/*
 * U_I2C2.cpp
 *
 *  Created on: 2016��2��7��
 *      Author: Romeli
 */
#include "U_I2C2.h"

/* I2C START mask */
#define CR1_START_Set           ((uint16_t)0x0100)
#define CR1_START_Reset         ((uint16_t)0xFEFF)
/* I2C STOP mask */
#define CR1_STOP_Set            ((uint16_t)0x0200)
#define CR1_STOP_Reset          ((uint16_t)0xFDFF)
/* I2C ACK mask */
#define CR1_ACK_Set             ((uint16_t)0x0400)
#define CR1_ACK_Reset           ((uint16_t)0xFBFF)

#define SCL_PIN GPIO_Pin_10
#define SDA_PIN GPIO_Pin_11

volatile uint8_t U_I2C2::I2C2_Busy = 0;
volatile uint8_t U_I2C2::I2C2_Direction;
volatile uint8_t U_I2C2::I2C2_Sub_Add_Flag = 1;

volatile uint8_t U_I2C2::I2C2_Device_Add = 0;
volatile uint8_t U_I2C2::I2C2_Tx_Buf[I2C2_TX_Buf_Size];
volatile uint8_t U_I2C2::I2C2_Tx_Index = 0;
volatile uint8_t U_I2C2::I2C2_Tx_Size = 0;
volatile uint8_t *U_I2C2::I2C2_Rx_Buf;
volatile uint8_t U_I2C2::I2C2_Rx_Index = 0;
volatile uint8_t U_I2C2::I2C2_Rx_Size = 0;

void U_I2C2::Init(uint32_t speed, uint8_t salveAdd) {
	GPIOInit();
	I2CInit(speed, salveAdd);
	NVICInit();
}

void U_I2C2::SendAsync(uint8_t dAdd, uint8_t wAdd, uint8_t* dataBuf,
		uint8_t size) {
	uint16_t i = 0;
	while (I2C2_Busy)
		;
	while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)) {
		i++;
		if (i > 30000) {
			ExitBusy();
			i = 0;
		}
	}
	I2C2_Sub_Add_Flag = 1;
	I2C2_Busy = 1;  //Set busy
	I2C2_Direction = I2C_Direction_Transmitter;
	I2C2_Device_Add = dAdd;

	I2C2_Tx_Index = 0;
	I2C2_Tx_Size = size + 1;
	I2C2_Tx_Buf[0] = wAdd;

	for (i = 0; i < size; ++i) {
		I2C2_Tx_Buf[i + 1] = *(dataBuf + i);
	}

	I2C2->CR1 |= CR1_ACK_Set;  //Set Auto ACK
	I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
	I2C2->CR1 |= I2C_CR1_START;  //Send Start Signal
}

void U_I2C2::SendAsync(uint8_t dAdd, uint8_t wAddH, uint8_t wAddL,
		uint8_t *dataBuf, uint8_t size) {
	uint16_t i = 0;
	while (I2C2_Busy)
		;
	while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)) {
		i++;
		if (i > 30000) {
			ExitBusy();
			i = 0;
		}
	}
	I2C2_Sub_Add_Flag = 1;
	I2C2_Busy = 1;  //Set busy
	I2C2_Direction = I2C_Direction_Transmitter;
	I2C2_Device_Add = dAdd;

	I2C2_Tx_Index = 0;
	I2C2_Tx_Size = 2 + size; //2byte wAdd + data size
	I2C2_Tx_Buf[0] = wAddH;
	I2C2_Tx_Buf[1] = wAddL;

	for (i = 0; i < size; ++i) {
		I2C2_Tx_Buf[i + 2] = *(dataBuf + i);
	}

	I2C2->CR1 |= CR1_ACK_Set;  //Set Auto ACK
	I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
	I2C2->CR1 |= I2C_CR1_START;  //Send Start Signal
}

void U_I2C2::ReceiveSync(uint8_t dAdd, uint8_t rAdd, uint8_t *dataBuf,
		uint8_t size) {
	uint16_t i = 0;
	while (I2C2_Busy)
		;
	while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)) {
		i++;
		if (i > 30000) {
			ExitBusy();
			i = 0;
		}
	}
	I2C2_Sub_Add_Flag = 1;
	I2C2_Busy = 1;  //Set busy
	I2C2_Direction = I2C_Direction_Receiver;
	I2C2_Device_Add = dAdd;

	I2C2_Tx_Index = 0;
	I2C2_Tx_Size = 1;
	I2C2_Tx_Buf[0] = rAdd;

	I2C2_Rx_Index = 0;
	I2C2_Rx_Size = size;
	I2C2_Rx_Buf = dataBuf;

	I2C2->CR1 |= CR1_ACK_Set;  //Set Auto ACK
	I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
	I2C2->CR1 |= I2C_CR1_START;  //Send Start Signal

	while (I2C2_Busy)
		;
}

void U_I2C2::ReceiveSync(uint8_t dAdd, uint8_t rAddH, uint8_t rAddL,
		uint8_t* dataBuf, uint8_t size) {
	uint16_t i = 0;
	while (I2C2_Busy)
		;
	while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)) {
		i++;
		if (i > 30000) {
			ExitBusy();
			i = 0;
		}
	}
	I2C2_Sub_Add_Flag = 1;
	I2C2_Busy = 1;  //Set busy
	I2C2_Direction = I2C_Direction_Receiver;
	I2C2_Device_Add = dAdd;

	I2C2_Tx_Index = 0;
	I2C2_Tx_Size = 2;  //2btye rAdd
	I2C2_Tx_Buf[0] = rAddH;
	I2C2_Tx_Buf[1] = rAddL;

	I2C2_Rx_Index = 0;
	I2C2_Rx_Size = size;
	I2C2_Rx_Buf = dataBuf;

	I2C2->CR1 |= CR1_ACK_Set;  //Set Auto ACK
	I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
	I2C2->CR1 |= I2C_CR1_START;  //Send Start Signal

	while (I2C2_Busy)
		;
}

void U_I2C2::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SCL_PIN | SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void U_I2C2::I2CInit(uint32_t speed, uint8_t salveAdd) {
	I2C_InitTypeDef I2C_InitStructure;
	speed = speed > 400000 ? 400000 : speed;

	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = speed;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = salveAdd;
	I2C_Init(I2C2, &I2C_InitStructure);
}

void U_I2C2::NVICInit() {
	NVIC_InitTypeDef NVIC_InitTypeStructure;

	NVIC_InitTypeStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
	NVIC_InitTypeStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitTypeStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitTypeStructure);

	NVIC_InitTypeStructure.NVIC_IRQChannel = I2C2_ER_IRQn;
	NVIC_InitTypeStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitTypeStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitTypeStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitTypeStructure);

	I2C_ClearITPendingBit(I2C2, I2C_IT_ERR);
	I2C_ITConfig(I2C2, I2C_IT_ERR, ENABLE);

	I2C_Cmd(I2C2, ENABLE);
}

void U_I2C2::ExitBusy() {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SCL_PIN | SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	uint16_t i = 0;

	for (uint8_t j = 0; j < 9; j++) {
		GPIO_SetBits(GPIOB, SCL_PIN);  //Set CLK
		while (i < 80)
			i++;
		i = 0;
		GPIO_ResetBits(GPIOB, SCL_PIN);  //Reset CLK
		while (i < 80)
			i++;
		i = 0;
	}

	GPIO_SetBits(GPIOB, SCL_PIN);  //Set CLK
	while (i < 80)
		i++;
	i = 0;
	//����һ���͵�ƽ����
	GPIO_SetBits(GPIOB, SDA_PIN);  //Set SDA
	while (i < 80)
		i++;
	i = 0;
	GPIO_ResetBits(GPIOB, SDA_PIN);  //Reset SDA
	while (i < 80)
		i++;
	i = 0;
	GPIO_SetBits(GPIOB, SDA_PIN);  //Set SDA Stop signal
	while (i < 80)
		i++;
	i = 0;

	I2C2->CR1 |= I2C_CR1_SWRST;  //Set I2C Reset
	I2C2->CR1 &= ~I2C_CR1_SWRST;  //Reset I2C Reset
	GPIOInit();
}

extern "C" void I2C2_EV_IRQHandler(void) {
	uint32_t Event = I2C_GetLastEvent(I2C2);

	switch (Event) {
	case I2C_EVENT_MASTER_MODE_SELECT:		//I2C Start signal send
		if (U_I2C2::I2C2_Tx_Index == 0 && U_I2C2::I2C2_Sub_Add_Flag)
			I2C_Send7bitAddress(I2C2, U_I2C2::I2C2_Device_Add,
			I2C_Direction_Transmitter);
		else
			I2C_Send7bitAddress(I2C2, U_I2C2::I2C2_Device_Add,
					U_I2C2::I2C2_Direction);
		break;
	case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:		//дģʽѡ��
		I2C2->DR = U_I2C2::I2C2_Tx_Buf[U_I2C2::I2C2_Tx_Index++];
		break;
	case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:		//��ģʽѡ��
		if (U_I2C2::I2C2_Rx_Size == 1) {
			I2C2->CR1 &= CR1_ACK_Reset;		//�ر��Զ�Ӧ��
			I2C2->CR1 |= CR1_STOP_Set;		//���ͽ���ʱ��
		}
		break;
	case I2C_EVENT_MASTER_BYTE_TRANSMITTING:		//TXE��־
		if (U_I2C2::I2C2_Direction == I2C_Direction_Transmitter) {
			if (U_I2C2::I2C2_Tx_Index < U_I2C2::I2C2_Tx_Size) {
				I2C2->DR = U_I2C2::I2C2_Tx_Buf[U_I2C2::I2C2_Tx_Index++];
			}
		}
		break;
	case I2C_EVENT_MASTER_BYTE_TRANSMITTED:		//TXE&BTF��־

		if (U_I2C2::I2C2_Tx_Index < U_I2C2::I2C2_Tx_Size)
			I2C2->DR = U_I2C2::I2C2_Tx_Buf[U_I2C2::I2C2_Tx_Index++];
		else {
			if (U_I2C2::I2C2_Direction == I2C_Direction_Receiver) {
				I2C2->CR1 |= I2C_CR1_START;		//���Ϳ�ʼʱ��
			} else {
				I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
				I2C2->CR1 |= CR1_STOP_Set;		//���ͽ���ʱ��
				U_I2C2::I2C2_Busy = 0;
			}
		}
		break;
	case I2C_EVENT_MASTER_BYTE_RECEIVED:		//RXE��־
		U_I2C2::I2C2_Rx_Buf[U_I2C2::I2C2_Rx_Index++] = I2C2->DR;	//��ȡ����
		if (U_I2C2::I2C2_Rx_Index == U_I2C2::I2C2_Rx_Size - 1) {
			I2C2->CR1 &= CR1_ACK_Reset;		//�ر��Զ�Ӧ��
			I2C2->CR1 |= CR1_STOP_Set;		//���ͽ���ʱ��
		}
		if (U_I2C2::I2C2_Rx_Index == U_I2C2::I2C2_Rx_Size) {
			I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
			U_I2C2::I2C2_Busy = 0;
		}
		break;
	default:
		break;
	}
}
extern "C" void I2C2_ER_IRQHandler(void) {
	if (I2C2->SR1 & 1 << 10) {		//Ӧ��ʧ��
		I2C2->SR1 &= ~(1 << 10); //����ж�
	}
	if (I2C2->SR1 & 1 << 14) { //��ʱ
		I2C2->SR1 &= ~(1 << 14); //����ж�
	}
	if (I2C2->SR1 & 1 << 11) { //����/Ƿ��
		I2C2->SR1 &= ~(1 << 11); //����ж�
	}
	if (I2C2->SR1 & 1 << 9) { //�ٲö�ʧ
		I2C2->SR1 &= ~(1 << 9); //����ж�
	}
	if (I2C2->SR1 & 1 << 8) { //���߳���
		I2C2->SR1 &= ~(1 << 8); //����ж�
	}
	I2C2->CR1 &= CR1_ACK_Reset;		//�ر��Զ�Ӧ��
	I2C2->CR1 |= CR1_STOP_Set;		//���ͽ���ʱ��
	U_I2C2::I2C2_Busy = 0;
}
