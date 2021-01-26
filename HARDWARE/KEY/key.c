#include "key.h"

uint8 counting = 0;
uint8 coun2 = 0;

//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_0;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOE2,3,4

	//初始化 WK_UP-->GPIOA.0	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0
	

}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
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
			while(KEY1);		//等待按键松开
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
			while(KEY2);		//等待按键松开
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
			
			while(KEY3);		//等待按键松开
			return counting1;
		}
	return counting1;
}
/***********************时间调整的扫描函数***************************/
u8 KEY_Scan21(void)
{	 
	 
		if(KEY2==1)	
		{
			coun2 ++ ;
			
			if(coun2 > 6)
			{
				coun2 = 0;
			}
			
			while(KEY2);		//等待按键松开
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
			
			while(KEY3);		//等待按键松开
			return coun3;
		}
	return coun3;
}



