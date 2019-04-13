#ifndef _CAN_DATABASE_H
#define _CAN_DATABASE_H
	#include "stm32f10x.h"
	
	#ifndef NULL
	#define NULL ((void *)0)
	#endif
	
	#define fp64 double
	#define fp32  float

	/*Data_Type*/
	#define READ_ONLY  0	//���ض�������д
	#define WRITE_ONLY 1	//����д�������
	
	
/*���������Ӧ��ID���б�*/
/*
IDλ��	[	(GS:GPS Slave  MS:Motor Slave)							]
NO.7	[			0--GS			|		1--Others				]
NO.6	[	0--MOSI		|	1--MISO	|	0--MS MOSI	|	1--Others	]
NO.5~0	[	ID numbers				|	ID numbers  &  Others		]
*/
	typedef enum
	{
//B0x-xxx-xxx ����&&GPSЭ����ͨ��ID*/
	//MOSI   B00-xxx-xxx
		MISO_ANG_UP = 0x02,		//����д,��������ӹ㲥��������
		MISO_ANG_DOWN = 0x03,
		
		MISO_MR2_WAKEUP_ID = 0x01,
		MISO_MR2_RISE_ID = 0xd0,
 		MISO_CYLINDER_ID = 0xd1,
	}ID_NUMDEF;	
		
	/*���غ�GPSЭ����֮��������ʽ*/

	
	typedef struct
	{
		uint8_t  Data_type;
		ID_NUMDEF  Data_ID;
		uint8_t* Data_ptr;
		uint8_t  Data_length;
		
		//��can���߽ӵ����ݺ󣬵��õĺ���
		void (*MenuFunc)(void);			//��ں���		
		uint8_t  Channel;
		uint8_t  Fifo_num;			//�ڽ��շ�����ID���õ�fifo��
	}Can_Data;

	union fi64_to_u8     //fp64�����ݣ���int64_t������ת��u8�����ݽ���can���߷���
	{
		fp64	 f64_data;
		int64_t  i64_data;
		int32_t  i32_data[2];
		uint16_t u16_data[4];
		uint8_t  u8_data[8];
	};
	
		union u16_to_u8
	{
		uint16_t u16_data;
		uint8_t u8_data[2];
	};
	
		union fi32_to_u8     //fp32�����ݣ���int32_t������ת��u8�����ݽ���can���߷���
	{
		fp32	f32_data;
		int32_t i32_data;
		uint32_t u32_data;
		int16_t i16_data[2];
		uint8_t u8_data[4];
	};
	
	extern uint8_t Can_Data_Num;
	extern Can_Data Can_Database[];
	extern uint8_t HASH_TABLE[256];

	extern union u16_to_u8 Angle_up;
	extern union u16_to_u8 Angle_down;
	
	extern uint8_t Cylinder_Flag;
	extern uint8_t MR2_StartFlag;
	extern uint8_t MR2_RiseFlag;

	void Hash_Table_init(void);
	void Write_Database(ID_NUMDEF ID_NUM);

#endif
