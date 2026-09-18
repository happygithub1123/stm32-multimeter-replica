#include "bsp_task.h"
#include "adc.h"
#include "gpio.h"
#include "bsp_tft.h"
#include "stdio.h"
#include "string.h"

//参考电压值，这个需要实际测量，理想分压应该为1.65v
#define VREF_VALUE	1.665F

//分压电阻上的电压值，，需要实际测量，理想应该为3.3V
#define RES_VOL_VALUE 3.287F

static float sum_tempValue[100] = {0};	//临时存放数据
static uint16_t tempNum = 0;						//数组下标


/*
 * 函数内容：冒泡排序
 * 函数参数：无
 * 返回值：无
 */
static void bubble_sort(float arr[], uint16_t len)
{
    uint16_t i = 0, j = 0;
    float temp = 0;
    for (i = 0; i < len - 1; i++)
    {
        for (j = 0; j < len - 1 - i; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
/*
 * 函数内容；得到对应ADC通道值
 * 函数参数：无
 * 返回值：无
 */
static uint16_t Get_ADC_Val(void)
{
    uint16_t val = 0;

    HAL_ADC_Start(&hadc1);     //启动ADC转换
    HAL_ADC_PollForConversion(&hadc1, 50);   //等待转换完成，50为最大等待时间，单位为ms

    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
		{
			val = HAL_ADC_GetValue(&hadc1);   //获取AD值
		}

    return val;
}

/*
 * 函数内容；得到ADC通道的平均值值
 * 函数参数：无
 * 返回值：无
 */
uint16_t Get_ADC_Average(uint16_t num)
{
    uint16_t i = 0,val = 0;
    uint32_t sum_val = 0;
    uint16_t max_value = 0,min_value = 9999;
    for(i =0;i<num;i++)
    {
        val = Get_ADC_Val();
        if(min_value > val){
            min_value = val;
        }
        if(max_value < val){
            max_value = val;
        }
        sum_val = sum_val +val;
    }
    sum_val = sum_val - max_value - min_value;
    sum_val = sum_val / (num - 2);
    return sum_val;
}

/*
 * 函数内容；设置ADC通道
 * 函数参数：无
 * 返回值：无
 */
void Set_ADC_Channel(uint32_t channel)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = channel;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
}

/*
 * 函数内容；清除任务数据
 * 函数参数：无
 * 返回值：无
 */
void Clear_TaskData(void)
{
	uint8_t i = 0;
	for(i=0;i<100;i++)  //清除计数
	{
			sum_tempValue[i] = 0;
	}
	tempNum = 0;
}

/*
 * 函数内容：测电压任务
 * 函数参数：无
 * 返回值：无
 */

void Voltage_Task(const uint16_t ref_vol_value)
{
	float TempVolValue = 0, Vol_Value = 0;					//临时电压数据&实际电压值
	float volMaxValue = 0, volMinValue = 9999;			//最大电压值&最小电压值
	char showData[32]={0};													//显示数据
	uint16_t value_sum = 0,i = 0;										//采集电压平均值、计数下标
	
	//设置通道为电压ADC引脚
	Set_ADC_Channel(ADC_CHANNEL_10);
	value_sum = Get_ADC_Average(200);	//获取200次平均值
	
	TempVolValue = ((value_sum*1.0f) / ref_vol_value)*1.21f; //换算为真实电压值
	sum_tempValue[tempNum] = TempVolValue;   									//记录每一个值
	tempNum++;
	if(tempNum >= 100)
	{
		bubble_sort(sum_tempValue,100);	//冒泡排序
		for(i=10;i<90;i++)
		{
			Vol_Value = Vol_Value + sum_tempValue[i];	
		}
		Vol_Value = Vol_Value / 80.0f;				//掐头去尾取100次平均值
		if(Vol_Value > 1.665f)
		{
			//如果是红正黑负
			volMaxValue = (492.0f * (sum_tempValue[89] - VREF_VALUE)) / 22.0f;  //计算区间内最大值
			volMinValue = (492.0f * (sum_tempValue[10] - VREF_VALUE)) / 22.0f;  //计算区间内最小值
			Vol_Value = (492.0f * (Vol_Value - VREF_VALUE)) / 22.0f;    			//计算真实电压值
			
			//减去零点漂移，这个最开始需注释掉，观察未测量时电压值，然后进行处理
			volMaxValue = volMaxValue + 0.175f;
			volMinValue = volMinValue + 0.175f;
			Vol_Value = Vol_Value + 0.175f;                     
			
			if(Vol_Value <= 0.1f){
					Vol_Value = 0;
			}
			sprintf(showData," ");
			TFT_ShowString(4,40,(uint8_t *)showData,RED,BLACK,32,0);
			memset(showData,0,32);
		}
		else
		{
			//如果是红负黑正
			volMinValue = (492.0f * (VREF_VALUE - sum_tempValue[89])) / 22.0f;  //计算区间内最大值
			volMaxValue = (492.0f * (VREF_VALUE - sum_tempValue[10])) / 22.0f;  //计算区间内最小值
			Vol_Value = (492.0f * (VREF_VALUE - Vol_Value)) / 22.0f;    		//计算真实电压值
			
			//减去零点漂移，这个最开始需注释掉，观察未测量时电压值，然后进行处理
			volMaxValue = volMaxValue - 0.175f;
			volMinValue = volMinValue - 0.175f;
			Vol_Value = Vol_Value - 0.175f;                    
			
			if(Vol_Value <= 0.1f){
					Vol_Value = 0;
			}
			sprintf(showData,"-");
			TFT_ShowString(4,40,(uint8_t *)showData,RED,BLACK,32,0);
			memset(showData,0,32);
		}

		sprintf(showData,"%4.2f",Vol_Value);	//显示实际电压值
		TFT_ShowString(20,40,(uint8_t *)showData,RED,BLACK,32,0);
		memset(showData,0,32);

		sprintf(showData,"%4.2f",volMaxValue);	//显示max
		TFT_ShowString(8,115,(uint8_t *)showData,RED,BLACK,12,0);
		memset(showData,0,32);

		sprintf(showData,"%4.2f",volMinValue);	//显示min
		TFT_ShowString(60,115,(uint8_t *)showData,RED,BLACK,12,0);
		memset(showData,0,32);
		
		Clear_TaskData();	//清除任务数据
	}
}

/*
 * 函数内容：测电阻任务
 * 函数参数：无
 * 返回值：无
 */
void Resistor_Task(struct KeyInfo *info,const uint16_t ref_vol_value)
{
	float TempVolValue = 0, Vol_Value = 0;//临时电压数据、实际电压值
	char showData[32]={0};								//显示数据
	//外部电压平均值、计数下标
	uint16_t value_sum = 0,i = 0;
	float Resistor_Value = 0,resMaxValue = 0, resMinValue = 0;	//实际电阻值、最大电压值、最小电压值
	
	//设置通道为电阻ADC引脚
	Set_ADC_Channel(ADC_CHANNEL_11);
	value_sum = Get_ADC_Average(200);	//获取200次平均值
	
	TempVolValue = ((value_sum*1.0f) / ref_vol_value)*1.21f; //换算为真实电压值
	sum_tempValue[tempNum] = TempVolValue;   									 //记录每一个值
	tempNum++;
	if(tempNum >= 100)
	{
		tempNum = 0;
		bubble_sort(sum_tempValue,100);  //冒泡排序
		for(i=10;i<90;i++)  
		{
			//取中间80个
			 Vol_Value = Vol_Value + sum_tempValue[i];
		}
		Vol_Value = Vol_Value / 80.0f;  				//取80个的平均值
		if(info->resistor_range == RES_0_100R)	//使用100R分压电阻来测量外部0~300R电阻	
		{
			if(Vol_Value > 2.5f)
			 {
						Resostor_Connect_1K();							//如果是超量程了，切换到下一阻值进行测量
						info->resistor_range = RES_100R_1K;
						HAL_Delay(10);
			 }
			 else
			 {
						Resistor_Value = (uint16_t)((100 * Vol_Value)/(RES_VOL_VALUE - Vol_Value));
						resMaxValue = (uint16_t)((100 * sum_tempValue[89])/(RES_VOL_VALUE - sum_tempValue[89]));
						resMinValue = (uint16_t)((100 * sum_tempValue[10])/(RES_VOL_VALUE - sum_tempValue[10]));
				 
						Resistor_Value = Resistor_Value - 8;    //减去误差值
						resMaxValue = resMaxValue - 8;
						resMinValue = resMinValue - 8;
				 
						if((Resistor_Value <= 1000) && (Resistor_Value >= 0))
						{								
							//对阻值范围进行限制
							sprintf(showData,"%5d",(uint16_t)Resistor_Value);	//显示实际电阻值
							TFT_ShowString(18,44,(uint8_t *)showData,RED,BLACK,32,0);
							memset(showData,0,32);
							sprintf(showData," R");	//显示单位
							TFT_ShowString(124,102,(uint8_t *)showData,YELLOW,BLACK,24,0);
							memset(showData,0,32);
						}
						if((resMaxValue <= 1000) && (resMinValue <= 1000) && (resMaxValue >= 0) && (resMinValue >= 0))
						{
							sprintf(showData,"%5d",(uint16_t)resMaxValue);	//显示max
							TFT_ShowString(8,115,(uint8_t *)showData,RED,BLACK,12,0);
							memset(showData,0,32);

							sprintf(showData,"%5d",(uint16_t)resMinValue);	//显示min
							TFT_ShowString(60,115,(uint8_t *)showData,RED,BLACK,12,0);
							memset(showData,0,32);
						}
			 }
		}
		else if(info->resistor_range == RES_100R_1K)  //使用1K分压电阻来测量外部300R~10K电阻
		{
			if(Vol_Value > 3.0f)
			{
				Resostor_Connect_10K();
				info->resistor_range = RES_1K_10K;
				HAL_Delay(10);
			}
			else
			{
						Resistor_Value = (uint16_t)((1000 * Vol_Value)/(RES_VOL_VALUE - Vol_Value));
						resMaxValue = ((1000 * sum_tempValue[89])/(RES_VOL_VALUE - sum_tempValue[89]));
						resMinValue = ((1000 * sum_tempValue[10])/(RES_VOL_VALUE - sum_tempValue[10]));
						Resistor_Value = Resistor_Value - 9;
						resMaxValue = resMaxValue - 9;
						resMinValue = resMinValue - 9;
				
						if((Resistor_Value <= 10000) && (Resistor_Value >= 0))
						{								
							//对阻值范围进行限制
							sprintf(showData,"%5d",(uint16_t)Resistor_Value);	//显示实际电阻值
							TFT_ShowString(18,44,(uint8_t *)showData,RED,BLACK,32,0);
							memset(showData,0,32);
							sprintf(showData," R");	//显示单位
							TFT_ShowString(124,102,(uint8_t *)showData,YELLOW,BLACK,24,0);
							memset(showData,0,32);
						}
						if((resMaxValue <= 10000) && (resMinValue <= 10000) && (resMaxValue >= 0) && (resMinValue >= 0))
						{
							sprintf(showData,"%5d",(uint16_t)resMaxValue);	//显示max
							TFT_ShowString(8,115,(uint8_t *)showData,RED,BLACK,12,0);
							memset(showData,0,32);

							sprintf(showData,"%5d",(uint16_t)resMinValue);	//显示min
							TFT_ShowString(60,115,(uint8_t *)showData,RED,BLACK,12,0);
							memset(showData,0,32);
						}
			}
		}
		else if(info->resistor_range == RES_1K_10K)   //使用外部10K分压电阻来测量10K~100K电阻
		{
			 if(Vol_Value > 3.0f)
			 {
					 Resostor_Connect_100K();
					 info->resistor_range = RES_10K_100K;
					 HAL_Delay(10);
			 }
			 else
			 {
						Resistor_Value = (10000 * Vol_Value)/(RES_VOL_VALUE - Vol_Value);
						resMaxValue = (10000 * sum_tempValue[89])/(RES_VOL_VALUE - sum_tempValue[89]);
						resMinValue = (10000 * sum_tempValue[10])/(RES_VOL_VALUE - sum_tempValue[10]);
				 
						Resistor_Value = Resistor_Value - 24.7f;	//减去误差值
						resMaxValue = resMaxValue - 24.7f;
						resMinValue = resMinValue - 24.7f;
				 
						Resistor_Value = Resistor_Value / 1000.0f;
						resMaxValue = resMaxValue / 1000.0f;
						resMinValue = resMinValue / 1000.0f;
						
						if((Resistor_Value <= 1000) && (Resistor_Value >= 0))
						{
							sprintf(showData,"%4.1f",Resistor_Value);	//显示实际电阻值
							TFT_ShowString(18,44,(uint8_t *)showData,RED,BLACK,32,0);
							memset(showData,0,32);
							sprintf(showData,"KR");	//显示单位
							TFT_ShowString(124,102,(uint8_t *)showData,YELLOW,BLACK,24,0);
							memset(showData,0,32);
						}
						if((resMaxValue <= 1000) && (resMinValue <= 1000) && (resMaxValue >= 0) && (resMinValue >= 0))
						{
							sprintf(showData,"%4.1f",resMaxValue);	//显示max
							TFT_ShowString(8,115,(uint8_t *)showData,RED,BLACK,12,0);
							memset(showData,0,32);

							sprintf(showData,"%4.1f",resMinValue);	//显示min
							TFT_ShowString(60,115,(uint8_t *)showData,RED,BLACK,12,0);
							memset(showData,0,32);
						}
			 }
		}
		else if(info->resistor_range == RES_10K_100K) //使用外部100K电阻来测量剩余范围的阻值
		{
				if(Vol_Value >= 3.29f)  //并不是外部电阻值过大，返回100R分压，重新开始测量
				{
						Resistor_Value = 0;
						resMaxValue = 0;
						resMinValue = 0;
						sprintf(showData,"%5d",(uint16_t)Resistor_Value);	//显示实际电阻值
						TFT_ShowString(18,44,(uint8_t *)showData,RED,BLACK,32,0);
						memset(showData,0,32);
						sprintf(showData," R");	//显示单位
						TFT_ShowString(124,102,(uint8_t *)showData,YELLOW,BLACK,24,0);
						memset(showData,0,32);
					
						sprintf(showData,"%5d",(uint16_t)resMaxValue);	//显示max
						TFT_ShowString(8,115,(uint8_t *)showData,RED,BLACK,12,0);
						memset(showData,0,32);

						sprintf(showData,"%5d",(uint16_t)resMinValue);	//显示min
						TFT_ShowString(60,115,(uint8_t *)showData,RED,BLACK,12,0);
						memset(showData,0,32);
				}
				else if(Vol_Value <= 0.3f)
				{
						Resostor_Connect_100R();  //默认100R分压
						info->resistor_range = RES_0_100R; //电阻量程
						HAL_Delay(10);
				}
				else
				{
					Resistor_Value = (100000.0f * Vol_Value)/(RES_VOL_VALUE - Vol_Value);
					resMaxValue = (100000.0f * sum_tempValue[89])/(RES_VOL_VALUE - sum_tempValue[89]);
					resMinValue = (100000.0f * sum_tempValue[10])/(RES_VOL_VALUE - sum_tempValue[10]);
				
					Resistor_Value = Resistor_Value - 172.4f;	//减去误差值
					resMaxValue = resMaxValue - 172.4f;
					resMinValue = resMinValue - 172.4f;
				
					Resistor_Value = Resistor_Value / 1000.0f;
					resMaxValue = resMaxValue / 1000.0f;
					resMinValue = resMinValue / 1000.0f;
					
					if((Resistor_Value <= 1000) && (Resistor_Value >= 0))
					{
						sprintf(showData,"%4.1f",Resistor_Value);	//显示实际电阻值
						TFT_ShowString(18,44,(uint8_t *)showData,RED,BLACK,32,0);
						memset(showData,0,32);
						sprintf(showData,"KR");	//显示单位
						TFT_ShowString(124,102,(uint8_t *)showData,YELLOW,BLACK,24,0);
						memset(showData,0,32);
					}
					else
					{
						Resistor_Value = 0;
						resMaxValue = 0;
						resMinValue = 0;
						sprintf(showData,"%5d",(uint16_t)Resistor_Value);	//显示实际电阻值
						TFT_ShowString(18,44,(uint8_t *)showData,RED,BLACK,32,0);
						memset(showData,0,32);
						sprintf(showData," R");	//显示单位
						TFT_ShowString(124,102,(uint8_t *)showData,YELLOW,BLACK,24,0);
						memset(showData,0,32);
					
						sprintf(showData,"%5d",(uint16_t)resMaxValue);	//显示max
						TFT_ShowString(8,115,(uint8_t *)showData,RED,BLACK,12,0);
						memset(showData,0,32);

						sprintf(showData,"%5d",(uint16_t)resMinValue);	//显示min
						TFT_ShowString(60,115,(uint8_t *)showData,RED,BLACK,12,0);
						memset(showData,0,32);
					}
					if((resMaxValue <= 1000) && (resMinValue <= 1000) && (resMaxValue >= 0) && (resMinValue >= 0))
					{
						sprintf(showData,"%4.1f",resMaxValue);	//显示max
						TFT_ShowString(8,115,(uint8_t *)showData,RED,BLACK,12,0);
						memset(showData,0,32);

						sprintf(showData,"%4.1f",resMinValue);	//显示min
						TFT_ShowString(60,115,(uint8_t *)showData,RED,BLACK,12,0);
						memset(showData,0,32);
					}
				}
		}
		Clear_TaskData();
	}
}

/*
 * 函数内容：测电流任务
 * 函数参数：无
 * 返回值：无
 */
void Electricity_Task(const uint16_t ref_vol_value)
{
	float TempVolValue = 0, Vol_Value = 0;		//临时电压数据、实际电压值
	char showData[32]={0};										//显示数据
	//实际电流值、外部电流平均值、计数下标
	uint16_t Electricity_Value = 0,value_sum = 0,i = 0;
	uint16_t Electricity_MaxValue = 0,Electricity_MinValue = 0;
	
	//设置通道为电流ADC引脚
	Set_ADC_Channel(ADC_CHANNEL_13);
	value_sum = Get_ADC_Average(200);	//获取200次平均值
	
	TempVolValue = ((value_sum*1.0f) / ref_vol_value)*1.21f; //换算为真实电压值
	sum_tempValue[tempNum] = TempVolValue;   									 //记录每一个值
	tempNum++;
	if(tempNum >= 100)
	{
		tempNum = 0;
		bubble_sort(sum_tempValue,100);  //冒泡排序
		for(i=10;i<90;i++)  //取中间80个
		{
			 Vol_Value = Vol_Value + sum_tempValue[i];
		}
		Vol_Value = Vol_Value / 80.0f;  //取80个的平均值
		if(Vol_Value > VREF_VALUE)   //此处需要做校准，标准应该是1.65V，但是实际输入值可能会偏大或偏小，这里要先获取实际电压值来进行校准
		{
			 Vol_Value = Vol_Value - VREF_VALUE;
			 sum_tempValue[89] = sum_tempValue[89] - VREF_VALUE;
			 sum_tempValue[10] = sum_tempValue[10] - VREF_VALUE;
		}
		else
		{
			 Vol_Value = VREF_VALUE - Vol_Value;
			 sum_tempValue[89] = VREF_VALUE - sum_tempValue[89];
			 sum_tempValue[10] = VREF_VALUE - sum_tempValue[10];
		}
		Electricity_Value = (uint16_t)((Vol_Value / 50.0f / 0.01f * 1000));
		Electricity_MaxValue = (uint16_t)((sum_tempValue[89] / 50.0f / 0.01f * 1000));
		Electricity_MinValue = (uint16_t)((sum_tempValue[10] / 50.0f / 0.01f * 1000));
		
		//减去静态误差，在第一次测试时需要注释
		Electricity_Value = Electricity_Value > 11 ? Electricity_Value - 11 : 0;
		Electricity_MaxValue = Electricity_MaxValue > 11 ? Electricity_MaxValue - 11 : 0;
		Electricity_MinValue = Electricity_MinValue > 11 ? Electricity_MinValue - 11 : 0;
		
		sprintf(showData,"%4d",Electricity_Value);	//显示实际电流值
		TFT_ShowString(18,40,(uint8_t *)showData,RED,BLACK,32,0);
		memset(showData,0,32);
		
		sprintf(showData,"%5d",(uint16_t)Electricity_MaxValue);	//显示max
		TFT_ShowString(8,115,(uint8_t *)showData,RED,BLACK,12,0);
		memset(showData,0,32);
	
		sprintf(showData,"%5d",(uint16_t)Electricity_MinValue);	//显示min
		TFT_ShowString(60,115,(uint8_t *)showData,RED,BLACK,12,0);
		memset(showData,0,32);
		
		Clear_TaskData();
	}
}

/*
 * 函数内容：测通断任务
 * 函数参数：无
 * 返回值：无
 */
void Buzzing_Task(const uint16_t ref_vol_value)
{
	float TempVolValue = 0, Vol_Value = 0;//临时电压数据、实际电压值
	char showData[32]={0};								//显示数据
	//外部电压平均值、计数下标
	uint16_t value_sum = 0,i = 0;
	float Resistor_Value = 0,resMaxValue = 0, resMinValue = 0;	//实际电阻值、最大电压值、最小电压值
	//设置通道为电阻ADC引脚
	Set_ADC_Channel(ADC_CHANNEL_11);
	value_sum = Get_ADC_Average(100);	//获取100次平均值
	
	TempVolValue = ((value_sum*1.0f) / ref_vol_value)*1.21f; //换算为真实电压值
	sum_tempValue[tempNum] = TempVolValue;   									 //记录每一个值
	tempNum++;
	if(tempNum >= 100)
	{
		tempNum = 0;
		bubble_sort(sum_tempValue,100);  //冒泡排序
		for(i=10;i<90;i++)  
		{
			//取中间80个
			 Vol_Value = Vol_Value + sum_tempValue[i];
		}
		Vol_Value = Vol_Value / 80.0f;  				//取80个的平均值
		if(Vol_Value >= 3.3f){
			 sum_tempValue[89] = 0;
			 sum_tempValue[10] = 0;
		}
		Resistor_Value = (uint16_t)(100 * Vol_Value)/(RES_VOL_VALUE - Vol_Value);
		resMaxValue = (100 * sum_tempValue[89])/(RES_VOL_VALUE - sum_tempValue[89]);
		resMinValue = (100 * sum_tempValue[10])/(RES_VOL_VALUE - sum_tempValue[10]);		
		if(Resistor_Value < 0){
			Resistor_Value = -Resistor_Value;
		}
		if((Resistor_Value <= 20 && Resistor_Value>= 0) && (resMaxValue >= 0) && (resMinValue >= 0))
		{
			//对阻值范围进行限制
			sprintf(showData,"%4d",(uint16_t)Resistor_Value);	//显示实际电阻值
			TFT_ShowString(18,44,(uint8_t *)showData,RED,BLACK,32,0);
			memset(showData,0,32);
			
			sprintf(showData,"%5d",(uint16_t)resMaxValue);	//显示max
			TFT_ShowString(8,115,(uint8_t *)showData,RED,BLACK,12,0);
			memset(showData,0,32);

			sprintf(showData,"%5d",(uint16_t)resMinValue);	//显示min
			TFT_ShowString(60,115,(uint8_t *)showData,RED,BLACK,12,0);
			memset(showData,0,32);
			Open_Beep();
		}
		else{
				Resistor_Value = 0;
				resMaxValue = 0;
				resMinValue = 0;
			
				sprintf(showData," .OL");
				TFT_ShowString(18,44,(uint8_t *)showData,RED,BLACK,32,0);
				memset(showData,0,32);
			
				sprintf(showData,"%5d",(uint16_t)resMaxValue);	//显示max
				TFT_ShowString(8,115,(uint8_t *)showData,RED,BLACK,12,0);
				memset(showData,0,32);

				sprintf(showData,"%5d",(uint16_t)resMinValue);	//显示min
				TFT_ShowString(60,115,(uint8_t *)showData,RED,BLACK,12,0);
				memset(showData,0,32);
			
				Closer_Beep();
		}
		Clear_TaskData();
	}
}

/*
 * 函数内容：测二极管档任务
 * 函数参数：无
 * 返回值：无
 */
void Dio_Task(const uint16_t ref_vol_value)
{
	float TempVolValue = 0, Vol_Value = 0;//临时电压数据、实际电压值
	char showData[32]={0};								//显示数据
	//外部电压平均值、计数下标
	uint16_t value_sum = 0,i = 0;
	//设置通道为电阻ADC引脚
	Set_ADC_Channel(ADC_CHANNEL_11);
	value_sum = Get_ADC_Average(200);	//获取200次平均值
	
	TempVolValue = ((value_sum*1.0f) / ref_vol_value)*1.21f; //换算为真实电压值
	sum_tempValue[tempNum] = TempVolValue;   									 //记录每一个值
	tempNum++;
	if(tempNum >= 100)
	{
		tempNum = 0;
		bubble_sort(sum_tempValue,100);  //冒泡排序
		for(i=10;i<90;i++)  
		{
			//取中间80个
			 Vol_Value = Vol_Value + sum_tempValue[i];
		}
		Vol_Value = Vol_Value / 80.0f;  				//取80个的平均值
		if(Vol_Value >= 3.25f)
		{
			sum_tempValue[89] = 0;
			sum_tempValue[10] = 0;
			
			sprintf(showData," .OL");
			TFT_ShowString(18,44,(uint8_t *)showData,RED,BLACK,32,0);
			memset(showData,0,32);

			sprintf(showData,"%5d",(uint16_t)sum_tempValue[89]);	//显示max
			TFT_ShowString(8,115,(uint8_t *)showData,RED,BLACK,12,0);
			memset(showData,0,32);

			sprintf(showData,"%5d",(uint16_t)sum_tempValue[10]);	//显示min
			TFT_ShowString(60,115,(uint8_t *)showData,RED,BLACK,12,0);
			memset(showData,0,32);
		}
		else
		{
			Vol_Value = 3.290f - Vol_Value;
			sprintf(showData,"%4.2f",Vol_Value);	//显示实际电压值
			TFT_ShowString(20,40,(uint8_t *)showData,RED,BLACK,32,0);
			memset(showData,0,32);

			sprintf(showData,"%4.2f",sum_tempValue[89]);	//显示max
			TFT_ShowString(8,115,(uint8_t *)showData,RED,BLACK,12,0);
			memset(showData,0,32);

			sprintf(showData,"%4.2f",sum_tempValue[10]);	//显示min
			TFT_ShowString(60,115,(uint8_t *)showData,RED,BLACK,12,0);
			memset(showData,0,32);
		}
		Clear_TaskData();
	}
}

