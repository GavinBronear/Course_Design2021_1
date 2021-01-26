#include "oled.h"
#include "codetab.h"
/* ----------------------------------------------------------------
              GND   电源地
              VCC   接5V或3.3v电源
              SCK   接PA5
              SDA   接PA7            
---------------------------------------------------*/

void OLED_Init(void)
{ 		 
 	GPIO_InitTypeDef  GPIO_InitStructure; 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能A端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_10;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOD3,6
 	GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_7);	

	delay_ms(400);
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}


//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{
	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
OLED_SCLK_Set() ;
//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
}

void IIC_Wait_Ack()
{
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}

/**********************************************
// IIC Write byte
**********************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
		m=da;
		//	OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
		da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
		}
}

/**********************************************
// IIC Write Command
**********************************************/
void OLED_WrCmd(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
	IIC_Stop();
}

/**********************************************
// IIC Write Data
**********************************************/
void OLED_WrDat(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
	IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
   		OLED_WrDat(dat);
	}
	else 
	{
   		OLED_WrCmd(dat);	
	}
}



/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}

//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   
	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}	
	   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}

void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //更新显示
}


/****************************显示数字变量**********************************/
void OLED_F6x8Str1(uint8 x,uint8 y,int d,uint8 t)
{
        unsigned char c=0,c1=0,c2=0,c3=0,c4=0,i=0,j=0;
       
        if(d < 0)
        {
          t+=10;
          d= -d;
        }
        switch(t)
        {
                
            case 1:
                while(1)
                {       
//                    OLED_Set_Pos(x-6,y);
//                    for(i=0;i<6;i++)
//                    OLED_WrDat(F6x81[11][i]);
                    
                    c=d;
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c][i]);
                    x+=6;
                    j++;
                    break;
                }break;
            case 2:
                while(1)
                {
//                    OLED_Set_Pos(x-6,y);
//                    for(i=0;i<6;i++)
//                    OLED_WrDat(F6x81[11][i]);
                    
                    c=d /10;
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c][i]);
                    c1=d%10;
                    OLED_Set_Pos(x+6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c1][i]);
                    x+=6;
                    j++;
                    break;
                }break;
             case 3:
                while(1)
                {
                    OLED_Set_Pos(x-6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[11][i]);
                    
                    c=d /100;
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c][i]);
                    c1=d/10%10;
                    OLED_Set_Pos(x+6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c1][i]);
                    c2=d%10;
                    OLED_Set_Pos(x+12,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c2][i]);
                    x+=6;
                    j++;
                    break;
                }break; 
              case 4:
                while(1)
                {
                    OLED_Set_Pos(x-6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[11][i]);
                    
                    c=d /1000;
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c][i]);
                    c1=d%1000/100;
                    OLED_Set_Pos(x+6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c1][i]);
                    c2=d%1000%100/10;
                    OLED_Set_Pos(x+12,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c2][i]);
                    c3=d%1000%100%10;
                    OLED_Set_Pos(x+18,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c3][i]);
                    x+=6;
                    j++;
                    break;
                }break;  
              case 5:
                while(1)
                {
                    OLED_Set_Pos(x-6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[11][i]);
                    
                    c=d /10000;
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c][i]);
                    c1=d%10000/1000;
                    OLED_Set_Pos(x+6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c1][i]);
                    c2=d%10000%1000/100;
                    OLED_Set_Pos(x+12,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c2][i]);
                    c3=d%10000%1000%100/10;
                    OLED_Set_Pos(x+18,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c3][i]);
                    c4=d%10000%1000%100%10;
                    OLED_Set_Pos(x+24,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c4][i]);
                    x+=6;
                    j++;
                    break;
                }break;
                
                case 11:
                while(1)
                {
                    
                    if(x>126){x=0;y++;}
                    
                    OLED_Set_Pos(x-6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[10][i]);
                    
                    c=d;
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c][i]);
                    x+=6;
                    j++;
                    break;
                }break;
            case 12:
                while(1)
                {
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x-6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[10][i]);
                  
                    c=d /10;
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c][i]);
                    c1=d%10;
                    OLED_Set_Pos(x+6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c1][i]);
                    x+=6;
                    j++;
                    break;
                }break;
             case 13:
                while(1)
                {
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x-6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[10][i]);
                  
                    c=d /100;
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c][i]);
                    c1=d/10%10;
                    OLED_Set_Pos(x+6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c1][i]);
                    c2=d%10;
                    OLED_Set_Pos(x+12,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c2][i]);
                    x+=6;
                    j++;
                    break;
                }break; 
              case 14:
                while(1)
                {
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x-6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[10][i]);
                    
                    c=d /1000;
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c][i]);
                    c1=d%1000/100;
                    OLED_Set_Pos(x+6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c1][i]);
                    c2=d%1000%100/10;
                    OLED_Set_Pos(x+12,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c2][i]);
                    c3=d%1000%100%10;
                    OLED_Set_Pos(x+18,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c3][i]);
                    x+=6;
                    j++;
                    break;
                }break;  
              case 15:
                while(1)
                {
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x-6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[10][i]);
                    
                    c=d /10000;
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c][i]);
                    c1=d%10000/1000;
                    OLED_Set_Pos(x+6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c1][i]);
                    c2=d%10000%1000/100;
                    OLED_Set_Pos(x+12,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c2][i]);
                    c3=d%10000%1000%100/10;
                    OLED_Set_Pos(x+18,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c3][i]);
                    c4=d%10000%1000%100%10;
                    OLED_Set_Pos(x+24,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[c4][i]);
                    x+=6;
                    j++;
                    break;
                }break;
                
          
        }
          
    
}
/**********************反向显示************************/
void OLED_F6x8Str2(uint8 x,uint8 y,int d,uint8 t)
{
        unsigned char c=0,c1=0,c2=0,c3=0,i=0,j=0;
        
        if(d < 0)
        {
          t+=10;
          d= -d;
        }
        switch(t)
        {
            case 1:
                while(1)
                {
                    c=d;
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c][i]);
                    x+=6;
                    j++;
                    break;
                }break;
            case 2:
                while(1)
                {
                    c=d /10;
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c][i]);
                    c1=d%10;
                    OLED_Set_Pos(x+6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c1][i]);
                    x+=6;
                    j++;
                    break;
                }break;
             case 3:
                while(1)
                {
                    c=d /100;
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c][i]);
                    c1=d/10%10;
                    OLED_Set_Pos(x+6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c1][i]);
                    c2=d%10;
                    OLED_Set_Pos(x+12,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c2][i]);
                    x+=6;
                    j++;
                    break;
                }break; 
                
								case 4:
                while(1)
                {

                    
                    c=d /1000;
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c][i]);
                    c1=d%1000/100;
                    OLED_Set_Pos(x+6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c1][i]);
                    c2=d%1000%100/10;
                    OLED_Set_Pos(x+12,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c2][i]);
                    c3=d%1000%100%10;
                    OLED_Set_Pos(x+18,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c3][i]);
                    x+=6;
                    j++;
                    break;
                }break;  
                
                
                
             case 11:
                while(1)
                {
                    OLED_Set_Pos(x-6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[10][i]);
                    
                    c=d;
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c][i]);
                    x+=6;
                    j++;
                    break;
                }break;
            case 12:
                while(1)
                {
                    OLED_Set_Pos(x-6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[10][i]);
                    
                    c=d /10;
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c][i]);
                    c1=d%10;
                    OLED_Set_Pos(x+6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c1][i]);
                    x+=6;
                    j++;
                    break;
                }break;
             case 13:
                while(1)
                {
                    OLED_Set_Pos(x-6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(F6x81[10][i]);
                    
                    c=d /100;
                    if(x>126){x=0;y++;}
                    OLED_Set_Pos(x,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c][i]);
                    c1=d/10%10;
                    OLED_Set_Pos(x+6,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c1][i]);
                    c2=d%10;
                    OLED_Set_Pos(x+12,y);
                    for(i=0;i<6;i++)
                    OLED_WrDat(~F6x81[c2][i]);
                    x+=6;
                    j++;
                    break;
                }break;
        }
          
    
}
/*******************************************************************************
功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7
*******************************************************************************/
void OLED_P6x8Str(uint8 x,uint8 y,uint8 ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WrDat(F6x8[c][i]);
		x+=6;
		j++;
	}
}
void OLED_P6x8Str_select(uint8 x,uint8 y,uint8 ch[])
{
  	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WrDat(~F6x8[c][i]);
		x+=6;
		j++;
	}
}
//反向显示
void OLED_P6x8Str_r(uint8 x,uint8 y,uint8 ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WrDat(~F6x8[c][i]);
		x+=6;
		j++;
	}
}

/*******************************************************************************
功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7
*******************************************************************************/
void OLED_P8x16Str(uint8 x,uint8 y,uint8 ch[])
{
	uint8 c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}
//反向显示
void OLED_P8x16Str_r(uint8 x,uint8 y,uint8 ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
		OLED_WrDat(~F8X16[c*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WrDat(~F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}


/******************************************************************************
功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7
*******************************************************************************/
void OLED_P16x16Ch(uint8 x,uint8 y,uint8 N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
	OLED_Set_Pos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	} 	  	
}

/******************************************************************************
功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7
*******************************************************************************/
void OLED_P16x8Ch(uint8 x,uint8 y,uint8 N)
{
	unsigned char wm=0;
	unsigned int adder=16*N;
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x8[adder]);
		adder += 1;
	}  	
}
/******************************************************************************
功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7						反向
*******************************************************************************/
void OLED_P16x8Ch1(uint8 x,uint8 y,uint8 N)
{
	unsigned char wm=0;
	unsigned int adder=16*N;
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(~F16x8[adder]);
		adder += 1;
	}  	
}
/*******************************************************************************
//函数名： void Float_to_Str ( float temp, char str[], uint8 size, uint8 pot)
//功能描述：将浮点数转换为字符串
//参数：待转换数字，存放字符串，字符串长度(不包括结束标识),小数点后精度
*******************************************************************************/

void Float_to_Str ( float f_temp, uint8 str[], uint8 size, uint8 pot )
{

	uint8 i=0;
        int32 temp;
    for( i= 0; i<size; i++)
            str[i]=' ';
    if( f_temp >= 0 )
    {
        
        for(i=0;i<pot;i++)
            f_temp = 10*f_temp;
        temp=(int32)f_temp;
        i=0;
        do
        {
            str[size-1]=temp%10+'0';
            temp/=10;
            size--;
            if( size == 0 )
                return;
            i++;
            if( i == pot )
            {
                str[size-1] = '.';
                size--;
                if( size == 0 )
                    return;
            }            
        }while(temp);
        if( i < pot )
        {
            while( i < pot )
            {
                str[size-1] = '0';
                    size--;
                if( size == 0 )
                    return;
                i++;
            }
            str[size-1] = '.';
            size--;
            if( size == 0 )
                return;
            str[size-1] = '0';
            size--;
            if( size == 0 )
                return;            
        }
        else if( i == pot )
        {
            str[size-1] = '0';
            size--;
            if( size == 0 )
                return;
        }
    }
    else
    {
        f_temp = -f_temp;
        
        for(i=0;i<pot;i++)
            f_temp = 10*f_temp;
        temp=(int32)f_temp;
        i=0;
        do
        {
            str[size-1]=temp%10+'0';
            temp/=10;
            size--;
            if( size == 0 )
                return;
            i++;
            if( i == pot )
            {
                str[size-1] = '.';
                size--;
                if( size == 0 )
                    return;
            }            
        }while(temp);
        if( i < pot )
        {
            while( i < pot )
            {
                str[size-1] = '0';
                    size--;
                if( size == 0 )
                    return;
                i++;
            }
            str[size-1] = '.';
            size--;
            if( size == 0 )
                return;
            str[size-1] = '0';
            size--;
            if( size == 0 )
                return;            
        }
        else if( i == pot )
        {
            str[size-1] = '0';
            size--;
            if( size == 0 )
                return;
        }
        str[size-1] = '-';
    }
}

/*******************************************************************************
//函数名： void Int_to_Str (int32 temp ,char str[] ,uint8 size)
//功能描述：将整数数转换为字符串
//参数：待转换数字，存放字符串，字符串长度(不包括结束标识)
//返回：无
*******************************************************************************/

void Int_to_Str(int32 temp ,uint8 str[] ,uint8 size)        //将数据转换成字符串
{
		uint8 i;
    for( i= 0; i<size; i++)
        str[i]=' ';

    if( temp >= 0 )
    {
        do
        {
            str[size-1]=temp%10+'0';
            temp/=10;
            size--;
            if( size == 0 )
                return;
        }while(temp);
    }
    else
    {
        temp = -temp;
        do
        {
            str[size-1]=temp%10+'0';
            temp/=10;
            size--;
            if( size == 0 )
                return;
        }while(temp);
        str[size-1] = '-';
    }
}






/*******************************************************************************
打印整型
******************************************************************************/
void PrintNum(uint8 x,uint8 y,uint16 num)
{
       uint8 c=0,i=0,flag=0;
       uint16 num_r=num;
 
      if(num==0)
         flag=1;
       else
       {
         while(num_r%10==0)
         {
            flag++;//标记0的个数
            num_r/=10;
         }
       }
       num_r=0;
       while(num!=0)
       {
         num_r*=10;
          num_r+=num%10;
          num/=10;
       }

 	while(num_r!=0)
	{
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
                c=num_r%10;
                num_r/=10;
		for(i=0;i<6;i++)
		OLED_WrDat(F6x8[c+16][i]);
		x+=6;
	}
       while(flag--)
        {
          if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WrDat(F6x8[16][i]);
		x+=6;
        }
}


