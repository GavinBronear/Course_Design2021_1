#ifndef __LED_H
#define __LED_H	 
#include "Systerm.h"

#define BLED  GPIO_Pin_12
#define BEEP  GPIO_Pin_15

#define BLED_ON  GPIO_ResetBits(GPIOB,BLED);
#define BLED_OFF GPIO_SetBits(GPIOB,BLED); 

#define BEEP_ON  GPIO_SetBits(GPIOB,BEEP);
#define BEEP_OFF  GPIO_ResetBits(GPIOB,BEEP); 

void LED_Init(void);//≥ı ºªØ
void BEEP_Init(void);
 				    
#endif
