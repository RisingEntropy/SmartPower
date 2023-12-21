#include "bsp_sIIC.h"



/***************IIC�������̺���*******************/
void I2Cs_Start(void);						//IIC��ʼ�ź�
void I2Cs_Stop(void);							//IICֹͣ�ź�
void I2Cs_SendByte(uint8_t byte); //IIC�ֽڷ���
void I2Cs_Ack(void);							//IICӦ���ź�
void I2Cs_NAck(void);							//IIC��Ӧ���ź�
uint8_t I2Cs_ReadByte(void);			//IIC�ֽڶ�ȡ
uint8_t I2Cs_WaitAck(void);				//IIC�ȴ�Ӧ��
/*********************************/


/*
************************************************
* �� �� ��: I2Cs_Delay
* ����˵��: I2C ����λ�ӳ٣���� 400KHz
* �� �Σ���
* �� �� ֵ: ��
************************************************
*/
static void I2Cs_Delay()
{
	uint8_t i;
	/*
�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
����������CPU ��Ƶ 72MHz ��MDK ���뻷����1 ���Ż�
ѭ������Ϊ 10 ʱ��SCL Ƶ�� = 205KHz
ѭ������Ϊ 7 ʱ��SCL Ƶ�� = 347KHz�� SCL �ߵ�ƽʱ�� 1.5us��SCL �͵�ƽʱ�� 2.87us
ѭ������Ϊ 5 ʱ��SCL Ƶ�� = 421KHz�� SCL �ߵ�ƽʱ�� 1.25us��SCL �͵�ƽʱ�� 2.
��375us
	*/
	for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	�� �� ��: I2Cs_Start
*	����˵��: CPU����I2C���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void I2Cs_Start()
{
	//SCL��SDA���ߵ�����£�SDA���ͷ��Ϳ�ʼ�ź�
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
*	�� �� ��: I2Cs_Stop
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void I2Cs_Stop()
{
	//SCL����ʱ��SDA�ӵͱ�ߣ�����ֹͣ�ź�
	I2Cs_SDA_0();
	I2Cs_SCL_1();
	I2Cs_Delay();
	I2Cs_SDA_1();
}

/*
*********************************************************************************************************
*	�� �� ��: I2Cs_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    �Σ�_byte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void I2Cs_SendByte(uint8_t byte)
{
	//��ʼ״̬SCLΪ0����SCL�ͣ��ı�SDA����SCL�ߣ�����SDAֵ
	for(int i=0;i<8;i++)//��λ��ǰ
	{
		if(byte & 0x80)//���λΪ1�����Ϊ1�����λΪ0�����Ϊ0
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
		byte <<=1; //��λ���λ�ƶ�һλ
		I2Cs_Delay();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: I2Cs_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t I2Cs_ReadByte()
{
	//��ʼSCLΪ0��SCL����ʱ��ֵ
	uint8_t byte=0;
	
	for(int i=0;i<8;i++)
	{
		byte<<=1;//���ƣ��ȶ���λ
		I2Cs_SCL_1();
		I2Cs_Delay();
		if(I2Cs_READ())//SDAΪ1����1�������0
		{
			byte++;//�ı����λ
		}
		I2Cs_SCL_0();
		I2Cs_Delay();
	}
	return byte;
}

/*
*********************************************************************************************************
*	�� �� ��: I2Cs_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t I2Cs_WaitAck()
{
	//��̬SCLΪ0��SDA����ν��������SDA�����ͷ�SDA�����ٽ�SCL���ߣ���ȡSDAֵ��SDAΪ0ΪӦ�𣬷����Ӧ��
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
	I2Cs_SCL_0();//SCL����
	I2Cs_Delay();
	return rec;
	
}

/*
*********************************************************************************************************
*	�� �� ��: I2Cs_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void I2Cs_Ack()
{
	//��һ��ʱ�ӵ�0
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
*	�� �� ��: I2Cs_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void I2Cs_NAck()
{
	//��һ��ʱ�ӵ�1
	I2Cs_SDA_1();
	I2Cs_Delay();
	I2Cs_SCL_1();
	I2Cs_Delay();
	I2Cs_SCL_0();
	I2Cs_Delay();
}


/*
*********************************************************************************************************
*	�� �� ��: I2Cs_CheckDev
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
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
*	�� �� ��: I2Cs_WriteBytes
*	����˵��: ����豸����һϵ������
*	��    �Σ�DevAddress:�豸��ַ
*					pData:���͵���������
*					pSize:���͵����ݸ���
*	�� �� ֵ: ����ֵ 0 ��ʾ����ʧ�ܣ����ͳɹ����ط��͵����ݸ���
*********************************************************************************************************
*/
uint16_t I2Cs_WriteBytes(uint8_t DevAddress,uint8_t *pData, uint16_t pSize)
{
	#if 0
		if(I2Cs_CheckDev(DevAddress)) return 0;
	#endif
	I2Cs_Start();
	I2Cs_SendByte(DevAddress|0x00);
	if(I2Cs_WaitAck())//���û��Ӧ����ֹͣ���ͷ�����
	{
		I2Cs_Stop();
		return 0;
	}
	for(int i=0;i<pSize;i++)
	{
		I2Cs_SendByte(pData[i]);
		if(I2Cs_WaitAck())//����һ���ֽں�ȴ�Ӧ��
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
*	�� �� ��: I2Cs_ReadBytes
*	����˵��: ��IIC�����϶�ȡһ������
*	��    �Σ�DevAddress : �豸��ַ
*			 	pSize : ���ݳ��ȣ���λΪ�ֽ�
*			 	pData : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ���ȡ�ɹ����ط��͵����ݸ���
*********************************************************************************************************
*/
uint16_t I2Cs_ReadBytes(uint8_t DevAddress,uint8_t *pData, uint16_t pSize)
{
	I2Cs_Start();
	I2Cs_SendByte(DevAddress|0x01);
	if(I2Cs_WaitAck())//���û��Ӧ����ֹͣ���ͷ�����
	{
		I2Cs_Stop();
		return 0;
	}
	for(int i=0;i<pSize;i++)
	{
		pData[i]=I2Cs_ReadByte();
		if(i==(pSize-1))
		{
			I2Cs_NAck();//���һ�����ݺ��ͷ�Ӧ��
		}
		else I2Cs_Ack();
	}
	I2Cs_Stop();
	
	return pSize;
	
}

/*
*********************************************************************************************************
*	�� �� ��: I2Cs_Mem_Read
*	����˵��: ��IIC�����ϵĴ����豸��ȡһ������
*	��    �Σ�DevAddress : �豸��ַ
* 			MemAdd:�ڴ��ַ
*				AddType:��ַ����
*			 	pSize : ���ݳ��ȣ���λΪ�ֽ�
*			 	pData : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ���ȡ�ɹ����ط��͵����ݸ���
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
	switch(AddType)//��Ӧ��ͬ�ĵ�ַ���ͣ�16Bit����8Bit
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
			I2Cs_SendByte((uint8_t)(MemAdd>>=8));//�ȷ��͸�8λ
			if(I2Cs_WaitAck())
			{
				I2Cs_Stop();
				return 0;
			}
			I2Cs_SendByte((uint8_t)MemAdd);//�ٷ��͵�8λ
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
*	�� �� ��: I2Cs_Mem_Write
*	����˵��: ��IIC�����ϵĴ����豸дһ������
*	��    �Σ�DevAddress : �豸��ַ
* 			MemAdd:�ڴ��ַ
*				AddType:��ַ����
*			 	pSize : ���ݳ��ȣ���λΪ�ֽ�
*			 	pData : Ҫд�������
*	�� �� ֵ: 0 ��ʾʧ�ܣ����ͳɹ����ط��͵����ݸ���
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
			I2Cs_SendByte((uint8_t)(MemAdd>>=8));//�ȷ��͸�8λ
			if(I2Cs_WaitAck())
			{
				I2Cs_Stop();
				return 0;
			}
			I2Cs_SendByte((uint8_t)MemAdd);//�ٷ��͵�8λ
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
*	�� �� ��: I2Cs_Init
*	����˵��: ��ʼ��GPIO��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void I2Cs_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	I2Cs_SDA_GPIO_CLK_ENABLE();//ʹ��ʱ��
	I2Cs_SCL_GPIO_CLK_ENABLE();
	
	GPIO_InitStruct.Pin=I2Cs_SDA_PIN|I2Cs_SCL_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_OD;//��©���
	GPIO_InitStruct.Pull=GPIO_NOPULL;//��������
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(I2Cs_GPIOx,&GPIO_InitStruct);
	
	I2Cs_Stop();//�ͷ�����
}
