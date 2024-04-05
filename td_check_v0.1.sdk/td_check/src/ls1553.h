/*
 * ls1553.h
 *
 *  Created on: 2023年2月17日
 *      Author: 15993
 */

#ifndef SRC_LS1553_H_
#define SRC_LS1553_H_

#include "platform.h"
#include "timerInit.h"

typedef struct {
	int head;
	int invalidValueL;
	int invalidValueH;
	int weaponKeyStatusContr1;
	int keyStatus1KeyAcess;
	int weaponKeyStatusContr2;
	int keyStatus2KeyAcess;
	int back[17];
	int reservDataWord[5];
	int checksum;
}LowSp1553A2;
LowSp1553A2 lspd1553a2;

typedef struct {
	int head;
	int invalidValueL;
	int invalidValueH;
	int keyMonitor1;
	int keyMonitor2;
	int back[18];
	int reservDataWord[6];
	int checksum;
}LowSp1553A3;
LowSp1553A3 lspd1553a3;

typedef struct {
	int head;
	int countryCode;
	int platformWordHex;
	int platformWordAscall[8];
	int reservDataWord[18];
	int checksum;
}LowSp1553A4;
LowSp1553A4 lspd1553a4;

//rt-bc cycle send 5s/per  待用
typedef struct{
	int head;
	int gjycode;
	int wqbs;
	int zbNum[8];
	int gnkzNum;
	int yzType;
	int zdtzType;
	int yuliu;
	int year;
	int pc;
	int wqsxNumH;
	int wqsxNumL;
	int back[10];
	int checksum;
}LowSp1553A5;
LowSp1553A5 lspd1553a5;

typedef struct {
	int head;
	int invalidValueL;
	int invalidValueH;
	int weaponCpZjJg;
	int weaponZjJg;
	int batteryJhJg;
	int gd3JcJg;
	int back[24];
	int checksum;
}LowSp1553A6;
LowSp1553A6 lspd1553a6;

typedef struct {
	int head;
	int invalidValueL;
	int invalidValueH;
	int dcjh;
	int back[23];
	int weaponGd2;
	int weaponGd3;
	int dddh;
	int yuliu;
	int checksum;
}LowSp1553A7;
LowSp1553A7 lspd1553a7;

typedef struct {
	int head;
	int data[30];	//数据内容由武器控制台确定
	int checksum;
}LowSp1553A8A9A10;
LowSp1553A8A9A10 lspd1553a8;
LowSp1553A8A9A10 lspd1553a9;
LowSp1553A8A9A10 lspd1553a10;

typedef struct{
	int head;		//头

	int gd1Vot;		//供电1检测电压
	int gd2Vot;		//供电2检测电压
	int gd3Vot;		//供电3检测电压
	int tdjsVot;	//
	int tdsdVot;
	int abjcVot;
	int abhfVot;
	int dcjhVot;
	int dddhVot;

	int gd2Num;
	int gd3Num;
	int tdjsNum;
	int abjcNum;
	int dcjhNum;
	int dddhNum;

	int gd2PulseWidth;
	int gd3PulseWidth;
	int tdjsPulseWidth;
	int abjcPulseWidth;
	int dcjhPulseWidth;
	int dddhPulseWidth;

	int gd23JgTime;
	int noReGd12JgTime;
	int noReGd23JgTime;
	int noReGd34JgTime;

	int sPulseResult;

	int yuliu1;
	int yuliu2;
	int yuliu3;
	int yuliu4;

	int checksum;

}AdcValue;
AdcValue  adcValue;

void rt_messageUpdate_A5(void);
void rt_messageUpdate_A6(void);
void rt_messageUpdate_A7(void);
void rt_messageInit_RT2BC(void);
void rt_messageInit_BC2RT(void);
void rt_RxTx_DataWrite(u32 addr,u32 *data,u32 len);
void rt_RxTx_DataRead(u32 addr,u32 *data,u32 len);
void rt2bc_Send(int *data,int subadress,int length);
void ls1553_init(void);

#endif /* SRC_LS1553_H_ */
