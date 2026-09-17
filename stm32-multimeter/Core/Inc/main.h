/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI_BL_Pin GPIO_PIN_0
#define SPI_BL_GPIO_Port GPIOA
#define SPI_CS_Pin GPIO_PIN_1
#define SPI_CS_GPIO_Port GPIOA
#define KEY3_Pin GPIO_PIN_2
#define KEY3_GPIO_Port GPIOA
#define SPI_RES_Pin GPIO_PIN_3
#define SPI_RES_GPIO_Port GPIOA
#define SPI_DC_Pin GPIO_PIN_4
#define SPI_DC_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_5
#define BUZZER_GPIO_Port GPIOC
#define CON_100K_Pin GPIO_PIN_8
#define CON_100K_GPIO_Port GPIOE
#define CON_1K_Pin GPIO_PIN_10
#define CON_1K_GPIO_Port GPIOE
#define CON_100R_Pin GPIO_PIN_11
#define CON_100R_GPIO_Port GPIOE
#define CON_10K_Pin GPIO_PIN_12
#define CON_10K_GPIO_Port GPIOE
#define RES_VOL_CON_Pin GPIO_PIN_12
#define RES_VOL_CON_GPIO_Port GPIOD
#define BLACK_CON_Pin GPIO_PIN_14
#define BLACK_CON_GPIO_Port GPIOD
#define KEY1_Pin GPIO_PIN_9
#define KEY1_GPIO_Port GPIOC
#define KEY2_Pin GPIO_PIN_3
#define KEY2_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

#define Res_Ohm			0x01
#define Res_KiloOhm	0x02
#define Res_TeraOhm	0x03
#define MAX_SAMPNUM 100	

struct KeyInfo
{
	uint8_t mode;						//当前模式
	uint8_t resistor_range;	//电阻量程
};

enum CurrentMode
{
	VOL_MODE = 0,
	RES_MODE = 1,
	CUR_MODE = 2,
	BUZ_MODE = 3,
	DIODE_MODE = 4,
	cur_max_index
};

enum ResistorRange
{
	RES_0_100R = 0,
	RES_100R_1K = 1,
	RES_1K_10K = 2,
	RES_10K_100K = 3,
	res_max_index
};

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
