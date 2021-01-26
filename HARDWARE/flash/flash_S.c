//================================================================================
//STM32F103         多功能通讯控制演示模块
//================================================================================
//FLASH_S.C/H       片内FLASH读写函数                          BY SAGAHDS 20190306
//================================================================================
#include "stm32f10x_flash.h"
#include "flash_S.h"
//#include "usart.h"
//================================================================================
//BY SAGAHDS 20190306
unsigned int FLASH_RDBUFF[256];     	//FLASH读缓存
unsigned int FLASH_WRBUFF[256];     	//FLASH写缓存
unsigned char FLASH_FLAG;     				//FLASH标志位
      #define FLASH_FLAG_ENABLE 0x01 	//置1安装了FLASH
//================================================================================
//BY SAGAHDS 20190306
void FLASH_Configuration(unsigned char Enable)	//初始化
{
	if(!Enable)
  {
		FLASH_FLAG = 0xFF - FLASH_FLAG_ENABLE;
   // USART_Put_String("FLASH No Installation.\r\n");
    return;
	}

	FLASH_FLAG = FLASH_FLAG_ENABLE;
	
//  USART_Put_String("FLASH Installation.\r\n");	
}

//BY SAGAHDS 20190306
unsigned int FLASH_ReadINT(unsigned long address)	//在指定地址读出int
{
	return *(__IO unsigned int*)address;
}

//BY SAGAHDS 20190306	//在指定地址读出数据到指定位置	//读出到int数组	//这个函数没用
void FLASH_ReadMoreINT(unsigned long startAddress, unsigned int *readData, unsigned int countToRead)	
{
	unsigned int dataIndex;
	for(dataIndex=0;dataIndex<countToRead;countToRead++)
		readData[dataIndex]=FLASH_ReadINT(startAddress+dataIndex*2);
}

//BY SAGAHDS 20190306	//在指定地址读出数据到FLASH_RDBUFF	//读出到char数组	//这里的数据长度是指的int的长度
//读取指定页面0x0800F800的数据
void FLASH_ReadMoreCHAR(unsigned int startAddress, unsigned int *readData, unsigned int countToRead)	
{
	unsigned int i;
	unsigned int data;
	unsigned long FLASH_Address;
	
	if(!(FLASH_FLAG & FLASH_FLAG_ENABLE))return;

	FLASH_Address = 0x0800F800 + startAddress;
	
	for(i=0; i<countToRead; i++)
	{
		data = FLASH_ReadINT(FLASH_Address + i*2);
		readData[i*2 + 0] = data % 256;
		readData[i*2 + 1] = data / 256;
	}
}

//BY SAGAHDS 20190306	//在指定地址写入数据从指定位置FLASH_RDBUFF	//读出到char数组	//这里的数据长度是指的int的长度
//写入到指定页面0x0800F800的数据
void FLASH_WriteMoreCHAR(unsigned int startAddress, unsigned int *writeData, unsigned int countToWrite)	
{
	unsigned int i;
	unsigned long data;
	unsigned long FLASH_Address;
	
	if(!(FLASH_FLAG & FLASH_FLAG_ENABLE))return;

	FLASH_Address = 0x0800F800 + startAddress;
	
	FLASH_Unlock();								//片内FLASH解锁
	FLASH_ErasePage(0x0800F800);	//片内FLASH擦除扇区
	
	for(i=0; i<countToWrite; i++)	//片内FLASH写入
	{
		data = writeData[i*2 + 0] + (writeData[i*2 + 1] * 256);
		FLASH_ProgramHalfWord(FLASH_Address + i*2, data);
	}
	
	FLASH_Lock();									//片内FLASH上锁
}

//BY SAGAHDS 20190306	//在指定地址写入数据从指定位置FLASH_RDBUFF	//读出到char数组	//这里的数据长度是指的int的长度
//写入到指定页面0x0800F800的数据
void FLASH_WriteMoreCHAR1(unsigned int startAddress, unsigned int writeData)	
{
	unsigned int i;
	unsigned long data;
	unsigned long FLASH_Address;
	
	if(!(FLASH_FLAG & FLASH_FLAG_ENABLE))return;

	FLASH_Address = 0x0800F800 + startAddress;
	
	FLASH_Unlock();								//片内FLASH解锁
	FLASH_ErasePage(0x0800F800);	//片内FLASH擦除扇区
	

		FLASH_ProgramHalfWord(FLASH_Address + i*2, data);

	
	FLASH_Lock();									//片内FLASH上锁
}

//BY SAGAHDS 20190306	//在指定地址读出数据到FLASH_RDBUFF	//读出到char数组	//这里的数据长度是指的int的长度
//读取指定页面0x0800F800的数据
void FLASH_ReadMoreCHAR1(unsigned int startAddress, unsigned int readData)	
{
	unsigned int i;
	unsigned int data;
	unsigned long FLASH_Address;
	
	if(!(FLASH_FLAG & FLASH_FLAG_ENABLE))return;

	FLASH_Address = 0x0800F800 + startAddress;
	

		Luyin_counting = FLASH_ReadINT(FLASH_Address + i*2);

}

//void FLASH_Unlock(void)																										//解锁函数
//FLASH_Status FLASH_ErasePage(uint32_t Page_Address)												//删除扇区
//FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data) 					//写四个字节
//FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data) 			//写两个字节
//FLASH_Status FLASH_ProgramOptionByteData(uint32_t Address, uint8_t Data)  //写一个字节
//void FLASH_Lock(void)																											//上锁函数
