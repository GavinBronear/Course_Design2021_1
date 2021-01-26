 #ifndef _DS1302_H_
#define _DS1302_H_

#include "Systerm.h"

extern u8 ascii_time[7];     //保存ascii格式数据  
  
extern u8 bcd_time[7];       //保存bcd码数据  


typedef struct DS1302DATA  
{  
    u8 year;    //年  
    u8 month;   //月 
    u8 day;     //日 
    u8 hour;    //时 
    u8 min;     //分 
    u8 sec;     //秒 
    u8 week;    //周 
}DS1302DATA;  
  
extern struct DS1302DATA ds1302Data;  
extern int	clock_gengxin[7];  
#define DS1302_RST      PBout(5)  
#define DS1302_CLK      PBout(7)  
#define DS1302_DATIN    PBin(6)  
#define DS1302_DATOUT   PBout(6)  
  
#define DS1302_DAT_INPUT()     {GPIOB->CRL&= 0XF0FFFFFF;GPIOB->CRL|= 8<<24;}  
#define DS1302_DAT_OUTPUT()    {GPIOB->CRL&= 0XF0FFFFFF;GPIOB->CRL|= 3<<24;}  
  
 
#define DS1302_SEC_REG          0x80        //写秒数据地址  
#define DS1302_MIN_REG          0x82        //写分数据地址
#define DS1302_HR_REG           0x84        //写时数据地址
#define DS1302_DATE_REG         0x86        //写日数据地址
#define DS1302_MONTH_REG        0x88        //写月数据地址
#define DS1302_DAY_REG          0x8a        //写星期几数据地址
#define DS1302_YEAR_REG         0x8c        //写年数据地址

#define DS1302_CONTROL_REG      0x8e        //写保护寄存器地址
#define DS1302_CHARGER_REG      0x90        //涓流充电寄电器     
#define DS1302_CLKBURST_REG     0xbe        //脉冲串寄存器  
#define DS1302_RAM_BASE         0X30                //RAM寄存器  
#define CLOCKSTOP                       0X80
#define CLOCKSTART                      0X00
  
void Ds1302_Gpio_Init(void);  
u8 Ds1302_Read(u8 address);
static u8 AsciiToBcd(u8 asciiData);
static u8 BcdToAscii(u8 bcdData);
void Ds1302_WriteByte(u8 dat);
void Ds1302_Write(u8 address,u8 dat);
void Ds1302_Write_Time_Singel(u8 address,u8 dat);
void Ds1302_Write_Time_All(u8 start);  
u8 Ds1302_ReadByte(void);
void Ds1302_Readtime(void);
void Ds1302_Write_Time_All1(void);  

	
#endif
