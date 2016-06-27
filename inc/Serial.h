/*
 * Serial.h
 *
 *  Created on: 2016年1月18日
 *      Author: Romeli
 *      Version: V1.0
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include "stm32f10x.h"
#include "misc.h"
#include <stdlib.h>
#include <stdio.h>

extern char USART1_RX_Buf[256];			//接收缓存器
extern uint8_t USART1_RX_SP;			//接收缓存器指针
extern uint8_t USART1_Read_SP;			//缓存器读取指针
extern uint8_t USART1_Read_Available;	//缓冲器未读字节
class SerialClass{
public:
	void begin(uint32_t BaudRate);
	void print(char *data);
	void print(long data);
	void print(int data);
	void print(float data,uint8_t ndigit);
	void print(double data,uint8_t ndigit);
	void println(char *data);
	void println(long data);
	void println(int data);
	void println(float data,uint8_t ndigit);
	void println(double data,uint8_t ndigit);
	uint8_t print_c(char c);
	void print_s(char* str);
	uint8_t available();
	char read();
	void read(char *buf,uint8_t len);
private:
};

extern SerialClass Serial;
#endif /* SERIAL_H_ */
