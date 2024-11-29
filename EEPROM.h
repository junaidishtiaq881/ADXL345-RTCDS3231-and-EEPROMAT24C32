/*
 * EEPROM.h
 *
 *  Created on: Nov 27, 2024
 *      Author: Admin
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "EEPROM.h"
#include "stdint.h"
#include "math.h"
#include "string.h"
#include "stdio.h"
#include "main.h"

// EEPROM ADDRESS (8 bits)
#define EEPROM_ADDRESS      0x57<<1
#define EEPROM_TIME_ADDRESS 0x0000 // EEPROM address to store the time
//page size and number of pages
#define PAGE_SIZE 32 // in bytes
#define PAGE_NUM 128 // pages
#define EEPROM_I2C &hi2c1


// Functions Prototypes of RTC EEPROM
uint16_t bytes_to_write (uint16_t size, uint16_t offset );
void EEPROM_Write (uint16_t pge,uint16_t offset, uint8_t *data,uint16_t size);
void EEPROM_Read (uint16_t pge,uint16_t offset, uint8_t *data,uint16_t size );
void EEPROM_PageErase (uint16_t pge);

#endif /* INC_EEPROM_H_ */
