/*
 * timerInit.c
 *
 *  Created on: 2023年2月17日
 *      Author: 15993
 */

#include <irqprocess.h>
#include "xscutimer.h"
#include "timerInit.h"
#include <stdio.h>
#include "fc1553.h"

//定时器中断处理程序
void timer_intr_handler(void *CallBackRef)
{
    XScuTimer *timer_ptr = (XScuTimer *) CallBackRef;
    irqFlag.timer = 1;
    //清除定时器中断标志
    XScuTimer_ClearInterruptStatus(timer_ptr);
}

//定时器初始化程序
int timer_init(XScuTimer *timer_ptr)
{
    int status;
    //私有定时器初始化
    XScuTimer_Config *timer_cfg_ptr;
    timer_cfg_ptr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
    if (NULL == timer_cfg_ptr)
        return XST_FAILURE;
    status = XScuTimer_CfgInitialize(timer_ptr, timer_cfg_ptr,
            timer_cfg_ptr->BaseAddr);
    if (status != XST_SUCCESS)
        return XST_FAILURE;

    XScuTimer_LoadTimer(timer_ptr, TIMER_LOAD_VALUE);  		// 加载计数周期
    XScuTimer_LoadTimer(timer_ptr, TIMER_LOAD_Value*1-1);	// 定时1s中断
    XScuTimer_EnableAutoReload(timer_ptr);             		// 设置自动装载模式

    return XST_SUCCESS;
}

void timerProcess(void)
{
	timer.count++;
	if(timer.count>=3){
		printf("timer test cycle 3/s:%d\r\n",timer.count);
		timer.count=0;
		#ifdef Fc1553START
		for(int i=0;i<128;i++)fcSendBuf[i] = 0xd7;
		fc1553_Send(fcSendBuf,128,0x01);
		errorIrqFlag = 1;
		#endif
	}
}
