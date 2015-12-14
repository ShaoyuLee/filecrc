#include "stm32f4xx_crc.h"
#include <stdint.h>
typedef uint32_t DWORD;
DWORD dwPolynomial = 0x04c11db7;
DWORD    CRC = 0xFFFFFFFF;    // init
DWORD cal_crc(DWORD *ptr, int len)
{
	DWORD    xbit;
	DWORD    data;
	
	while (len--) {
		xbit = 1 << 31;

		data = *ptr++;
		for (int bits = 0; bits < 32; bits++) {
			if (CRC & 0x80000000) {
				CRC <<= 1;
				CRC ^= dwPolynomial;
			}
			else
				CRC <<= 1;
			if (data & xbit)
				CRC ^= dwPolynomial;

			xbit >>= 1;
		}
	}
	return CRC;
}


void CRC_enable(void){}
void CRC_disable(void){}
void CRC_ResetDR(void)
{
	CRC = 0xFFFFFFFF;
}
uint32_t CRC_CalcCRC(uint32_t Data);
uint32_t CRC_CalcBlockCRC(uint32_t pBuffer[], uint32_t BufferLength)
{
	uint32_t crc;
	crc = cal_crc(pBuffer, BufferLength);
	return crc;
}
uint32_t CRC_GetCRC(void);
void CRC_SetIDRegister(uint8_t IDValue);
uint8_t CRC_GetIDRegister(void);