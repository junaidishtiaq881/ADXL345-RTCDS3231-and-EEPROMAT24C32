/*
 * RTC.h
 *
 *  Created on: Nov 27, 2024
 *      Author: Admin
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_
#include "math.h"
#include "string.h"
#include "stdio.h"
#include "stm32l4xx_hal.h"
// RTC DS3231 Register Addresses
#define DS3231_ADDR         0x68<<1 // I2C address of RTC DS3231 (0x68 for default configuration)
#define DS3231_REG_SECONDS  0X00    // Second register
#define DS3231_REG_MINUTES  0X01    // Minute register
#define DS3231_REG_HOURS    0X02    // Hour register
#define DS3231_REG_DAY      0X03    // Day register
#define DS3231_REG_MONTH    0X05    // Month register
#define DS3231_REG_YEAR     0X06    // Year register
#define DS3231_REG_CONTROL  0x0E    // Control register
#define DS3231_REG_STATUS   0x0F    // Status register

// RTC DS3231 Function prototypes
uint8_t decToBcd(uint8_t val);
uint8_t bcdToDec(uint8_t val);
uint8_t Read(uint8_t reg);
void Write(uint8_t reg, uint8_t value);
void SetSeconds(uint8_t seconds);
void SetMinutes(uint8_t minutes);
void SetHours(uint8_t hours);
void SetDay(uint8_t day);
void SetMonth(uint8_t month);
void SetYear(uint8_t year);
uint8_t GetSeconds(void);
uint8_t GetMinutes(void);
uint8_t GetHours(void);
uint8_t GetDay(void);
uint8_t GetMonth(void);
uint8_t GetMonth(void);
uint8_t GetYear(void);
HAL_StatusTypeDef DS3231_Init(I2C_HandleTypeDef *hi2c);

#endif /* INC_RTC_H_ */
