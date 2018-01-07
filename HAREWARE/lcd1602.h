/*
 * LCD1602驱动程序
 * PE0-PE7 对应 LCD1602的D0-D7   
 * PD13    RS 
 * PD14    WR 
 * PD15    E                    
 **/
#ifndef __LCD1602_H
#define __LCD1602_H

#include <stm32f10x.h>
#include <iot.h>


/*
 * 此项目所用LCD是不带背光版本，
 * 所以LCD状态只有两种：
 * LCD_Display:显示文字
 * LCD_No_Display：没有显示文字
 **/
typedef enum
{ LCD_Display = 0x00,
  LCD_No_Display			
}LCD_Stat;

#define Set_RS()    GPIO_SetBits(LCD_RS_GPIO, LCD_RS_PIN);  	// 数据  
#define Reset_RS()  GPIO_ResetBits(LCD_RS_GPIO, LCD_RS_PIN); 	// 命令状态   
#define Set_RW()    GPIO_SetBits(LCD_RW_GPIO, LCD_RW_PIN); 		// 读 
#define Reset_RW()  GPIO_ResetBits(LCD_RW_GPIO, LCD_RW_PIN);	// 写   
#define Set_E()     GPIO_SetBits(LCD_E_GPIO, LCD_E_PIN); 			// 使能 
#define Reset_E()   GPIO_ResetBits(LCD_E_GPIO, LCD_E_PIN);		// 失能 

#define LCD_CLEAR_SCREEN  LCD1602_WriteCmd(0x01);							//清屏指令
#define LCD_OPEN_DISPLAY  LCD1602_WriteCmd(0x0C);							//打开显示
#define LCD_CLOSE_DISPLAY LCD1602_WriteCmd(0x08);							//关闭显示


void LCD1602_Init(void);
static void LCD1602_GpioConfig(void); 
static void LCD1602_BusyWait(void);  
static void LCD1602_WriteCmd(uint8_t Cmd); 
static void LCD1602_WriteData(uint8_t Data);  
void LCD1602_WriteString(uint8_t* p);   
void LCD1602_ChangeMode(void);

#endif
