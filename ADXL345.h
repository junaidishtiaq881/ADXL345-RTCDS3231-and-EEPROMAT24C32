/*
 * ADXL345.h
 *
 *  Created on: Nov 27, 2024
 *      Author: Admin
 */

#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_
#include "math.h"
#include "string.h"
#include "stdio.h"
#include "stm32l4xx_hal.h"


// ADXL345 Register Addresses
#define ADXL345_ADDR                 0x53 << 1
#define ADXL345_REG_ADDRESS          0x00 // Device ID
#define ADXL345_THRESH_TAP           0x1D // Tap Threshold
#define ADXL345_TAP_DURATION         0x21 // Tap Duration
#define ADXL345_TAP_LATENCY          0x22 // Tap Latency
#define ADXL345_TAP_WINDOW           0x23 // Tap Window
#define ADXL345_TAP_AXES             0x2A // Axis Control for Single/Double Tap
#define ADXL345_ACT_TAP_STATUS       0x2B // Source of Tap/Double Tap
#define ADXL345_DATARATEPWRMODECTRL  0x2C // Data Rate and power mode control
#define ADXL345_POWER_CTL            0x2D // Power Save features control
#define ADXL345_INT_ENABLE           0x2E // Interrupt enable control
#define ADXL345_INT_MAP              0x2F // Interrupt mapping control
#define ADXL345_REG_INT_SOURCE       0x30 // Source of interrupts
#define ADXL345_REG_DATA_FORMAT      0x31 // Data Format Control
#define ADXL345_REG_DATAX0           0x32 //X axis data
#define ADXL345_REG_DATAX1           0x33
#define ADXL345_REG_DATAY0           0x34 //  Y axis data
#define ADXL345_REG_DATAY1           0x35
#define ADXL345_REG_DATAZ0           0x36 //  Z axis data
#define ADXL345_REG_DATAZ1           0x37

// ADXL345 Function Prototypes
void adxl_write (uint8_t reg, uint8_t value);
void adxl_read_values (uint8_t reg);
void adxl_read_address (uint8_t reg);
void adxl_init (void);
int16_t adxl_readx(void);
int16_t adxl_ready(void);
int16_t adxl_readz(void);
uint8_t adxl_readtapstatus(void);
uint8_t read_register(uint8_t reg);


#endif /* INC_ADXL345_H_ */
