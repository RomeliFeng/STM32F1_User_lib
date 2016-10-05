/*
 * Serial.h
 *
 *  Created on: 2016Äê1ÔÂ18ÈÕ
 *      Author: Romeli
 *      Version: V1.0
 */

#ifndef SERIALDMA_H_
#define SERIALDMA_H_

#include "stm32f10x.h"

class SerialClass
{
public:
	void begin(uint32_t BaudRate);
	void print(long data);
	void print(double data, uint8_t ndigit);

	inline void print(char data)
	{
		print((long) data);
	}
	inline void print(int data)
	{
		print((long) data);
	}
	inline void print(float data, uint8_t ndigit)
	{
		print((double) data, ndigit);
	}
	inline void println(char *data)
	{
		print(data);
		print("\r\n", 2);
	}
	inline void println(long data)
	{
		print(data);
		print("\r\n", 2);
	}
	inline void println(char data)
	{
		print(data);
		print("\r\n", 2);
	}
	inline void println(int data)
	{
		print(data);
		print("\r\n", 2);
	}
	inline void println(float data, uint8_t ndigit)
	{
		print(data, ndigit);
		print("\r\n", 2);
	}
	inline void println(double data, uint8_t ndigit)
	{
		print(data, ndigit);
		print("\r\n", 2);
	}
	void print(char *data)
	{
		print(data, getlen(data));
	}
	void write(char c);
	uint8_t available();
	uint8_t checkFrame();
	char read();
	void read(char *buf, uint8_t len);
private:
	void print(char *data, uint8_t len);
	uint8_t getlen(char *data);
};

extern SerialClass Serial;
#endif /* SERIALDMA_H_ */
