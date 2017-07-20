/*
 * USPI2.cpp
 *
 *  Created on: 2017年7月19日
 *      Author: Romeli
 */

#include <U_SPI2.h>

uint8_t U_SPI2::TxBuf[SPI2_TXBUF_SIZE];
uint8_t U_SPI2::TxBuf_SP = 0;
uint8_t U_SPI2::RxBuf[SPI2_RXBUF_SIZE];
uint8_t U_SPI2::RxBuf_SP = 0;
uint8_t U_SPI2::DMARxBuf[SPI2_TXBUF_SIZE];
uint8_t U_SPI2::DMARxBuf_SP;

volatile bool U_SPI2::Busy = false;

void U_SPI2::Init(uint16_t SPI2_Speed) {
	GPIOInit();
	SPIInit(SPI2_Speed);
#ifdef USE_DMA
	DMAInit();
#endif
	SPI_Cmd(SPI2, ENABLE);
}

void U_SPI2::SendSync(uint8_t* data, uint16_t size) {
#ifdef USE_DMA
	Busy = true;
	for (uint16_t i = 0; i < size; ++i) {
		TxBuf[i] = data[i];
	}
	DMA1_Channel5->CMAR = (uint32_t) TxBuf;
	DMA1_Channel5->CNDTR = size;
	DMA_Cmd(DMA1_Channel5, ENABLE);
	while (Busy)
		;
#else
	while (size--) {
		SPI_I2S_SendData(SPI2, *(data++));
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
		;
	}
#endif
}

void U_SPI2::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //SPI CLK
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //SPI MOSI
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //SPI MISO
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //SPI NSS
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void U_SPI2::SPIInit(uint16_t SPI2_Speed) {
	SPI_InitTypeDef SPI_InitStructure;

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI2_Speed;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI2, &SPI_InitStructure);
}

void U_SPI2::DMAInit() {
	DMA_InitTypeDef DMA_InitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;

	//开启DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&SPI2->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) TxBuf; //临时设置，无效
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 10; //临时设置，无效
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);

	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&SPI2->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) DMARxBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = SPI2_RXBUF_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel4, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);

	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
}

extern "C" void DMA1_Channel5_IRQHandler(void) {
	DMA_Cmd(DMA1_Channel5, DISABLE);
	DMA_ClearFlag(DMA1_FLAG_TC5);
	U_SPI2::Busy = false;
}
