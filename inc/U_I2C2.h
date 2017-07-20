/*
 * U_I2C2.h
 *
 *  Created on: 20170712
 *      Author: Romeli
 *      Used Pin: 	|  PB10-I2C2_SCL	  |
 *          		|  PB11-I2C2_SDA   |
 */

#ifndef U_I2C2_H_
#define U_I2C2_H_

#include "cmsis_device.h"

#define I2C2_TX_Buf_Size 64

class U_I2C2 {
public:
	volatile static uint8_t I2C2_Busy;
	volatile static uint8_t I2C2_Direction;
	volatile static uint8_t I2C2_Sub_Add_Flag;

	volatile static uint8_t I2C2_Device_Add;
	volatile static uint8_t I2C2_Tx_Buf[I2C2_TX_Buf_Size];
	volatile static uint8_t I2C2_Tx_Index;
	volatile static uint8_t I2C2_Tx_Size;
	volatile static uint8_t *I2C2_Rx_Buf;
	volatile static uint8_t I2C2_Rx_Index;
	volatile static uint8_t I2C2_Rx_Size;

	static void Init(uint32_t speed = 400000, uint8_t salveAdd = 0x44);
	static void SendAsync(uint8_t dAdd, uint8_t wAdd, uint8_t *dataBuf,
			uint8_t size);
	static inline void SendAsync(uint8_t dAdd, uint8_t wAdd, uint8_t data) {
		SendAsync(dAdd, wAdd, &data, 1);
	}
	static void SendAsync(uint8_t dAdd, uint8_t wAddH, uint8_t wAddL,
			uint8_t *dataBuf, uint8_t size);
	static void SendAsync(uint8_t dAdd, uint8_t wAddH, uint8_t wAddL,
			uint8_t data) {
		SendAsync(dAdd, wAddH, wAddL, &data, 1);
	}

	static void ReceiveSync(uint8_t dAdd, uint8_t rAdd, uint8_t *dataBuf,
			uint8_t size);
	static uint8_t ReceiveSync(uint8_t dAdd, uint8_t rAdd) {
		uint8_t tmp;
		ReceiveSync(dAdd, rAdd, &tmp, 1);
		return tmp;
	}
	static void ReceiveSync(uint8_t dAdd, uint8_t rAddH, uint8_t rAddL,
			uint8_t *dataBuf, uint8_t size);
	static uint8_t ReceiveSync(uint8_t dAdd, uint8_t rAddH, uint8_t rAddL) {
		uint8_t tmp;
		ReceiveSync(dAdd, rAddH, rAddL, &tmp, 1);
		return tmp;
	}
private:
	static void GPIOInit();
	static void I2CInit(uint32_t speed, uint8_t salveAdd);
	static void NVICInit();
	static void ExitBusy();
};

#endif /* U_I2C2_H_ */
