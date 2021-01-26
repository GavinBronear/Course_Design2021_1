/*------------------------------------------------------
课题：物联网天气风铃时钟
组员：蔡泽楷 罗高宇 吴昊天
日期：2021.1.19
------------------------------------------------------*/
#include "Systerm.h" 
#include "esp8266.h"
#include "weather.h"
#include "bellring.h"
/*****************秒表变量*********************/
u32 Stopwatch_counting = 0;
u8 Stopwatch_Start = 0;
u8 Stopwatch_RST = 0;
u8 key=0;	
u8 key1=0;	
u8 key2=0;		 	
u8 key3=0;
u8 weather_code=99;
u8 temperature;
u8 clock_en=0;
/*****************时间调整*********************/

int	clock_gengxin[7] = {0,1,1};
 int main(void)
 {		 	 
	 uint8 Add1=0,Add2=0,Add3=0,Add4=0,Add5=0,Add6 = 0;
	 u32 Stopwatch_count[2];
	 uint8 i;
	 u8 clockctrl,weatherctrl=2;//用于确保天气和时间只在规定时间更新
	 uint16 Shi = 0,Fen = 0,Miao = 0;
	 uint16 Alarm_clock_S1 = 0,Alarm_clock_F1 = 0,Alarm_clock_M1 = 0;
	  /*****************时间调整变量*********************/
	 u32 t = 2000;	 
	 Systerm_Init();
 	 printf("%s\r\n",Get_Current_Weather());
	while(1)
	{	
		key = KEY_Scan();		//读取按键值		
		/*****************界面一：秒表*********************/
		if(key == 1)													
		{
				if(!Add1)
				{
						OLED_Clear();		//先清屏
					  Add1 = 1;
				}
				Add2 = 0;
				Add3 = 0;
				Add4 = 0;
				Add5 = 0;
				Add6 = 0;			
				for(i=10;i<12;i++)
				{
					OLED_P16x16Ch(46+(i-10)*16,0,i);//一界面显示“秒表”
				}
				for(i=14;i<16;i++)
				{
					OLED_P16x8Ch(1+(i-14)*16,3,i);//一界面显示“计时”
				}
				if(KEY2==1)
				{
						Stopwatch_Start = 1;
				}
				else if(KEY3==1)
				{
						Stopwatch_Start = 0;
				}
				else if(WK_UP==1)
				{
						Stopwatch_counting = 0;
						Stopwatch_Start = 0;
				}
				
				Stopwatch_count[1] = Stopwatch_counting/10;
				Stopwatch_count[0] = Stopwatch_counting%10;
				
				OLED_F6x8Str1(84,6,Stopwatch_count[1],4);
				OLED_P6x8Str(108,6,".");
				OLED_F6x8Str1(114,6,Stopwatch_count[0],1);
		}
		/*****************界面二：闹钟*********************/
		else if(key == 2)
		{
				if(!Add3)
				{
						OLED_Clear();		//先清屏
					  Add3 = 1;
				}
				Add1 = 0;
				Add2 = 0;
				Add4 = 0;
				Add5 = 0;
				Add6 = 0;
				
				for(i=29;i<31;i++)
				{
					OLED_P16x16Ch(46+(i-29)*16,0,i);//二界面显示“闹钟”
				}
				

				key1 =1; 		//读取按键值		
				if(key1 == 1)
				{
						for(i=20;i<23;i++)
						{
							OLED_P16x8Ch1(5+(i-20)*16,3,i);//二界面显示“定时一”
						}
						OLED_P16x8Ch1(5+3*16,3,29);//二界面显示“：”
						
						key2 = KEY_Scan2();		//读取按键值
						if(key2==3)key2=0;
						if(key2 == 0)
						{
								OLED_F6x8Str2(67,3,Shi,2);		//shi
								OLED_P6x8Str(82,3,":");
								OLED_F6x8Str1(91,3,Fen,2);		//shi
								OLED_P6x8Str(106,3,":");
								OLED_F6x8Str1(115,3,Miao,2);		//shi
								if(KEY3 == 1)
								{
										delay_ms(100);
										if(KEY3 == 1)
										{
											Shi++;
											if(Shi>23)
											{
													Shi = 0;
											}
											//while(KEY3);
										}
								}
								else if(KEY4==1){
									delay_ms(20);
									if(KEY4==1){
										if(Shi>0)
												Shi--;
										else
												Shi=23;
										while(KEY4);
									}										
								}
								
						}
						else if(key2 == 1)
						{
								OLED_F6x8Str1(67,3,Shi,2);		//时
								OLED_P6x8Str(82,3,":");
								OLED_F6x8Str2(91,3,Fen,2);		//分
								OLED_P6x8Str(106,3,":");
								OLED_F6x8Str1(115,3,Miao,2);		//秒
								if(KEY3 == 1)
								{
										delay_ms(100);
										if(KEY3 == 1)
										{
											Fen++;
											if(Fen>59)
											{
													Fen = 0;
											}
										
										}
								}
								else if(KEY4==1){
									delay_ms(20);
									if(KEY4==1){
										if(Fen>0)
												Fen--;
										else
												Fen=59;
										while(KEY4);
									}
									
								}
						}
						else if(key2 == 2)
						{
								OLED_F6x8Str1(67,3,Shi,2);		//时
								OLED_P6x8Str(82,3,":");
								OLED_F6x8Str1(91,3,Fen,2);		//分
								OLED_P6x8Str(106,3,":");
								OLED_F6x8Str2(115,3,Miao,2);		//秒
								if(KEY3 == 1)
									{
											delay_ms(100);
											if(KEY3 == 1)
											{
												Miao++;
												if(Miao>59)
												{
														Miao = 0;
												}
												
											}
									}
						}
						if(WK_UP == 1)
						{
								delay_ms(20);
								if(WK_UP == 1){
									Alarm_clock_S1 = Shi;
									Alarm_clock_F1 = Fen;
									Alarm_clock_M1 = Miao;
									counting = 0;
									if(clock_en==1)clock_en=0;
									else clock_en=1;
									while(WK_UP);
								}
						}
						if(clock_en){
							OLED_P6x8Str(100,6,"O");
							OLED_P6x8Str(106,6,"N");
							OLED_P6x8Str(112,6," ");
						}else{
							OLED_P6x8Str(100,6,"O");
							OLED_P6x8Str(106,6,"F");
							OLED_P6x8Str(112,6,"F");
						}
									
				}
						/*******************************定时初始界面结束*******************************/
				
				
		}
		/*************************************************三界面：时间调整*********************************************/
		else if(key == 3)
		{
				if(!Add4)
				{
						OLED_Clear();		//先清屏
					  Add4 = 1;
				}
				Add1 = 0;
				Add2 = 0;
				Add3 = 0;
				Add5 = 0;
				Add6 = 0;
				
				for(i=31;i<35;i++)
				{
					OLED_P16x16Ch(28+(i-31)*16,0,i);//显示“界面调整”
				}
				
				key3 = KEY_Scan21();
				
				
				if(key3 == 1)
				{
						OLED_P16x8Ch(5,3,30);//四界面显示“时”
						OLED_P16x8Ch(5,6,31);//四界面显示“间”
					
						OLED_P16x8Ch(40,2,0);//四界面显示“年”
						OLED_P16x8Ch(61,2,29);//四界面显示“：”
						OLED_P16x8Ch(40,3,1);//四界面显示“月”
						OLED_P16x8Ch(61,3,29);//四界面显示“：”
						OLED_P16x8Ch(40,4,2);//四界面显示“日”
						OLED_P16x8Ch(61,4,29);//四界面显示“：”
						OLED_P16x8Ch(40,5,3);//四界面显示“时”
						OLED_P16x8Ch(61,5,29);//四界面显示“：”
						OLED_P16x8Ch(40,6,4);//四界面显示“分”
						OLED_P16x8Ch(61,6,29);//四界面显示“：”
						OLED_P16x8Ch(40,7,5);//四界面显示“秒”
						OLED_P16x8Ch(61,7,29);//四界面显示“：”
					
						OLED_F6x8Str2(96,2,t+clock_gengxin[0],4);		//shi
						OLED_F6x8Str1(108,3,clock_gengxin[1],2);		//shi
						OLED_F6x8Str1(108,4,clock_gengxin[2],2);		//shi
						OLED_F6x8Str1(108,5,clock_gengxin[3],2);		//shi
						OLED_F6x8Str1(108,6,clock_gengxin[4],2);		//shi
						OLED_F6x8Str1(108,7,clock_gengxin[5],2);		//shi
						if(KEY3 == 1)
						{
								delay_ms(100);
								if(KEY3 == 1)
								{
									clock_gengxin[0]++;
									if(clock_gengxin[0]>99)
									{
											clock_gengxin[0] = 0;
											t+=100;
									}
								}
						}
						else if(KEY4 == 1)
						{
								delay_ms(100);
								if(KEY4 == 1)
								{
									clock_gengxin[0]--;
									if(clock_gengxin[0]<0)
									{
											clock_gengxin[0] = 99;
											t-=100;
									}
								}
						}
						
				}
				else if(key3 == 2)
				{
						OLED_P16x8Ch(5,3,30);//四界面显示“时”
						OLED_P16x8Ch(5,6,31);//四界面显示“间”
					
						OLED_P16x8Ch(40,2,0);//四界面显示“年”
						OLED_P16x8Ch(61,2,29);//四界面显示“：”
						OLED_P16x8Ch(40,3,1);//四界面显示“月”
						OLED_P16x8Ch(61,3,29);//四界面显示“：”
						OLED_P16x8Ch(40,4,2);//四界面显示“日”
						OLED_P16x8Ch(61,4,29);//四界面显示“：”
						OLED_P16x8Ch(40,5,3);//四界面显示“时”
						OLED_P16x8Ch(61,5,29);//四界面显示“：”
						OLED_P16x8Ch(40,6,4);//四界面显示“分”
						OLED_P16x8Ch(61,6,29);//四界面显示“：”
						OLED_P16x8Ch(40,7,5);//四界面显示“秒”
						OLED_P16x8Ch(61,7,29);//四界面显示“：”
					
						OLED_F6x8Str1(96,2,2000+clock_gengxin[0],4);		//年
						OLED_F6x8Str2(108,3,clock_gengxin[1],2);		//月
						OLED_F6x8Str1(108,4,clock_gengxin[2],2);		//日
						OLED_F6x8Str1(108,5,clock_gengxin[3],2);		//时
						OLED_F6x8Str1(108,6,clock_gengxin[4],2);		//分
						OLED_F6x8Str1(108,7,clock_gengxin[5],2);		//秒
						if(KEY3 == 1)
						{
								delay_ms(100);
								if(KEY3 == 1)
								{
									clock_gengxin[1]++;
									if(clock_gengxin[1]>12)
									{
											clock_gengxin[1] = 1;
									}
								}
						}
						else if(KEY4 == 1)
						{
								delay_ms(100);
								if(KEY4 == 1)
								{
									clock_gengxin[1]--;
									if(clock_gengxin[1]<1)
									{
											clock_gengxin[1] = 12;
									}
								}
						}
				}
				else if(key3 == 3)
				{
						OLED_P16x8Ch(5,3,30);//四界面显示“时”
						OLED_P16x8Ch(5,6,31);//四界面显示“间”
					
						OLED_P16x8Ch(40,2,0);//四界面显示“年”
						OLED_P16x8Ch(61,2,29);//四界面显示“：”
						OLED_P16x8Ch(40,3,1);//四界面显示“月”
						OLED_P16x8Ch(61,3,29);//四界面显示“：”
						OLED_P16x8Ch(40,4,2);//四界面显示“日”
						OLED_P16x8Ch(61,4,29);//四界面显示“：”
						OLED_P16x8Ch(40,5,3);//四界面显示“时”
						OLED_P16x8Ch(61,5,29);//四界面显示“：”
						OLED_P16x8Ch(40,6,4);//四界面显示“分”
						OLED_P16x8Ch(61,6,29);//四界面显示“：”
						OLED_P16x8Ch(40,7,5);//四界面显示“秒”
						OLED_P16x8Ch(61,7,29);//四界面显示“：”
					
						OLED_F6x8Str1(96,2,2000+clock_gengxin[0],4);		//shi
						OLED_F6x8Str1(108,3,clock_gengxin[1],2);		//shi
						OLED_F6x8Str2(108,4,clock_gengxin[2],2);		//shi
						OLED_F6x8Str1(108,5,clock_gengxin[3],2);		//shi
						OLED_F6x8Str1(108,6,clock_gengxin[4],2);		//shi
						OLED_F6x8Str1(108,7,clock_gengxin[5],2);		//shi
						if(KEY3 == 1)
						{
								delay_ms(100);
								if(KEY3 == 1)
								{
									clock_gengxin[2]++;
									if(clock_gengxin[2]>31)
									{
											clock_gengxin[2] = 1;
									}									
								}
						}
						else if(KEY4 == 1)
						{
								delay_ms(100);
								if(KEY4 == 1)
								{
									clock_gengxin[2]--;
									if(clock_gengxin[2]<1)
									{
											clock_gengxin[2] = 31;
									}
								}
						}
		
				}
				else if(key3 == 4)
				{
						OLED_P16x8Ch(5,3,30);//四界面显示“时”
						OLED_P16x8Ch(5,6,31);//四界面显示“间”					
						OLED_P16x8Ch(40,2,0);//四界面显示“年”
						OLED_P16x8Ch(61,2,29);//四界面显示“：”
						OLED_P16x8Ch(40,3,1);//四界面显示“月”
						OLED_P16x8Ch(61,3,29);//四界面显示“：”
						OLED_P16x8Ch(40,4,2);//四界面显示“日”
						OLED_P16x8Ch(61,4,29);//四界面显示“：”
						OLED_P16x8Ch(40,5,3);//四界面显示“时”
						OLED_P16x8Ch(61,5,29);//四界面显示“：”
						OLED_P16x8Ch(40,6,4);//四界面显示“分”
						OLED_P16x8Ch(61,6,29);//四界面显示“：”
						OLED_P16x8Ch(40,7,5);//四界面显示“秒”
						OLED_P16x8Ch(61,7,29);//四界面显示“：”					
						OLED_F6x8Str1(96,2,2000+clock_gengxin[0],4);		//shi
						OLED_F6x8Str1(108,3,clock_gengxin[1],2);		//shi
						OLED_F6x8Str1(108,4,clock_gengxin[2],2);		//shi
						OLED_F6x8Str2(108,5,clock_gengxin[3],2);		//shi
						OLED_F6x8Str1(108,6,clock_gengxin[4],2);		//shi
						OLED_F6x8Str1(108,7,clock_gengxin[5],2);		//shi
						if(KEY3 == 1)
						{
								delay_ms(100);
								if(KEY3 == 1)
								{
									clock_gengxin[3]++;
									if(clock_gengxin[3]>23)
									{
											clock_gengxin[3] = 0;
									}
								}
						}
						else if(KEY4 == 1)
						{
								delay_ms(100);
								if(KEY4 == 1)
								{
									clock_gengxin[3]--;
									if(clock_gengxin[3]<1)
									{
											clock_gengxin[3] = 24;
									}
								}
						}
				}
				else if(key3 == 5)
				{
						OLED_P16x8Ch(5,3,30);//四界面显示“时”
						OLED_P16x8Ch(5,6,31);//四界面显示“间”
					
						OLED_P16x8Ch(40,2,0);//四界面显示“年”
						OLED_P16x8Ch(61,2,29);//四界面显示“：”
						OLED_P16x8Ch(40,3,1);//四界面显示“月”
						OLED_P16x8Ch(61,3,29);//四界面显示“：”
						OLED_P16x8Ch(40,4,2);//四界面显示“日”
						OLED_P16x8Ch(61,4,29);//四界面显示“：”
						OLED_P16x8Ch(40,5,3);//四界面显示“时”
						OLED_P16x8Ch(61,5,29);//四界面显示“：”
						OLED_P16x8Ch(40,6,4);//四界面显示“分”
						OLED_P16x8Ch(61,6,29);//四界面显示“：”
						OLED_P16x8Ch(40,7,5);//四界面显示“秒”
						OLED_P16x8Ch(61,7,29);//四界面显示“：”
					
						OLED_F6x8Str1(96,2,2000+clock_gengxin[0],4);		//shi
						OLED_F6x8Str1(108,3,clock_gengxin[1],2);		//shi
						OLED_F6x8Str1(108,4,clock_gengxin[2],2);		//shi
						OLED_F6x8Str1(108,5,clock_gengxin[3],2);		//shi
						OLED_F6x8Str2(108,6,clock_gengxin[4],2);		//shi
						OLED_F6x8Str1(108,7,clock_gengxin[5],2);		//shi
						if(KEY3 == 1)
						{
								delay_ms(100);
								if(KEY3 == 1)
								{
									clock_gengxin[4]++;
									if(clock_gengxin[4]>59)
									{
											clock_gengxin[4] = 0;
									}
								}
						}
						else if(KEY4 == 1)
						{
								delay_ms(100);
								if(KEY4 == 1)
								{
									clock_gengxin[4]--;
									if(clock_gengxin[4]<0)
									{
											clock_gengxin[4] = 59;
									}
								}
						}
				}
				else if(key3 == 6)
				{
						OLED_P16x8Ch(5,3,30);//四界面显示“时”
						OLED_P16x8Ch(5,6,31);//四界面显示“间”
					
						OLED_P16x8Ch(40,2,0);//四界面显示“年”
						OLED_P16x8Ch(61,2,29);//四界面显示“：”
						OLED_P16x8Ch(40,3,1);//四界面显示“月”
						OLED_P16x8Ch(61,3,29);//四界面显示“：”
						OLED_P16x8Ch(40,4,2);//四界面显示“日”
						OLED_P16x8Ch(61,4,29);//四界面显示“：”
						OLED_P16x8Ch(40,5,3);//四界面显示“时”
						OLED_P16x8Ch(61,5,29);//四界面显示“：”
						OLED_P16x8Ch(40,6,4);//四界面显示“分”
						OLED_P16x8Ch(61,6,29);//四界面显示“：”
						OLED_P16x8Ch(40,7,5);//四界面显示“秒”
						OLED_P16x8Ch(61,7,29);//四界面显示“：”
					
						OLED_F6x8Str1(96,2,2000+clock_gengxin[0],4);		//shi
						OLED_F6x8Str1(108,3,clock_gengxin[1],2);		//shi
						OLED_F6x8Str1(108,4,clock_gengxin[2],2);		//shi
						OLED_F6x8Str1(108,5,clock_gengxin[3],2);		//shi
						OLED_F6x8Str1(108,6,clock_gengxin[4],2);		//shi
						OLED_F6x8Str2(108,7,clock_gengxin[5],2);		//shi
						if(KEY3 == 1)
						{
								delay_ms(100);
								if(KEY3 == 1)
								{
									clock_gengxin[5]++;
									if(clock_gengxin[5]>59)
									{
											clock_gengxin[5] = 0;
									}
								}
						}
						else if(KEY4 == 1)
						{
								delay_ms(100);
								if(KEY4 == 1)
								{
									clock_gengxin[5]--;
									if(clock_gengxin[5]<0)
									{
											clock_gengxin[5] = 59;
									}
								}
						}
				}
				else
				{
						
						OLED_P16x8Ch(5,3,30);//四界面显示“时”
						OLED_P16x8Ch(5,6,31);//四界面显示“间”					
						OLED_P16x8Ch(40,2,0);//四界面显示“年”
						OLED_P16x8Ch(61,2,29);//四界面显示“：”
						OLED_P16x8Ch(40,3,1);//四界面显示“月”
						OLED_P16x8Ch(61,3,29);//四界面显示“：”
						OLED_P16x8Ch(40,4,2);//四界面显示“日”
						OLED_P16x8Ch(61,4,29);//四界面显示“：”
						OLED_P16x8Ch(40,5,3);//四界面显示“时”
						OLED_P16x8Ch(61,5,29);//四界面显示“：”
						OLED_P16x8Ch(40,6,4);//四界面显示“分”
						OLED_P16x8Ch(61,6,29);//四界面显示“：”
						OLED_P16x8Ch(40,7,5);//四界面显示“秒”
						OLED_P16x8Ch(61,7,29);//四界面显示“：”
						OLED_F6x8Str1(96,2,2000+clock_gengxin[0],4);		//shi
						OLED_F6x8Str1(108,3,clock_gengxin[1],2);		//shi
						OLED_F6x8Str1(108,4,clock_gengxin[2],2);		//shi
						OLED_F6x8Str1(108,5,clock_gengxin[3],2);		//shi
						OLED_F6x8Str1(108,6,clock_gengxin[4],2);		//shi
						OLED_F6x8Str1(108,7,clock_gengxin[5],2);		//shi
				}
				if(WK_UP == 1)
				{
						delay_ms(100);
						if(WK_UP == 1)
						{
								OLED_P16x8Ch(5,3,30);//四界面显示“时”
								OLED_P16x8Ch(5,6,31);//四界面显示“间”
							
								OLED_P16x8Ch(40,2,0);//四界面显示“年”
								OLED_P16x8Ch(61,2,29);//四界面显示“：”
								OLED_P16x8Ch(40,3,1);//四界面显示“月”
								OLED_P16x8Ch(61,3,29);//四界面显示“：”
								OLED_P16x8Ch(40,4,2);//四界面显示“日”
								OLED_P16x8Ch(61,4,29);//四界面显示“：”
								OLED_P16x8Ch(40,5,3);//四界面显示“时”
								OLED_P16x8Ch(61,5,29);//四界面显示“：”
								OLED_P16x8Ch(40,6,4);//四界面显示“分”
								OLED_P16x8Ch(61,6,29);//四界面显示“：”
								OLED_P16x8Ch(40,7,5);//四界面显示“秒”
								OLED_P16x8Ch(61,7,29);//四界面显示“：”
								OLED_F6x8Str1(96,2,2000+clock_gengxin[0],4);		//shi
								OLED_F6x8Str1(108,3,clock_gengxin[1],2);		//shi
								OLED_F6x8Str1(108,4,clock_gengxin[2],2);		//shi
								OLED_F6x8Str1(108,5,clock_gengxin[3],2);		//shi
								OLED_F6x8Str1(108,6,clock_gengxin[4],2);		//shi
								OLED_F6x8Str1(108,7,clock_gengxin[5],2);		//shi
								coun2 = 0;
								Ds1302_Write_Time_All1();
						}
				}
				
		}
		/*****************界面四：闹钟*********************/
		else if(key == 4)
		{
			BEEP_ON;
				if(!Add5)
				{
						OLED_Clear();		//先清屏
					  Add5 = 1;
				}
				Add1 = 0;
				Add2 = 0;
				Add3 = 0;
				Add4 = 0;
				Add6 = 0;
				
				for(i=35;i<39;i++)
				{
					OLED_P16x16Ch(30+(i-35)*16,1,i);//主界面显示“闹钟响了”
				}
				for(i=39;i<44;i++)
				{
					OLED_P16x16Ch(28+(i-39)*16,4,i);//主界面显示“风铃动动！”
				}
				 
			
			bellring();
			printf("ringing\r\n");
		}
		else															//主界面---------------时间、天气、气温
		{
				if(!Add6)
				{
						OLED_Clear();		//先清屏
					  Add6 = 1;
				}
				Add1 = 0;
				Add2 = 0;
				Add3 = 0;
				Add4 = 0;
				Add5 = 0;
				BEEP_OFF;
				for(i=0;i<7;i++)
				{
					OLED_P16x16Ch(8+i*16,0,i);//主界面显示“多功能电子时钟”
				}
				
				for(i=0;i<3;i++)
				{
					OLED_P16x8Ch(1+i*16,3,i);//主界面显示“年月日”
				}
				for(i=3;i<6;i++)
				{
					OLED_P16x8Ch(1+(i-3)*16,4,i);//主界面显示“时分秒”
				}
				
       
				
				
				clock_NYR();
				clock_SFM();
				if(clock_en){
					OLED_P6x8Str(100,6,"O");
					OLED_P6x8Str(106,6,"N");
				}else{
					OLED_P6x8Str(100,6,"O");
					OLED_P6x8Str(106,6,"F");
					OLED_P6x8Str(112,6,"F");
				}
				
			OLED_F6x8Str1(45,6,temperature,2);//更新温度显示
			OLED_P6x8Str(62,6,"C");
			
			switch(weather_code){			//更新天气显示
				case 0:
				case 1:
				case 2:
				case 3:
						OLED_P16x16Ch(5,6,12);
						break;
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
						OLED_P16x16Ch(5,5,13);
						OLED_P16x16Ch(21,5,14);
						break;
				case 9:
						OLED_P16x16Ch(5,6,15);
						break;
				case 10:
						OLED_P16x16Ch(5,5,17);
						OLED_P16x16Ch(21,5,18);
						break;
				case 11:
				case 12:
						OLED_P16x16Ch(5,5,16);
						OLED_P16x16Ch(21,5,17);
						OLED_P16x16Ch(37,5,18);
						break;
				case 13:
						OLED_P16x16Ch(5,5,19);
						OLED_P16x16Ch(21,5,18);
						break;
				case 14:
						OLED_P16x16Ch(5,5,20);
						OLED_P16x16Ch(21,5,18);
						break;
			case 15:
						OLED_P16x16Ch(5,5,21);
						OLED_P16x16Ch(21,5,18);
						break;
			case 16:
			case 17:
			case 18:
						OLED_P16x16Ch(5,5,22);
						OLED_P16x16Ch(21,5,18);
						break;
			case 19:
						OLED_P16x16Ch(5,5,23);
						OLED_P16x16Ch(21,5,18);
						break;
			case 30:
						OLED_P16x16Ch(5,5,24);
						break;
			default:
						OLED_F6x8Str1(5,5,'N',1);	
		}
	
		}
		
		if(ascii_time[2] == Alarm_clock_S1 && ascii_time[1] == Alarm_clock_F1&&clock_en)	//闹钟
		{
			if(!clockctrl){
				clockctrl=1;
				key=4;
			}
			
		}else 
		{
			clockctrl=0;
		}
		if(weatherctrl==2){
			printf("%s\r\n",Get_Current_Weather());
			weatherctrl=0;
		}
		else if(ascii_time[1] == 0 && ascii_time[1] == 0){
			if(!weatherctrl){
				printf("%s\r\n",Get_Current_Weather());
				weatherctrl=1;
			}
		}else{
			weatherctrl=0;
		}
		

/***********************************程序结束**************************************/		
	}
	
 }
