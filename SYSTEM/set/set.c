#include <set.h>
#include <stdio.h>
#include <string.h>
#include <lcd1602.h>
#include <stmflash.h>
#include <iot.h>

/*
 * set_value存放设置过的值。
 * SET_curtainStat：窗帘的开合程度（单位：百分比；0：全开；100：全关）
 **/

static SET_Value set_value;
static int16_t SET_curtainStat;

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
#if DEBUG_FLAG
		printf("Enter setup mode");
#endif
		
		/*此处获取之前设置的温度、光照、湿度值*/
		SET_ToTemperature();
	} else if (system_mode == SYSTEM_Mode_Set) {//退出
		system_mode = SYSTEM_Mode_Normal;
		SET_CurrentOption = SET_OPTION_N;
	
		/*保存设置的值*/
		int16_t TEXT_Buffer[4];
		TEXT_Buffer[0] = set_value.temperature;
		TEXT_Buffer[1] = set_value.illumination;
		TEXT_Buffer[2] = set_value.humidity;
		TEXT_Buffer[3] = SET_curtainStat;
		STMFLASH_Write(FLASH_SAVE_ADDR, (u16 *)TEXT_Buffer, 2);

#if DEBUG_FLAG
		printf("Settings saved successfully");
		printf("Enter normal mode");
#endif
	}
}

/*
 * 切换下一个设置选项，可循环切换
 **/
void SET_SwitchOption(void)
{
#if DEBUG_FLAG
	if (system_mode == SYSTEM_Mode_Normal)
		printf("System mode is normal mode");
	else if (system_mode == SYSTEM_Mode_Set)
		printf("System mode is set mode");
#endif
	
	switch(SET_CurrentOption) {
		case SET_OPTION_T:
			SET_ToIllumination();
#if DEBUG_FLAG
			printf("Enter the humidity setting");
#endif
			break;
		case SET_OPTION_I:
			SET_ToHumidity();
#if DEBUG_FLAG
			printf("Enter the light settings");
#endif
			break;
		case SET_OPTION_H:
			SET_ToTemperature();
#if DEBUG_FLAG
			printf("Enter the temperature setting");
#endif
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
	
	if (set_value.temperature < 0) {
		sprintf((char *)LCD_Buf, "Set temperature:-%d", set_value.temperature * -1);
	} else {
		sprintf((char *)LCD_Buf, "Set temperature:%d", set_value.temperature);
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
	
	sprintf((char *)LCD_Buf, "Set illumination:-%d", set_value.illumination);
	LCD1602_WriteString(LCD_Buf);
}

/*
 * 切换到湿度设置选项
 **/
static void SET_ToHumidity(void) 
{
	uint8_t LCD_Buf[36] = "\0";
	
	SET_CurrentOption = SET_OPTION_H;
	
	sprintf((char *)LCD_Buf, "Set humidity:-%d", set_value.humidity);
	LCD1602_WriteString(LCD_Buf);
}

/*
 * 根据当前所在的设置选项增加相应的数值
 **/
void SET_AddValue(void)
{
	switch(SET_CurrentOption) {
		case SET_OPTION_T:
			set_value.temperature++;
			SET_ToTemperature();
			break;
		case SET_OPTION_I:
			set_value.illumination++;
			SET_ToIllumination();
			break;
		case SET_OPTION_H:
			set_value.humidity++;
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
			set_value.temperature--;
			SET_ToTemperature();
			break;
		case SET_OPTION_I:
			set_value.illumination--;
			SET_ToIllumination();
			break;
		case SET_OPTION_H:
			set_value.humidity--;
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
	
	set_value.temperature = TEXT_Buffer[0];
	set_value.illumination = TEXT_Buffer[1];
	set_value.humidity = TEXT_Buffer[2];
	SET_curtainStat = TEXT_Buffer[3];
	
#if DEBUG_FLAG
	printf("Reading setup information:");
	printf("temperature:%d", set_value.temperature);
	printf("illumination:%d", set_value.illumination);
	printf("humidity:%d", set_value.humidity);
#endif
}

/* 
 * 获取以前设置的值。
 * 包括温度值、湿度值和光照值
 **/
void SET_GetSetValue(SET_Value *SetValue_p)
{
	SetValue_p->temperature = set_value.temperature;
	SetValue_p->illumination = set_value.illumination;
	SetValue_p->humidity = set_value.humidity;
}
