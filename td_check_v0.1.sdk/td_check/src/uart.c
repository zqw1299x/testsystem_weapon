/*
 * uart.c
 *
 *  Created on: 2023年3月29日
 *      Author: 15993
 */

#include <irqprocess.h>
#include "xparameters.h"
#include "xuartps.h"
#include "xil_printf.h"
#include "stdio.h"
#include "uart.h"
#include "common.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "sleep.h"

//UART初始化函数
int uart_init(XUartPs* uart_ps)
{
    int status;
    XUartPs_Config *uart_cfg;

    uart_cfg = XUartPs_LookupConfig(UART_DEVICE_ID);
    if (NULL == uart_cfg)return XST_FAILURE;
    status = XUartPs_CfgInitialize(uart_ps, uart_cfg, uart_cfg->BaseAddress);
    if (status != XST_SUCCESS)return XST_FAILURE;

    //UART设备自检
    status = XUartPs_SelfTest(uart_ps);
    if (status != XST_SUCCESS)return XST_FAILURE;

    //设置工作模式:正常模式
    XUartPs_SetOperMode(uart_ps, XUARTPS_OPER_MODE_NORMAL);
    //设置波特率:115200
    XUartPs_SetBaudRate(uart_ps,115200);
    //设置RxFIFO的中断触发等级
    XUartPs_SetFifoThreshold(uart_ps, 1);

    return XST_SUCCESS;
}

//UART中断处理函数
void uart_intr_handler(void *call_back_ref)
{
    XUartPs *uart_instance_ptr = (XUartPs *) call_back_ref;
    u32 rec_data = 0 ;
    u32 isr_status ;                           //中断状态标志
    recvdata = 0xff;

    //读取中断ID寄存器，判断触发的是哪种中断
    isr_status = XUartPs_ReadReg(uart_instance_ptr->Config.BaseAddress,XUARTPS_IMR_OFFSET);
    isr_status &= XUartPs_ReadReg(uart_instance_ptr->Config.BaseAddress,XUARTPS_ISR_OFFSET);

    //判断中断标志位RxFIFO是否触发
    if (isr_status & (u32)XUARTPS_IXR_RXOVR){
        rec_data = XUartPs_RecvByte(XPAR_PS7_UART_0_BASEADDR);
        strcat((char*)rxbuf,(char*)&rec_data);
        irqFlag.uart = 1;
        //清除中断标志
        XUartPs_WriteReg(uart_instance_ptr->Config.BaseAddress,XUARTPS_ISR_OFFSET, XUARTPS_IXR_RXOVR) ;
    }
    //XUartPs_SendByte(XPAR_PS7_UART_0_BASEADDR,rec_data);
}

void uart_plEmioControl(void)
{
	char portID[4];
	char ioCMD[2];
	usleep(1000*100);
	if(strstr((char*)rxbuf,"cort")){		//IO指令控制
		printf("rxbuf:%s\r\n",(char*)rxbuf);
		strncpy(ioCMD,(char*)rxbuf+4,2);
		//printf("ioCMD:%d\r\n",atoi(ioCMD));
		recvdata = char_2_hex((uint8_t*)ioCMD,2);
		printf("cmd:%02lx\r\n",recvdata);
		memset(rxbuf,0,1024);
	}else if(strstr((char*)rxbuf,"port")){
		printf("rxbuf:%s\r\n",(char*)rxbuf);
		strncpy(portID,(char*)rxbuf+4,4);
		//fcPortID = atoi(portID);
		fcPortID = char_2_hex((uint8_t*)portID,4);
		printf("writProtID:%0x\r\n",fcPortID);
		#ifdef FC1553START
		WriteReg(0x6014000c,fcPortID);		//设置NT地址
		printf("readProtID:%02lx\r\n",ReadReg(0x6014000c));
		#endif
		memset(rxbuf,0,1024);
	}
	switch(recvdata)
	{
		case 0x00:regValueSet(plEmio,0,0);
			break;
		case 0x01:regValueSet(plEmio,0,1);
			break;
		case 0x10:regValueSet(plEmio,1,0);
			break;
		case 0x11:regValueSet(plEmio,1,1);
			break;

		case 0x20:regValueSet(plEmio,2,0);
			break;
		case 0x21:regValueSet(plEmio,2,1);
			break;
		case 0x30:regValueSet(plEmio,3,0);
			break;
		case 0x31:regValueSet(plEmio,3,1);
			break;

		case 0x40:regValueSet(plEmio,4,0);
			break;
		case 0x41:regValueSet(plEmio,4,1);
			break;
		case 0x50:regValueSet(plEmio,5,0);
			break;
		case 0x51:regValueSet(plEmio,5,1);
			break;

		case 0x60:regValueSet(plEmio,6,0);
			break;
		case 0x61:regValueSet(plEmio,6,1);
			break;
		case 0x70:regValueSet(plEmio,7,0);
			break;
		case 0x71:regValueSet(plEmio,7,1);
			break;

		case 0x80:regValueSet(plEmio,8,0);
			break;
		case 0x81:regValueSet(plEmio,8,1);
			break;
		case 0x90:regValueSet(plEmio,9,0);
			break;
		case 0x91:regValueSet(plEmio,9,1);
			break;

		case 0xa0:regValueSet(plEmio,10,0);
			break;
		case 0xa1:regValueSet(plEmio,10,1);
			break;
		case 0xb0:regValueSet(plEmio,11,0);
			break;
		case 0xb1:regValueSet(plEmio,11,1);
			break;
		default: break;
	}
	recvdata = 0xff;
}
