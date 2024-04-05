/*
 * uart.h
 *
 *  Created on: 2023年3月29日
 *      Author: 15993
 */

#ifndef SRC_UART_H_
#define SRC_UART_H_

#include "xuartps.h"
#include "xuartps_hw.h"
#include "xscugic.h"
#include "xparameters.h"

#define UART_DEVICE_ID     XPAR_PS7_UART_0_DEVICE_ID    //串口设备ID
#define UART_INT_IRQ_ID    XPAR_XUARTPS_0_INTR          //串口中断ID

XScuGic Intc;              //中断控制器驱动程序实例
XUartPs Uart_Ps;           //串口驱动程序实例

u32 recvdata;
u8  rxbuf[1024];
int fcPortID;

int uart_init(XUartPs* uart_ps);
void uart_intr_handler(void *call_back_ref);
void uart_plEmioControl(void);

#endif /* SRC_UART_H_ */
