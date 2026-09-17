#include "bsp_timer.h"

volatile uint16_t keyTimer;			//按键定时器
volatile uint16_t wdgTimer;			//看门狗定时器

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3)
	{
		keyTimer++;
		if(keyTimer >= 10000){
			keyTimer = 0;		//防止数据越界
		}
		wdgTimer++;
		if(wdgTimer >= 10000){
			wdgTimer = 0;		//防止数据越界
		}
	}
}
