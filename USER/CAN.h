#ifndef _CAN_H
#define _CAN_H
	#include "stm32f10x_can.h"             
	#include "can_database.h"

	void Can1_Init(void);
	void Can_SendData(CanTxMsg* TxM);
	void RCC_Configuration(void);
	
#endif
