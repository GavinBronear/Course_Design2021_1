//================================================================================
//STM32F103         �๦��ͨѶ������ʾģ��
//================================================================================
//FLASH_S.C/H       Ƭ��FLASH��д����                          BY SAGAHDS 20190306
//================================================================================
#include "stm32f10x_flash.h"
#include "flash_S.h"
//#include "usart.h"
//================================================================================
//BY SAGAHDS 20190306
unsigned int FLASH_RDBUFF[256];     	//FLASH������
unsigned int FLASH_WRBUFF[256];     	//FLASHд����
unsigned char FLASH_FLAG;     				//FLASH��־λ
      #define FLASH_FLAG_ENABLE 0x01 	//��1��װ��FLASH
//================================================================================
//BY SAGAHDS 20190306
void FLASH_Configuration(unsigned char Enable)	//��ʼ��
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
unsigned int FLASH_ReadINT(unsigned long address)	//��ָ����ַ����int
{
	return *(__IO unsigned int*)address;
}

//BY SAGAHDS 20190306	//��ָ����ַ�������ݵ�ָ��λ��	//������int����	//�������û��
void FLASH_ReadMoreINT(unsigned long startAddress, unsigned int *readData, unsigned int countToRead)	
{
	unsigned int dataIndex;
	for(dataIndex=0;dataIndex<countToRead;countToRead++)
		readData[dataIndex]=FLASH_ReadINT(startAddress+dataIndex*2);
}

//BY SAGAHDS 20190306	//��ָ����ַ�������ݵ�FLASH_RDBUFF	//������char����	//��������ݳ�����ָ��int�ĳ���
//��ȡָ��ҳ��0x0800F800������
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

//BY SAGAHDS 20190306	//��ָ����ַд�����ݴ�ָ��λ��FLASH_RDBUFF	//������char����	//��������ݳ�����ָ��int�ĳ���
//д�뵽ָ��ҳ��0x0800F800������
void FLASH_WriteMoreCHAR(unsigned int startAddress, unsigned int *writeData, unsigned int countToWrite)	
{
	unsigned int i;
	unsigned long data;
	unsigned long FLASH_Address;
	
	if(!(FLASH_FLAG & FLASH_FLAG_ENABLE))return;

	FLASH_Address = 0x0800F800 + startAddress;
	
	FLASH_Unlock();								//Ƭ��FLASH����
	FLASH_ErasePage(0x0800F800);	//Ƭ��FLASH��������
	
	for(i=0; i<countToWrite; i++)	//Ƭ��FLASHд��
	{
		data = writeData[i*2 + 0] + (writeData[i*2 + 1] * 256);
		FLASH_ProgramHalfWord(FLASH_Address + i*2, data);
	}
	
	FLASH_Lock();									//Ƭ��FLASH����
}

//BY SAGAHDS 20190306	//��ָ����ַд�����ݴ�ָ��λ��FLASH_RDBUFF	//������char����	//��������ݳ�����ָ��int�ĳ���
//д�뵽ָ��ҳ��0x0800F800������
void FLASH_WriteMoreCHAR1(unsigned int startAddress, unsigned int writeData)	
{
	unsigned int i;
	unsigned long data;
	unsigned long FLASH_Address;
	
	if(!(FLASH_FLAG & FLASH_FLAG_ENABLE))return;

	FLASH_Address = 0x0800F800 + startAddress;
	
	FLASH_Unlock();								//Ƭ��FLASH����
	FLASH_ErasePage(0x0800F800);	//Ƭ��FLASH��������
	

		FLASH_ProgramHalfWord(FLASH_Address + i*2, data);

	
	FLASH_Lock();									//Ƭ��FLASH����
}

//BY SAGAHDS 20190306	//��ָ����ַ�������ݵ�FLASH_RDBUFF	//������char����	//��������ݳ�����ָ��int�ĳ���
//��ȡָ��ҳ��0x0800F800������
void FLASH_ReadMoreCHAR1(unsigned int startAddress, unsigned int readData)	
{
	unsigned int i;
	unsigned int data;
	unsigned long FLASH_Address;
	
	if(!(FLASH_FLAG & FLASH_FLAG_ENABLE))return;

	FLASH_Address = 0x0800F800 + startAddress;
	

		Luyin_counting = FLASH_ReadINT(FLASH_Address + i*2);

}

//void FLASH_Unlock(void)																										//��������
//FLASH_Status FLASH_ErasePage(uint32_t Page_Address)												//ɾ������
//FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data) 					//д�ĸ��ֽ�
//FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data) 			//д�����ֽ�
//FLASH_Status FLASH_ProgramOptionByteData(uint32_t Address, uint8_t Data)  //дһ���ֽ�
//void FLASH_Lock(void)																											//��������
