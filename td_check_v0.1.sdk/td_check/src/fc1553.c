/*
 * fc1553.c
 *
 *  Created on: 2023年2月20日
 *      Author: 15993
 */
#include <fc1553lib/calcCheckSum.h>
#include <fc1553lib/drvFC1553.h>
#include <fc1553lib/drvSP.h>
#include "fc1553.h"
#include "string.h"
#include "platform.h"
#include "timerInit.h"
#include "common.h"
#include "uart.h"
/*****************scope***********************/
void fc1553_Init(void)
{
	int status = 0;
	ntconfig.m_csTOV 		  = 0x2580;
	ntconfig.m_csdRxTOV 	  = 0x1388;
	ntconfig.m_linkSpeed	  = 1;
	ntconfig.m_ntSubAddrPlot1 = 0xffffffff;
	ntconfig.m_ntSubAddrPlot2 = 0xffffffff;
	ntconfig.m_ntSubAddrPlot3 = 0xffffffff;
	ntconfig.m_ntSubAddrPlot4 = 0xffffffff;
	/****************************/
	fcParameterRead();
	/************************************************/
	status = drvFcInit();							 //Fc driver Init
	if(status >= 0)printf("display sign ID:%d\r\n",status);
	else{
		printf("FC Init error status:%d\r\n",status);
	}
	printf("value:%02lx\r\n",ReadReg(0x6014000c));
	/****************************/
	status = ReadReg(0x6014000c);
	if(status == 0x00000102)printf("display sign ID:%d\r\n",status);
	/************************************************/
	status = drvFcStart((unsigned char *)&ntconfig);  //Fc Start Parameter binding
	if(status >= 0)printf("device start okay -- status:%d\r\n",status);
	else{
		printf("device start fail -- status:%d\r\n",status);
	}
	/************************************************/
	status = drvFcGetLinkStatus(linkStatus);		  //get the status of fc link
	if(status >= 0){
		printf("status ask okay -- linkStatus:%0x\r\n",linkStatus[0]);
		if(linkStatus[0] != 0){
			printf("way A connect fail\r\n");
			return;
		}
	}else {
		printf("linkStatus ask error -- status:%d\r\n",status);
	}
}

void fc1553_Send(unsigned char *data,int length,unsigned short subaddr)
{
	int status = 0;
	msgconfig.m_ntSubAddr = subaddr;
	msgconfig.m_modeCode  = 0x10;						//子地址为0时有效
	/****************************/
	status = drvFcGetLinkStatus(linkStatus);			//get the status of fc link
	if(status >= 0){
		printf("linkStatus ask okay -- linkStatus:%0x\r\n",linkStatus[0]);//bit0 = 0代表A路正常，bit0 = 1代表A路异常
		if(linkStatus[0] != 0){
			printf("way A connect fail\r\n");
			return;
		}
	}else {
		printf("linkStatus ask error -- status:%d\r\n",status);
	}
	/************************************************/
	status = drvFcSend((unsigned char *)&msgconfig,data,128);
	if(status >= 0)printf("send okay -- status:%d\r\n",status);
	else {
		printf("send error -- status:%d\r\n",status);
	}
	/************************************************/
	status = drvFcSendCheck(&msgntstatus);
	if(msgntstatus.m_dataLen > 0){
		printf("msgNtstatus.m_ntSubAddr:%0x------msgNtstatus.m_dataLen:%0x\r\n",msgntstatus.m_ntSubAddr,msgntstatus.m_dataLen);
	}else {
		printf("sendCheck status ask error -- status:%d\r\n",status);
	}
}

void fc1553_Recv(unsigned char *data,int length)
{
	int status = 0;
	int i=0;
	memset(&msgfcstatus,0,sizeof(msgfcstatus));
	status = drvFcRecv(data,length,(unsigned char *)&msgfcstatus);//发送接收长度如何定义的，可否更改，最大是多少
	if(msgfcstatus.m_dataLen > 0){
		printf("1553 recv data okay -- status:%d\r\n",status);
		for(i=0;i<msgfcstatus.m_dataLen;i++)printf("%d ",data[i]);
		printf("\r\n");
		printf("msgFcstatus.m_ntSubAddr:%0x------msgFcstatus.m_dataLen:%0x\r\n",msgfcstatus.m_ntSubAddr,msgfcstatus.m_dataLen);
	}else {
			if(errorIrqFlag == 1){
				printf("recv data error -- status:%d\r\n",status);
				errorIrqFlag=0;
			}
	}
}

void fcParameterRead(void)
{
    int value,value1,value2,value3,value4;
	value = ReadReg(0x6010d000);
	printf("value1:%0x\r\n",value);

	value1 = ReadReg(0x600251dc);
	printf("value2:%0x\r\n",value1);

	value2 = ReadReg(0x6002006c);
	printf("value3:%0x\r\n",value2);

	value3 = ReadReg(0x60101000);
	printf("value4:%0x\r\n",value3);

	WriteReg(0x6014000c,0x00000102);//设置NT地址
	value4 = ReadReg(0x6014000c);
	printf("value5:%0x\r\n",value4);
	/********************************************/
}
