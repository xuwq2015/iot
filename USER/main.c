#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include <iot.h>
#include <key.h>
#include <l298n.h>
#include <lcd1602.h>
#include <delay.h>
#include <ds18b20.h>
#include <adc.h>
#include <set.h>

#if (IOT_DEBUG)
	#include <usart.h>
#endif

 
/************************************************
基于stm32f103zet6芯片
************************************************/

extern int16_t SET_temperature;
extern int16_t SET_illumination;
extern int16_t SET_humidity;

int main(void)
{	
	int16_t temperature;
	int16_t illumination;
	int16_t humidity;
	uint8_t LCD_Buf[36] = "\0";
	
	delay_init();	    	 //延时函数初始化
 
	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
#if (IOT_DEBUG)
	uart_init(115200);
#endif
	
	LCD1602_Init(); 			//初始化LCD1602
	LCD1602_WriteString("The system is initializing...");

	L298N_Init();					//初始化L298N电机驱动模块
	KEY_Init();						//初始化按键及按键中断
	DS18B20_Init();				//初始化DS18B20
	Adc_Init();						//初始化光敏、湿度模块
	
	SET_GetValue();				//从flash中读取设置的值
 
	while(1) {
		memset(LCD_Buf, 0, 36);
		temperature = DS18B20_Get_Temp();				//获取温度
		illumination = Get_Illumination();			//获取光照强度值
		humidity = Get_Humidity();							//获取湿度值
		if (temperature < 0) {
			temperature = -temperature;
			sprintf((char *)LCD_Buf, "t:-%d i:%d h:%d", temperature, illumination, humidity);
		} else {
			sprintf((char *)LCD_Buf, "t:%d i:%d h:%d", temperature, illumination, humidity);
		}
		LCD1602_WriteString(LCD_Buf);
		delay_ms(60000);
	}
}
