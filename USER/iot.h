#ifndef __IOT_H
#define __IOT_H

#include <stm32f10x.h>

/*
 * 串口调试开关
 * IOT_DEBUG==1时打开串口调试
 * IOT_DEBUG==0时关闭串口调试
 **/
#define IOT_DEBUG 1

/*
 * 此系统拥有两种模式，正常模式(normal)及
 * 设置模式(set)。
 * 在不同模式下每个按键定义的功能不同。
 **/
typedef enum {
	SYSTEM_Mode_Normal = 0x00,
	SYSTEM_Mode_Set
}SYSTEM_Mode;

/*
typedef struct {
	int8_t temperature;
	int8_t illumination;
	int8_t humidity;
} environment_st;
*/
/*
 * RH:湿度
 * C:温度
 * RG:光照强度
 **/

/*
 * 与按键引脚相关的宏定义
 **/
#define KEY_S1_GPIO GPIOB
#define KEY_S1_PIN GPIO_Pin_10

#define KEY_S2_GPIO GPIOC
#define KEY_S2_PIN GPIO_Pin_4

#define KEY_S3_GPIO GPIOC
#define KEY_S3_PIN GPIO_Pin_13



/*
 * 与LCD1602引脚相关的宏定义
 **/
#define LCD_RS_GPIO GPIOD
#define LCD_RS_PIN GPIO_Pin_13

#define LCD_RW_GPIO GPIOD
#define LCD_RW_PIN GPIO_Pin_14

#define LCD_E_GPIO GPIOD
#define LCD_E_PIN GPIO_Pin_15

#define LCD_DB_GPIO GPIOE
typedef enum {
	LCD_DB0_PIN = GPIO_Pin_0,
	LCD_DB1_PIN = GPIO_Pin_1,
	LCD_DB2_PIN = GPIO_Pin_2,
	LCD_DB3_PIN = GPIO_Pin_3,
	LCD_DB4_PIN = GPIO_Pin_4,
	LCD_DB5_PIN = GPIO_Pin_5,
	LCD_DB6_PIN = GPIO_Pin_6,
	LCD_DB7_PIN = GPIO_Pin_7,
}LCD_DB_PIN;

#define LCD_RCC_APB2Periph_GPIO (RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO)



/*
 * DS18B20相关定义
 **/
#define DS18B20_GPIO GPIOA
#define DS18B20_Pin 0
#define DS18B20_RCC_APB2Periph_GPIO RCC_APB2Periph_GPIOA

#endif
