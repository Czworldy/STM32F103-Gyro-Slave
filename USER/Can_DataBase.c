/*
每加一个CAN总线设备
	1、更改Can_Data_Num定义值
	2、要在Can_Database列表中添加总线信息，特别注意不要搞错
	3、void Hash_Table_init(void)中，添加新加设备在Can_Database列表中的序号
	4、添加ID_NUMDEF枚举定义中加入新添ID代表的命令意义
*/
#include "stm32f10x.h"
#include "can_database.h"
#include "can.h"
#include "usart.h"
uint8_t Can_Data_Num=0;

union u16_to_u8 Angle_up;
union u16_to_u8 Angle_down;

uint8_t MR2_StartFlag = 0;
uint8_t MR2_RiseFlag = 2;	
uint8_t Cylinder_Flag = 2;

void Null(void){}
//新加ID时，在ID_NUMDEF中定义相应ID的意义 
Can_Data Can_Database[]=
{
	//Data_type		Data_ID							*Data_ptr													Data_length		*MenuFunc		Channel		Fifo_num
	//主控&&手动车上层通信
	//{WRITE_ONLY, W_GAME_PROCESS_ID,  (uint8_t*)(&CAN_GAME_PROCESS),           1,        Null,          1,     CAN_Filter_FIFO1},
	{WRITE_ONLY,   MISO_ANG_UP,       	(uint8_t*)(&Angle_up.u8_data),        			2,          NULL,         	1,        CAN_Filter_FIFO1},
	{WRITE_ONLY,   MISO_ANG_DOWN,       (uint8_t*)(&Angle_down.u8_data),        		2,          NULL,         	1,        CAN_Filter_FIFO1},
	
	{READ_ONLY,    MISO_MR2_WAKEUP_ID,  (uint8_t*)(&MR2_StartFlag),  			    1,             NULL,     				  1,        CAN_Filter_FIFO1},
	{WRITE_ONLY,   MISO_MR2_RISE_ID,  	(uint8_t*)(&MR2_RiseFlag),  			    1,             Null,   				  1,  	 	CAN_Filter_FIFO1},
	{WRITE_ONLY,   MISO_CYLINDER_ID,	(uint8_t*)(&Cylinder_Flag),					1,			   Null,		    	  1,		CAN_Filter_FIFO1},
};

//HASH_TABLE初始值为255，认为Can_Data_Num值小于256，即设备数少于256个
uint8_t HASH_TABLE[256]={255};

void Hash_Table_init(void)
{
	int i;
	
	Can_Data_Num = sizeof(Can_Database) / sizeof(Can_Database[0]);
	for(i=0;i<256;i++)
	{
		HASH_TABLE[i] = 255;
	}
	for(i=0;i<Can_Data_Num;i++)
	{
		HASH_TABLE[Can_Database[i].Data_ID] = i;
	}
}

void Write_Database(ID_NUMDEF ID_NUM)
{
	uint8_t j;
	CanTxMsg TxMessage;
	/* Check the parameters */
	if((HASH_TABLE[ID_NUM] >= Can_Data_Num)&&(Can_Database[HASH_TABLE[ID_NUM]].Data_type!=READ_ONLY))
	{
		return;
	}
	
	TxMessage.StdId=Can_Database[HASH_TABLE[ID_NUM]].Data_ID;
	TxMessage.ExtId=0;
	TxMessage.IDE=CAN_ID_STD;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.DLC=Can_Database[HASH_TABLE[ID_NUM]].Data_length;
	
	for(j=0;j<Can_Database[HASH_TABLE[ID_NUM]].Data_length;j++)
	{
		TxMessage.Data[j]=(*(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+j));
	}
	
	Can_SendData(&TxMessage);
	
}
