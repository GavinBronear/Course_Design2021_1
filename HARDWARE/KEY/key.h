#ifndef _KEY_H_
#define _KEY_H_

#include "Systerm.h"

#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)//读取按键0
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)//读取按键1
#define KEY3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)//读取按键2 
#define KEY4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//读取按键2 
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//读取按键3(WK_UP) 

#define BF_start GPIO_SetBits(GPIOA,GPIO_Pin_4); 
#define TBF_stop GPIO_ResetBits(GPIOA,GPIO_Pin_4); 

#define LY_start GPIO_SetBits(GPIOA,GPIO_Pin_3); 
#define LY_stop GPIO_ResetBits(GPIOA,GPIO_Pin_3); 

#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	2	//KEY1按下
#define KEY2_PRES	3	//KEY2按下
#define KEY3_PRES	4	//KEY2按下
#define WKUP_PRES   5	//KEY_UP按下(即WK_UP/KEY_UP)


extern unsigned char counting ;
extern unsigned char coun2;

void KEY_Init(void);//IO初始化
u8 KEY_Scan(void);  	//按键扫描函数		
u8 KEY_Scan2(void);
u8 KEY_Scan3(void);

u8 KEY_Scan21(void);
u8 KEY_Scan31(void);
#endif
