#include "bsp_key.h"
#include "bsp_tft.h"
#include "gpio.h"
#include "adc.h"
#include "bsp_task.h"

/*
 * 函数内容：按键扫描
 * 函数参数：无
 * 返回值：判断是哪个按键按下，支持长按、双击还是单击判断
 */
uint8_t key_scanf(void)
{
    uint8_t ret = KEY_ERROR;    //按键返回值
    static uint8_t keyCnt[3] = {0};  //用于按键计时消抖
    static uint8_t keyFcnt[3] = {0}; //用于判断按键多久间隔算双击
    static uint8_t keyCount[3] = {0};    //按键按下的次数
    static uint8_t keyLongFlag[3] = {0}; //按键长按标志位，只有初级进入时判断长按才有效
    if(GET_KEY1_IN == GPIO_PIN_RESET){   
        keyCnt[KEY1]++;           //计时变量，记录按键按下低电平时间
        if(keyCnt[KEY1] >= 120){
            //防止数据越界
            keyCnt[KEY1] = 120;
        }
    }     
    if(GET_KEY1_IN == GPIO_PIN_SET)
    {
        //如果按键抬起
        if((keyCnt[KEY1] >= 100) && (keyLongFlag[KEY1] == 0)){
            //按键大于1s且之前没有多击，证明是长按
            ret = Key1LongPress;
        }
        else if(keyCnt[KEY1] >= 2)
        {
            //按键按下时间大于20ms，证明是一次按下
            keyCount[KEY1]++;     //按键按下的次数增加
            keyFcnt[KEY1] = DOUBLE_TIME;   //在300ms内再次按下算双击
            keyLongFlag[KEY1] = 1;    //已经有一次短按了，才能等待多击结束后才可长按
        }
        keyCnt[KEY1] = 0;
        if(keyFcnt[KEY1])
        {
            keyFcnt[KEY1]--;  //间隔时间不断减少
            if(keyFcnt[KEY1] <= 0)    
            {
                //间隔时间到了
                if(keyCount[KEY1] == 1){
                    //单击
                    ret = Key1Press;
                }
                else if(keyCount[KEY1] ==2){
                    //双击
                    ret = Key1DoublePress;
                }
                keyFcnt[KEY1] = 0;
                keyCount[KEY1] = 0;
                keyLongFlag[KEY1] = 0;
            }
        }
    }

    if(GET_KEY2_IN == GPIO_PIN_RESET){   
        keyCnt[KEY2]++;           //计时变量，记录按键按下低电平时间
        if(keyCnt[KEY2] >= 120){
            //防止数据越界
            keyCnt[KEY2] = 120;
        }
    }     
    if(GET_KEY2_IN == GPIO_PIN_SET)
    {
        //如果按键抬起
        if((keyCnt[KEY2] >= 100) && (keyLongFlag[KEY2] == 0)){
            //按键大于1s且之前没有多击，证明是长按
            ret = Key2LongPress;
        }
        else if(keyCnt[KEY2] >= 2)
        {
            //按键按下时间大于20ms，证明是一次按下
            keyCount[KEY2]++;     //按键按下的次数增加
            keyFcnt[KEY2] = DOUBLE_TIME;   //在300ms内再次按下算双击
            keyLongFlag[KEY2] = 1;    //已经有一次短按了，才能等待多击结束后才可长按
        }
        keyCnt[KEY2] = 0;
        if(keyFcnt[KEY2])
        {
            keyFcnt[KEY2]--;  //间隔时间不断减少
            if(keyFcnt[KEY2] <= 0)    
            {
                //间隔时间到了
                if(keyCount[KEY2] == 1){
                    //单击
                    ret = Key2Press;
                }
                else if(keyCount[KEY2] ==2){
                    //双击
                    ret = Key2DoublePress;
                }
                keyFcnt[KEY2] = 0;
                keyCount[KEY2] = 0;
                keyLongFlag[KEY2] = 0;
            }
        }
    }
		
		if(GET_KEY3_IN == GPIO_PIN_RESET){   
        keyCnt[KEY3]++;           //计时变量，记录按键按下低电平时间
        if(keyCnt[KEY3] >= 120){
            //防止数据越界
            keyCnt[KEY3] = 120;
        }
    }     
    if(GET_KEY3_IN == GPIO_PIN_SET)
    {
        //如果按键抬起
        if((keyCnt[KEY3] >= 100) && (keyLongFlag[KEY3] == 0)){
            //按键大于1s且之前没有多击，证明是长按
            ret = Key3LongPress;
        }
        else if(keyCnt[KEY3] >= 2)
        {
            //按键按下时间大于20ms，证明是一次按下
            keyCount[KEY3]++;     //按键按下的次数增加
            keyFcnt[KEY3] = DOUBLE_TIME;   //在300ms内再次按下算双击
            keyLongFlag[KEY3] = 1;    //已经有一次短按了，才能等待多击结束后才可长按
        }
        keyCnt[KEY3] = 0;
        if(keyFcnt[KEY3])
        {
            keyFcnt[KEY3]--;  //间隔时间不断减少
            if(keyFcnt[KEY3] <= 0)    
            {
                //间隔时间到了
                if(keyCount[KEY3] == 1){
                    //单击
                    ret = Key3Press;
                }
                else if(keyCount[KEY3] ==2){
                    //双击
                    ret = Key3DoublePress;
                }
                keyFcnt[KEY3] = 0;
                keyCount[KEY3] = 0;
                keyLongFlag[KEY3] = 0;
            }
        }
    }
    return ret;
}

/*
 * 函数内容：按键处理
 * 函数参数：
						keyValue--按键值
						struct KeyInfo *info--静态信息
 * 返回值：无
 */
void key_handle(uint8_t keyValue,struct KeyInfo *info)
{
	switch(keyValue)
	{
		case Key1Press:
			break;
		case Key1DoublePress:
			break;
		case Key1LongPress:
			break;
		case Key2Press:
			break;
		case Key2DoublePress:
			break;
		case Key2LongPress:
			break;
		case Key3Press:
			break;
		case Key3DoublePress:
		{
			info->mode++;
			if(info->mode >= cur_max_index)
			{
				info->mode = 0;
			}
			Clear_TaskData();							//进行任务切换时清空任务数据
			Mode_Switch(info->mode);
			info->resistor_range = RES_0_100R;	//进行任务切换后，电阻默认100R分压
			TFT_Fill(4,40,100,72,BLACK);	//将显示区域清空
			TFT_ShowCursor(*info);				//更新静态显示
		}
			break;
		case Key3LongPress:
			break;
		default:
			break;
	}
}

/*
 * 函数内容：模式切换所做处理
 * 函数参数：
						mode--模式
 * 返回值：无
 */
void Mode_Switch(uint8_t mode)
{
	switch(mode)
	{
		case VOL_MODE:
		{
			Relay_Connect_VOL();   	 	//继电器切换到电压测量端
			DisCon_GND();   					//黑表笔不导通到GND
		}
			break;
		case RES_MODE:
		{
			Relay_Connect_RES();  		//继电器切换到电阻测量端
			Connect_GND();          	//黑表笔导通到GND
			Resostor_Connect_100R();  //默认100R分压
		}
			break;
		case CUR_MODE:
		{
			Relay_Connect_CUR();			//继电器切换到电流测量端
			DisCon_GND();   					//黑表笔不导通到GND
		}
			break;
		case DIODE_MODE:
		{
			Relay_Connect_RES();  		//继电器切换到电阻测量端
			Connect_GND();          	//黑表笔导通到GND
			Resostor_Connect_100R();  //默认100R分压
		}
		break;
		case BUZ_MODE:
		{
			Relay_Connect_RES();  		//继电器切换到电阻测量端
			Connect_GND();          	//黑表笔导通到GND
			Resostor_Connect_100R();  //默认100R分压
		}
		break;
		default:
		{
			DisCon_GND();   					//黑表笔不导通到GND
		}
			break;
	}
}
