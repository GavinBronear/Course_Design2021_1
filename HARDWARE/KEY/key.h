#ifndef _KEY_H_
#define _KEY_H_

#include "Systerm.h"

#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)//��ȡ����0
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)//��ȡ����1
#define KEY3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)//��ȡ����2 
#define KEY4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//��ȡ����2 
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//��ȡ����3(WK_UP) 

#define BF_start GPIO_SetBits(GPIOA,GPIO_Pin_4); 
#define TBF_stop GPIO_ResetBits(GPIOA,GPIO_Pin_4); 

#define LY_start GPIO_SetBits(GPIOA,GPIO_Pin_3); 
#define LY_stop GPIO_ResetBits(GPIOA,GPIO_Pin_3); 

#define KEY0_PRES 	1	//KEY0����
#define KEY1_PRES	2	//KEY1����
#define KEY2_PRES	3	//KEY2����
#define KEY3_PRES	4	//KEY2����
#define WKUP_PRES   5	//KEY_UP����(��WK_UP/KEY_UP)


extern unsigned char counting ;
extern unsigned char coun2;

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(void);  	//����ɨ�躯��		
u8 KEY_Scan2(void);
u8 KEY_Scan3(void);

u8 KEY_Scan21(void);
u8 KEY_Scan31(void);
#endif
