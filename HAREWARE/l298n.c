#include <l298n.h>
#include <stm32f10x.h>

void L298N_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/* 初始化IO口PA2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 初始化IO口PA3 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//将IN1、IN2对应的引脚电平拉低
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
}

//上升
void L298N_Rise(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	//将IN2对应的引脚电平拉高
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
}

//下降
void L298N_Decline(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
}

//停止
void L298N_stop(void)
{
	//将IN1、IN2对应的引脚电平拉低
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
}
