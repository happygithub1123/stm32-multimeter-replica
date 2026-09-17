#ifndef __BSP_TASK_H
#define __BSP_TASK_H

#include "main.h"

void Clear_TaskData(void);
void Voltage_Task(void);
void Resistor_Task(struct KeyInfo *info);
void Electricity_Task(void);
void Buzzing_Task(void);
void Dio_Task(void);
#endif

