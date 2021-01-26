#include "Systerm.h"
#include "timer.h"
#include "bellring.h"
#include "esp8266.h"
#include "usart.h"
#include "weather.h"
void Systerm_Init(void)
{
	/**********延时初始化*********/
	delay_init();
	/***********usart初始化************/
	uart_init(115200);
	Usart2_Init(115200);
	/***********esp8266初始化************/
	ESP8266_Init();
	/**********OLED初始化***************/
	OLED_Init();			//初始化OLED  
	OLED_Clear();
	/***********DS1302初始化***************/
	Ds1302_Gpio_Init();
	/***********按键初始化***************/
	KEY_Init();
	/***********蜂鸣器初始化***************/
	BEEP_Init();
	/***********同步时间************/
	Update_Time();
	/***********pwm初始化************/
	TIM3_PWM_Init(199,7199);
	/***********定时器中断初始化**********/
	TIM2_Config(1999,3599);//Tim2用于更新时间
	NVIC_Config();
}
/**************************************时钟及其温湿度模块的读取********************************************/
void clock_NYR(void)
{
		/***********************************日期显示**************************************/		
		OLED_F6x8Str1(1+55,3,ascii_time[6]+2000,4); //nian
		OLED_F6x8Str1(31+55,3,-ascii_time[4],2); 	//yue
		OLED_F6x8Str1(49+55,3,-ascii_time[3],2);		//ri
}

void clock_SFM(void)
{
		/***********************************日期显示**************************************/		
		
		OLED_F6x8Str1(67-66+55,4,ascii_time[2],2);		//shi
		
		OLED_F6x8Str1(85-66+55,4,-ascii_time[1],2);		//fen
		
		OLED_F6x8Str1(103-66+55,4,-ascii_time[0],2);		//miao
}

/**************************************定时器中断的配置********************************************/
//初始化TIM2定时器及中断
//每次进入中断服务程序间隔时间为
//((1+arr )/72M)*(1+psc )=((1+1999)/72M)*(1+35999)=1秒
void TIM2_Config(u16 arr,u16 psc)
{
    //定时器时间是1s

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    //清除中断标志位
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//TIM_ClearFlag(TIM2,TIM_FLAG_Update);//两者作用相同

    //自动装载的周期值0-0xffff，72M/36000=2000
    TIM_TimeBaseStructure.TIM_Period = arr;
    //时钟除数预分频值0-oxffff，下面是36000分频
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    ////普通和高级才有下面两行
    //时钟分割，暂时为0，高级应用才用
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //计数模式，向上下，中央对齐123
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

    //开启中断
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    //开启外设
    TIM_Cmd(TIM2,ENABLE);
}

//初始化中断向量控制器NVIC

void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


//--------------------stm32f10x_it.c------------------------
//中断函数中自己编写
void TIM2_IRQHandler(void)
{
    //判断TIM3更新中断是否发生
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
    {
        //必须清楚标志位
			TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
				//状态取反
			if(key!=4)Ds1302_Readtime();
			//if(key==0)clock_SFM();		
			if(Stopwatch_Start)
			{
				Stopwatch_counting++;
			}
			
    }
}


/**************************************定时器中断的配置********************************************/





//初始化TIM2定时器及中断
//每次进入中断服务程序间隔时间为
//((1+arr )/72M)*(1+psc )=((1+999)/72M)*(1+35999)=0.5秒
void TIM4_Config(u16 arr,u16 psc)
{	
    //定时器时间是1s
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    //清除中断标志位
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//TIM_ClearFlag(TIM2,TIM_FLAG_Update);//两者作用相同

    //自动装载的周期值0-0xffff，72M/36000=2000
    TIM_TimeBaseStructure.TIM_Period = arr;
    //时钟除数预分频值0-oxffff，下面是36000分频
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    ////普通和高级才有下面两行
    //时钟分割，暂时为0，高级应用才用
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //计数模式，向上下，中央对齐123
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

    //开启中断
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    //开启外设
    TIM_Cmd(TIM4,ENABLE);
}


