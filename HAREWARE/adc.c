#include <adc.h>
#include <delay.h>
#include <iot.h>

#if DEBUG_FLAG
	#include <stdio.h>
#endif

void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1, ENABLE);
	
	
	//初始化光敏模块引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	//设置ADC1时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	ADC_DeInit(ADC1);//复位ADC1
	
	//初始化ADC1参数，设置ADC1的工作模式以及规则序列的相关信息。
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;									//是否连续转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//右对其
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//外部中断
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;									//独立模式
	ADC_InitStructure.ADC_NbrOfChannel = 1;															//通道个数
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;												//是否使用扫描模式
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);				//使能ADC1
	
	ADC_ResetCalibration(ADC1);		//使能复位校准
	while(ADC_GetResetCalibrationStatus(ADC1));		//等待复位校准结束
	ADC_StartCalibration(ADC1);		//开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));				//等待校准结束
	
#if DEBUG_FLAG
	printf("adc initialization is completed");
	printf("The current illumination value:%d", Get_Illumination());
	printf("The current humidity value:%d", Get_Humidity());
#endif
}

/*
 * 获取ADC值
 * ch：通道值0~3
 **/
u16 Get_Adc(u8 ch)
{
	//设置指定ADC规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

u16 Get_Illumination(void)
{
	u32 temp_val = 0;
	u8 t;
	for(t = 0; t < 10; t++)
	{
		temp_val += Get_Adc(ADC_Channel_8);
		delay_ms(5);
	}
	return temp_val / 10;
}

u16 Get_Humidity(void)
{
	u32 temp_val = 0;
	u8 t;
	for(t = 0; t < 10; t++)
	{
		temp_val += Get_Adc(ADC_Channel_9);
		delay_ms(5);
	}
	return temp_val / 10;
}
