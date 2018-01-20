#ifndef __DS18B20_H
#define __DS18B20_H

#include <sys.h>
#include <iot.h>

//IO方向设置
#define DS18B20_IO_IN()  {DS18B20_GPIO->CRH&=0XFFFF0FFF;DS18B20_GPIO->CRH|=8<<12;}
#define DS18B20_IO_OUT() {DS18B20_GPIO->CRH&=0XFFFF0FFF;DS18B20_GPIO->CRH|=3<<12;}

//IO操作函数											   
#define	DS18B20_DQ_OUT PAout(0) //数据端口	PA0
#define	DS18B20_DQ_IN  PAin(0)  //数据端口	PA0

u8 DS18B20_Init(void);
void DS18B20_Rst(void);
u8 DS18B20_Check(void);
int16_t DS18B20_Get_Temp(void);
void DS18B20_Start(void);
void DS18B20_Write_Byte(u8 dat);
u8 DS18B20_Read_Byte(void);
u8 DS18B20_Read_Bit(void);

#endif
