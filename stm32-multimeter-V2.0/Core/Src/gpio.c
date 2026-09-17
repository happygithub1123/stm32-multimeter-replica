/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SPI_BL_Pin|SPI_CS_Pin|SPI_RES_Pin|SPI_DC_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CON_100R_Pin|CON_1K_Pin|CON_10K_Pin|CON_100K_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CUR_CON_Pin|RES_CON_Pin|VOL_CON_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BLACK_CON_GPIO_Port, BLACK_CON_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = SPI_BL_Pin|SPI_RES_Pin|SPI_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = SPI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SPI_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = KEY3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin
                           PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = CON_100R_Pin|CON_1K_Pin|CON_10K_Pin|CUR_CON_Pin
                          |CON_100K_Pin|RES_CON_Pin|VOL_CON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = BUZZER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BUZZER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = KEY1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = KEY2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = BLACK_CON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BLACK_CON_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
/*
 * 函数内容：继电器连接到电阻测量端
 * 函数参数：无
 * 返回值：无
 */
void Relay_Connect_RES(void)
{
	//先关闭所有继电器，再打开对应继电器
	HAL_GPIO_WritePin(VOL_CON_GPIO_Port, VOL_CON_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RES_CON_GPIO_Port, RES_CON_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CUR_CON_GPIO_Port, CUR_CON_Pin, GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(RES_CON_GPIO_Port, RES_CON_Pin, GPIO_PIN_SET);
}
/*
 * 函数内容：继电器连接到电压测量端
 * 函数参数：无
 * 返回值：无
 */
void Relay_Connect_VOL(void)
{
	//先关闭所有继电器，再打开对应继电器
	HAL_GPIO_WritePin(VOL_CON_GPIO_Port, VOL_CON_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RES_CON_GPIO_Port, RES_CON_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CUR_CON_GPIO_Port, CUR_CON_Pin, GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(VOL_CON_GPIO_Port, VOL_CON_Pin, GPIO_PIN_SET);
}
/*
 * 函数内容：继电器连接到电流测量端
 * 函数参数：无
 * 返回值：无
 */
void Relay_Connect_CUR(void)
{
	//先关闭所有继电器，再打开对应继电器
	HAL_GPIO_WritePin(VOL_CON_GPIO_Port, VOL_CON_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RES_CON_GPIO_Port, RES_CON_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CUR_CON_GPIO_Port, CUR_CON_Pin, GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(CUR_CON_GPIO_Port, CUR_CON_Pin, GPIO_PIN_SET);
}
/*
 * 函数内容：使公共端连通GND
 * 函数参数：无
 * 返回值：无
 */
void Connect_GND(void)
{
	HAL_GPIO_WritePin(BLACK_CON_GPIO_Port, BLACK_CON_Pin, GPIO_PIN_SET);
}
/*
 * 函数内容：使公共端断开GND
 * 函数参数：无
 * 返回值：无
 */
void DisCon_GND(void)
{
	HAL_GPIO_WritePin(BLACK_CON_GPIO_Port, BLACK_CON_Pin, GPIO_PIN_RESET);
}
/*
 * 函数内容：电阻分压控制IO连接到100R
 * 函数参数：无
 * 返回值：无
 */
void Resostor_Connect_100R(void)
{
    HAL_GPIO_WritePin(CON_100R_GPIO_Port,CON_100R_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CON_1K_GPIO_Port,CON_1K_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(CON_10K_GPIO_Port,CON_10K_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(CON_100K_GPIO_Port,CON_100K_Pin,GPIO_PIN_SET);
}

/*
 * 函数内容：电阻分压控制IO连接到1K
 * 函数参数：无
 * 返回值：无
 */
void Resostor_Connect_1K(void)
{
    HAL_GPIO_WritePin(CON_100R_GPIO_Port,CON_100R_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(CON_1K_GPIO_Port,CON_1K_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CON_10K_GPIO_Port,CON_10K_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(CON_100K_GPIO_Port,CON_100K_Pin,GPIO_PIN_SET);
}

/*
 * 函数内容：电阻分压控制IO连接到10K
 * 函数参数：无
 * 返回值：无
 */
void Resostor_Connect_10K(void)
{
    HAL_GPIO_WritePin(CON_100R_GPIO_Port,CON_100R_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(CON_1K_GPIO_Port,CON_1K_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(CON_10K_GPIO_Port,CON_10K_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CON_100K_GPIO_Port,CON_100K_Pin,GPIO_PIN_SET);
}

/*
 * 函数内容：电阻分压控制IO连接到100K
 * 函数参数：无
 * 返回值：无
 */
void Resostor_Connect_100K(void)
{
    HAL_GPIO_WritePin(CON_100R_GPIO_Port,CON_100R_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(CON_1K_GPIO_Port,CON_1K_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(CON_10K_GPIO_Port,CON_10K_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(CON_100K_GPIO_Port,CON_100K_Pin,GPIO_PIN_RESET);
}

/*
 * 函数内容：打开蜂鸣器
 * 函数参数：无
 * 返回值：无
 */
void Open_Beep(void)
{
	 HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
}

/*
 * 函数内容：关闭蜂鸣器
 * 函数参数：无
 * 返回值：无
 */
void Closer_Beep(void)
{
	 HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_RESET);
}
/* USER CODE END 2 */
