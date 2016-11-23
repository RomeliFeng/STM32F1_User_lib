/*
 * SPI.cpp
 *
 *  Created on: 2016年5月21日
 *      Author: Romeli
 */
#include "SPI.h"

SPIClass SPI;

void SPI_GPIO_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;
#ifdef USE_SPI1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //SPI CLK
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//SPI MISO
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//SPI MOSI
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#ifdef USE_SPI2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //SPI CLK
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //SPI MISO
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //SPI MOSI
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
}

void SPIClass::Init() {
	SPI_InitTypeDef SPI_InitStructure;

	SPI_GPIO_Init();

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //内部NSS信号由SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_Speed; //预分频值为2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7; //用于CRC值计算的多项式

	SPI_Init(SPI_CH, &SPI_InitStructure); //初始化配置
	SPI_Cmd(SPI_CH, ENABLE);

}

uint8_t SPIClass::transfer(uint8_t data) {
	SPI_I2S_SendData(SPI_CH, data);
	while (SPI_I2S_GetFlagStatus(SPI_CH, SPI_I2S_FLAG_RXNE) == RESET)
		//等待时钟结束，以便接收数据
		;
	return SPI_I2S_ReceiveData(SPI_CH);
}

