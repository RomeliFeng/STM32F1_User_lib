/*
 * HardwareSerial.h
 *
 *  Created on: 2016Äê1ÔÂ18ÈÕ
 *      Author: Romeli
 *      Version: V1.0
 */

#ifndef HARDWARESERIAL_H_
#define HARDWARESERIAL_H_

#include "stm32f10x.h"
#include "Parse.h"

#define USE_DMA

#define USART1_RX_Frame_Size 64
#define USART1_TX_Buf_Size 128
#define USART1_RX_Buf_Size 128

static char NewLine[] = { '\r', '\n', '\0' };

class SerialClass: public Parse {
public:
	void begin(uint32_t);

	void print(long data, uint8_t base = 10);
	inline void print(char data, uint8_t base = 10) {
		print((long) data, base);
	}
	inline void print(int data, uint8_t base = 10) {
		print((long) data, base);
	}

	void print(double data, uint8_t ndigit = 2);
	inline void print(float data, uint8_t ndigit = 2) {
		print((double) data, ndigit);
	}

	inline void println() {
		print(NewLine, 2);
	}
	inline void println(char *data) {
		print(data);
		println();
	}
	inline void println(long data, uint8_t base = 10) {
		print(data, base);
		println();
	}
	inline void println(char data, uint8_t base = 10) {
		println((long) data, base);
	}
	inline void println(int data, uint8_t base = 10) {
		println((long) data, base);
	}
	inline void println(double data, uint8_t ndigit = 2) {
		print(data, ndigit);
		println();
	}
	inline void println(float data, uint8_t ndigit = 2) {
		print((double) data, ndigit);
	}
	void print(char *data) {
		print(data, getlen(data));
	}
	long nextInt();
	double nextFloat();
	void write(char);
	uint8_t available();
	bool checkFrame();
	char peek();
	char peekNextDigit(bool detectDecimal = false);
	char read();
	void read(char* buf, uint8_t len);
	void flush();
private:
	void print(char* data, uint8_t len);
	uint8_t getlen(char* data);
	void ReadSPInc();
	void ReadSPDec();
};

extern SerialClass Serial;
#endif /* HARDWARESERIAL_H_ */
