//================================================================================
//STM32F103         �๦��ͨѶ������ʾģ��
//================================================================================
//FLASH_S.C/H       Ƭ��FLASH��д����                          BY SAGAHDS 20190306
//================================================================================
#ifndef __FLASH_S_H
#define __FLASH_S_H
//================================================================================
//BY SAGAHDS 20190306
extern u16 Luyin_counting;

extern unsigned int FLASH_RDBUFF[256];     	//FLASH������
extern unsigned int FLASH_WRBUFF[256];     	//FLASHд����
extern unsigned char FLASH_FLAG;     					//FLASH��־λ
             #define FLASH_FLAG_ENABLE 0x01 	//��1��װ��FLASH
//================================================================================
void FLASH_Configuration(unsigned char Enable);					//��ʼ��
unsigned int FLASH_ReadINT(unsigned long address);	//��ָ����ַ����int
void FLASH_ReadMoreINT(unsigned long startAddress, unsigned int *readData, unsigned int countToRead);			//��ָ����ַ�������ݵ�ָ��λ��
void FLASH_ReadMoreCHAR(unsigned int startAddress, unsigned int *readData, unsigned int countToRead);		//��ָ����ַ�������ݵ�ָ��λ��
void FLASH_WriteMoreCHAR(unsigned int startAddress, unsigned int *writeData, unsigned int countToWrite);	//��ָ����ַд�����ݴ�ָ��λ��
void FLASH_WriteMoreCHAR1(unsigned int startAddress, unsigned int writeData)	;
void FLASH_ReadMoreCHAR1(unsigned int startAddress, unsigned int readData)	;
//================================================================================
#endif
//================================================================================

