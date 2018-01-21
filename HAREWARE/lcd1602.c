#include <lcd1602.h>
#include <delay.h>
#include <iot.h>

#if DEBUG_FLAG
	#include <stdio.h>
	#include <set.h>
#endif

//LCD状态标记
static LCD_Stat LCD_Status;

//uint8_t Buffer1[]={"FGasdkkk"};  


//初始化LCD所需IO
static void LCD1602_GpioConfig(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LCD_RCC_APB2Periph_GPIO, ENABLE);
	
	//初始化与LCD数据引脚有关的GPIO端口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;             
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_Init(LCD_DB_GPIO, &GPIO_InitStructure);  
  
	//初始化与LCD RS引脚相连的GPIO端口
	GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN;
	GPIO_Init(LCD_RS_GPIO, &GPIO_InitStructure);
	
	//初始化与LCD RW引脚相连的GPIO端口
	GPIO_InitStructure.GPIO_Pin = LCD_RW_PIN;
	GPIO_Init(LCD_RW_GPIO, &GPIO_InitStructure);
	
	//初始化与LCD E引脚相连的GPIO端口
	GPIO_InitStructure.GPIO_Pin = LCD_E_PIN;
	GPIO_Init(LCD_E_GPIO, &GPIO_InitStructure);
}

/*
 * 读忙函数
 * 此函数内使用while循环不停读取LCD_DB7_PIN引脚上的电平信号
 * 直到LCD_DB7_PIN引脚电平编为0(低电平)为止
 **/
static void LCD1602_BusyWait(void) {  
	u8 sta;
	
	DATAOUT(0xff);
	Reset_RS();
	Set_RW();
	do
	{
		Set_E();
		delay_ms(5);	//延时5ms，非常重要
 		sta = GPIO_ReadInputDataBit(LCD_DB_GPIO, LCD_DB7_PIN);//读取状态字
		Reset_E();
	}while(sta & 0x80);//bit7等于1表示液晶正忙，重复检测直到其等于0为止
}  

//发送指令
static void LCD1602_WriteCmd(uint8_t cmd) {     
	LCD1602_BusyWait();    
	Reset_RS();    
	Reset_RW();      
	DATAOUT(cmd);    
	Set_E();     
	Reset_E();  
}   

static void LCD1602_WriteData(uint8_t Data) {     
	LCD1602_BusyWait();    
	Set_RS();    
	Reset_RW();        
	DATAOUT(Data); 
	Set_E();     
	Reset_E();
}    

/*
 * 写显示数据函数
 * 每次写入之前会先清屏
 **/
void LCD1602_WriteString(uint8_t* p) {   
	//清屏
	LCD_CLEAR_SCREEN;
	
	LCD1602_WriteCmd(0x80);  
	while(*p != '\0'){
		LCD1602_WriteData(*p++);
	}  
}   

//LCD初始化函数
void LCD1602_Init(void) { 
	LCD1602_GpioConfig();
	
	//功能设置命令:8位总线、单行显示、5x10的点阵字符
	LCD1602_WriteCmd(0x24);
	
	//显示开关控制:开显示,无光标,不闪烁
	LCD_OPEN_DISPLAY;   
	
	//光标和显示模式设置:光标右移，文字移动
	LCD1602_WriteCmd(0x06);    
	
	//清屏
	LCD_CLEAR_SCREEN;
	LCD_Status = LCD_Display;
}

/*
 * 改变LCD当前模式
 **/
void LCD1602_ChangeMode(void)
{
#if DEBUG_FLAG
	if (system_mode == SYSTEM_Mode_Normal)
		printf("System mode is normal mode");
	else if (system_mode == SYSTEM_Mode_Set)
		printf("System mode is set mode");
#endif
	
	if (LCD_Status == LCD_Display) {
#if DEBUG_FLAG
		printf("Close lcd");
#endif
		//显示开关控制:关显示,无光标,不闪烁
		LCD_CLOSE_DISPLAY;
		LCD_Status = LCD_No_Display;
	} else {
#if DEBUG_FLAG
		printf("Open lcd");
#endif
		//显示开关控制:开显示,无光标,不闪烁
		LCD_OPEN_DISPLAY;
		LCD_Status = LCD_Display;
	}
}
