#ifndef __BSP_TFTINIT_H
#define __BSP_TFTINIT_H

#include "main.h"

#define USE_HORIZONTAL 3  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 128
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 128
#endif

#define LCD_RES_Clr()  HAL_GPIO_WritePin(SPI_RES_GPIO_Port,SPI_RES_Pin,GPIO_PIN_RESET)//RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(SPI_RES_GPIO_Port,SPI_RES_Pin,GPIO_PIN_SET)

#define LCD_DC_Clr()   HAL_GPIO_WritePin(SPI_DC_GPIO_Port,SPI_DC_Pin,GPIO_PIN_RESET)//DC
#define LCD_DC_Set()   HAL_GPIO_WritePin(SPI_DC_GPIO_Port,SPI_DC_Pin,GPIO_PIN_SET)
 		     
#define LCD_CS_Clr()   HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_RESET)//CS
#define LCD_CS_Set()   HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_SET)

#define LCD_BLK_Clr()  HAL_GPIO_WritePin(SPI_BL_GPIO_Port,SPI_BL_Pin,GPIO_PIN_RESET)//BLK
#define LCD_BLK_Set()  HAL_GPIO_WritePin(SPI_BL_GPIO_Port,SPI_BL_Pin,GPIO_PIN_SET)


void TFT_WR_DATA8(uint8_t data);
void TFT_WR_DATA(uint16_t data);
void TFT_WR_REG(uint8_t reg);
void TFT_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void TFT_Init(void);

#endif

