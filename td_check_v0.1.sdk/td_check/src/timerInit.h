/*
 * timerInit.h
 *
 *  Created on: 2023��2��17��
 *      Author: 15993
 */

#ifndef SRC_TIMERINIT_H_
#define SRC_TIMERINIT_H_

#include "xscutimer.h"

XScuTimer 		 Timer;            						//��ʱ����������ʵ��
#define TIMER_DEVICE_ID     XPAR_XSCUTIMER_0_DEVICE_ID  //��ʱ��ID
#define TIMER_IRPT_INTR     XPAR_SCUTIMER_INTR          //��ʱ���ж�ID

//˽�ж�ʱ����ʱ��Ƶ�� = CPUʱ��Ƶ��/2 = 333MHz
//0.2s��˸һ��,0.2*1000_000_000/(1000/333) - 1 = 3F83C3F
//0.2s��˸һ��,(ns)*1000_000_000/(1000/333) - 1 = 3F83C3F  //��ʱ����ʱns
#define TIMER_LOAD_VALUE    0x3F83C3F                      //��ʱ��װ��ֵ
#define TIMER_LOAD_Value    1000000000/(1000/333)

typedef struct{
	char irqFlag;
	int  count;
}TIMER;
TIMER timer;

int  timer_init(XScuTimer *timer_ptr);
void timer_intr_handler(void *CallBackRef);
void timerSend(u32 *buf);
void timerProcess(void);

#endif /* SRC_TIMERINIT_H_ */
