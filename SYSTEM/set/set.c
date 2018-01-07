#include <set.h>
#include <stdio.h>
#include <string.h>
#include <lcd1602.h>
#include <stmflash.h>

/*
 * 存放设置过的值。
 * SET_temperature：设置的温度的值
 * SET_illumination：设置的光照的值
 * SET_humidity：设置的湿度的值
 * SET_curtainStat：窗帘的开合程度（单位：百分比；0：全开；100：全关）
 **/
int16_t SET_temperature = 0;
int16_t SET_illumination = 0;
int16_t SET_humidity = 0;
int16_t SET_curtainStat = 0;

/*
 * system_mode：保存系统当前模式（设置模式或正常模式）
 * SET_CurrentOption：
 * 如果系统处于设置模式时，此变量用来保存当前
 * 正处于那种设置选项。
 * 设置选项包括：
 * 1、设置温度
 * 2、设置光照
 * 3、设置湿度
 **/
SYSTEM_Mode system_mode = SYSTEM_Mode_Normal;
static int8_t SET_CurrentOption = SET_OPTION_N;

#define FLASH_SAVE_ADDR  0x0807FFF7

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
		int16_t TEXT_Buffer[4];
		TEXT_Buffer[0] = SET_temperature;
		TEXT_Buffer[1] = SET_illumination;
		TEXT_Buffer[2] = SET_humidity;
		TEXT_Buffer[3] = SET_curtainStat;
		STMFLASH_Write(FLASH_SAVE_ADDR, (u16 *)TEXT_Buffer, 2);
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
	
	if (SET_temperature < 0) {
		sprintf((char *)LCD_Buf, "Set temperature:-%d", SET_temperature * -1);
	} else {
		sprintf((char *)LCD_Buf, "Set temperature:%d", SET_temperature);
	}
		
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

void SET_GetValue(void)
{
	int16_t TEXT_Buffer[4];
	STMFLASH_Read(FLASH_SAVE_ADDR, (u16 *)TEXT_Buffer, 4);
	
	SET_temperature = TEXT_Buffer[0];
	SET_illumination = TEXT_Buffer[1];
	SET_humidity = TEXT_Buffer[2];
	SET_curtainStat = TEXT_Buffer[3];
}
