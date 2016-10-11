/*
 * Parse.h
 *
 *  Created on: 2016Äê10ÔÂ6ÈÕ
 *      Author: Romeli
 */

#ifndef PARSE_H_
#define PARSE_H_

class Parse {
public:
	void byNumber(unsigned long num, unsigned char base, char* str);
	inline void byNumber(unsigned int num, unsigned char base,
			char* str) {
		byNumber((unsigned long) num, base, str);
	}
	inline void byNumber(unsigned char num, unsigned char base,
			char* str) {
		byNumber((unsigned long) num, base, str);
	}
	void byNumber(long num, unsigned char base, char* str);
	inline void byNumber(int num, unsigned char base, char* str) {
		byNumber((long) num, base, str);
	}
	inline void byNumber(char num, unsigned char base, char* str) {
		byNumber((long) num, base, str);
	}

	void byFloat(double flo, unsigned char ndigit, char* str);
	inline void byFloat(float flo, unsigned char ndigit, char* str) {
		byFloat((double) flo, ndigit, str);
	}
private:
	unsigned char getLen(unsigned long num, unsigned char base);
	double pow10(unsigned char power);
};

#endif /* PARSE_H_ */
