#include "bellring.h"
#include "delay.h"
#include "stm32f10x.h"



void bellring()
{
	
	 //TIM3_PWM_Init(199,7199);
	
	 TIM_SetCompare2(TIM3,195);
	 delay_ms(520);
	 TIM_SetCompare2(TIM3,190);
	 delay_ms(520);
	 TIM_SetCompare2(TIM3,185);
	 delay_ms(520);
	 TIM_SetCompare2(TIM3,180);
	 delay_ms(520);
	 TIM_SetCompare2(TIM3,175);
	 delay_ms(1000);
	 TIM_SetCompare2(TIM3,185);
	 delay_ms(520);
	 TIM_SetCompare2(TIM3,180);
	 delay_ms(520);
	 TIM_SetCompare2(TIM3,175);
	
	
	
}
