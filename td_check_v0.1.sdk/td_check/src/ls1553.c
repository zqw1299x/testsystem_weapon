/*
 * ls1553.c
 *
 *  Created on: 2023年2月17日
 *      Author: 15993
 */
#include "ls1553.h"
#include "string.h"
#include "xil_printf.h"
#include "platform.h"
#include <stdio.h>
#include "common.h"
#include "sleep.h"

/*********************************************************/
void ls1553_init(void)
{
	u32 version = 0;

	rtTxBuff.BaseAddress = 0x50002000;					//Init rt_message发送缓存区基地址
	rtRxBuff.BaseAddress = 0x50000000;					//Init rt_message接收缓存区基地址
	WriteReg(0x40000000+0x04,rtTxBuff.BaseAddress); 	//将发送缓存区基地址写入寄存器 装订
	WriteReg(0x40000000+0x08,rtRxBuff.BaseAddress); 	//将接收缓存区基地址写入寄存器 装订

	WriteReg(0x40000000+0x10,0x00000021);   			//rt装订寄存器rt_ok置位1，复位，设置rtAddr为5
	version = ReadReg(0x40000000+0x00);					//读取ls1553 IP核版本号
	printf("ls1553 ip ver:%lx\r\n",version);

	vectorInitValue = 0x00;
	WriteReg(vectorCtl,vectorInitValue);				//矢量字寄存器写入复位值

	memset((char*)&adcValue,0,sizeof(adcValue));		//初始化adcValue
	adcValue.head = 0x6d6e;								//ascall码 M,N

	rt_messageUpdate_A5();
	rt_messageUpdate_A6();
}

void rt_messageInit_BC2RT(void)
{
	int sumbuf[30],clearbuf[24]={0x00000000};
	int i=0;
	/*************************************************************///A.2 BC-RT
	lspd1553a2.head 					= 0x0400;
	lspd1553a2.invalidValueL 			= 0x00000000;
	lspd1553a2.invalidValueH 			= 0x00000000;
	lspd1553a2.weaponKeyStatusContr1 	= 0x00;
	lspd1553a2.keyStatus1KeyAcess    	= 0x00;
	lspd1553a2.weaponKeyStatusContr2 	= 0x00;
	lspd1553a2.keyStatus2KeyAcess    	= 0x00;
	lspd1553a2.checksum 				= 0;
	/*******************************/
	memcpy(lspd1553a2.back,clearbuf,18);
	memcpy(lspd1553a2.reservDataWord,clearbuf,6);
	memcpy(sumbuf,&lspd1553a2,sizeof(lspd1553a2));
	for(i=0;i<sizeof(lspd1553a2)/4-1;i++)sumbuf[29]+=sumbuf[i];
	lspd1553a2.checksum =sumbuf[29];
	for(i=0;i<sizeof(sumbuf);i++)sumbuf[i]=0;
	/*************************************************************///A.4 BC-RT
	lspd1553a4.head 					= 0x0421;
	lspd1553a4.countryCode 				= 0x434e;
	lspd1553a4.platformWordHex 			= 0x00;
	lspd1553a4.checksum 				= 0x00;
	/*******************************/
	memcpy(lspd1553a4.platformWordAscall,clearbuf,8);
	memcpy(lspd1553a4.reservDataWord,clearbuf,18);
	for(i=0;i<sizeof(lspd1553a4)/4-1;i++)sumbuf[29]+=sumbuf[i];
	lspd1553a4.checksum =sumbuf[29];
	for(i=0;i<sizeof(sumbuf);i++)sumbuf[i]=0;
	/*************************************************************///数据装订消息帧(BC-RT)
	lspd1553a8.head = 0x0070;
	memcpy(lspd1553a8.data,clearbuf,30);
	lspd1553a8.checksum = 0;
	for(i=0;i<sizeof(lspd1553a8)/4-1;i++)sumbuf[29]+=sumbuf[i];
	lspd1553a8.checksum =sumbuf[29];
	for(i=0;i<sizeof(sumbuf);i++)sumbuf[i]=0;
	/************************************************************/
}

void rt_messageInit_RT2BC(void)
{
	int sumbuf[30],clearbuf[24]={0x00000000};
	int i=0;
	/*************************************************************///A.3 RT-BC
	lspd1553a3.head 					= 0x0420;
	lspd1553a3.invalidValueL 			= 0x00000000;
	lspd1553a3.invalidValueH 			= 0x00000000;
	lspd1553a3.keyMonitor1 				= 0x00;
	lspd1553a3.keyMonitor2 				= 0x00;
	lspd1553a3.checksum 				= 0x00;
	/*******************************/
	memcpy(lspd1553a3.back,clearbuf,18);
	memcpy(lspd1553a3.reservDataWord,clearbuf,6);
	for(i=0;i<sizeof(lspd1553a3)/4-1;i++)sumbuf[29]+=sumbuf[i];
	lspd1553a3.checksum =sumbuf[29];
	for(i=0;i<sizeof(sumbuf);i++)sumbuf[i]=0;
	/*************************************************************///数据装订回报消息帧(RT-BC)
	lspd1553a9.head = 0x0070;
	memcpy(lspd1553a9.data,clearbuf,30);
	lspd1553a9.checksum = 0;
	for(i=0;i<sizeof(lspd1553a9)/4-1;i++)sumbuf[29]+=sumbuf[i];
	lspd1553a9.checksum =sumbuf[29];
	for(i=0;i<sizeof(sumbuf);i++)sumbuf[i]=0;
	/************************************************************///数据接收回报消息帧(RT-BC)
	lspd1553a10.head = 0x0070;
	memcpy(lspd1553a10.data,clearbuf,30);
	lspd1553a10.checksum = 0;
	for(i=0;i<sizeof(lspd1553a10)/4-1;i++)sumbuf[29]+=sumbuf[i];
	lspd1553a10.checksum =sumbuf[29];
	for(i=0;i<sizeof(sumbuf);i++)sumbuf[i]=0;
	/************************************************************/
}

void rt_messageUpdate_A5(void)//武器描述信息(RT-BC) addr5
{
	lspd1553a5.head 		= 0x0421;
	lspd1553a5.gjycode 		= 0x434E;
	lspd1553a5.wqbs    		= 0x0010;
	lspd1553a5.zbNum[0]		= 0x5459;		//TY
	lspd1553a5.zbNum[1]		= 0x4d4e;		//MN
	lspd1553a5.zbNum[2]		= 0x5120;		//Q+空
	lspd1553a5.zbNum[3]		= 0x2020;		//0x2020
	lspd1553a5.zbNum[4]		= 0x2020;
	lspd1553a5.zbNum[5]		= 0x2020;
	lspd1553a5.zbNum[6]		= 0x2020;
	lspd1553a5.zbNum[7]		= 0x204d;		//空+M
	lspd1553a5.gnkzNum    	= 0x0000;
	lspd1553a5.yzType     	= 0x2020;
	lspd1553a5.zdtzType   	= 0x2020;
	lspd1553a5.yuliu		= 0x0000;
	lspd1553a5.year       	= 0x0000;
	lspd1553a5.pc           = 0x0000;
	lspd1553a5.wqsxNumH     = 0x0000;
	lspd1553a5.wqsxNumL     = 0x0000;
	memset((char*)lspd1553a5.back,0,sizeof(lspd1553a5.back));
	lspd1553a5.checksum		= 0xe13d;
	//start send
	rt2bc_Send((int *)&lspd1553a5,1,sizeof(lspd1553a5)/4);
}

void rt_messageUpdate_A6(void)//RT-BC addr6 上电1.5s完成
{
	lspd1553a6.head 		 = 0x0060;
	lspd1553a6.invalidValueL = 0xfff0;
	lspd1553a6.invalidValueH = 0x7fff;
	lspd1553a6.weaponCpZjJg  = 0x00ff;
	lspd1553a6.weaponZjJg 	 = 0x0000;
	lspd1553a6.batteryJhJg   = 0x0000;
	lspd1553a6.gd3JcJg		 = 0x0000;
	lspd1553a6.checksum 	 = 0x00;
	memset((char*)lspd1553a6.back,0,sizeof(lspd1553a6.back));
	lspd1553a6.checksum      = 0x203c;
	//start send
	rt2bc_Send((int *)&lspd1553a6,2,sizeof(lspd1553a6)/4);
}

void rt_messageUpdate_A7(void)//BC-RT
{
	lspd1553a7.head 		 = 0x0090;
	lspd1553a7.invalidValueL = 0x00000000;
	lspd1553a7.invalidValueH = 0x00000000;
	lspd1553a7.dcjh			 = 0x00ff;
	memset((char*)lspd1553a7.back,0,sizeof(lspd1553a7.back));
	lspd1553a7.weaponGd2  	 = 0x00ff;
	lspd1553a7.weaponGd3 	 = 0x00ff;
	lspd1553a7.dddh   		 = 0x00ff;
	lspd1553a7.yuliu		 = 0x00ff;
	lspd1553a7.checksum 	 = 0x00;
	printf("lspdLenth:%d\r\n",sizeof(lspd1553a7));
}

void rt2bc_Send(int *data,int subadress,int length)
{
	data[length-1] = check_GJB1188A((unsigned int *)data, length);
	rt_RxTx_DataWrite(rtTxBuff.BaseAddress+subadress*128,(u32 *)data,length);
	regValueSet(vectorCtl,subadress,1);
}

void rt_RxTx_DataWrite(u32 addr,u32 *data,u32 len)
{
	int i=0;

	printf("writeLength:%ld --- data:",len);
	for(i=0;i<len;i++)printf("%04lx ",data[i]);
	printf("\r\n");
	for(i=0;i<len;i++){
		WriteReg(addr+i*4, data[i]);
	}

	i=0;
}

void rt_RxTx_DataRead(u32 addr,u32 *data,u32 len)
{
	int i=0;

	//printf("readLength:%ld --- data:",len);
	for(i=0;i<len;i++){
		data[i] = ReadReg(addr+i*4);
		//printf("%lx ",data[i]);
	}//printf("\r\n");

	i=0;
}
