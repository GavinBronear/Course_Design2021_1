#include "key.h"

uint8 counting = 0;
uint8 coun2 = 0;

//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_0;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0
	

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(void)
{	 
	static uint8 count = 0;
		count=key;
		if(KEY1==1)	
		{
			
			count ++ ;
			if(count >= 4)
			{
				count = 0;
			}			
			while(KEY1);		//�ȴ������ɿ�
			return count;
		}
	return count;
}

u8 KEY_Scan2(void)
{	 	 
	counting=key2;
		if(KEY2==1)	
		{
			counting ++ ;			
			if(counting > 3)
			{
				counting = 0;
			}			
			while(KEY2);		//�ȴ������ɿ�
			return counting;
		}
	return counting;
}
u8 KEY_Scan3(void)
{	 
	static uint8 counting1 = 1;
		if(KEY3==1)	
		{
			counting1 ++ ;
			
			if(counting1 > 3)
			{
				counting1 = 1;
			}
			
			while(KEY3);		//�ȴ������ɿ�
			return counting1;
		}
	return counting1;
}
/***********************ʱ�������ɨ�躯��***************************/
u8 KEY_Scan21(void)
{	 
	 
		if(KEY2==1)	
		{
			coun2 ++ ;
			
			if(coun2 > 6)
			{
				coun2 = 0;
			}
			
			while(KEY2);		//�ȴ������ɿ�
			return coun2;
		}
	return coun2;
}
u8 KEY_Scan31(void)
{	 
	static uint8 coun3 = 0;
		if(KEY3==1)	
		{
			coun3 ++ ;
			
			if(coun3 > 3)
			{
				coun3 = 0;
			}
			
			while(KEY3);		//�ȴ������ɿ�
			return coun3;
		}
	return coun3;
}



