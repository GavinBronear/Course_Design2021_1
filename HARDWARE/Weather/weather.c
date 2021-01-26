#include "weather.h"
#include "esp8266.h"
#include "sys.h"
#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"
#include "delay.h"
#include "stdlib.h"
#include "DS1302.h"

char* Get_Current_Weather(){
	unsigned char* res;
	unsigned char* request;
	unsigned char* weather;
	u8 code;
	res=NULL;
	ESP8266_Clear();
	delay_ms(100);
	printf("a1\r\n");
	while(ESP8266_SendCmd("AT+CIPSTART=0,\"TCP\",\"api.seniverse.com\",80\r\n","CONNECT",200));
	printf("connected\r\n");
	request=(unsigned char*)"GET https://api.seniverse.com/v3/weather/now.json?key=S5sOBFwzppacrQ583&location=fuzhou&language=en&unit=c\r\n";
	while(ESP8266_SEND((char*)request,0,500));
	printf("SEND OK\r\n");
	delay_ms(20);
	ESP8266_Clear();
	do {
		printf("trying\r\n");
		delay_ms(10);
		res=ESP8266_GetIPD(250);
	}while(res==NULL);
	delay_ms(5);
	if(res!=NULL)printf("%s\r\n",res);
	else printf("failed to get \r\n");
	weather=(unsigned char *)strstr((char *)res,"\"code\":");
	weather=(unsigned char *)strchr((char *)weather,':');
	if((weather+2)!=NULL&&(weather+3!=NULL)){
		if(*(weather+3)!='\"'){
			code=(weather[2]-'0')*10+weather[3]-'0';
		}
		else
			code=*(weather+2)-'0';
		weather_code=code;
		printf("code=%d\r\n",weather_code);
	}	
	//get temperature
	weather=(unsigned char *)strstr((char *)res,"\"temperature\":");
	weather=(unsigned char *)strchr((char *)weather,':');
	printf("restemp=%s\r\n",weather);
	if((weather+2)!=NULL&&(weather+3!=NULL)){
		if(*(weather+3)!='\"'){
			code=(weather[2]-'0')*10+weather[3]-'0';
		}
		else{
			code=*(weather+2)-'0';
		}
		temperature=code;
		printf("code=%d\r\n",temperature);
	}	
	return "got";
}

void Update_Time(){
	unsigned char* res;
	char * request;
	char * time;
	printf("in\r\n");
	ESP8266_Clear();
	while(ESP8266_SendCmd("AT+CIPSTART=1,\"TCP\",\"www.beijing-time.org\",80\r\n","CONNECT",200));
	printf("connected\r\n");
	request="Host:www.beijing-time.org\r\n";
	while(ESP8266_SEND(request,1,500));
	printf("SEND OK\r\n");
	ESP8266_Clear();
	delay_ms(20);
	do {
		printf("trying\r\n");
		delay_ms(10);
		res=ESP8266_GetDate(250);
	}while(res==NULL);
	delay_ms(5);
	if(res!=NULL)printf("%s\r\n",res);
	else printf("failed to get\r\n");
	time=strstr((const char*)res,"2021");
	time=strchr((const char*)time,' ');
	clock_gengxin[0]=21;
	clock_gengxin[1]=1;
	clock_gengxin[2]=20;
	clock_gengxin[3]=((time[1]-'0')*10+(time[2]-'0')+8)%24;
	time=strchr((const char*)time,':');
	clock_gengxin[4]=(time[1]-'0')*10+(time[2]-'0');
	time++;
	time=strchr((const char*)time,':');
	clock_gengxin[5]=(time[1]-'0')*10+(time[2]-'0');//+1)%60;
	/*if(clock_gengxin[5]==0){
		clock_gengxin[4]++;
	}
	if(clock_gengxin[4]>59){
		clock_gengxin[4]=0;
		clock_gengxin[5]=(clock_gengxin[5]+1)%24;
	}*/
	Ds1302_Write_Time_All1();
}
