#ifndef __ADC_H
#define __ADC_H

#include <stm32f10x_exti.h>

void Adc_Init(void);
u16 Get_Adc(u8 ch);
u16 Get_Illumination(void);
u16 Get_Humidity(void);

#endif
