#include "Systerm.h"
#include "timer.h"
#include "bellring.h"
#include "esp8266.h"
#include "usart.h"
#include "weather.h"
void Systerm_Init(void)
{
	/**********��ʱ��ʼ��*********/
	delay_init();
	/***********usart��ʼ��************/
	uart_init(115200);
	Usart2_Init(115200);
	/***********esp8266��ʼ��************/
	ESP8266_Init();
	/**********OLED��ʼ��***************/
	OLED_Init();			//��ʼ��OLED  
	OLED_Clear();
	/***********DS1302��ʼ��***************/
	Ds1302_Gpio_Init();
	/***********������ʼ��***************/
	KEY_Init();
	/***********��������ʼ��***************/
	BEEP_Init();
	/***********ͬ��ʱ��************/
	Update_Time();
	/***********pwm��ʼ��************/
	TIM3_PWM_Init(199,7199);
	/***********��ʱ���жϳ�ʼ��**********/
	TIM2_Config(1999,3599);//Tim2���ڸ���ʱ��
	NVIC_Config();
}
/**************************************ʱ�Ӽ�����ʪ��ģ��Ķ�ȡ********************************************/
void clock_NYR(void)
{
		/***********************************������ʾ**************************************/		
		OLED_F6x8Str1(1+55,3,ascii_time[6]+2000,4); //nian
		OLED_F6x8Str1(31+55,3,-ascii_time[4],2); 	//yue
		OLED_F6x8Str1(49+55,3,-ascii_time[3],2);		//ri
}

void clock_SFM(void)
{
		/***********************************������ʾ**************************************/		
		
		OLED_F6x8Str1(67-66+55,4,ascii_time[2],2);		//shi
		
		OLED_F6x8Str1(85-66+55,4,-ascii_time[1],2);		//fen
		
		OLED_F6x8Str1(103-66+55,4,-ascii_time[0],2);		//miao
}

/**************************************��ʱ���жϵ�����********************************************/
//��ʼ��TIM2��ʱ�����ж�
//ÿ�ν����жϷ��������ʱ��Ϊ
//((1+arr )/72M)*(1+psc )=((1+1999)/72M)*(1+35999)=1��
void TIM2_Config(u16 arr,u16 psc)
{
    //��ʱ��ʱ����1s

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    //����жϱ�־λ
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//TIM_ClearFlag(TIM2,TIM_FLAG_Update);//����������ͬ

    //�Զ�װ�ص�����ֵ0-0xffff��72M/36000=2000
    TIM_TimeBaseStructure.TIM_Period = arr;
    //ʱ�ӳ���Ԥ��Ƶֵ0-oxffff��������36000��Ƶ
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    ////��ͨ�͸߼�������������
    //ʱ�ӷָ��ʱΪ0���߼�Ӧ�ò���
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //����ģʽ�������£��������123
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

    //�����ж�
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    //��������
    TIM_Cmd(TIM2,ENABLE);
}

//��ʼ���ж�����������NVIC

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
//�жϺ������Լ���д
void TIM2_IRQHandler(void)
{
    //�ж�TIM3�����ж��Ƿ���
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
    {
        //���������־λ
			TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
				//״̬ȡ��
			if(key!=4)Ds1302_Readtime();
			//if(key==0)clock_SFM();		
			if(Stopwatch_Start)
			{
				Stopwatch_counting++;
			}
			
    }
}


/**************************************��ʱ���жϵ�����********************************************/





//��ʼ��TIM2��ʱ�����ж�
//ÿ�ν����жϷ��������ʱ��Ϊ
//((1+arr )/72M)*(1+psc )=((1+999)/72M)*(1+35999)=0.5��
void TIM4_Config(u16 arr,u16 psc)
{	
    //��ʱ��ʱ����1s
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    //����жϱ�־λ
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//TIM_ClearFlag(TIM2,TIM_FLAG_Update);//����������ͬ

    //�Զ�װ�ص�����ֵ0-0xffff��72M/36000=2000
    TIM_TimeBaseStructure.TIM_Period = arr;
    //ʱ�ӳ���Ԥ��Ƶֵ0-oxffff��������36000��Ƶ
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    ////��ͨ�͸߼�������������
    //ʱ�ӷָ��ʱΪ0���߼�Ӧ�ò���
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //����ģʽ�������£��������123
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

    //�����ж�
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    //��������
    TIM_Cmd(TIM4,ENABLE);
}


