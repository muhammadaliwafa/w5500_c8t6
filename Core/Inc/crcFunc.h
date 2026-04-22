/*
 * crcFunc.h
 *
 *  Created on: Apr 21, 2026
 *      Author: me
 */

#ifndef INC_CRCFUNC_H_
#define INC_CRCFUNC_H_

uint32_t HW_CRC_Calculate(uint32_t *data, uint32_t length);
uint32_t SW_CRC_Calculate(uint32_t *data, uint32_t length);
uint32_t crc32_w25q(uint32_t startAddr);
uint8_t verifikasi(uint32_t startAddr);

#endif /* INC_CRCFUNC_H_ */
