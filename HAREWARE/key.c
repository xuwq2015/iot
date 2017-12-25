#include <key.h>
#include <delay.h>
#include <l298n.h>
#include <stm32f10x.h>
#include <stm32f10x_exti.h>

void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* 初始化IO口PC4 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	set_exti();
}

static void set_exti(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//使能复用功能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//GPIOC.4	  中断线以及中断初始化配置 边沿触发 //S2
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	//GPIOC.13	  中断线以及中断初始化配置 边沿触发 //S3
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
	EXTI_InitStruct.EXTI_Line = EXTI_Line13;
	EXTI_Init(&EXTI_InitStruct);
	
	
	/*
	 * 使能按键S2、S3所在的外部中断通道
	 * S2、S3按键抢占优先级为1
	 * S2子优先级为0
	 * S3子优先级为0
	 **/
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	//NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_Init(&NVIC_InitStruct);
}

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

void EXTI15_10_IRQHandler (void)
{
	delay_ms(10);	
	
	//判断S3按键当前状态
	if (KEY_S3 == KEY_PRESS)
		L298N_Decline();
	else if (KEY_S3 == KEY_RELEASE)
		L298N_stop();
	
	EXTI_ClearITPendingBit(EXTI_Line13); //清除LINE13上的中断标志位
}
