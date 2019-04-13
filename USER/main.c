#include "stm32f10x.h"
#include "usart.h"
#include "can.h"
#include "tim.h"
#include "io.h"
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	USART_GPIO_Init();
	USART_DMA_Config(115200,115200,0); //use USART1 & USART2
	IO_Config();

	TIM2_3_Config();
	Can1_Init();
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	isGet = 0;
	while (1)
	{

		if(isGet == 1)
		{
			Gerege = !Gerege;
			Write_Database(MISO_MR2_WAKEUP_ID);
			isGet = 0;
		}
		switch(MR2_RiseFlag)
		{
			case 1:
				Rise = SWITCH_ON;
				MR2_RiseFlag = 2;
				break;
			case 0:
				Rise = SWITCH_OFF;
				MR2_RiseFlag = 2;
				break;
			
		}
		if(Cylinder_Flag == 1)
		{
			Legs = SWITCH_ON;
			Cylinder_Flag = 2;
		}
		else if(Cylinder_Flag == 0)
		{
			Legs = SWITCH_OFF;
			Cylinder_Flag = 2;
		}
// 		if(Gyro_RxBuffer1[0] != 0x55)
// 		{
// 			INTX_DISABLE();
// 			DMA_Cmd(DMA1_Channel5, DISABLE);
// 			DMA_Cmd(DMA1_Channel6, DISABLE);
// 			USART_DMA_EN1();
// 			USART_DMA_EN0();
// 			INTX_ENABLE();
// 		}
	}
}



