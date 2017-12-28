#include <stm32f10x.h>
#include <key.h>
#include <l298n.h>
#include <lcd1602.h>
#include <delay.h>
#include <usart.h>

 
/************************************************
基于stm32f103zet6芯片
************************************************/

int main(void)
{	
	delay_init();	    	 //延时函数初始化
 
	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
 
	uart_init(115200);	 	//串口初始化为115200
	LCD1602_Init(); 			//初始化LCD1602
	LCD1602_WriteString("The system is initializing...");
 
	L298N_Init();					//初始化L298N电机驱动模块
	KEY_Init();						//初始化按键及按键中断
 
	while(1){
		delay_ms(1000);
	}
}
