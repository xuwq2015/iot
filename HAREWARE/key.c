#include <key.h>
#include <delay.h>
#include <l298n.h>
#include <lcd1602.h>
#include <stm32f10x.h>
#include <stm32f10x_exti.h>

void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//使能按键时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//初始化S1按键
	GPIO_InitStructure.GPIO_Pin = KEY_S1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(KEY_S1_GPIO, &GPIO_InitStructure);
	
	//初始化S2按键
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* 初始化S3按键 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	set_exti();
}

/*
 * 按键中断初始化函数
 * S1按键为下降沿触发，S2和S3为边沿触发
 * S1、S2、S3按键抢占优先级为1
 * S1响应优先级为1，S2、S3响应优先级为0
 * S1、S3中断服务函数为EXTI15_10_IRQHandler
 * S2中断服务函数为EXTI4_IRQHandler
 **/
static void set_exti(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//使能复用功能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//S1按键中断线以及中断初始化配置 下降沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);
	EXTI_InitStruct.EXTI_Line = EXTI_Line10;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	//S2按键中断线以及中断初始化配置 边沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	//S3按键中断线以及中断初始化配置 边沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
	EXTI_InitStruct.EXTI_Line = EXTI_Line13;
	EXTI_Init(&EXTI_InitStruct);
	
	
	
	//使能S1所在的外部中断通道
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn; 
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_Init(&NVIC_InitStruct);
	
	//使能S2所在的外部中断通道
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_Init(&NVIC_InitStruct);
	
	//使能S3所在的外部中断通道
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_Init(&NVIC_InitStruct);
}

/*
 * S2按键中断服务函数
 **/
void EXTI4_IRQHandler(void)
{
	delay_ms(10);		//消除抖动

	//判断S2按键当前状态
	if (KEY_S2 == KEY_PRESS)
		L298N_Rise();
	else if (KEY_S2 == KEY_RELEASE)
		L298N_stop();
	
	EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE4上的中断标志位
}

/*
 * S1和S3按键中断服务函数
 * S2按键是边沿触发，所以按下、松开都会触发中断，根据S2当前状态
 * 来调用L298N的驱动函数。
 * S1为下边沿触发，只有按下时才会触发中断，所以需要调用延时函数
 * 来判断是短按还是长按。
 * 注意：要先判断S1是否为按下状态，在判断S3是否为松开状态，不然
 * 会使S1按键失效。
 **/
void EXTI15_10_IRQHandler (void)
{
	delay_ms(10);	
	
	//判断是否为S1按键按下
	if (KEY_S1 == KEY_PRESS) {
		delay_ms(1300);
		if (KEY_S1 == KEY_PRESS) {//长按
		} else if (KEY_S1 == KEY_RELEASE) {//短按
			LCD1602_ChangeMode();
		}
		EXTI_ClearITPendingBit(EXTI_Line10);
		return;
	}
	
	//判断S3按键当前状态
	if (KEY_S3 == KEY_PRESS) {
		L298N_Decline();
	} else if (KEY_S3 == KEY_RELEASE ) {
		L298N_stop();
	}
	
	EXTI_ClearITPendingBit(EXTI_Line13); //清除LINE13上的中断标志位
}
