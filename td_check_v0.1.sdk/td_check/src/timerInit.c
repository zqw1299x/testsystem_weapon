/*
 * timerInit.c
 *
 *  Created on: 2023��2��17��
 *      Author: 15993
 */

#include <irqprocess.h>
#include "xscutimer.h"
#include "timerInit.h"
#include <stdio.h>
#include "fc1553.h"

//��ʱ���жϴ������
void timer_intr_handler(void *CallBackRef)
{
    XScuTimer *timer_ptr = (XScuTimer *) CallBackRef;
    irqFlag.timer = 1;
    //�����ʱ���жϱ�־
    XScuTimer_ClearInterruptStatus(timer_ptr);
}

//��ʱ����ʼ������
int timer_init(XScuTimer *timer_ptr)
{
    int status;
    //˽�ж�ʱ����ʼ��
    XScuTimer_Config *timer_cfg_ptr;
    timer_cfg_ptr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
    if (NULL == timer_cfg_ptr)
        return XST_FAILURE;
    status = XScuTimer_CfgInitialize(timer_ptr, timer_cfg_ptr,
            timer_cfg_ptr->BaseAddr);
    if (status != XST_SUCCESS)
        return XST_FAILURE;

    XScuTimer_LoadTimer(timer_ptr, TIMER_LOAD_VALUE);  		// ���ؼ�������
    XScuTimer_LoadTimer(timer_ptr, TIMER_LOAD_Value*1-1);	// ��ʱ1s�ж�
    XScuTimer_EnableAutoReload(timer_ptr);             		// �����Զ�װ��ģʽ

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
