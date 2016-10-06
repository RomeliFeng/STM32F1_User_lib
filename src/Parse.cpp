/*
 * Parse.cpp
 *
 *  Created on: 2016Äê10ÔÂ6ÈÕ
 *      Author: Romeli
 */

#include "Parse.h"
#include "string.h"

void Parse::byNumber(unsigned long num, unsigned char base, char* str) {
	unsigned char len = getLen(num, base);
	str += len;
	*str = '\0';
	do {
		char c = num % base;
		num /= base;

		*--str = c < 10 ? c + '0' : c + 'A' - 10;
	} while (num);
}

void Parse::byNumber(long num, unsigned char base, char* str) {
	if (num < 0) {
		num = -num;
		*str++ = '-';
	}
	byNumber((unsigned long) num, base, str);
}

void Parse::byFloat(double flo, unsigned char ndigit, char* str) {
	unsigned long int_part;
	unsigned long rem_part;
	char tmp[20];
	// Handle negative flos
	if (flo < 0.0) {
		*str++ = '-';
		flo = -flo;
	}
	*str = '\0';

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;
	for (unsigned char i = 0; i < ndigit; ++i)
		rounding /= 10.0;

	flo += rounding;

	// Extract the integer part of the number and print it
	int_part = (unsigned long) flo;
	rem_part = (unsigned long) ((flo - (double) int_part) * pow10(ndigit));
	byNumber(int_part, 10, tmp);
	strcat(str, tmp);
	strcat(str, ".");
	byNumber(rem_part, 10, tmp);
	strcat(str, tmp);
}

unsigned char Parse::getLen(unsigned long num, unsigned char base) {
	unsigned char i;
	if (num == 0) {
		return 1;
	}
	for (i = 0; num != 0; ++i) {
		num /= base;
	}
	return i;
}

double Parse::pow10(unsigned char power) {
	if (power == 0)
		return 1.0;
	else
		return 10.0 * pow10(--power);
}
