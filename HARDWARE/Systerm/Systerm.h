#ifndef _SYSTERM_H_
#define _SYSTERM_H_

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
#include "led.h"
#include "ds1302.h"
#include "stdio.h"
#include "key.h"



extern u32 Stopwatch_counting;
extern u8 Stopwatch_Start;

extern u8 Luyin_start;
extern u16 Luyin_counting;

extern u8 Fangyin_start;
extern u16 Fangyin_counting;

extern u8 key;
extern u8 key2;

void Systerm_Init(void);
void LED_HL(void);
void clock_NYR(void);
void clock_SFM(void);
void Temperature_and_humidity(void);

void TIM2_Config(u16 arr,u16 psc);
void TIM4_Config(u16 arr,u16 psc);
void NVIC_Config(void);
void NVIC1_Config(void);

#endif
