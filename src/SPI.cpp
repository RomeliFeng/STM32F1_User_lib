/*
 * SPI.cpp
 *
 *  Created on: 2016��5��21��
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

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //�ڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_Speed; //Ԥ��ƵֵΪ2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7; //����CRCֵ����Ķ���ʽ

	SPI_Init(SPI_CH, &SPI_InitStructure); //��ʼ������
	SPI_Cmd(SPI_CH, ENABLE);

}

uint8_t SPIClass::transfer(uint8_t data) {
	SPI_I2S_SendData(SPI_CH, data);
	while (SPI_I2S_GetFlagStatus(SPI_CH, SPI_I2S_FLAG_RXNE) == RESET)
		;
	return SPI_I2S_ReceiveData(SPI_CH);
}

