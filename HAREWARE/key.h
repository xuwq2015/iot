#ifndef __KEY_H
#define __KEY_H

#include <sys.h>
#include <iot.h>

//按键状态
typedef enum {
	KEY_PRESS = 0x00,		//按键按下
	KEY_RELEASE					//按键松开
} KEY_STATUS;

#define KEY_S1 PBin(10)   	//PC4
#define KEY_S2 PCin(4)   		//PC4
#define KEY_S3 PCin(13)   	//PC13

//#define KEY_PRESS	0				//按键按下
//#define KEY_RELEASE 1			//按键松开

void KEY_Init(void);					//初始化按键
static void set_exti(void);		//初始化按键中断

void KEY_S1_Press(void);
void KEY_S2_Press(void);
void KEY_S2_Release(void);
void KEY_S3_Press(void);
void KEY_S3_Release(void);

#endif
