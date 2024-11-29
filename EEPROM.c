/*
 * EEPROM.c
 *
 *  Created on: Nov 27, 2024
 *      Author: Admin
 */
#include "EEPROM.h"
#include "stdint.h"
#include "math.h"
#include "string.h"
#include "stdio.h"

// Global Arrays(Buffer) to reading and writing data from EEPROM
static uint8_t dataRead[128] ;
static uint8_t dataWrite[100] ;
static uint8_t dataW[] = "Hello from the Zero Axis!. This is a second test of the testing phase and lets check whether it's working or not";
// Functions Definitions of EEPROM
uint16_t bytes_to_write (uint16_t size,uint16_t offset )
{
	if ((size+offset)< PAGE_SIZE){
		return size; }
		else return (PAGE_SIZE-offset);
}
void EEPROM_Write (uint16_t pge,uint16_t offset, uint8_t *data,uint16_t size )
{
	int paddrposition = log2(PAGE_SIZE); // finding out the no of bits where the page addressing starts
	uint16_t strtpge = pge;              // calculating the start and end page
	uint16_t endpge = pge + ((size+offset)/PAGE_SIZE);
	uint16_t numofpages = (endpge-strtpge)+ 1; // no of pages to be written
	uint16_t position = 0;

	// calculating the data
	for(int i=0; i<numofpages;i++){
		uint16_t memaddr = strtpge<<paddrposition | offset;  // calculating address of memory location and adding pge address with byte address
		uint16_t bytesrem = bytes_to_write(size,offset);       // calculating remaining bytes to be written

		HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDRESS, memaddr, 2, &data[position], bytesrem, 1000); // writing data to the EEPROM

		strtpge += 1; // incrementing the page, so that new page address can be selected for further write
		offset = 0;  // while writing on a new page so offset will be 0
		size = size - bytesrem; // reducing the size of bytes
		position += bytesrem;
		HAL_Delay(5);    // delay 5ms according to datasheet
	}
}
void EEPROM_Read (uint16_t pge,uint16_t offset,uint8_t *data, uint16_t size)
{
	int paddrposition = log2(PAGE_SIZE);
	uint16_t strtpge = pge;
	uint16_t endpge = pge + ((size+offset)/PAGE_SIZE);
	uint16_t numofpages = (endpge - strtpge)+ 1;
	uint16_t position = 0;

	for(int i=0; i<numofpages;i++){
		uint16_t memaddr = strtpge<<paddrposition | offset;
		uint16_t bytesrem = bytes_to_write(size, offset);
		HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDRESS, memaddr, 2, &data[position], bytesrem, 1000);
		strtpge += 1 ;
		offset = 0;
		size = size - bytesrem;
		position += bytesrem;
	}
}
void EEPROM_PageErase (uint16_t pge)
{
	// calculating memory address based on the page no
	int paddrposition = log2(PAGE_SIZE);
	uint16_t memaddr = pge<<paddrposition;

	// creating the buffer to store the reset values
	uint8_t data[PAGE_SIZE];
	memset(data, 0xff, PAGE_SIZE);

	//writing the data to the EEPROM
	HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDRESS, memaddr, 2, data, PAGE_SIZE, 1000);

	HAL_Delay(5);

}


