/*
*软件I2C模拟驱动程序*
*------------------*
*作者：LO_StartNet
*最后修改:2022-7-4
*
*参考于野火的软件IIC
*/
#ifndef _BSP_SIIC_H_
#define _BSP_SIIC_H_

#include <stdint.h>
/****************************************/

#include "stm32l0xx_hal.h"   //此处引入使用的芯片头文件

/************GPIO设置***************/
/*
*选定IIC的引脚
*移植时修改此处的定义
*/

#define I2Cs_SDA_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()  //IIC数据线引脚的时钟
#define I2Cs_SCL_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()  //IIC时钟线引脚的时钟

#if 0  //这里区分了使用同一个GPIOX和不同GPIOX的区别
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
#define I2Cs_SDA_PIN GPIO_PIN_6  //数据线的PIN
#define I2Cs_SCL_PIN GPIO_PIN_5  //时钟线的PIN

#define I2Cs_SDA_1() HAL_GPIO_WritePin(I2Cs_GPIOx,I2Cs_SDA_PIN,GPIO_PIN_SET)   //数据线拉高
#define I2Cs_SDA_0() HAL_GPIO_WritePin(I2Cs_GPIOx,I2Cs_SDA_PIN,GPIO_PIN_RESET) //数据线拉低
#define I2Cs_SCL_1() HAL_GPIO_WritePin(I2Cs_GPIOx,I2Cs_SCL_PIN,GPIO_PIN_SET)   //时钟线拉高
#define I2Cs_SCL_0() HAL_GPIO_WritePin(I2Cs_GPIOx,I2Cs_SCL_PIN,GPIO_PIN_RESET) //时钟线拉低
	
#define I2Cs_READ() HAL_GPIO_ReadPin(I2Cs_GPIOx,I2Cs_SDA_PIN)   //读取数据线状态
#endif

/****************其他定义*******************/

typedef enum{
	I2C_ADD_8BIT=0,
	I2C_ADD_16BIT,
}I2CAddType;//地址类型定义



/***************数据发送接收函数*******************/
void I2Cs_Init(void);			//初始化函数，需最先调用																																											//IIC初始化函数
uint16_t I2Cs_WriteBytes(uint8_t DevAddress,uint8_t *pData, uint16_t pSize);																		//数据发送函数
uint8_t I2Cs_CheckDev(uint8_t DevAddress);																																			//设备检查函数
uint16_t I2Cs_ReadBytes(uint8_t DevAddress,uint8_t *pData, uint16_t pSize);																			//数据读取函数
uint16_t I2Cs_Mem_Write(uint8_t DevAddress,uint16_t MemAdd,I2CAddType AddType,uint8_t *pData, uint16_t pSize);	//储存设备数据写入函数
uint16_t I2Cs_Mem_Read(uint8_t DevAddress,uint16_t MemAdd,I2CAddType AddType,uint8_t *pData, uint16_t pSize);		//储存设备数据读取函数
/***************IIC基本过程函数*******************/
//void I2Cs_Start(void);						//IIC开始信号
//void I2Cs_Stop(void);							//IIC停止信号
//void I2Cs_SendByte(uint8_t byte); //IIC字节发送
//void I2Cs_Ack(void);							//IIC应答信号
//void I2Cs_NAck(void);							//IIC非应答信号
//uint8_t I2Cs_ReadByte(void);			//IIC字节读取
//uint8_t I2Cs_WaitAck(void);				//IIC等待应答
/*********************************/
#endif
