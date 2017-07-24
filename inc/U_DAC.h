/*
 * U_DAC.h
 *
 *  Created on: 2017年7月20日
 *      Author: Romeli
 */

#ifndef U_DAC_H_
#define U_DAC_H_

#include "cmsis_device.h"

class U_DAC {
public:
	static void Init();
	static void RefreshData(float vol);
	static void RefreshData(uint16_t data);
private:
	static void GPIOInit();
	static void DACInit();
};

#endif /* U_DAC_H_ */
