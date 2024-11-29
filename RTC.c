/*
 * RTC.c
 *
 *  Created on: Nov 27, 2024
 *      Author: Admin
 */
#include "RTC.h"
#include "stdint.h"
#include "math.h"
#include "string.h"
#include "stdio.h"

extern I2C_HandleTypeDef hi2c1;
// Global Variables of setting time and date of RTC DS3231
//uint8_t seconds;
//uint8_t minutes;
//uint8_t hours;
//uint8_t day;
//uint8_t month;
//uint8_t year;

// Helper functions of RTC to convert BCD to binary
uint8_t decToBcd(uint8_t val) {
    return ((val / 10) << 4) + (val % 10);
}
// Function to convert binary to BCD
uint8_t bcdToDec(uint8_t val) {
    return ((val >> 4) * 10) + (val & 0x0F);
}
// Initialize the DS3231 RTC
HAL_StatusTypeDef DS3231_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t data[2];

    // Wake up DS3231 if it is in the "sleep" mode
    data[0] = DS3231_REG_CONTROL;
    data[1] = 0x00;  // Normal mode (turn off the oscillator stop bit)
    return HAL_I2C_Master_Transmit(hi2c, DS3231_ADDR, data, 2, HAL_MAX_DELAY);
}

// Function Definition of getting and setting Time and Date
uint8_t Read(uint8_t reg) {
    uint8_t data;
    HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    return data;
}
void Write(uint8_t reg, uint8_t value) {
    HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 100);
}
void SetSeconds(uint8_t seconds) {
    Write(0x00, decToBcd(seconds));  // Register 0x00 is for seconds
}
void SetMinutes(uint8_t minutes) {
    Write(0x01, decToBcd(minutes));  // Register 0x01 is for minutes
}
void SetHours(uint8_t hours) {
    Write(0x02, decToBcd(hours));  // Register 0x02 is for hours
}
void SetDay(uint8_t day){
	uint8_t day_reg = 0;
	// Ensure the day is in range (1 = Sunday, ..., 7 = Saturday)
	    if (day >= 1 && day <= 7) {
	        day_reg = day;  //  day as 1 = Sunday to 7 = Saturday
	    }
	    // Write the day of the week to the appropriate register (0x03: weekday register in DS3231)
	    Write(DS3231_REG_DAY, decToBcd(day));
}
void SetMonth(uint8_t month){
	uint8_t month_reg = 0;
	// Ensure the month is in range (1 = January, ..., 12 = December)
	if (month >=1 && month <=12){
		month_reg = month;
	}
	Write(DS3231_REG_MONTH, decToBcd(month));
}
void SetYear(uint8_t year){
	uint8_t year_reg  = 0;
	// Ensure the year is in range (00-99)
	if (year>= 00 && year <= 99){
		year_reg = year;
	}
	Write(DS3231_REG_YEAR, decToBcd(year));
}
uint8_t GetYear(void){
	return bcdToDec(Read(DS3231_REG_YEAR));
}
uint8_t GetMonth(void){
	return bcdToDec(Read(DS3231_REG_MONTH));
}
uint8_t GetDay(void){
	return bcdToDec(Read(DS3231_REG_DAY));
}
uint8_t GetSeconds() {
    return bcdToDec(Read(0x00));  // Register 0x00 is for seconds
}
uint8_t GetMinutes(void) {
    return bcdToDec(Read(0x01));  // Register 0x01 is for minutes
}
uint8_t GetHours(void) {
    return bcdToDec(Read(0x02));  // Register 0x02 is for hours
}
// Pre-Setting time and date

