/*
 * main.cd
 *
 * Created on: 2023年3月1日
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
	plIoInitValue   = 0x00000c00;               //连锁1、连锁2、自闭和、td锁定、安保恢复置0.电池激活,弹动点火置0其对应的电压控制置1
	WriteReg(plEmio,plIoInitValue);  			//axi_gpio输出寄存器中写入复位值
    irq_init();									//rt_message,rt_addr,axi_gpio,adc,timer,uart中断初始化
	ls1553_init();								//ls1553参数初始化
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

