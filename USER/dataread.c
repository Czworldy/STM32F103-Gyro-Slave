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
		switch(ucRxBuffer0[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
		{
			case 0x50:	memcpy(&stcTime[0],&ucRxBuffer0[2],8);break;//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����
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
	
	ucRxBuffer[ucRxCnt++]=ucData;	//���յ������ݴ��뻺������
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
		{
			case 0x50:	memcpy(&stcTime[1],&ucRxBuffer[2],8);break;//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����
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
		ucRxCnt=0;//��ջ�����
		
	}
}

