/*
 * USPI1.h
 *
 *  Created on: 2017年7月19日
 *      Author: Romeli
 */

#ifndef SRC_USPI1_H_
#define SRC_USPI1_H_

#include "cmsis_device.h"

#define USE_DMA

#define SPI1_Speed_18M SPI_BaudRatePrescaler_4
#define SPI1_Speed_9M SPI_BaudRatePrescaler_8
#define SPI1_Speed_4_5M SPI_BaudRatePrescaler_16
#define SPI1_Speed_2_25M SPI_BaudRatePrescaler_32
#define SPI1_Speed_1_125M SPI_BaudRatePrescaler_64
#define SPI1_Speed_562_5K SPI_BaudRatePrescaler_128
#define SPI1_Speed_281_25K SPI_BaudRatePrescaler_256

#define SPI1_TXBUF_SIZE 128
#define SPI1_RXBUF_SIZE 128

class U_SPI1 {
public:
	static uint8_t TxBuf[SPI1_TXBUF_SIZE];
	static uint8_t TxBuf_SP;
	static uint8_t RxBuf[SPI1_RXBUF_SIZE];
	static uint8_t RxBuf_SP;
	static uint8_t DMARxBuf[SPI1_TXBUF_SIZE];
	static uint8_t DMARxBuf_SP;

	static volatile bool Busy;

	static void Init(uint16_t SPI1_Speed);
	static void SendSync(uint8_t* data, uint16_t size);
	static inline void SendSync(uint8_t data) {
		SendSync(&data, 1);
	}
private:
	static void GPIOInit();
	static void SPIInit(uint16_t SPI1_Speed);
	static void DMAInit();
};

#endif /* SRC_USPI1_H_ */
