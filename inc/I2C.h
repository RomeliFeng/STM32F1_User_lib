/*
 * I2C.h
 *
 *  Created on: 2016Äê2ÔÂ7ÈÕ
 *      Author: Romeli
 *      Used Pin: 	|  PB6-I2C1_SCL	  |
 *          		|  PB7-I2C1_SDA   |
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f10x.h"

#define SlaveAddress 0x44

void I2C_GPIO_Init();

class I2CClass {
public:
	void Init(uint32_t Speed);
	void Send(uint8_t D_Add, uint8_t W_Add, uint8_t data);
	void Send(uint8_t D_Add, uint8_t W_Add, uint8_t *dataBuf, uint8_t size);
	uint8_t Receive(uint8_t D_Add, uint8_t R_Add);
	void Receive(uint8_t D_Add, uint8_t R_Add, uint8_t *dataBuf, uint8_t size);
};

extern I2CClass I2C;
#endif /* I2C_H_ */
