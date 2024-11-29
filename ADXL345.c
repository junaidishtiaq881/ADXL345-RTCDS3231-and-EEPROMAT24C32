/*
 * ADXL345.c
 *
 *  Created on: Nov 27, 2024
 *      Author: Admin
 */
#include "ADXL345.h"
#include "stdint.h"
#include "math.h"
#include "string.h"
#include "stdio.h"

extern I2C_HandleTypeDef hi2c1;
// Global Variables of ADXL345
int16_t x,y,z;
uint8_t data_rec[6];

// Functions Definitions of ADXL345
uint8_t read_register(uint8_t reg) {
	uint8_t value;
	    HAL_I2C_Master_Transmit(&hi2c1, ADXL345_ADDR, &reg, 1, HAL_MAX_DELAY);
	    HAL_I2C_Master_Receive(&hi2c1, ADXL345_ADDR, &value, 1, HAL_MAX_DELAY);
	    return value;
}
void adxl_write (uint8_t reg, uint8_t value)
{
	uint8_t data[2] = {reg, value};
	HAL_I2C_Master_Transmit(&hi2c1, ADXL345_ADDR, data, 2, HAL_MAX_DELAY);
}
void adxl_read_values (uint8_t reg)
{
	HAL_I2C_Mem_Read (&hi2c1, ADXL345_ADDR, reg, 1, (uint8_t *)data_rec, 6, 100);
}
void adxl_init (void)
{
	read_register (ADXL345_REG_ADDRESS); // read the DEVID

	adxl_write (ADXL345_REG_DATA_FORMAT, 0x01);  // data_format range= +- 4g --> for that setting  0 1
	adxl_write (ADXL345_POWER_CTL, 0x00);   // resetting all bits of PWR_CTRL register
	adxl_write (ADXL345_POWER_CTL, 0x08);   // PWR_CTRL measure and wake up at 8hz frequency(8 samples per second), setting 3rd bit of power_ctrl reg
	adxl_write (ADXL345_THRESH_TAP, 200);  // setting a tap threshold to 80*62.5mg = ~5g
	adxl_write (ADXL345_TAP_DURATION, 10); // adjusting a tap duration to 20*625micros = 12.5 ms
	adxl_write (ADXL345_TAP_LATENCY, 80);  // setting tap latency 80*1.25ms =  ~100 ms
	adxl_write (ADXL345_TAP_WINDOW, 240);   // setting tap window to 200*1.25ms = 250ms
	adxl_write (ADXL345_TAP_AXES, 0x07);    // enabling tap detection on all axes
	adxl_write (ADXL345_INT_ENABLE, 0x20); // enabling single & double tap interrup
	adxl_write (0x2F, 0x00);
}
uint8_t  adxl_readtapstatus(void){
	// Read the INT_SOURCE register
	    uint8_t int_source = read_register(ADXL345_REG_INT_SOURCE);
	    // Check if single-tap bit is set
	        if (int_source & 0x40) {
	            // Single-tap detected
	        	 return 1;
	        }
	        else if(int_source & 0x20)
	        {
	        	// Double tap detected
	        	return 2;
	        }

	        return 0;
}

int16_t adxl_readx(void)
{
	int16_t x;
	adxl_read_values (ADXL345_REG_DATAX0);
	x = ((data_rec[1]<<8)|data_rec[0]);
	return x;
}
int16_t adxl_ready(void)
{
	int16_t y;
	adxl_read_values (ADXL345_REG_DATAY0);
	y = ((data_rec[3]<<8)|data_rec[2]);
	return y;
}
int16_t adxl_readz(void)
{
	int16_t z;
	adxl_read_values (ADXL345_REG_DATAZ0);
	z = ((data_rec[5]<<8)|data_rec[4]);
	return z;
}


