#include <lcd1602.h>

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
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;   
	//浮空输入：http://blog.sina.com.cn/s/blog_bf63e2650102wihq.html
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_DB_GPIO, &GPIO_InitStructure);
	
	Reset_RS();
	Set_RW();  
	Reset_E();
	//Delay(5);
	Set_E();
	//Delay(25);
	while(GPIO_ReadInputDataBit(LCD_DB_GPIO, LCD_DB7_PIN) == 1);
	Reset_E();

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_DB_GPIO, &GPIO_InitStructure);
}  

//发送指令
void LCD1602_WriteCmd(uint8_t Cmd) {     
	LCD1602_BusyWait();    
	Reset_RS();    
	Reset_RW();    
	Reset_E();     
	// Delay(5);    
	Set_E();   
	GPIO_Write(LCD_DB_GPIO, Cmd);    
	/*     
	GPIO_WriteBit(GPIOE,GPIO_Pin_10,(BitAction)((Cmd&0x80)>>7));//D7    
	GPIO_WriteBit(GPIOE,GPIO_Pin_9,(BitAction)((Cmd&0x40)>>6)); //D6    
	GPIO_WriteBit(GPIOE,GPIO_Pin_8,(BitAction)((Cmd&0x20)>>5)); //D5    
	GPIO_WriteBit(GPIOE,GPIO_Pin_7,(BitAction)((Cmd&0x10)>>4)); //D4    
	GPIO_WriteBit(GPIOD,GPIO_Pin_1,(BitAction)((Cmd&0x08)>>3)); //D3    
	GPIO_WriteBit(GPIOD,GPIO_Pin_0,(BitAction)((Cmd&0x04)>>2)); //D2    
	GPIO_WriteBit(GPIOD,GPIO_Pin_15,(BitAction)((Cmd&0x02)>>1)); //D1    
	GPIO_WriteBit(GPIOD,GPIO_Pin_14,(BitAction)((Cmd&0x01))); //D0   
	*/     
	//Delay(25);    
	Reset_E();  
}   

static void LCD1602_WriteData(uint8_t Data) {     
	LCD1602_BusyWait();    
	Set_RS();    
	Reset_RW();    
	Reset_E();
	//  Delay(5);    
	Set_E();     
	GPIO_Write(LCD_DB_GPIO, (0xff00&(Data)));       
	/*     
	GPIO_WriteBit(GPIOE,GPIO_Pin_10,(BitAction)((Data&0x80)>>7));//D7    
	GPIO_WriteBit(GPIOE,GPIO_Pin_9,(BitAction)((Data&0x40)>>6)); //D6    
	GPIO_WriteBit(GPIOE,GPIO_Pin_8,(BitAction)((Data&0x20)>>5)); //D5    
	GPIO_WriteBit(GPIOE,GPIO_Pin_7,(BitAction)((Data&0x10)>>4)); //D4    
	GPIO_WriteBit(GPIOD,GPIO_Pin_1,(BitAction)((Data&0x08)>>3)); //D3    
	GPIO_WriteBit(GPIOD,GPIO_Pin_0,(BitAction)((Data&0x04)>>2)); //D2    
	GPIO_WriteBit(GPIOD,GPIO_Pin_15,(BitAction)((Data&0x02)>>1)); //D1   
	GPIO_WriteBit(GPIOD,GPIO_Pin_14,(BitAction)((Data&0x01))); //D0    
	*/    
	// Delay(25);   
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
	while(*p!='\0'){
		LCD1602_WriteData(*p++);
		//Buffer[i++]=Read_Data();
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
	if (LCD_Status == LCD_Display) {
		//显示开关控制:关显示,无光标,不闪烁
		LCD_CLOSE_DISPLAY;
		LCD_Status = LCD_No_Display;
	} else {
		//显示开关控制:开显示,无光标,不闪烁
		LCD_OPEN_DISPLAY;
		LCD_Status = LCD_Display;
	}
}

/*  
uint8_t Read_Data(void) {    
	uint8_t Value;    
	Busy_Wait();    
	Reset_WR();    
	Reset_E();    
	Delay(5);    
	Set_E();     
	Value=GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_10)||GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_9)
	||GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_8)||GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_7)
	||GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_1)||GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_0)
	||GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_15)||GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_14); 
	Delay(25);    
	Reset_E();    
	return Value; 
}  
*/
