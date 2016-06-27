/*
 * I2C.cpp
 *
 *  Created on: 2016��2��7��
 *      Author: Romeli
 */
#include "I2C.h"

I2CClass I2C;

void I2CClass::init()
{
	I2C_InitTypeDef I2C_InitStructure;

	I2C_GPIO_Init();

	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = SlaveAddress;

	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
}
void I2C_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  //��������Ҫ��I2Cʱ�Ӳſ���������ģʽ

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void I2CClass::send(uint8_t D_Add, uint8_t data)
{
//	//�ȴ����߿���
//	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
//		;
//	//���Ϳ�ʼ�ź�
//	I2C_GenerateSTART(I2C1, ENABLE);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
//		;
//	//�����豸��ַ+д�ź�
//	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Transmitter);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//		;
//	//����Ҫд�������
//	I2C_SendData(I2C1, data);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//		;
//	//����ֹͣʱ��
//	I2C_GenerateSTOP(I2C1, ENABLE);
	send(D_Add, &data, 1);
}

void I2CClass::send(uint8_t D_Add, uint8_t W_Add, uint8_t data)
{
//	//�ȴ����߿���
//	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
//		;
//	//���Ϳ�ʼ�ź�
//	I2C_GenerateSTART(I2C1, ENABLE);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
//		;
//	//�����豸��ַ+д�ź�
//	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Transmitter);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//		;
//	//����д��ַ
//	I2C_SendData(I2C1, W_Add);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//		;
//	//����Ҫд�������
//	I2C_SendData(I2C1, data);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//		;
//	//����ֹͣʱ��
//	I2C_GenerateSTOP(I2C1, ENABLE);
	send(D_Add, W_Add, &data, 1);
}

void I2CClass::send(uint8_t D_Add, uint8_t* dataBuf, uint8_t size)
{
	//�ȴ����߿���
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		;
	//���Ϳ�ʼ�ź�
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		;
	//�����豸��ַ+д�ź�
	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;
	//ѭ������Ҫд�������
	while (size)
	{
		I2C_SendData(I2C1, *dataBuf);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
			;
		dataBuf++;
		size--;
	}

	//����ֹͣʱ��
	I2C_GenerateSTOP(I2C1, ENABLE);
}
void I2CClass::send(uint8_t D_Add, uint8_t W_Add, uint8_t* dataBuf, uint8_t size)
{
	//�ȴ����߿���
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		;
	//���Ϳ�ʼ�ź�
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		;
	//�����豸��ַ+д�ź�
	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;
	//����д��ַ
	I2C_SendData(I2C1, W_Add);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;
	//ѭ������Ҫд�������
	while (size)
	{
		I2C_SendData(I2C1, *dataBuf);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
			;
		dataBuf++;
		size--;
	}

	//����ֹͣʱ��
	I2C_GenerateSTOP(I2C1, ENABLE);
}

uint8_t I2CClass::receive(uint8_t D_Add)
{
	uint8_t data;
	receive(D_Add, &data, 1);
//	//�ȴ����߿���
//	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
//		;
//	//���Ϳ�ʼ�ź�
//	I2C_GenerateSTART(I2C1, ENABLE);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
//		;
//	//�����豸��ַ+���ź�
//	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Receiver);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
//		;
//	//�ر�����Ӧ��ֻ����һ��byte
//	I2C_AcknowledgeConfig(I2C1, DISABLE);
//
//	//�ȴ����ݵ���
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
//		;
//
//	//�ӼĴ�����ȡ������
//	data = I2C_ReceiveData(I2C1);
//
//	//����ֹͣʱ��
//	I2C_GenerateSTOP(I2C1, ENABLE);
//
//	//������Ӧ��
//	I2C_AcknowledgeConfig(I2C1, ENABLE);
//
//	//���ض���������
	return data;
}

uint8_t I2CClass::receive(uint8_t D_Add, uint8_t R_Add)
{
	uint8_t data;
	receive(D_Add, R_Add, &data, 1);
//	//�ȴ����߿���
//	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
//		;
//	//���Ϳ�ʼ�ź�
//	I2C_GenerateSTART(I2C1, ENABLE);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
//		;
//	//�����豸��ַ+д�ź�
//	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Transmitter);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//		;
//	//���Ͷ���ַ
//	I2C_SendData(I2C1, R_Add);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//		;
//	//�ڶ��η��Ϳ�ʼ�ź�
//	I2C_GenerateSTART(I2C1, ENABLE);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
//		;
//	//�����豸��ַ+���ź�
//	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Receiver);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
//		;
//	//�ر�����Ӧ��ֻ����һ��byte
//	I2C_AcknowledgeConfig(I2C1, DISABLE);
//	//�ȴ����ݵ���
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
//		;
//	//�ӼĴ�����ȡ������
//	data = I2C_ReceiveData(I2C1);
//	//����ֹͣʱ��
//	I2C_GenerateSTOP(I2C1, ENABLE);
//	//������Ӧ��
//	I2C_AcknowledgeConfig(I2C1, ENABLE);
//	//���ض���������
	return data;
}

void I2CClass::receive(uint8_t D_Add, uint8_t *dataBuf, uint8_t size)
{
	//�ȴ����߿���
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		;
	//���Ϳ�ʼ�ź�
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		;
	//�����豸��ַ+���ź�
	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		;
	/* Clear EV6 by setting again the PE bit */
	I2C_Cmd(I2C1, ENABLE);
	//������Ӧ��
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	//ѭ����ȡ����
	while (size)
	{
		//�ȴ����ݵ���
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
			;
		{
			/* Test on EV7 and clear it */

			/* Read a byte from the EEPROM */
			*dataBuf = I2C_ReceiveData(I2C1);

			/* Point to the next location where the byte read will be saved */
			dataBuf++;

			/* Decrement the read bytes counter */
			size--;
			if (size == 1)
			{
				/* Disable Acknowledgement */
				I2C_AcknowledgeConfig(I2C1, DISABLE);
				/* Send STOP Condition */
				I2C_GenerateSTOP(I2C1, ENABLE);
			}
		}
	}

	//������Ӧ��
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}

void I2CClass::receive(uint8_t D_Add, uint8_t R_Add, uint8_t* dataBuf, uint8_t size)
{
	//�ȴ����߿���
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		;
	//���Ϳ�ʼ�ź�
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		;
	//�����豸��ַ+д�ź�
	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;
	//���Ͷ���ַ
	I2C_SendData(I2C1, R_Add);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;
	//�ڶ��η��Ϳ�ʼ�ź�
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		;
	//�����豸��ַ+���ź�
	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		;
	//������Ӧ��
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	//ѭ����ȡ����
	while (size)
	{

		/* Test on EV7 and clear it */
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
			;
		{
			/* Read a byte from the EEPROM */
			*dataBuf = I2C_ReceiveData(I2C1);

			/* Point to the next location where the byte read will be saved */
			dataBuf++;

			/* Decrement the read bytes counter */
			size--;
			if (size == 1)
			{
				/* Disable Acknowledgement */
				I2C_AcknowledgeConfig(I2C1, DISABLE);

				/* Send STOP Condition */
				I2C_GenerateSTOP(I2C1, ENABLE);
			}
		}
	}
	//������Ӧ��
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}
