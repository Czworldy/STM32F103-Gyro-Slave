#include "can.h"

#define CAN1_BAUDRATE 1000

//是否使能相应can总线，及其FIFO
#define USE_CAN1_R0	1
#define USE_CAN1_R1	1
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;                    //定义外部高速晶体启动状态枚举变量
  RCC_DeInit();                                    //复位RCC外部设备寄存器到默认值
  RCC_HSEConfig(RCC_HSE_ON);                       //打开外部高速晶振
  HSEStartUpStatus = RCC_WaitForHSEStartUp();      //等待外部高速时钟准备好
  if(HSEStartUpStatus == SUCCESS)                  //外部高速时钟已经准别好
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //开启FLASH预读缓冲功能，加速FLASH的读取。所有程序中必须的用法.位置：RCC初始化子函数里面，时钟起振之后
    FLASH_SetLatency(FLASH_Latency_2);                    //flash操作的延时
		
    RCC_HCLKConfig(RCC_SYSCLK_Div1);               //配置AHB(HCLK)时钟等于==SYSCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);                //配置APB2(PCLK2)钟==AHB时钟
    RCC_PCLK1Config(RCC_HCLK_Div1);                //配置APB1(PCLK1)钟==AHB1/2时钟
         
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //配置PLL时钟 == 外部高速晶体时钟 * 9 = 72MHz
    RCC_PLLCmd(ENABLE);                                   //使能PLL时钟
   
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)    //等待PLL时钟就绪
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //配置系统时钟 = PLL时钟
    while(RCC_GetSYSCLKSource() != 0x08)                  //检查PLL时钟是否作为系统时钟
    {
    }
  }

}
void Can_NVIC_Config(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
#if  USE_CAN1_R0
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
#endif
#if  USE_CAN1_R1
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
#endif
}

void Can_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Configure CAN1 IOs **********************************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
#if USE_CAN1_R0 || USE_CAN1_R1
	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	         //PA11:CAN1-RX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			   //输入上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;					 //PA12:CAN1-TX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			 //复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
#endif
}

void CAN_CAN_Config(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	
	/* Enable CAN clock */
#if USE_CAN1_R0 || USE_CAN1_R1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	CAN_DeInit(CAN1);
#endif

	/* CAN cell init */
	CAN_StructInit(&CAN_InitStructure);

	CAN_InitStructure.CAN_TTCM=DISABLE;//失能时间触发通讯模式
	CAN_InitStructure.CAN_ABOM=DISABLE;//失能自动离线管理
	CAN_InitStructure.CAN_AWUM=DISABLE;//失能自动唤醒模式
	CAN_InitStructure.CAN_NART=DISABLE;//失能非自动重传输模式,即自动重传
	CAN_InitStructure.CAN_RFLM=DISABLE;//失能接收FIFO锁定模式,即FIFO溢出时新数据覆盖
	CAN_InitStructure.CAN_TXFP=ENABLE;//失能发送FIFO优先级,报文顺序由发送顺序决定 DISABLE:ID决定
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//正常模式 

	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;		   //BTR-SJW 重新同步跳跃宽度 1个时间单元
	CAN_InitStructure.CAN_BS1=CAN_BS1_2tq;		   //BTR-TS1 时间段1 占用了2个时间单元
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;		   //BTR-TS1 时间段2 占用了3个时间单元
	
	//总线波特率,波特率计算请参见 F1手册“22.7.7 Bit timing”
	//此处，APB1时钟为72M状态，虽然手册上说，APB1只能最大为36M(你自己强行倍频到72MHz,APB1总线最大频率 36MHz)
#if CAN1_BAUDRATE == 1000 /* 1MBps */
  CAN_InitStructure.CAN_Prescaler =12/2;			   //BTR-BRP 波特率分频器  定义了时间单元的时间长度 72/(1+2+3)/12=1Mbps
#elif CAN1_BAUDRATE == 500 /* 500KBps */
  CAN_InitStructure.CAN_Prescaler =24/2;
#elif CAN1_BAUDRATE == 250 /* 250KBps */
  CAN_InitStructure.CAN_Prescaler =48/2;
#elif CAN1_BAUDRATE == 125 /* 125KBps */
  CAN_InitStructure.CAN_Prescaler =96/2;
#endif 

#if USE_CAN1_R0 || USE_CAN1_R1
	CAN_Init(CAN1, &CAN_InitStructure);	
#endif
}

void Can_FILTER_Config(void)
{
	uint8_t i=0;
	uint8_t can1_filter = 0;
	uint16_t id_temp;
	
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	for(i=0;i<Can_Data_Num-4;i++)
	{
		if(Can_Database[i].Data_type==WRITE_ONLY)
		{
			CAN_FilterInitStructure.CAN_FilterNumber = can1_filter;
			can1_filter++;
			id_temp = (uint16_t)(Can_Database[i].Data_ID)<<5;
			CAN_FilterInitStructure.CAN_FilterIdHigh = id_temp;
			CAN_FilterInitStructure.CAN_FilterIdLow = id_temp;
			CAN_FilterInitStructure.CAN_FilterMaskIdHigh = id_temp;
			CAN_FilterInitStructure.CAN_FilterMaskIdLow = id_temp;
			CAN_FilterInitStructure.CAN_FilterFIFOAssignment = Can_Database[i].Fifo_num;
			
			CAN_FilterInit(&CAN_FilterInitStructure);
		}
	}
    i = Can_Data_Num-4;
    CAN_FilterInitStructure.CAN_FilterNumber = can1_filter;
    can1_filter=13;
    id_temp = (uint16_t)(Can_Database[i].Data_ID)<<5;
    CAN_FilterInitStructure.CAN_FilterIdHigh = id_temp;
    
    i++;
    id_temp = (uint16_t)(Can_Database[i].Data_ID)<<5;
    CAN_FilterInitStructure.CAN_FilterIdLow = id_temp;
    i++;
    id_temp = (uint16_t)(Can_Database[i].Data_ID)<<5;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = id_temp;
    i++;
    id_temp = (uint16_t)(Can_Database[i].Data_ID)<<5;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = id_temp;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = Can_Database[i].Fifo_num;
    
    CAN_FilterInit(&CAN_FilterInitStructure);
}

void Can_IT_Config(void)
{
#if  USE_CAN1_R0
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
#endif
#if  USE_CAN1_R1
	CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE);
#endif
}

void Can1_Init(void)
{
	Hash_Table_init();
	Can_NVIC_Config();
	Can_GPIO_Config();
	CAN_CAN_Config();
	Can_FILTER_Config();
	Can_IT_Config();
}

/*can总线接收时判断有点多，望大家见谅
由于can总线挂载设备较多，每块板子都有各自的程序，所有板子都不出
问题的概率较小。Can_Database数据库是外设与主控通信的公共信息，外
设与主控的数据库必须完全统一。否则，就会造成通信故障。在接收时，
进行判断可以快速发现两板之间的信息不统一问题。
*/
//主控向GPS板发送的命令
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	uint8_t i;
	CanRxMsg   RxMessage;
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	if(HASH_TABLE[RxMessage.StdId] < Can_Data_Num)
	{
		if(Can_Database[HASH_TABLE[RxMessage.StdId]].Data_length == RxMessage.DLC)
			if(Can_Database[HASH_TABLE[RxMessage.StdId]].Data_type == WRITE_ONLY)
				if(Can_Database[HASH_TABLE[RxMessage.StdId]].Channel == 1)
					if(Can_Database[HASH_TABLE[RxMessage.StdId]].Fifo_num == CAN_Filter_FIFO0)
					{
						for(i=0;i<RxMessage.DLC;i++)
							(*(Can_Database[HASH_TABLE[RxMessage.StdId]].Data_ptr+i)) = RxMessage.Data[i];
						Can_Database[HASH_TABLE[RxMessage.StdId]].MenuFunc();
					}
						
	}else
	{
		return;
	}
}

//主控向GPS板发送的数据
void CAN1_RX1_IRQHandler(void)
{
	uint8_t i;
	CanRxMsg   RxMessage;
	
	CAN_Receive(CAN1, CAN_FIFO1, &RxMessage);
	if(HASH_TABLE[RxMessage.StdId] < Can_Data_Num)
	{
		if(Can_Database[HASH_TABLE[RxMessage.StdId]].Data_length == RxMessage.DLC)
			if(Can_Database[HASH_TABLE[RxMessage.StdId]].Data_type == WRITE_ONLY)
				if(Can_Database[HASH_TABLE[RxMessage.StdId]].Channel == 1)
					if(Can_Database[HASH_TABLE[RxMessage.StdId]].Fifo_num == CAN_Filter_FIFO1)
					{
						for(i=0;i<RxMessage.DLC;i++)
							(*(Can_Database[HASH_TABLE[RxMessage.StdId]].Data_ptr+i)) = RxMessage.Data[i];
						Can_Database[HASH_TABLE[RxMessage.StdId]].MenuFunc();
					}
	}else
	{
		return;
	}
}

uint8_t Can_State = 0;
void Can_SendData(CanTxMsg* TxM)
{
    static int i=0;
    i++;
    
	if(CAN_Transmit(CAN1,TxM)==CAN_NO_MB)
    {
        Can_State = 0;
        if(i == 100)
        {
            CAN_DeInit(CAN1);
            
            Can1_Init();
        }
    }
    else
    {
        Can_State = 1;
        i=0;
    }
}

