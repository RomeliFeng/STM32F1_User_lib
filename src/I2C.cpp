/*
 * I2C.cpp
 *
 *  Created on: 2016年2月7日
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  //！！！需要打开I2C时钟才可设置引脚模式

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void I2CClass::send(uint8_t D_Add, uint8_t data)
{
//	//等待总线空闲
//	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
//		;
//	//发送开始信号
//	I2C_GenerateSTART(I2C1, ENABLE);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
//		;
//	//发送设备地址+写信号
//	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Transmitter);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//		;
//	//发送要写入的数据
//	I2C_SendData(I2C1, data);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//		;
//	//发送停止时序
//	I2C_GenerateSTOP(I2C1, ENABLE);
	send(D_Add, &data, 1);
}

void I2CClass::send(uint8_t D_Add, uint8_t W_Add, uint8_t data)
{
//	//等待总线空闲
//	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
//		;
//	//发送开始信号
//	I2C_GenerateSTART(I2C1, ENABLE);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
//		;
//	//发送设备地址+写信号
//	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Transmitter);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//		;
//	//发送写地址
//	I2C_SendData(I2C1, W_Add);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//		;
//	//发送要写入的数据
//	I2C_SendData(I2C1, data);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//		;
//	//发送停止时序
//	I2C_GenerateSTOP(I2C1, ENABLE);
	send(D_Add, W_Add, &data, 1);
}

void I2CClass::send(uint8_t D_Add, uint8_t* dataBuf, uint8_t size)
{
	//等待总线空闲
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		;
	//发送开始信号
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		;
	//发送设备地址+写信号
	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;
	//循环发送要写入的数据
	while (size)
	{
		I2C_SendData(I2C1, *dataBuf);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
			;
		dataBuf++;
		size--;
	}

	//发送停止时序
	I2C_GenerateSTOP(I2C1, ENABLE);
}
void I2CClass::send(uint8_t D_Add, uint8_t W_Add, uint8_t* dataBuf, uint8_t size)
{
	//等待总线空闲
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		;
	//发送开始信号
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		;
	//发送设备地址+写信号
	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;
	//发送写地址
	I2C_SendData(I2C1, W_Add);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;
	//循环发送要写入的数据
	while (size)
	{
		I2C_SendData(I2C1, *dataBuf);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
			;
		dataBuf++;
		size--;
	}

	//发送停止时序
	I2C_GenerateSTOP(I2C1, ENABLE);
}

uint8_t I2CClass::receive(uint8_t D_Add)
{
	uint8_t data;
	receive(D_Add, &data, 1);
//	//等待总线空闲
//	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
//		;
//	//发送开始信号
//	I2C_GenerateSTART(I2C1, ENABLE);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
//		;
//	//发送设备地址+读信号
//	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Receiver);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
//		;
//	//关闭主动应答：只接收一个byte
//	I2C_AcknowledgeConfig(I2C1, DISABLE);
//
//	//等待数据到达
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
//		;
//
//	//从寄存器中取出数据
//	data = I2C_ReceiveData(I2C1);
//
//	//发送停止时序
//	I2C_GenerateSTOP(I2C1, ENABLE);
//
//	//打开主动应答
//	I2C_AcknowledgeConfig(I2C1, ENABLE);
//
//	//返回读到的数据
	return data;
}

uint8_t I2CClass::receive(uint8_t D_Add, uint8_t R_Add)
{
	uint8_t data;
	receive(D_Add, R_Add, &data, 1);
//	//等待总线空闲
//	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
//		;
//	//发送开始信号
//	I2C_GenerateSTART(I2C1, ENABLE);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
//		;
//	//发送设备地址+写信号
//	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Transmitter);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//		;
//	//发送读地址
//	I2C_SendData(I2C1, R_Add);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//		;
//	//第二次发送开始信号
//	I2C_GenerateSTART(I2C1, ENABLE);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
//		;
//	//发送设备地址+读信号
//	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Receiver);
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
//		;
//	//关闭主动应答：只接收一个byte
//	I2C_AcknowledgeConfig(I2C1, DISABLE);
//	//等待数据到达
//	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
//		;
//	//从寄存器中取出数据
//	data = I2C_ReceiveData(I2C1);
//	//发送停止时序
//	I2C_GenerateSTOP(I2C1, ENABLE);
//	//打开主动应答
//	I2C_AcknowledgeConfig(I2C1, ENABLE);
//	//返回读到的数据
	return data;
}

void I2CClass::receive(uint8_t D_Add, uint8_t *dataBuf, uint8_t size)
{
	//等待总线空闲
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		;
	//发送开始信号
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		;
	//发送设备地址+读信号
	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		;
	/* Clear EV6 by setting again the PE bit */
	I2C_Cmd(I2C1, ENABLE);
	//打开主动应答
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	//循环读取数据
	while (size)
	{
		//等待数据到达
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

	//打开主动应答
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}

void I2CClass::receive(uint8_t D_Add, uint8_t R_Add, uint8_t* dataBuf, uint8_t size)
{
	//等待总线空闲
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		;
	//发送开始信号
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		;
	//发送设备地址+写信号
	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;
	//发送读地址
	I2C_SendData(I2C1, R_Add);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;
	//第二次发送开始信号
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		;
	//发送设备地址+读信号
	I2C_Send7bitAddress(I2C1, D_Add, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		;
	//打开主动应答
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	//循环读取数据
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
	//打开主动应答
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}
