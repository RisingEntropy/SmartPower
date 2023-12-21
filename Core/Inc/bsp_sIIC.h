/*
*���I2Cģ����������*
*------------------*
*���ߣ�LO_StartNet
*����޸�:2022-7-4
*
*�ο���Ұ������IIC
*/
#ifndef _BSP_SIIC_H_
#define _BSP_SIIC_H_

#include <stdint.h>
/****************************************/

#include "stm32l0xx_hal.h"   //�˴�����ʹ�õ�оƬͷ�ļ�

/************GPIO����***************/
/*
*ѡ��IIC������
*��ֲʱ�޸Ĵ˴��Ķ���
*/

#define I2Cs_SDA_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()  //IIC���������ŵ�ʱ��
#define I2Cs_SCL_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()  //IICʱ�������ŵ�ʱ��

#if 0  //����������ʹ��ͬһ��GPIOX�Ͳ�ͬGPIOX������
#define I2Cs_SDA_GPIOx GPIOA
#define I2Cs_SCL_GPIOx GPIOA
#define I2Cs_SDA_PIN GPIO_PIN_3
#define I2Cs_SCL_PIN GPIO_PIN_2

#define I2Cs_SDA_1() HAL_GPIO_WritePin(I2Cs_SDA_GPIOx,I2Cs_SDA_PIN,GPIO_PIN_SET)
#define I2Cs_SDA_0() HAL_GPIO_WritePin(I2Cs_SDA_GPIOx,I2Cs_SDA_PIN,GPIO_PIN_RESET)
#define I2Cs_SCL_1() HAL_GPIO_WritePin(I2Cs_SCL_GPIOx,I2Cs_SCL_PIN,GPIO_PIN_SET)
#define I2Cs_SCL_0() HAL_GPIO_WritePin(I2Cs_SCL_GPIOx,I2Cs_SCL_PIN,GPIO_PIN_RESET)
	
#define I2Cs_READ() HAL_GPIO_ReadPin(I2Cs_SDA_GPIOx,I2Cs_SDA_PIN)
#else

#define I2Cs_GPIOx GPIOB				 //GPIOX
#define I2Cs_SDA_PIN GPIO_PIN_6  //�����ߵ�PIN
#define I2Cs_SCL_PIN GPIO_PIN_5  //ʱ���ߵ�PIN

#define I2Cs_SDA_1() HAL_GPIO_WritePin(I2Cs_GPIOx,I2Cs_SDA_PIN,GPIO_PIN_SET)   //����������
#define I2Cs_SDA_0() HAL_GPIO_WritePin(I2Cs_GPIOx,I2Cs_SDA_PIN,GPIO_PIN_RESET) //����������
#define I2Cs_SCL_1() HAL_GPIO_WritePin(I2Cs_GPIOx,I2Cs_SCL_PIN,GPIO_PIN_SET)   //ʱ��������
#define I2Cs_SCL_0() HAL_GPIO_WritePin(I2Cs_GPIOx,I2Cs_SCL_PIN,GPIO_PIN_RESET) //ʱ��������
	
#define I2Cs_READ() HAL_GPIO_ReadPin(I2Cs_GPIOx,I2Cs_SDA_PIN)   //��ȡ������״̬
#endif

/****************��������*******************/

typedef enum{
	I2C_ADD_8BIT=0,
	I2C_ADD_16BIT,
}I2CAddType;//��ַ���Ͷ���



/***************���ݷ��ͽ��պ���*******************/
void I2Cs_Init(void);			//��ʼ�������������ȵ���																																											//IIC��ʼ������
uint16_t I2Cs_WriteBytes(uint8_t DevAddress,uint8_t *pData, uint16_t pSize);																		//���ݷ��ͺ���
uint8_t I2Cs_CheckDev(uint8_t DevAddress);																																			//�豸��麯��
uint16_t I2Cs_ReadBytes(uint8_t DevAddress,uint8_t *pData, uint16_t pSize);																			//���ݶ�ȡ����
uint16_t I2Cs_Mem_Write(uint8_t DevAddress,uint16_t MemAdd,I2CAddType AddType,uint8_t *pData, uint16_t pSize);	//�����豸����д�뺯��
uint16_t I2Cs_Mem_Read(uint8_t DevAddress,uint16_t MemAdd,I2CAddType AddType,uint8_t *pData, uint16_t pSize);		//�����豸���ݶ�ȡ����
/***************IIC�������̺���*******************/
//void I2Cs_Start(void);						//IIC��ʼ�ź�
//void I2Cs_Stop(void);							//IICֹͣ�ź�
//void I2Cs_SendByte(uint8_t byte); //IIC�ֽڷ���
//void I2Cs_Ack(void);							//IICӦ���ź�
//void I2Cs_NAck(void);							//IIC��Ӧ���ź�
//uint8_t I2Cs_ReadByte(void);			//IIC�ֽڶ�ȡ
//uint8_t I2Cs_WaitAck(void);				//IIC�ȴ�Ӧ��
/*********************************/
#endif
