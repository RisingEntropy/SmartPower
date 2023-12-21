#include "bsp_sIIC.h"



/***************IIC基本过程函数*******************/
void I2Cs_Start(void);						//IIC开始信号
void I2Cs_Stop(void);							//IIC停止信号
void I2Cs_SendByte(uint8_t byte); //IIC字节发送
void I2Cs_Ack(void);							//IIC应答信号
void I2Cs_NAck(void);							//IIC非应答信号
uint8_t I2Cs_ReadByte(void);			//IIC字节读取
uint8_t I2Cs_WaitAck(void);				//IIC等待应答
/*********************************/


/*
************************************************
* 函 数 名: I2Cs_Delay
* 功能说明: I2C 总线位延迟，最快 400KHz
* 形 参：无
* 返 回 值: 无
************************************************
*/
static void I2Cs_Delay()
{
	uint8_t i;
	/*
下面的时间是通过逻辑分析仪测试得到的。
工作条件：CPU 主频 72MHz ，MDK 编译环境，1 级优化
循环次数为 10 时，SCL 频率 = 205KHz
循环次数为 7 时，SCL 频率 = 347KHz， SCL 高电平时间 1.5us，SCL 低电平时间 2.87us
循环次数为 5 时，SCL 频率 = 421KHz， SCL 高电平时间 1.25us，SCL 低电平时间 2.
→375us
	*/
	for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	函 数 名: I2Cs_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void I2Cs_Start()
{
	//SCL和SDA拉高的情况下，SDA拉低发送开始信号
	I2Cs_SCL_1();
	I2Cs_SDA_1();
	I2Cs_Delay();
	I2Cs_SDA_0();
	I2Cs_Delay();
	I2Cs_SCL_0();
	I2Cs_Delay();
}	

/*
*********************************************************************************************************
*	函 数 名: I2Cs_Stop
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void I2Cs_Stop()
{
	//SCL拉高时，SDA从低变高，发送停止信号
	I2Cs_SDA_0();
	I2Cs_SCL_1();
	I2Cs_Delay();
	I2Cs_SDA_1();
}

/*
*********************************************************************************************************
*	函 数 名: I2Cs_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_byte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void I2Cs_SendByte(uint8_t byte)
{
	//初始状态SCL为0，当SCL低，改变SDA；当SCL高，发送SDA值
	for(int i=0;i<8;i++)//高位在前
	{
		if(byte & 0x80)//最高位为1，结果为1，最高位为0，结果为0
		{
			I2Cs_SDA_1();
		}
		else 
		{
			I2Cs_SDA_0();
		}
		I2Cs_Delay();
		I2Cs_SCL_1();
		I2Cs_Delay();
		I2Cs_SCL_0();
		if(i==7) I2Cs_SDA_1();
		byte <<=1; //低位向高位移动一位
		I2Cs_Delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: I2Cs_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t I2Cs_ReadByte()
{
	//初始SCL为0，SCL拉高时读值
	uint8_t byte=0;
	
	for(int i=0;i<8;i++)
	{
		byte<<=1;//左移，先读高位
		I2Cs_SCL_1();
		I2Cs_Delay();
		if(I2Cs_READ())//SDA为1，读1，否则读0
		{
			byte++;//改变最低位
		}
		I2Cs_SCL_0();
		I2Cs_Delay();
	}
	return byte;
}

/*
*********************************************************************************************************
*	函 数 名: I2Cs_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t I2Cs_WaitAck()
{
	//初态SCL为0，SDA无所谓。先拉高SDA（即释放SDA），再将SCL拉高，读取SDA值，SDA为0为应答，否则非应答
	uint8_t rec;
	
	I2Cs_SDA_1();
	I2Cs_Delay();
	I2Cs_SCL_1();
	I2Cs_Delay();
	if(I2Cs_READ())
	{
		rec=1;
	}
	else 
	{
		rec=0;
	}
	I2Cs_SCL_0();//SCL拉低
	I2Cs_Delay();
	return rec;
	
}

/*
*********************************************************************************************************
*	函 数 名: I2Cs_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void I2Cs_Ack()
{
	//给一个时钟的0
	I2Cs_SDA_0();
	I2Cs_Delay();
	I2Cs_SCL_1();
	I2Cs_Delay();
	I2Cs_SCL_0();
	I2Cs_Delay();
	I2Cs_SDA_1();
}

/*
*********************************************************************************************************
*	函 数 名: I2Cs_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void I2Cs_NAck()
{
	//给一个时钟的1
	I2Cs_SDA_1();
	I2Cs_Delay();
	I2Cs_SCL_1();
	I2Cs_Delay();
	I2Cs_SCL_0();
	I2Cs_Delay();
}


/*
*********************************************************************************************************
*	函 数 名: I2Cs_CheckDev
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t I2Cs_CheckDev(uint8_t DevAddress)
{
	uint8_t result;
	I2Cs_Start();
	I2Cs_SendByte(DevAddress|0x00);
	result=I2Cs_WaitAck();
	I2Cs_Stop();
	
	return result;
}

/*
*********************************************************************************************************
*	函 数 名: I2Cs_WriteBytes
*	功能说明: 向从设备发送一系列数据
*	形    参：DevAddress:设备地址
*					pData:发送的数据数组
*					pSize:发送的数据个数
*	返 回 值: 返回值 0 表示发送失败，发送成功返回发送的数据个数
*********************************************************************************************************
*/
uint16_t I2Cs_WriteBytes(uint8_t DevAddress,uint8_t *pData, uint16_t pSize)
{
	#if 0
		if(I2Cs_CheckDev(DevAddress)) return 0;
	#endif
	I2Cs_Start();
	I2Cs_SendByte(DevAddress|0x00);
	if(I2Cs_WaitAck())//如果没有应答则停止并释放总线
	{
		I2Cs_Stop();
		return 0;
	}
	for(int i=0;i<pSize;i++)
	{
		I2Cs_SendByte(pData[i]);
		if(I2Cs_WaitAck())//发送一个字节后等待应答
		{
			I2Cs_Stop();
			return 0;
		}
	}
	I2Cs_Stop();
	return pSize;
	
}

/*
*********************************************************************************************************
*	函 数 名: I2Cs_ReadBytes
*	功能说明: 从IIC总线上读取一串数据
*	形    参：DevAddress : 设备地址
*			 	pSize : 数据长度，单位为字节
*			 	pData : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，读取成功返回发送的数据个数
*********************************************************************************************************
*/
uint16_t I2Cs_ReadBytes(uint8_t DevAddress,uint8_t *pData, uint16_t pSize)
{
	I2Cs_Start();
	I2Cs_SendByte(DevAddress|0x01);
	if(I2Cs_WaitAck())//如果没有应答则停止并释放总线
	{
		I2Cs_Stop();
		return 0;
	}
	for(int i=0;i<pSize;i++)
	{
		pData[i]=I2Cs_ReadByte();
		if(i==(pSize-1))
		{
			I2Cs_NAck();//最后一个数据后发送非应答
		}
		else I2Cs_Ack();
	}
	I2Cs_Stop();
	
	return pSize;
	
}

/*
*********************************************************************************************************
*	函 数 名: I2Cs_Mem_Read
*	功能说明: 从IIC总线上的储存设备读取一串数据
*	形    参：DevAddress : 设备地址
* 			MemAdd:内存地址
*				AddType:地址类型
*			 	pSize : 数据长度，单位为字节
*			 	pData : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，读取成功返回发送的数据个数
*********************************************************************************************************
*/
uint16_t I2Cs_Mem_Read(uint8_t DevAddress,uint16_t MemAdd,I2CAddType AddType,uint8_t *pData, uint16_t pSize)
{
	I2Cs_Start();
	I2Cs_SendByte(DevAddress|0x00);
	if(I2Cs_WaitAck())
	{
		I2Cs_Stop();
		return 0;
	}
	switch(AddType)//对应不同的地址类型，16Bit或者8Bit
	{
		case I2C_ADD_8BIT:
			I2Cs_SendByte(MemAdd);
			if(I2Cs_WaitAck())
			{
				I2Cs_Stop();
				return 0;
			}
			break;
		case I2C_ADD_16BIT:
			I2Cs_SendByte((uint8_t)(MemAdd>>=8));//先发送高8位
			if(I2Cs_WaitAck())
			{
				I2Cs_Stop();
				return 0;
			}
			I2Cs_SendByte((uint8_t)MemAdd);//再发送低8位
			if(I2Cs_WaitAck())
			{
				I2Cs_Stop();
				return 0;
			}
	}
	
	I2Cs_Stop();
	
	uint16_t re;
	re = I2Cs_ReadBytes(DevAddress,pData,pSize);
	
	return re;
	
}

/*
*********************************************************************************************************
*	函 数 名: I2Cs_Mem_Write
*	功能说明: 向IIC总线上的储存设备写一串数据
*	形    参：DevAddress : 设备地址
* 			MemAdd:内存地址
*				AddType:地址类型
*			 	pSize : 数据长度，单位为字节
*			 	pData : 要写入的数据
*	返 回 值: 0 表示失败，发送成功返回发送的数据个数
*********************************************************************************************************
*/
uint16_t I2Cs_Mem_Write(uint8_t DevAddress,uint16_t MemAdd,I2CAddType AddType,uint8_t *pData, uint16_t pSize)
{
	I2Cs_Start();
	I2Cs_SendByte(DevAddress|0x00);
	if(I2Cs_WaitAck())
	{
		I2Cs_Stop();
		return 0;
	}
	switch(AddType)
	{
		case I2C_ADD_8BIT:
			I2Cs_SendByte(MemAdd);
			if(I2Cs_WaitAck())
			{
				I2Cs_Stop();
				return 0;
			}
			break;
		case I2C_ADD_16BIT:
			I2Cs_SendByte((uint8_t)(MemAdd>>=8));//先发送高8位
			if(I2Cs_WaitAck())
			{
				I2Cs_Stop();
				return 0;
			}
			I2Cs_SendByte((uint8_t)MemAdd);//再发送低8位
			if(I2Cs_WaitAck())
			{
				I2Cs_Stop();
				return 0;
			}
	}
	
	for(int i=0;i<pSize;i++)
	{
		I2Cs_SendByte(pData[i]);
		if(I2Cs_WaitAck())
		{
			I2Cs_Stop();
			return 0;
		}
	}
	I2Cs_Stop();
	
	return pSize;
}

/*
*********************************************************************************************************
*	函 数 名: I2Cs_Init
*	功能说明: 初始化GPIO口
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

void I2Cs_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	I2Cs_SDA_GPIO_CLK_ENABLE();//使能时钟
	I2Cs_SCL_GPIO_CLK_ENABLE();
	
	GPIO_InitStruct.Pin=I2Cs_SDA_PIN|I2Cs_SCL_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_OD;//开漏输出
	GPIO_InitStruct.Pull=GPIO_NOPULL;//无上下拉
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(I2Cs_GPIOx,&GPIO_InitStruct);
	
	I2Cs_Stop();//释放总线
}
