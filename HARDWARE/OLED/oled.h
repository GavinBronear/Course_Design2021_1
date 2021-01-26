#ifndef _OLED_H_
#define _OLED_H_

#include "Systerm.h"
#include "stdlib.h"	  

typedef unsigned char       uint8;  /*  8 bits */
typedef unsigned short int  uint16; /* 16 bits */
typedef unsigned long int   uint32; /* 32 bits */
typedef unsigned long long  uint64; /* 64 bits */
typedef char                int8;   /*  8 bits */
typedef short int           int16;  /* 16 bits */
typedef long  int           int32;  /* 32 bits */

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC端口定义----------------  					   
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_11)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_11)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_10)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_10)
		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot); 
void OLED_Set_Pos(unsigned char x, unsigned char y);


void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);


void IIC_Start(void);
void IIC_Stop(void);
void OLED_WrCmd(unsigned char IIC_Command);
void OLED_WrDat(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);

void OLED_F6x8Str1(uint8 x,uint8 y,int d,uint8 t);
void OLED_F6x8Str2(uint8 x,uint8 y,int d,uint8 t);
void OLED_P6x8Str(uint8 x,uint8 y,uint8 ch[]);
void OLED_P6x8Str_select(uint8 x,uint8 y,uint8 ch[]);
void OLED_P6x8Str_r(uint8 x,uint8 y,uint8 ch[]);
void OLED_P8x16Str(uint8 x,uint8 y,uint8 ch[]);
void OLED_P8x16Str_r(uint8 x,uint8 y,uint8 ch[]);
void OLED_P16x16Ch(uint8 x,uint8 y,uint8 N);
void OLED_P16x8Ch(uint8 x,uint8 y,uint8 N);

void Float_to_Str ( float f_temp, uint8 str[], uint8 size, uint8 pot );

void Int_to_Str(int32 temp ,uint8 str[] ,uint8 size);        //将数据转换成字符串;
void PrintNum(uint8 x,uint8 y,uint16 num);
void OLED_P16x8Ch1(uint8 x,uint8 y,uint8 N);			//反向汉字


void IIC_Wait_Ack(void);
#endif  
