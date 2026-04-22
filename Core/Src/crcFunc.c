/*
 * crcFunc.c
 *
 *  Created on: Apr 21, 2026
 *      Author: me
 */

#include "main.h"
#include "crcFunc.h"
#include "W25Qxx.h"

uint32_t HW_CRC_Calculate(uint32_t *data, uint32_t length)
{
    uint32_t i;

    /* Reset CRC data register */
    CRC->CR = CRC_CR_RESET;

    /* Calculate CRC */
    for(i = 0; i < length; i++)
    {
        CRC->DR = data[i];
    }

    /* Return CRC value */
    return CRC->DR;
}

uint32_t SW_CRC_Calculate(uint32_t *data, uint32_t length)
{
    uint32_t crc = 0xFFFFFFFF; // Initial value
    uint32_t polynomial = 0x04C11DB7; // STM32 Hardware CRC polynomial
    uint32_t i, j;

    for(i = 0; i < length; i++)
    {
        crc ^= data[i]; // XOR with input data

        // Process 32 bits
        for(j = 0; j < 32; j++)
        {
            if(crc & 0x80000000)
            {
                crc = (crc << 1) ^ polynomial;
            }
            else
            {
                crc = (crc << 1);
            }
        }
    }

    return crc;
}

uint32_t crc32_w25q(uint32_t startAddr)
{
    uint32_t addr = startAddr+10;
    uint32_t remaining = w25q_read_u32(startAddr);

    uint8_t w25q_buf[1024];
//    printf("hasil baca crc %08X\n", crc_read);

    // Reset CRC hardware
    CRC->CR = CRC_CR_RESET;

    while (remaining > 0)
    {

        uint32_t chunk = (remaining >= 1024) ? 1024 : remaining;

        // Baca dari W25Q
        W25Q_FastRead(addr/256, addr%256, chunk, w25q_buf);


        // Hitung jumlah word 32-bit
        uint32_t wordCount = chunk / 4;
        uint32_t leftover  = chunk % 4;

//        CRC->CR = CRC_CR_RESET;
        // Proses data 32-bit aligned dulu
        uint32_t *w = (uint32_t *)w25q_buf;
//        printf("data : %08X\n", w[0]);
        for (uint32_t i = 0; i < wordCount; i++)
        {
            CRC->DR = w[i];
        }

        // Jika ada sisa byte (1–3 byte)
        if (leftover)
        {
            uint32_t last = 0xFFFFFFFF;

            uint8_t *b = &w25q_buf[wordCount * 4];
            for (uint32_t i = 0; i < leftover; i++)
            {
                ((uint8_t *)&last)[i] = b[i];
            }

            CRC->DR = last;
        }

        addr += chunk;
        remaining -= chunk;
    }

//    if(crc_read==CRC->DR){
//    	printf("HOREEEE...\n");
//    }

    // Hasil akhir CRC32
    return CRC->DR;
}

uint8_t verifikasi(uint32_t startAddr){
	uint32_t crc_read = w25q_read_u32(startAddr+4);
	uint32_t crc = crc32_w25q(startAddr);
	return crc==crc_read?1:0;
}
