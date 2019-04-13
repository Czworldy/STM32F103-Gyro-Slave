#include "io.h"
static void EXTI_Config(void)
{
	GPIO_InitTypeDef GIS;
	EXTI_InitTypeDef EIS;
	NVIC_InitTypeDef NIS;
	
	GIS.GPIO_Pin = GPIO_Pin_5;
	GIS.GPIO_Mode = GPIO_Mode_IPU;
	GIS.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GIS);
	
	EIS.EXTI_Line=EXTI_Line5;
	EIS.EXTI_Mode=EXTI_Mode_Interrupt;
	EIS.EXTI_Trigger=EXTI_Trigger_Falling;
	EIS.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EIS);
	EXTI_ClearITPendingBit(EXTI_Line5);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
	
	NIS.NVIC_IRQChannel=EXTI9_5_IRQn;
	NIS.NVIC_IRQChannelPreemptionPriority=0;
	NIS.NVIC_IRQChannelSubPriority=0;
	NIS.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NIS);
	
}

void IO_Config(void)
{
	GPIO_InitTypeDef GIS;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GIS.GPIO_Mode=GPIO_Mode_Out_PP;
	//GIS.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GIS.GPIO_Speed=GPIO_Speed_50MHz;
	
	GIS.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_8;
	GPIO_Init(GPIOC,&GIS);
	GPIO_ResetBits(GPIOC,GPIO_Pin_8);
	
	GIS.GPIO_Pin=GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_3;
	GPIO_Init(GPIOB,&GIS);

	GIS.GPIO_Pin=GPIO_Pin_15;
	GPIO_Init(GPIOA,&GIS);
	
	Gerege = SWITCH_OFF;
	Legs = SWITCH_OFF;
	Rise = SWITCH_OFF;
	WideLeg = SWITCH_OFF;

  EXTI_Config();
}
void EXTI9_5_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line5) != RESET)
		{
//			delay_ms(50);
//			Gerege = !Gerege;
 			MR2_StartFlag = 1;
//			Write_Database(MISO_MR2_WAKEUP_ID);
// 			isGet = 1;
// 			EXTI->EMR &= ~EXTI_Line5;
// 			EXTI->IMR &= ~EXTI_Line5;
			delayms = 100;
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
			TIM_Cmd(TIM2,ENABLE);
		}
		EXTI_ClearITPendingBit(EXTI_Line5);
}

