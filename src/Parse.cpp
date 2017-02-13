/*
 * Parse.cpp
 *
 *  Created on: 2016年10月6日
 *      Author: Romeli
 */

#include "Parse.h"

unsigned char Parse::byNumber(unsigned long num, unsigned char base,
		unsigned char* str) {
	unsigned char len = getLen(num, base);
	str += len;
	*str = '\0';
	do {
		char c = num % base;
		num /= base;
		*--str = c < 10 ? c + '0' : c + 'A' - 10;
	} while (num);
	return len;
}

unsigned char Parse::byNumber(long num, unsigned char base,
		unsigned char* str) {
	if (num < 0) {
		num = -num;
		*str++ = '-';
	}
	return byNumber((unsigned long) num, base, str);
}

unsigned char Parse::byFloat(double flo, unsigned char ndigit,
		unsigned char* str) {
	unsigned char len, len2;
	unsigned long int_part;
	double rem_part;
	unsigned char dot[1] = { '0' };
	unsigned char tmp[20];
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
	rem_part = (flo - (double) int_part);
	len = byNumber(int_part, 10, tmp); //转换整数部分并获取字符串长度
	len = strcat(str, 0, tmp, len); //拼接最终字符串并获取长度
	if (ndigit > 0) {
		len = strcat(str, len, dot, 1);
	}

	while (ndigit-- > 0) {
		rem_part *= 10.0;
		int_part = (int) rem_part;
		rem_part -= int_part;
		len2 = byNumber(int_part, 10, tmp);
		len = strcat(str, len, tmp, len2);
	}
	return len;
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

unsigned char strcat(unsigned char* str_to, unsigned char str_to_len,
		unsigned char* str_from, unsigned char str_from_len) {
	unsigned char i;
	for (i = 0; i < str_from_len; ++i) { //循环累加
		str_to[str_to_len + i] = str_from[i]; //在第一个字符串后添加第二个字符串的内容
	}
	str_to[str_to_len + i] = '\0'; //最后加上\0
	return str_to_len + str_from_len; //长度不含\0
}
