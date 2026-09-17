#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "main.h"

#define KEY_ERROR   0x00
#define KEY_OK      0x01

#define GET_KEY1_IN        HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)
#define GET_KEY2_IN        HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)
#define GET_KEY3_IN        HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)

#define DOUBLE_TIME 40  //400ms内按下两次算双击

#define Key1Press 0x10
#define Key2Press 0x11
#define Key3Press 0x12

#define Key1DoublePress 0x20
#define Key2DoublePress 0x21
#define Key3DoublePress 0x22

#define Key1LongPress  0x30
#define Key2LongPress  0x31
#define Key3LongPress  0x32

#define STATE_TURE	0x01
#define STATE_FALSE	0x00

enum KEYNum
{
    KEY1 = 0,
    KEY2,
		KEY3
};

uint8_t key_scanf(void);
void key_handle(uint8_t keyValue,struct KeyInfo *info);
void Mode_Switch(uint8_t mode);
#endif
