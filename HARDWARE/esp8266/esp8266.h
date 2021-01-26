#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "sys.h"



#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志


void ESP8266_Init(void);
void Usart2_Init(unsigned int baud);
void ESP8266_Clear(void);
unsigned char* ESP8266_Get(unsigned short timeout);
u8 ESP8266_WaitRecive(void);
u8 ESP8266_SendCmd(char *cmd, char *res, u16 time);

void ESP8266_SendData(unsigned char *data, unsigned short len,u16 timeout);

unsigned char *ESP8266_GetIPD(unsigned short timeOut);
void Usart2_SendString(unsigned char *str, unsigned int len);
u8 ESP8266_SEND(char * Messege,u8 LinkID,u16 timeout);
unsigned char *ESP8266_GetDate(unsigned short timeOut);

#endif
