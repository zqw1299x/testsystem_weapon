/*
 * main.cd
 *
 * Created on: 2023��3��1��
 *     Author: 15993
 */
/*********************************************/
#include <irqprocess.h>
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "sleep.h"
#include "common.h"
#include "ls1553.h"
#include "fc1553.h"
#include "uart.h"
/************************/
int main()
{
    init_platform();
    /***************************************/
	printf("this program version is:v1.0\r\n");
	plIoInitValue   = 0x00000c00;               //����1������2���Ապ͡�td�����������ָ���0.��ؼ���,���������0���Ӧ�ĵ�ѹ������1
	WriteReg(plEmio,plIoInitValue);  			//axi_gpio����Ĵ�����д�븴λֵ
    irq_init();									//rt_message,rt_addr,axi_gpio,adc,timer,uart�жϳ�ʼ��
	ls1553_init();								//ls1553������ʼ��
	#ifdef Fc1553START
	fc1553_Init();
	#endif
	printf("this program is running\r\n");
    /***************************************/
    while(1)
    {
    	irqProcess();
		#ifdef Fc1553START
    	fc1553_Recv(fcRecvBuf,1024);
		#endif
    }
    cleanup_platform();
	return 0;
}

