
#include "tim.h"

u16 delayms = 50;
u8 isGet;
static void TIM_Base_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_InitStr;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InitStr.TIM_Period = 999;
	TIM_InitStr.TIM_Prescaler = 71; //72MHz  400Hz定时器
	TIM_InitStr.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStr.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM2 , &TIM_InitStr);
	
	TIM_ClearFlag(TIM2 , TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2 , TIM_IT_Update , ENABLE);
	
	TIM_Cmd(TIM2 , ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);//暂时失能

// 	TIM_InitStr.TIM_Period = 999;
// 	TIM_InitStr.TIM_Prescaler = 71;
// 	TIM_TimeBaseInit(TIM3 , &TIM_InitStr);
// 	
// 	TIM_ClearFlag(TIM3 , TIM_FLAG_Update);
// 	
// 	TIM_ITConfig(TIM3 , TIM_IT_Update , ENABLE);
	
//	TIM_Cmd(TIM3 , ENABLE);
	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);//暂时失能
// 	TIM_InitStr.TIM_Period = 999;
// 	TIM_InitStr.TIM_Prescaler = 719;
// 	TIM_TimeBaseInit(TIM3 , &TIM_InitStr);
// 	
// 	TIM_ClearFlag(TIM4 , TIM_FLAG_Update);
// 	
// 	TIM_ITConfig(TIM4 , TIM_IT_Update , ENABLE);
// 	
// 	TIM_Cmd(TIM4 , ENABLE);
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , DISABLE);//暂时失能
}

static void TIM_NVIC_Config(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0 ;         
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
// 	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0 ;         
// 	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
// 	
// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
// 	
// 	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3 ;         
// 	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
}
void SysTick_Init(void);
void TIM2_3_Config(void)
{
	TIM_Base_Config();
	TIM_NVIC_Config();
	SysTick_Init();
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{	
		--delayms;
		if(delayms == 0)
		{
//			Gerege = !Gerege;
//			Write_Database(MISO_MR2_WAKEUP_ID);
//			delayms = 50;
			isGet = 1;
			TIM_Cmd(TIM2,DISABLE);
		}
		TIM_ClearITPendingBit(TIM2 , TIM_IT_Update);
	}
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{	
 	
		TIM_ClearITPendingBit(TIM3 , TIM_IT_Update);
	}
}

void SysTick_Init(void)
{
	if (SysTick_Config(SystemCoreClock / 1000))   //Setup SysTick Timer for 1 ms interrupts
	{
		/* Capture error */ 
		while (1);
	}
}

void SysTick_Handler(void)
{
  Write_Database(MISO_ANG_UP);
// 	Write_Database(MISO_ANG_DOWN); 
//	LED = !LED;
}


