#ifndef __BSP_TASK_H
#define __BSP_TASK_H

#include "main.h"

void Set_ADC_Channel(uint32_t channel);
uint16_t Get_ADC_Average(uint16_t num);
void Clear_TaskData(void);
void Voltage_Task(const uint16_t ref_vol_value);
void Resistor_Task(struct KeyInfo *info,const uint16_t ref_vol_value);
void Electricity_Task(const uint16_t ref_vol_value);
void Buzzing_Task(const uint16_t ref_vol_value);
void Dio_Task(const uint16_t ref_vol_value);
#endif

