/*
 * U_USART1.cpp
 *
 *  Created on: 2016锟斤拷1锟斤拷18锟斤拷
 *      Author: Romeli
 */
#include "U_USART1.h"

#include <stdlib.h>
#include <stdio.h>

SerialClass Serial;

extern void Serial_Event();

typedef struct _SendBuf_Typedef {
	uint8_t data[USART1_TX_Buf_Size]; //data
	uint16_t sp = 0; //data sp
	bool busy = false; //busy flag
} SendBuf_Typedef;

typedef struct _ReceiveBuf_Typedef {
	uint8_t data[USART1_RX_Buf_Size]; //data
	uint16_t sp = 0; //data sp
	uint16_t read_sp = 0; //read sp
	bool frame = false; //new frame received flag
} ReceiveBuf_Typedef;

volatile static SendBuf_Typedef Tx_Buf;
volatile static ReceiveBuf_Typedef Rx_Buf;

#ifdef USE_DMA
volatile static SendBuf_Typedef Tx_Buf2;
volatile static bool DMA_Tx_Busy = false;
volatile static uint8_t DMA_Tx_Ch = 1; //1:Tx_Buf.data;2:Tx_Buf2.data
volatile static uint8_t DMA_Tx_Buf[USART1_RX_Buf_Size];
#endif

void SerialClass::begin(uint32_t BaudRate) {
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//寮�鍚疷SART1鍜孏PIOA鏃堕挓
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,
			ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	//璁剧疆PA9澶嶇敤杈撳嚭妯″紡锛圱X锛�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//璁剧疆PA10娴┖杈撳叆妯″紡锛圧X锛�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//閰嶇疆USART1 鍏ㄥ弻宸� 鍋滄浣�1 鏃犳牎楠�
	USART_DeInit(USART1);
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART1, &USART_InitStructure);

#ifdef USE_DMA
	DMA_InitTypeDef DMA_InitStructure;

	//寮�鍚疍MA鏃堕挓
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) Tx_Buf.data;//涓存椂璁剧疆锛屾棤鏁�
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 10;//涓存椂璁剧疆锛屾棤鏁�
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	//鍙戦�丏MA涓嶅紑鍚�
//	DMA_Cmd(DMA1_Channel4, ENABLE);

	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) DMA_Tx_Buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = USART1_RX_Buf_Size;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel5, ENABLE);

	//閰嶇疆DMA涓柇
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	//涓插彛鍙戦�佹帴鏀剁殑DMA鍔熻兘
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
#endif

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;

	NVIC_Init(&NVIC_InitStructure);

#ifndef USE_DMA
	//寮�鍚覆鍙ｇ殑瀛楄妭鎺ユ敹涓柇
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif
	//寮�鍚覆鍙ｇ殑甯ф帴鏀朵腑鏂�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	USART_Cmd(USART1, ENABLE);
}

void SerialClass::print(long data, uint8_t base) {
	uint8_t str[20];
	uint8_t len = byNumber(data, base, str);
	print(str, len);
}
void SerialClass::print(double data, uint8_t ndigit) {
	uint8_t str[20];
	uint8_t len = byFloat(data, ndigit, str);
	print(str, len);
}

void SerialClass::print(uint8_t *data, uint16_t len) {
#ifdef USE_DMA
	//鍒ゆ柇褰撳墠鏄惁姝ｅ湪鍙戦��
	if (DMA_Tx_Busy) {
		//鏍规嵁姝ｅ湪浣跨敤鐨勭紦鍐插尯杞崲鍒扮┖闂茬紦鍐插尯
		switch (DMA_Tx_Ch) {
		case 1:
			//缂撳啿鍖�1姝ｅ湪浣跨敤涓紝濉厖缂撳啿鍖�2
			//缃綅缂撳啿鍖�2蹇欐爣蹇楋紝闃叉鍏朵粬涓柇璇慨鏀�
			SWCH_1: Tx_Buf2.busy = true;
			while (len--) {
				//閫愬瓧鑺傛惉杩愭暟鎹�
				Tx_Buf2.data[Tx_Buf2.sp++] = *data++;
				//鍒ゆ柇缂撳啿鍖�2鏄惁宸叉弧锛堥渶瑕侀伩鍏嶆鐘跺喌锛�
				if (Tx_Buf2.sp == USART1_TX_Buf_Size) {
					//缂撳啿鍖�2宸叉弧锛屽叧闂紦鍐插尯2蹇欐爣蹇�
					Tx_Buf2.busy = false;
					//鍒ゆ柇鏄惁姝ｅ湪鍙戦��
					if (!DMA_Tx_Busy) {
						//鍙戦�侀槦鍒楃┖闂诧紝鍏堝皢婊＄殑缂撳啿鍖�2鍙戦�佸嚭鍘�
						DMASend(2);
					} else {
						//鍙戦�佷腑锛岀瓑寰呭彂閫佸畬鎴�
						while (DMA_Tx_Ch != 2)
							;
					}
					//璺宠浆鍒扮紦鍐插尯1缁х画濉厖
					goto SWCH_2;
				}
			}
			//缂撳啿鍖�2濉厖瀹屾垚锛屽叧闂紦鍐插尯2蹇欐爣蹇�
			Tx_Buf2.busy = false;
			//濡傛灉鍙戦�佸繖鏍囧織鏈疆浣嶏紝绔嬪嵆鍙戦�佺紦鍐插尯2鐨勬暟鎹�
			if (!DMA_Tx_Busy) {
				//浣跨敤缂撳啿鍖�2鍙戦�佹暟鎹�
				DMASend(2);
			}
			//鑻ュ彂閫侀槦鍒楀繖锛屽皢鍦ㄥ彂閫佸畬褰撳墠闃熷垪鍚庤嚜鍔ㄨ浇鍏�
			break;
		case 2:
			//缂撳啿鍖�2姝ｅ湪浣跨敤涓紝濉厖缂撳啿鍖�1
			//缃綅缂撳啿鍖�1蹇欐爣蹇楋紝闃叉鍏朵粬涓柇璇慨鏀�
			SWCH_2: Tx_Buf.busy = true;
			while (len--) {
				//閫愬瓧鑺傛惉杩愭暟鎹�
				Tx_Buf.data[Tx_Buf.sp++] = *data++;
				//鍒ゆ柇缂撳啿鍖�1鏄惁宸叉弧锛堥渶瑕侀伩鍏嶆鐘跺喌锛�
				if (Tx_Buf.sp == USART1_TX_Buf_Size) {
					//缂撳啿鍖�1宸叉弧锛屽叧闂紦鍐插尯1蹇欐爣蹇�
					Tx_Buf.busy = false;
					//鍒ゆ柇鏄惁姝ｅ湪鍙戦��
					if (!DMA_Tx_Busy) {
						//鍙戦�侀槦鍒楃┖闂诧紝鍏堝皢婊＄殑缂撳啿鍖�1鍙戦�佸嚭鍘�
						DMASend(1);
					} else {
						//鍙戦�佷腑锛岀瓑寰呭彂閫佸畬鎴�
						while (DMA_Tx_Ch != 1)
							;
					}
					//璺宠浆鍒扮紦鍐插尯2缁х画濉厖
					goto SWCH_1;
				}
			}
			//缂撳啿鍖�1濉厖瀹屾垚锛屽叧闂紦鍐插尯2蹇欐爣蹇�
			Tx_Buf.busy = false;
			//濡傛灉鍙戦�佸繖鏍囧織鏈疆浣嶏紝绔嬪嵆鍙戦�佺紦鍐插尯2鐨勬暟鎹�
			if (!DMA_Tx_Busy) {
				DMASend(1);
			}
			//鑻ュ彂閫侀槦鍒楀繖锛屽皢鍦ㄥ彂閫佸畬褰撳墠闃熷垪鍚庤嚜鍔ㄨ浇鍏�
			break;
		default:
			break;
		}
	} else {
		//DMA绌洪棽锛岀洿鎺ヤ娇鐢ㄧ紦鍐插尯1鍙戦�佹暟鎹�
		while (len--) {
			Tx_Buf.data[Tx_Buf.sp++] = *data++;
		}
		DMASend(1);
	}
#else
	while (len--) {
		write(*data++);
	}
#endif
}

#ifdef USE_DMA
void SerialClass::DMASend(uint8_t ch) {
	volatile uint8_t *TX_Buf_Add;
	uint16_t TX_Len;
	switch (ch) {
	case 1:
		TX_Buf_Add = Tx_Buf.data;
		TX_Len = Tx_Buf.sp;
		break;
	case 2:
		TX_Buf_Add = Tx_Buf2.data;
		TX_Len = Tx_Buf2.sp;
		break;
	default:
		TX_Buf_Add = Tx_Buf.data;
		TX_Len = Tx_Buf.sp;
		break;
	}
	DMA_Tx_Ch = ch;
	DMA_Tx_Busy = true;
	DMA1_Channel4->CMAR = (uint32_t) TX_Buf_Add;
	DMA1_Channel4->CNDTR = TX_Len;
	DMA_Cmd(DMA1_Channel4, ENABLE);
}
#endif

//涓插彛鍙戦�佷竴涓瓧鑺�
void SerialClass::write(uint8_t c) {
#ifdef USE_DMA
	print(&c, 1);
#else
	USART1->DR = (c & (uint16_t) 0x01FF);
	while (!(USART1->SR & USART_FLAG_TXE))
	;
#endif
}

//璇诲彇涓�涓瓧鑺傛暟鎹紝涓嶇Щ鍔ㄦ寚閽�
uint8_t SerialClass::peek() {
	if (Rx_Buf.read_sp == Rx_Buf.sp)
		return -1;
	else
		return Rx_Buf.data[Rx_Buf.read_sp];
}

uint8_t SerialClass::peekNextDigit(bool detectDecimal) {
	char data = peek();
	if (data == '-') {
		ReadSPInc();
		return data;
	} else if (data >= '0' && data <= '9') {	//is digit
		ReadSPInc();
		return data;
	} else if (data == '.' && detectDecimal) {
		ReadSPInc();
		return data;
	} else
		return -1;
}

uint8_t SerialClass::read() {
	uint8_t data = peek();
	ReadSPInc();
	return data;
}

void SerialClass::read(uint8_t *buf, uint16_t len) {
	while (len--) {
		*buf++ = read();
	}
}

long SerialClass::nextInt() {
	long data = 0;
	bool isNeg = false;
	char c;
	while (available() > 0) {
		c = peekNextDigit();
		if (c == -1) {
			break;
		} else if (c == '-') {
			if (isNeg) {
				ReadSPDec();
				break;
			}

			else {
				isNeg = true;
				continue;
			}
		}
		data = data * 10 + c - '0';
	}
	return isNeg ? -data : data;
}

double SerialClass::nextFloat() {
	double data = 0.0;
	double frac = 1.0;
	bool isNeg = false;
	bool isFra = false;
	char c;
	while (available() > 0) {
		c = peekNextDigit(true);
		if (c == -1) {
			break;
		} else if (c == '-') {
			if (isNeg) {
				ReadSPDec();
				break;
			}

			else {
				isNeg = true;
				continue;
			}
		} else if (c == '.') {
			if (isFra) {
				ReadSPDec();
				break;
			} else {
				isFra = true;
				continue;
			}
		}
		if (isFra) {
			frac *= 0.1;
		}
		data = data * 10 + c - '0';
	}
	data = isNeg ? -data : data;
	data = isFra ? data * frac : data;
	return data;
}

//璁＄畻骞惰繑鍥炰覆鍙ｇ紦鍐蹭腑鍓╀綑鏈瀛楄妭
uint16_t SerialClass::available() {
	return Rx_Buf.sp >= Rx_Buf.read_sp ? Rx_Buf.sp - Rx_Buf.read_sp :
	USART1_RX_Buf_Size - Rx_Buf.read_sp + Rx_Buf.sp;
}

//鍒ゆ柇甯ф帴鏀舵爣蹇�
bool SerialClass::checkFrame() {
	if (Rx_Buf.frame != 0) {
		Rx_Buf.frame = 0;
		return true;
	} else
		return false;
}

//鍒ゆ柇鍙戦�佸繖鏍囧織
bool SerialClass::checkBusy() {
	return DMA_Tx_Busy;
}

//灏嗚鍙栨寚閽堣缃负鎺ユ敹缂撳啿鎸囬拡锛屼涪寮冧箣鍓嶇殑鏁版嵁
void SerialClass::clear() {
	Rx_Buf.read_sp = Rx_Buf.sp;
}

uint16_t SerialClass::getlen(uint8_t* data) {
	uint16_t len = 0;
	while (*data++ != '\0')
		len++;
	return len;
}

//涓插彛璇诲彇鎸囬拡+1
inline void SerialClass::ReadSPInc() {
	if (Rx_Buf.read_sp != Rx_Buf.sp)
		++Rx_Buf.read_sp;
	if (Rx_Buf.read_sp == USART1_RX_Buf_Size) {
		Rx_Buf.read_sp = 0;
	}
}

//涓插彛璇诲彇鎸囬拡-1
inline void SerialClass::ReadSPDec() {
	if (Rx_Buf.read_sp == 0) {
		Rx_Buf.read_sp = USART1_RX_Buf_Size - 1;
	} else {
		--Rx_Buf.read_sp;
	}
}

//涓插彛鎺ユ敹涓柇
extern "C" void USART1_IRQHandler(void) {
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
		Rx_Buf.frame = 1;
		USART_ReceiveData(USART1);
#ifdef USE_DMA
		//鍏抽棴DMA鎺ユ敹
		DMA_Cmd(DMA1_Channel5, DISABLE);
		uint16_t len = USART1_RX_Buf_Size - DMA1_Channel5->CNDTR;
		//娓呴櫎DMA鏍囧織
		DMA_ClearFlag(
		DMA1_FLAG_GL5 | DMA1_FLAG_TC5 | DMA1_FLAG_TE5 | DMA1_FLAG_HT5);
		//澶嶄綅DMA鎺ユ敹鍖哄ぇ灏�
		DMA1_Channel5->CNDTR = USART1_RX_Buf_Size;
		//寰幆鎼繍鏁版嵁
		for (uint16_t i = 0; i < len; ++i) {
			Rx_Buf.data[Rx_Buf.sp++] = DMA_Tx_Buf[i];
			if (Rx_Buf.sp == USART1_RX_Buf_Size) {
				Rx_Buf.sp = 0;
			}
		}
		//寮�鍚疍MA鎺ユ敹
		DMA_Cmd(DMA1_Channel5, ENABLE);
#endif
		//涓插彛甯ф帴鏀朵簨浠�
		Serial_Event();
	}
#ifndef USE_DMA
	//涓插彛瀛楄妭鎺ユ敹涓柇缃綅
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		//鎼繍鏁版嵁鍒扮紦鍐插尯
		Rx_Buf.data[Rx_Buf.sp] = USART1->DR;
		Rx_Buf.sp++;
		if (Rx_Buf.sp == USART1_RX_Buf_Size)
		Rx_Buf.sp = 0;
	}
#endif
	//涓插彛甯ч敊璇腑鏂�
	if ((USART1->SR & USART_FLAG_ORE) != (uint16_t) RESET)
		USART_ReceiveData(USART1);
}

#ifdef USE_DMA
//DMA涓插彛鍙戦�佸畬鎴愪腑鏂�
extern "C" void DMA1_Channel4_IRQHandler(void) {
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_ClearFlag(DMA1_FLAG_TC4);
	//鍒ゆ柇褰撳墠浣跨敤鐨勭紦鍐查�氶亾
	switch (DMA_Tx_Ch) {
	case 1:
		//缂撳啿鍖�1鍙戦�佸畬鎴愶紝缃綅鎸囬拡
		Tx_Buf.sp = 0;
		//鍒ゆ柇缂撳啿鍖�2鏄惁鏈夋暟鎹紝骞朵笖蹇欐爣蹇楁湭缃綅锛堥槻姝㈠～鍏呭埌涓�鍗婂彂閫佸嚭鍘伙級
		if (Tx_Buf2.sp != 0 && Tx_Buf2.busy == false) {
			//褰撳墠浣跨敤缂撳啿鍖哄垏鎹负缂撳啿鍖�2锛屽苟鍔犺浇DMA鍙戦��
			DMA_Tx_Ch = 2;
			DMA1_Channel4->CMAR = (uint32_t) Tx_Buf2.data;
			DMA1_Channel4->CNDTR = Tx_Buf2.sp;
			DMA_Cmd(DMA1_Channel4, ENABLE);
		} else {
			//鏃犳暟鎹渶瑕佸彂閫侊紝娓呴櫎鍙戦�侀槦鍒楀繖鏍囧織
			DMA_Tx_Busy = false;
		}
		break;
	case 2:
		//缂撳啿鍖�2鍙戦�佸畬鎴愶紝缃綅鎸囬拡
		Tx_Buf2.sp = 0;
		//鍒ゆ柇缂撳啿鍖�1鏄惁鏈夋暟鎹紝骞朵笖蹇欐爣蹇楁湭缃綅锛堥槻姝㈠～鍏呭埌涓�鍗婂彂閫佸嚭鍘伙級
		if (Tx_Buf.sp != 0 && Tx_Buf2.busy == false) {
			//褰撳墠浣跨敤缂撳啿鍖哄垏鎹负缂撳啿鍖�1锛屽苟鍔犺浇DMA鍙戦��
			DMA_Tx_Ch = 1;
			DMA1_Channel4->CMAR = (uint32_t) Tx_Buf.data;
			DMA1_Channel4->CNDTR = Tx_Buf.sp;
			DMA_Cmd(DMA1_Channel4, ENABLE);
		} else {
			//鏃犳暟鎹渶瑕佸彂閫侊紝娓呴櫎鍙戦�侀槦鍒楀繖鏍囧織
			DMA_Tx_Busy = false;
		}
		break;
	default:
		break;
	}
}

#endif

void __attribute__((weak)) Serial_Event() {

}

