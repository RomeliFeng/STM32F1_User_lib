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
extern uint8_t USART1_RX_SP;				//接收缓存器指针
extern uint8_t USART1_Read_SP;			//缓存器读取指针
extern uint8_t USART1_Read_Available;	//缓冲器未读字节
class Serial{
public:
	Serial();
	static void begin(uint32_t BaudRate);
	static void print(char *data);
	static void print(long data);
	static void print(int data);
	static void print(double data,uint8_t ndigit);
	static void println(char *data);
	static void println(long data);
	static void println(int data);
	static void println(double data,uint8_t ndigit);
	static uint8_t print_c(char c);
	static void print_s(char* str);
	static uint8_t available();
	static char read();
	static void read(char *buf,uint8_t len);
private:
};


#endif /* SERIAL_H_ */
