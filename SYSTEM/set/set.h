#ifndef __SET_H
#define __SET_H

//设置选项
typedef enum {
	SET_OPTION_T = 0x01,		//温度
	SET_OPTION_I,						//光照
	SET_OPTION_H,						//湿度
	SET_OPTION_N						//无、未知
} SET_OPTION;

void SET_EnterOrQuit(void);

void SET_SwitchOption(void);

static void SET_ToTemperature(void);
static void SET_ToIllumination(void);
static void SET_ToHumidity(void);

void SET_AddValue(void);
void SET_ReduceValue(void);

#endif
