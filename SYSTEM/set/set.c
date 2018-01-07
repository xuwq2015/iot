#include <set.h>
#include <iot.h>
#include <stdio.h>
#include <string.h>
#include <lcd1602.h>

static short SET_temperature = 0;		//温度值
static int8_t SET_illumination = 0;	//光照值
static int8_t SET_humidity = 0;			//湿度值

//保存系统当前模式（设置模式或正常模式）
SYSTEM_Mode system_mode = SYSTEM_Mode_Normal;
static int8_t SET_CurrentOption = SET_OPTION_N;

/*
 * 调用此函数可以退出或进入设置模式
 **/
void SET_EnterOrQuit(void)
{
	if (system_mode == SYSTEM_Mode_Normal) {//进入
		system_mode = SYSTEM_Mode_Set;
		SET_CurrentOption = SET_OPTION_T;
		
		/*此处获取之前设置的温度、光照、湿度值*/
		SET_ToTemperature();
	} else if (system_mode == SYSTEM_Mode_Set) {//退出
		system_mode = SYSTEM_Mode_Normal;
		SET_CurrentOption = SET_OPTION_N;
		
		/*保存设置的值*/
	}
}

/*
 * 切换下一个设置选项，可循环切换
 **/
void SET_SwitchOption(void)
{
	switch(SET_CurrentOption) {
		case SET_OPTION_T:
			SET_ToIllumination();
			break;
		case SET_OPTION_I:
			SET_ToHumidity();
			break;
		case SET_OPTION_H:
			SET_ToTemperature();
		case SET_OPTION_N:
			break;
	}
}

/*
 * 切换到温度设置选项
 **/
static void SET_ToTemperature(void) 
{
	uint8_t LCD_Buf[36] = "\0";
	
	SET_CurrentOption = SET_OPTION_T;
	
	sprintf((char *)LCD_Buf, "Set temperature:-%d", SET_temperature);
	LCD1602_WriteString(LCD_Buf);
}

/*
 * 切换到光照设置选项
 **/
static void SET_ToIllumination(void) 
{
	uint8_t LCD_Buf[36] = "\0";
	
	SET_CurrentOption = SET_OPTION_I;
	
	sprintf((char *)LCD_Buf, "Set illumination:-%d", SET_illumination);
	LCD1602_WriteString(LCD_Buf);
}

/*
 * 切换到湿度设置选项
 **/
static void SET_ToHumidity(void) 
{
	uint8_t LCD_Buf[36] = "\0";
	
	SET_CurrentOption = SET_OPTION_H;
	
	sprintf((char *)LCD_Buf, "Set humidity:-%d", SET_humidity);
	LCD1602_WriteString(LCD_Buf);
}

/*
 * 根据当前所在的设置选项增加相应的数值
 **/
void SET_AddValue(void)
{
	switch(SET_CurrentOption) {
		case SET_OPTION_T:
			SET_temperature++;
			SET_ToTemperature();
			break;
		case SET_OPTION_I:
			SET_illumination++;
			SET_ToIllumination();
			break;
		case SET_OPTION_H:
			SET_humidity++;
			SET_ToHumidity();
			break;
		case SET_OPTION_N:
			break;
	}
}

/*
 * 根据当前所在的设置选项减少相应的数值
 **/
void SET_ReduceValue(void)
{
		switch(SET_CurrentOption) {
		case SET_OPTION_T:
			SET_temperature--;
			SET_ToTemperature();
			break;
		case SET_OPTION_I:
			SET_illumination--;
			SET_ToIllumination();
			break;
		case SET_OPTION_H:
			SET_humidity--;
			SET_ToHumidity();
			break;
		case SET_OPTION_N:
			break;
	}
}
