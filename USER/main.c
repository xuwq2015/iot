#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include <iot.h>
#include <key.h>
#include <l298n.h>
#include <lcd1602.h>
#include <delay.h>
#include <ds18b20.h>

 
/************************************************
基于stm32f103zet6芯片
************************************************/

int main(void)
{	
	short temperature;
	uint8_t LCD_Buf[36] = "\0";
	
	delay_init();	    	 //延时函数初始化
 
	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
	LCD1602_Init(); 			//初始化LCD1602
	LCD1602_WriteString("The system is initializing...");

	L298N_Init();					//初始化L298N电机驱动模块
	KEY_Init();						//初始化按键及按键中断
	DS18B20_Init();				//初始化DS18B20
 
	while(1) {
		memset(LCD_Buf, 0, 36);
		temperature = DS18B20_Get_Temp();
		if (temperature < 0) {
			temperature = -temperature;
			sprintf((char *)LCD_Buf, "temperature:-%d", temperature);
		} else {
			sprintf((char *)LCD_Buf, "temperature:%c", temperature);
		}
		LCD1602_WriteString(LCD_Buf);
		delay_ms(60000);
	}
}
