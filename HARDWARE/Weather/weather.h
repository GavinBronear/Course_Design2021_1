#ifndef _WEATHER_H
#define _WEATHER_H
#include "sys.h"

extern int clock_gengxin[7];
extern u8 weather_code;
extern u8 temperature;
char* Get_Current_Weather(void);
void Update_Time(void);
#endif


