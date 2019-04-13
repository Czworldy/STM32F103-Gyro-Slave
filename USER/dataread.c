#include "dataread.h"

struct STime	stcTime[2];
struct SAcc 	stcAcc[2];
struct SGyro 	stcGyro[2];
struct SAngle 	stcAngle[2];
struct SMag 	stcMag[2];
struct SDStatus stcDStatus[2];
struct SPress 	stcPress[2];
struct SLonLat 	stcLonLat[2];
struct SGPSV 	stcGPSV[2];
struct SQ       stcQ[2];

float Angleyaw;

unsigned char ucRxBuffer0[250];
unsigned char ucRxBuffer1[250];

void ReadGyroData0(void)
{
		switch(ucRxBuffer0[1])//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
		{
			case 0x50:	memcpy(&stcTime[0],&ucRxBuffer0[2],8);break;//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
			case 0x51:	memcpy(&stcAcc[0],&ucRxBuffer0[2],8);break;
			case 0x52:	memcpy(&stcGyro[0],&ucRxBuffer0[2],8);break;
			case 0x53:	memcpy(&stcAngle[0],&ucRxBuffer0[2],8);break;
			case 0x54:	memcpy(&stcMag[0],&ucRxBuffer0[2],8);break;
			case 0x55:	memcpy(&stcDStatus[0],&ucRxBuffer0[2],8);break;
			case 0x56:	memcpy(&stcPress[0],&ucRxBuffer0[2],8);break;
			case 0x57:	memcpy(&stcLonLat[0],&ucRxBuffer0[2],8);break;
			case 0x58:	memcpy(&stcGPSV[0],&ucRxBuffer0[2],8);break;
			case 0x59:	memcpy(&stcQ[0],&ucRxBuffer0[2],8);break;
		}
}
void ReadGyroData1(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=ucData;	//将收到的数据存入缓冲区中
	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//数据不满11个，则返回
	else
	{
		switch(ucRxBuffer[1])//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
		{
			case 0x50:	memcpy(&stcTime[1],&ucRxBuffer[2],8);break;//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
			case 0x51:	memcpy(&stcAcc[1],&ucRxBuffer[2],8);break;
			case 0x52:	memcpy(&stcGyro[1],&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle[1],&ucRxBuffer[2],8);break;
			case 0x54:	memcpy(&stcMag[1],&ucRxBuffer[2],8);break;
			case 0x55:	memcpy(&stcDStatus[1],&ucRxBuffer[2],8);break;
			case 0x56:	memcpy(&stcPress[1],&ucRxBuffer[2],8);break;
			case 0x57:	memcpy(&stcLonLat[1],&ucRxBuffer[2],8);break;
			case 0x58:	memcpy(&stcGPSV[1],&ucRxBuffer[2],8);break;
			case 0x59:	memcpy(&stcQ[1],&ucRxBuffer[2],8);break;
		}
		ucRxCnt=0;//清空缓存区
		
	}
}

