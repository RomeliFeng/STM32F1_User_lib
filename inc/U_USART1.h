/*
 * U_USART1.h
 *
 *  Created on: 2016Äê1ÔÂ18ÈÕ
 *      Author: Romeli
 *      Version: V1.0
 */

#ifndef U_USART1_H_
#define U_USART1_H_

#include "cmsis_device.h"
#include "Parse.h"

//#define USE_DMA

#define USART1_RX_Frame_Size 128
#define USART1_TX_Buf_Size 128
#define USART1_RX_Buf_Size 128

static char NewLine[] = { '\r', '\n' };

class SerialClass: public Parse {
public:
	void begin(uint32_t);

	void print(long data, uint8_t base = 10);
	void print(char data, uint8_t base = 10) {
		print((long) data, base);
	}
	void print(int data, uint8_t base = 10) {
		print((long) data, base);
	}

	void print(double data, uint8_t ndigit = 2);
	void print(float data, uint8_t ndigit = 2) {
		print((double) data, ndigit);
	}

	void println() {
		print(NewLine, 2);
	}
	void println(char *data) {
		print(data);
		println();
	}
	void println(long data, uint8_t base = 10) {
		print(data, base);
		println();
	}
	void println(char data, uint8_t base = 10) {
		println((long) data, base);
	}
	void println(int data, uint8_t base = 10) {
		println((long) data, base);
	}
	void println(double data, uint8_t ndigit = 2) {
		print(data, ndigit);
		println();
	}
	void println(float data, uint8_t ndigit = 2) {
		print((double) data, ndigit);
	}
	void print(char *data) {
		print(data, getlen(data));
	}
	void print(char* data, uint16_t len);
	long nextInt();
	double nextFloat();
	void write(char);
	uint16_t available();
	bool checkFrame();
	char peek();
	char peekNextDigit(bool detectDecimal = false);
	char read();
	void read(char* buf, uint16_t len);
	void flush();
private:
	void DMASend(uint8_t ch);
	uint16_t getlen(char* data);
	void ReadSPInc();
	void ReadSPDec();
};

extern SerialClass Serial;
#endif /* U_USART1_H_ */
