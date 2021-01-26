#include "ds1302.h"  
#include "stdio.h"  
#include "Delay.h"  
 


#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//




	//														年 月 日	时 时 秒 周 
struct DS1302DATA ds1302Data = {19,7,5,19,35,40,6};  
u8 ascii_time[7] = {0};     //保存ascii格式数据  
u8 bcd_time[7] = {0};       //保存bcd码数据  
  
static u8 AsciiToBcd(u8 asciiData)  
{  
    u8 bcdData = 0;  
    bcdData = (((asciiData/10)<<4)|((asciiData%10)));  
    return bcdData;  
}

static u8 BcdToAscii(u8 bcdData)  
{  
    u8 asciiData = 0;  
    asciiData = (((bcdData&0xf0)>>4)*10 + (bcdData&0x0f));  
    return asciiData;  
}  
  
//IO初始化
void Ds1302_Gpio_Init(void)  
{     
		GPIO_InitTypeDef  GPIO_InitStructure;
 	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能A端口时钟
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_6;	 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOD3,6
		GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_7);	
}  
  
//读取一个字节的时序，在下降沿进行读
u8 Ds1302_ReadByte(void)  
{  
    u8 i = 0, dat = 0;  
    DS1302_DAT_INPUT();  
    delay_us(5);  
    for(i = 0; i <8; i++)  
    {  
        dat >>= 1;  
        if(DS1302_DATIN == 1)dat |= 0x80;//低电平读取  
        DS1302_CLK = 1;//后拉高获得下一字节数据  
        delay_us(2);  
        DS1302_CLK = 0;  
        delay_us(2);  
    }  
    return dat;  
}  
  
//写入一个字节的时序，在上升沿进行写
void Ds1302_WriteByte(u8 dat)  
{  
    u8 i = 0, data = dat;  
    DS1302_DAT_OUTPUT();   
    DS1302_CLK = 0;  
    delay_us(2);  
    for(i = 0; i < 8; i++)  
    {  
        DS1302_DATOUT = data&0x01;  
        delay_us(2);//延时等待低电平稳定  
        DS1302_CLK = 1;//拉高写入  
        delay_us(2);  
        DS1302_CLK = 0;  
        data >>= 1;  
    }  
}  
  
//写入一个寄存器一个字节数据
void Ds1302_Write(u8 address,u8 dat)  
{  
    DS1302_RST = 0;  
    DS1302_CLK = 0;  //只有在clk低电平时，拉高rst才有用
    DS1302_RST = 1;  
    Ds1302_WriteByte(address);  
    Ds1302_WriteByte(dat);  
    DS1302_CLK = 1;  
    DS1302_RST = 0;  
}  
  
//单个写入时间
void Ds1302_Write_Time_Singel(u8 address,u8 dat)  
{  
    Ds1302_Write(DS1302_CONTROL_REG,0x00);  //取消写保护  
    Ds1302_Write(address,dat);  
    Ds1302_Write(DS1302_CONTROL_REG,0x80);  //打开写保护  
}  
  
//一次完成所有时间的更新
//start当前时钟运行还是停止
void Ds1302_Write_Time_All(u8 start)  
{  
    Ds1302_Write(DS1302_CONTROL_REG,0x00);      //取消写保护 
    Ds1302_Write(DS1302_SEC_REG,(AsciiToBcd(ds1302Data.sec)|start));  
    Ds1302_Write(DS1302_MIN_REG,AsciiToBcd(ds1302Data.min));  
    Ds1302_Write(DS1302_HR_REG,AsciiToBcd(ds1302Data.hour));  
    Ds1302_Write(DS1302_DATE_REG,AsciiToBcd(ds1302Data.day));  
    Ds1302_Write(DS1302_MONTH_REG,AsciiToBcd(ds1302Data.month));  
    Ds1302_Write(DS1302_DAY_REG,AsciiToBcd(ds1302Data.week));  
    Ds1302_Write(DS1302_YEAR_REG,AsciiToBcd(ds1302Data.year));  
    Ds1302_Write(DS1302_CONTROL_REG,0x80);  //打开写保护  
}  

void Ds1302_Write_Time_All1(void)  
{  
    Ds1302_Write(DS1302_CONTROL_REG,0x00);      //取消写保护 
		Ds1302_Write(DS1302_SEC_REG,(AsciiToBcd(clock_gengxin[5])|0X00));  
		Ds1302_Write(DS1302_MIN_REG,AsciiToBcd(clock_gengxin[4]));  
		Ds1302_Write(DS1302_HR_REG,AsciiToBcd(clock_gengxin[3]));  
		Ds1302_Write(DS1302_DATE_REG,AsciiToBcd(clock_gengxin[2]));  
		Ds1302_Write(DS1302_MONTH_REG,AsciiToBcd(clock_gengxin[1]));  
		Ds1302_Write(DS1302_DAY_REG,AsciiToBcd(clock_gengxin[6]));  
		Ds1302_Write(DS1302_YEAR_REG,AsciiToBcd(clock_gengxin[0]));  
		Ds1302_Write(DS1302_CONTROL_REG,0x80);  //打开写保护  
} 
  
//读取一个字节
u8 Ds1302_Read(u8 address)  
{  
    u8 data = 0;  
    DS1302_RST = 0;  
    DS1302_CLK = 0; //只有在clk低电平时，拉高rst才有用
    DS1302_RST = 1;  
    Ds1302_WriteByte(address|0x01); //读取地址需要与0x01相或，最低为变成1  
    data = Ds1302_ReadByte();  
    DS1302_CLK = 1;  
    DS1302_RST = 0;  
    return data;  
}  
  
//读取实时间的时候默认让时间走起来  
void Ds1302_Readtime(void)  
{  
    ds1302Data.sec = BcdToAscii(Ds1302_Read(DS1302_SEC_REG));  //秒 
    ds1302Data.min = BcdToAscii(Ds1302_Read(DS1302_MIN_REG));  //分 
    ds1302Data.hour = BcdToAscii(Ds1302_Read(DS1302_HR_REG));   //小时  
    ds1302Data.day = BcdToAscii(Ds1302_Read(DS1302_DATE_REG)); //日
    ds1302Data.month = BcdToAscii(Ds1302_Read(DS1302_MONTH_REG)); //月  
    ds1302Data.week = BcdToAscii(Ds1302_Read(DS1302_DAY_REG));  //星期几 
    ds1302Data.year = BcdToAscii(Ds1302_Read(DS1302_YEAR_REG)); //年
		ascii_time[0] = ds1302Data.sec;
		ascii_time[1] = ds1302Data.min;
		ascii_time[2] = ds1302Data.hour;
		ascii_time[3] = ds1302Data.day;
		ascii_time[4] = ds1302Data.month;
		ascii_time[5] = ds1302Data.week;
		ascii_time[6] = ds1302Data.year;
} 

