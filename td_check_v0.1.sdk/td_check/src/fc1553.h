/*
 * fc1553.h
 *
 *  Created on: 2023Äê2ÔÂ20ÈÕ
 *      Author: 15993
 */

#ifndef SRC_FC1553_H_
#define SRC_FC1553_H_
#include <fc1553lib/drvFC1553.h>

//#define Fc1553START

unsigned short 	linkStatus[4];

char errorIrqFlag;
unsigned char fcSendBuf[1024*64];
unsigned char fcRecvBuf[1024*64];

struct ST_drvFcDevNtConfig  ntconfig;
struct ST_drvFcNtMsgCfg 	msgconfig;
struct ST_drvFcNtMsgStatus  msgntstatus;
struct ST_drvFcNtMsgStatus  msgfcstatus;

void fc1553_Init(void);
void fc1553_Send(unsigned char * data,int length,unsigned short subaddr);
void fc1553_Recv(unsigned char * data,int length);
void fcParameterRead(void);

#endif /* SRC_FC1553_H_ */
