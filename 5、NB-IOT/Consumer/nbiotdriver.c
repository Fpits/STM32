#include "nbiotdriver.h"
#include "usart.h"
#include "string.h"
#include "time.h"
#include "led.h"


static uint8_t CurrentRty;
extern tsUsart2type Usart2type;
static teATCmdNum  ATRecvCmdNum;
static teATCmdNum  ATCurrentCmdNum;
static teATCmdNum  ATNextCmdNum;
static tsTimeType TimeNB;
static teNB_TaskStatus NB_TaskStatus;

tsATCmds ATCmds[] = 
{
	{"AT+CIPMODE=1\r\n","OK",2000,NO_RECV,3},
	{"AT+CIPSTART='TCP','60.166.18.9',7500\r\n","OK",2000,NO_RECV,3},
	{"AT+CFUN=0\r\n","OK",2000,NO_RECV,3},
	{"AT+CFUN=0\r\n","OK",2000,NO_RECV,3},
	{"AT+CFUN=0\r\n","OK",2000,NO_RECV,3},
	{"AT+CFUN=0\r\n","OK",2000,NO_RECV,3},
	{"AT+CFUN=0\r\n","OK",2000,NO_RECV,3},
	{"AT+CFUN=0\r\n","OK",2000,NO_RECV,3},
};


void ATSend(teATCmdNum ATCmdNum)
{
	//清空接收缓存区
	memset(Usart2type.Usart2RecvBuff,0,USART2_RECV_SIZE);
	
	ATCmds[ATCmdNum].ATStatus = NO_RECV;
	
	ATRecvCmdNum = ATCmdNum;
	
	HAL_UART_Transmit(&huart2,(uint8_t *)ATCmds[ATCmdNum].ATSendString,strlen(ATCmds[ATCmdNum].ATSendString),0xfff);
	HAL_UART_Transmit(&huart1,(uint8_t *)ATCmds[ATCmdNum].ATSendString,strlen(ATCmds[ATCmdNum].ATSendString),0xfff);
	//打开超时定时器
	SetTime(&TimeNB,ATCmds[ATCmdNum].TimeOut);
	
	//打开发送指示灯
	SetLedRun(LED_TX);
}

void ATRecv(void)
{
	if(Usart2type.Usart2RecvFlag == 1)
	{
		if(strstr((const char *)Usart2type.Usart2RecvBuff,(const char *)ATCmds[ATRecvCmdNum].ATRecvString) != NULL)
		{
			ATCmds[ATRecvCmdNum].ATStatus = SUCCESS_RECV;
		}
		
		SetLedRun(LED_RX);
		HAL_UART_Transmit(&huart1,Usart2type.Usart2RecvBuff,Usart2type.Usart2RecvLen,0xfff);
		Usart2type.Usart2RecvFlag = 0;
		Usart2type.Usart2RecvLen = 0;
		printf("hello\r\n");
	}
}


void NB_Init(void)
{
	NB_TaskStatus = NB_SEND;
	ATCurrentCmdNum = AT_CFUN1;
	ATNextCmdNum = AT_CFUN1;
}



void NB_Task(void)
{
	while(1)
	{
		switch(NB_TaskStatus)
		{
			case NB_IDLE:
				return;
			case NB_SEND:
					if(ATCurrentCmdNum != ATNextCmdNum)
					{
						CurrentRty = ATCmds[ATCurrentCmdNum].RtyNum;
					}
					ATSend(ATCurrentCmdNum);
					NB_TaskStatus = NB_WAIT;
				return;
			case NB_WAIT:
					ATRecv();
					if(ATCmds[ATCurrentCmdNum].ATStatus == SUCCESS_RECV)
					{
						if(ATCurrentCmdNum == AT_CFUN0)
						{
							NB_TaskStatus = NB_ACCESS;
							break;
						}
						else
						{
							ATCurrentCmdNum += 1;
							ATNextCmdNum = ATCurrentCmdNum + 1;
							NB_TaskStatus = NB_SEND;
							break;
						}
					}
					else if(CompareTime(&TimeNB))
					{
						ATCmds[ATCurrentCmdNum].ATStatus = TIME_OUT;
						if(CurrentRty > 0)
						{
							CurrentRty--;
							ATNextCmdNum = ATCurrentCmdNum;
							NB_TaskStatus = NB_SEND;
							break;
						}
						else 
						{
							NB_Init();
							return;
						}
						
					}
				return;
			case NB_ACCESS:
					LedOn(LED_NET);
					NB_TaskStatus = NB_IDLE;
				break;
			default:
				break;
				
		}
	}
}








