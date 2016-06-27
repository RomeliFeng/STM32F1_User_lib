/*
 * SPI.cpp
 *
 *  Created on: 2016年5月21日
 *      Author: Romeli
 */
#include "SPI.h"

SPIClass SPI;

void SPI_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	//SPI CLK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//SPI MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//SPI MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void SPIClass::init()
{
	SPI_InitTypeDef SPI_InitStructure;

	SPI_GPIO_Init();

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; //时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; //第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //内部NSS信号由SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_Speed; //预分频值为2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7; //用于CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure); //初始化配置
	SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPIClass::transfer(uint8_t data)
{
	SPI_I2S_SendData(SPI1, data);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		//等待时钟结束，以便接收数据
		;
	return SPI_I2S_ReceiveData(SPI1);
}

