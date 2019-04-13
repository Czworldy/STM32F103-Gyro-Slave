 
#include "usart.h"


__align(32) volatile u8 Gyro_RxBuffer0[Gyro_RxBufferSize];
__align(32) volatile u8 Gyro_RxBuffer1[Gyro_RxBufferSize];



void USART_GPIO_Init(void)
{
	GPIO_InitTypeDef GIS;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3,ENABLE);

	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
	GIS.GPIO_Speed=GPIO_Speed_50MHz;
	
	GIS.GPIO_Mode=GPIO_Mode_AF_PP;
	GIS.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_2;
	GPIO_Init(GPIOA,&GIS);
	GIS.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOC,&GIS);
	
	GIS.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GIS.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_3;
	GPIO_Init(GPIOA,&GIS);
	GIS.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOC,&GIS);
}
void USART1_Mode_Init(u32 Baudrate)
{
	USART_InitTypeDef UIS;
	UIS.USART_BaudRate=Baudrate;
	UIS.USART_WordLength=USART_WordLength_8b;
	UIS.USART_StopBits=USART_StopBits_1;
	UIS.USART_Parity=USART_Parity_No;
	UIS.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	UIS.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Init(USART1,&UIS);
	USART_Cmd(USART1,ENABLE);
}

void USART2_Mode_Init(u32 Baudrate)
{
	USART_InitTypeDef UIS;
	UIS.USART_BaudRate=Baudrate;
	UIS.USART_WordLength=USART_WordLength_8b;
	UIS.USART_StopBits=USART_StopBits_1;
	UIS.USART_Parity=USART_Parity_No;
	UIS.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	UIS.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	
// 	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Init(USART2,&UIS);
	USART_Cmd(USART2,ENABLE);
}

void USART3_Mode_Init(u32 Baudrate)
{
	USART_InitTypeDef UIS;
	UIS.USART_BaudRate=Baudrate;
	UIS.USART_WordLength=USART_WordLength_8b;
	UIS.USART_StopBits=USART_StopBits_1;
	UIS.USART_Parity=USART_Parity_No;
	UIS.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	UIS.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	
// 	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_Init(USART3,&UIS);
	USART_Cmd(USART3,ENABLE);
}

void USART_DMA_Config(u32 BaudRate1,u32 BaudRate2,u32 BaudRate3)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	if(BaudRate1!=0)
		USART1_Mode_Init(BaudRate1);
	if(BaudRate2!=0)
		USART2_Mode_Init(BaudRate2);
	if(BaudRate3!=0)
		USART3_Mode_Init(BaudRate3);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);	 //ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Gyro_RxBuffer0;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = Gyro_RxBufferSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址固定
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel5, DISABLE);
	
// 	DMA_DeInit(DMA1_Channel6);
// 	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);	 //ADC地址
// 	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Gyro_RxBuffer1;//内存地址
// 	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
// 	DMA_InitStructure.DMA_BufferSize = Gyro_RxBufferSize;
// 	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
// 	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址固定
// 	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//8位
// 	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
// 	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输
// 	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
// 	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
// 	DMA_Init(DMA1_Channel6, &DMA_InitStructure);

// 	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
// 	
// 	USART_ClearFlag(USART2, USART_FLAG_TC);
// 	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
// 	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);
// 	DMA_Cmd(DMA1_Channel6, DISABLE);
	
	USART_DMA_EN0();
	//USART_DMA_EN1();
}

void USART_SendByte(USART_TypeDef *USARTx, uint8_t dat)
{
	USART_SendData(USARTx, dat);
	while (!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));
}

uint8_t USART_GetByte(USART_TypeDef *USARTx)
{
	uint8_t dat;
	while (!USART_GetFlagStatus(USARTx, USART_FLAG_RXNE));
	dat = (USART_ReceiveData(USARTx) & 0xff);
	return dat;
}

void USART_puts(USART_TypeDef * USARTx, char * str)
{
	while (*str)
	{
		USART_SendByte(USARTx, *str++);
	}
}

void USART_printf(USART_TypeDef * USARTx, char *fmt, ...)
{
	va_list ap;
	char str[128];

	va_start(ap,fmt);
	vsprintf(str,fmt,ap);
	va_end(ap);
	USART_puts(USARTx, str);
}


void DMA1_Channel5_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC5) == SET)
	{
		Angle_up.u8_data[0] = Gyro_RxBuffer0[6];
		Angle_up.u8_data[1] = Gyro_RxBuffer0[7];
		DMA_ClearITPendingBit(DMA1_IT_TC5);
	}
}
void DMA1_Channel6_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC6) == SET)
	{
		Angle_down.u8_data[0] = Gyro_RxBuffer1[6];
		Angle_down.u8_data[1] = Gyro_RxBuffer1[7];
		DMA_ClearITPendingBit(DMA1_IT_TC6);
	}
}
void USART_DMA_EN0(void)
{
	static uint8_t gyro_start = 0, j = 0;
	static uint8_t gyro_temp[Gyro_RxBufferSize] = {0};
	
	while (!gyro_start)
	{
		if (j == 0 || j == 1)
		{
			gyro_temp[j] = USART_GetByte(USART1);
			if (gyro_temp[j] == 0x55)
			{
				j = 1;
				gyro_temp[j] = USART_GetByte(USART1);
				if (gyro_temp[j] == 0x53)
				{
					j = 2;
				}
				else
				{
					j = 0;
				}
			}
			else
			{
				j = 0;
			}
		}
		else
		{
			gyro_temp[j++] = USART_GetByte(USART1);
			switch (j)
			{
				case 11:
					if (((gyro_temp[0] + gyro_temp[1] + gyro_temp[2] + gyro_temp[3] + gyro_temp[4] + gyro_temp[5] + 
						gyro_temp[6] + gyro_temp[7] + gyro_temp[8] + gyro_temp[9]) & 0xff) == gyro_temp[10])
					{
						gyro_start = 1;
						while (1)
						{
							if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
							{
								break;
							}
						}
						DMA_Cmd(DMA1_Channel5, ENABLE);
					}
					else
					{
						j = 0;
					}
					break;
				default:
					break;
			}
		}
	}
}
void USART_DMA_EN1(void)
{
	static uint8_t gyro_start = 0, j = 0;
	static uint8_t gyro_temp[Gyro_RxBufferSize] = {0};
	
	while (!gyro_start)
	{
		if (j == 0 || j == 1)
		{
			gyro_temp[j] = USART_GetByte(USART2);
			if (gyro_temp[j] == 0x55)
			{
				j = 1;
				gyro_temp[j] = USART_GetByte(USART2);
				if (gyro_temp[j] == 0x53)
				{
					j = 2;
				}
				else
				{
					j = 0;
				}
			}
			else
			{
				j = 0;
			}
		}
		else
		{
			gyro_temp[j++] = USART_GetByte(USART2);
			switch (j)
			{
				case 11:
					if (((gyro_temp[0] + gyro_temp[1] + gyro_temp[2] + gyro_temp[3] + gyro_temp[4] + gyro_temp[5] + 
						gyro_temp[6] + gyro_temp[7] + gyro_temp[8] + gyro_temp[9]) & 0xff) == gyro_temp[10])
					{
						gyro_start = 1;
						while (1)
						{
							if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
							{
								break;
							}
						}
						DMA_Cmd(DMA1_Channel6, ENABLE);
					}
					else
					{
						j = 0;
					}
					break;
				default:
					break;
			}
		}
	}
}
// void USART1_IRQHandler(void)
// {
// 	u8 c;
// 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
// 	{ 	
// 		c = USART1->DR;
// 	} 
// }

// void USART2_IRQHandler(void)
// {
// 	u8 c;
// 	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
// 	{
// 		c = USART2->DR;
// 	}
// }

// void USART3_IRQHandler(void)
// {
// 	u8 c;
// 	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
// 	{
// 		c = USART3->DR;
// 	}
// }

