#ifndef __SET_H
#define __SET_H

#include <iot.h>

//设置选项
typedef enum {
	SET_OPTION_T = 0x01,		//温度
	SET_OPTION_I,						//光照
	SET_OPTION_H,						//湿度
	SET_OPTION_N						//无、未知
} SET_OPTION;

typedef struct {
	int16_t temperature;
	int16_t illumination;
	int16_t humidity;
} SET_Value;

extern SYSTEM_Mode system_mode;

void SET_EnterOrQuit(void);

void SET_SwitchOption(void);

static void SET_ToTemperature(void);
static void SET_ToIllumination(void);
static void SET_ToHumidity(void);

void SET_AddValue(void);
void SET_ReduceValue(void);

void SET_GetValue(void);

void SET_GetSetValue(SET_Value *SetValue_p);

#endif
