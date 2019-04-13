#ifndef _USART_H
#define _USART_H
#include "stm32f10x.h"
#include "stdio.h"
#include "stdarg.h"
#include "dataread.h"
#include "can_database.h"

#define Gyro_RxBufferSize 11

void USART_GPIO_Init(void);
void USART_DMA_Config(u32 BaudRate1,u32 BaudRate2,u32 BaudRate3);
void USART_SendByte(USART_TypeDef *USARTx, uint8_t dat);
uint8_t USART_GetByte(USART_TypeDef *USARTx);
void USART_puts(USART_TypeDef * USARTx, char * str);
void USART_printf(USART_TypeDef * USARTx, char *fmt, ...);
void USART_DMA_EN0(void);
void USART_DMA_EN1(void);

extern __align(32) volatile u8 Gyro_RxBuffer0[Gyro_RxBufferSize];
extern __align(32) volatile u8 Gyro_RxBuffer1[Gyro_RxBufferSize];

#endif






