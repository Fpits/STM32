#ifndef __NBIOT_H_
#define __NBIOT_H_
#include "stm32f1xx.h"




typedef enum
{
	SUCCESS_RECV = 0,
	TIME_OUT,
	NO_RECV
}teATStatus;	//AT接收状态


typedef enum
{
	NB_IDLE = 0,
	NB_SEND,
	NB_WAIT,
	NB_ACCESS
}teNB_TaskStatus;

typedef enum
{
	AT_CFUN0 = 0,
	AT_CFUN1,
	AT_CFUN2,
	AT_CFUN3,
	AT_CFUN4,
	AT_CFUN5,
	AT_CFUN6,
	AT_CFUN7,
}teATCmdNum;	//AT编号


typedef struct
{
	char *ATSendString;
	char *ATRecvString;
	uint16_t TimeOut;
	teATStatus ATStatus;
	uint8_t RtyNum;
}tsATCmds;	//AT指令结构体

void ATSend(teATCmdNum ATCmdNum);
void ATRecv(void);
void NB_Init(void);
void NB_Task(void);


#endif
