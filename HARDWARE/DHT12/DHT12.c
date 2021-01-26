#include"DHT12.h"



float Temprature,Humi;//定义温湿度变量 ，此变量为全局变量
u8 Sensor_AnswerFlag=0;//定义传感器响应标志
u8 Sensor_ErrorFlag;  //定义读取传感器错误标志
 
 
/********************************************\
|* 功能： 初始化SDA(PB14),CLK(PB13)	        *|
\********************************************/
void DHT12_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//使能GPIOB	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;//PB13,PB14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14); 	//PB13,PB14 输出低(SCL上电立即拉低)
}
 
 
/********************************************\
|* 功能： 读传感器发送的单个字节	        *|
\********************************************/
u8 DHT12_Rdata(void)
{
	u8 i;
	u16 j;
	u8 data=0,bit=0;
	
	for(i=0;i<8;i++)
	{
		while(!READ_SDA)//检测上次低电平是否结束
		{
			if(++j>=50000) //防止进入死循环
			{
				break;
			}
		}
		//延时Min=26us Max70us 跳过数据"0" 的高电平		 
		delay_us(30);
 
		//判断传感器发送数据位
		bit=0;
		if(READ_SDA)
		{
			bit=1;
		}
		j=0;
		while(READ_SDA)	//等待高电平结束
		{
			if(++j>=50000) //防止进入死循环
			{
				break;
			}		
		}
		data<<=1;
		data|=bit;
	}
	return data;
}
 
 
/********************************************\
|* 功能：DHT12读取温湿度函数       *|
\********************************************/
//变量：Humi_H：湿度高位；Humi_L：湿度低位；Temp_H：温度高位；Temp_L：温度低位；Temp_CAL：校验位
//数据格式为：湿度高位（湿度整数）+湿度低位（湿度小数）+温度高位（温度整数）+温度低位（温度小数）+ 校验位
//校验：校验位=湿度高位+湿度低位+温度高位+温度低位
u8 DHT12_READ(void)
{
	u32 j;
	u8 Humi_H,Humi_L,Temp_H,Temp_L,Temp_CAL,temp;
 
	//主机发送起始信号
	SDA_OUT() //设为输出模式
	SEND_SDA=0;	//主机把数据总线（SDA）拉低
	delay_ms(20);//拉低一段时间（至少18ms）， 通知传感器准备数据
	SEND_SDA=1;	 //释放总线
	SDA_IN();	//设为输入模式，判断传感器响应信号
	delay_us(30);//延时30us
 
	Sensor_AnswerFlag=0;	//传感器响应标志
	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
	if(READ_SDA==0)
	{
		Sensor_AnswerFlag=1;	//收到起始信号
 
		j=0;
		while((!READ_SDA)) //判断从机发出 80us 的低电平响应信号是否结束	
		{
			if(++j>=500) //防止进入死循环
			{
				Sensor_ErrorFlag=1;
				break;
			}
		}
 
		j=0;
		while(READ_SDA)//判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
		{
			if(++j>=800) //防止进入死循环
			{
				Sensor_ErrorFlag=1;
				break;
			}		
		}
		//接收数据
		Humi_H=DHT12_Rdata();
		Humi_L=DHT12_Rdata();
		Temp_H=DHT12_Rdata();	
		Temp_L=DHT12_Rdata();
		Temp_CAL=DHT12_Rdata();
 
		temp=(u8)(Humi_H+Humi_L+Temp_H+Temp_L);//只取低8位
 
		if(Temp_CAL==temp)//如果校验成功，往下运行
		{
			Humi=Humi_H*10+Humi_L; //湿度
	
			if(Temp_L&0X80)	//为负温度
			{
				Temprature =0-(Temp_H*10+((Temp_L&0x7F)));
			}
			else   //为正温度
			{
				Temprature=Temp_H*10+Temp_L;//为正温度
			}
			//判断数据是否超过量程（温度：-20℃~60℃，湿度20％RH~95％RH）
			if(Humi>950) 
			{
			  Humi=950;
			}
			if(Humi<200)
			{
				Humi =200;
			}
			if(Temprature>600)
			{
			  Temprature=600;
			}
			if(Temprature<-200)
			{
				Temprature = -200;
			}
			Temprature=Temprature/10;//计算为温度值
			Humi=Humi/10; //计算为湿度值
//			printf("\r\n温度为:  %.1f  ℃\r\n",Temprature); //显示温度
//			printf("湿度为:  %.1f  %%RH\r\n",Humi);//显示湿度	
//			OLED_F6x8Str1(94,6,Temp_H,4);
//			OLED_F6x8Str1(94,7,Temp_L,4);
		}
		
//		else
//		{
//		 	printf("CAL Error!!\r\n");
//			printf("%d \r%d \r%d \r%d \r%d \r%d \r\n",Humi_H,Humi_L,Temp_H,Temp_L,Temp_CAL,temp);
//		}
	}
	else
	{
		Sensor_ErrorFlag=0;  //未收到传感器响应
//		printf("Sensor Error!!\r\n");
	}
 
	return 1;
}

