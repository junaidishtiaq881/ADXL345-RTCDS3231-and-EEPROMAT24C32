/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ADXL345.h"
#include "RTC.h"
#include "EEPROM.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Global Arrays(Buffer) to reading and writing data from EEPROM
uint8_t dataRead[128] ;
uint8_t dataWrite[100] ;
uint8_t dataW[] = "Hello from the Zero Axis!. This is a second test of the testing phase and lets check whether it's working or not";
static int16_t x,y,z;
// Global Variables of ADXL345
uint8_t flag = 0; // Initially false
uint8_t tapStatus;
// Global Variables of setting time and date of RTC DS3231
uint8_t seconds;
uint8_t minutes;
uint8_t hours;
uint8_t day;
uint8_t month;
uint8_t year;

char timestring[16]; // buffer for concatenating time
char readstring[16]; // buffer for reading time from EEPROM
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  // Initializing ADXL345
  adxl_init();
  // Initializing RTC DS3231
  DS3231_Init(&hi2c1);

  // Who Am I function checking RTC DS3231
   if (HAL_I2C_IsDeviceReady(&hi2c1, DS3231_ADDR,5, HAL_MAX_DELAY)!= HAL_OK){
	   printf("error");
   }
   if (HAL_I2C_IsDeviceReady(&hi2c1, ADXL345_ADDR,5, HAL_MAX_DELAY)!= HAL_OK){
   	   printf("error");
      }
  /* for (int i=0;i<100;i++)
     {
  	  EEPROM_PageErase(i);
    }*/
  /*
     for (int i=0;i<100;i++){
  	  dataWrite[i] =  i+10;
    }*/
   x = adxl_readx();
   y = adxl_ready();
   z = adxl_readz();

   EEPROM_Read(3, 0, (uint8_t *)readstring, 8);
   sscanf(readstring, "%02d:%02d:%02d", &seconds, &minutes, &hours);
   SetSeconds(seconds);
   SetMinutes(minutes);
   SetHours(hours);
    /*HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDRESS, (3<<6), 2, dataWrite, 40, 1000);
    HAL_Delay(5);
    HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDRESS, (3<<6), 2, dataRead, 32, 1000);*/
    // Incrementing Page size by using EEPROM write function
  //  EEPROM_Write(3, 10, dataW, 100);
   // EEPROM_Read(3, 0, dataRead, 128);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  seconds  = GetSeconds();
	  minutes = GetMinutes();
	  hours = GetHours();
	 	// Continuously check for ADXL-tap
		flag = adxl_readtapstatus();// find out why single tap not detecting
	 	if (flag == 1) {
	 		SetSeconds(53);
	 	    SetMinutes(18);
	 	    SetHours(2);
	 	    SetDay(5);
	 	    SetMonth(11);
	 	    SetYear(24);
	 	    seconds  = GetSeconds();
	 	    printf("Current seconds: %d\n", seconds);
	 	    minutes = GetMinutes();
	 	    printf("Current minutes: %d\n", minutes);
	 	    hours = GetHours();
	 	    printf("Current hours: %d\n", hours);

	 	    // Set Date
	 	    day = GetDay();
	 	    printf("Current day: %d\n", day);
	 	    month = GetMonth();
	 	    printf("Current month: %d\n", month);
	 	    year = GetYear();
	 	    printf("Current Year: %02d\n", year);
	 	}
	 	else if (flag == 2){
	 		// storing Current RTC time in EEPROM

	 		sprintf(timestring, "%02d:%02d:%02d",seconds,minutes,hours);
	 	    EEPROM_Write (3, 0, (uint8_t *)timestring, strlen(timestring));

	 	    EEPROM_Read(3, 0, (uint8_t *)readstring, 8);

	 	}


	 	 // Add a small delay to reduce polling frequency
	 	 HAL_Delay(10);


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00303D5B;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
