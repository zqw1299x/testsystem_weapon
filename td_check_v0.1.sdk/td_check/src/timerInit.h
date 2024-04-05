/*
 * timerInit.h
 *
 *  Created on: 2023年2月17日
 *      Author: 15993
 */

#ifndef SRC_TIMERINIT_H_
#define SRC_TIMERINIT_H_

#include "xscutimer.h"

XScuTimer 		 Timer;            						//定时器驱动程序实例
#define TIMER_DEVICE_ID     XPAR_XSCUTIMER_0_DEVICE_ID  //定时器ID
#define TIMER_IRPT_INTR     XPAR_SCUTIMER_INTR          //定时器中断ID

//私有定时器的时钟频率 = CPU时钟频率/2 = 333MHz
//0.2s闪烁一次,0.2*1000_000_000/(1000/333) - 1 = 3F83C3F
//0.2s闪烁一次,(ns)*1000_000_000/(1000/333) - 1 = 3F83C3F  //定时器定时ns
#define TIMER_LOAD_VALUE    0x3F83C3F                      //定时器装载值
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
