/*
 * uart.h
 *
 *  Created on: 2023��3��29��
 *      Author: 15993
 */

#ifndef SRC_UART_H_
#define SRC_UART_H_

#include "xuartps.h"
#include "xuartps_hw.h"
#include "xscugic.h"
#include "xparameters.h"

#define UART_DEVICE_ID     XPAR_PS7_UART_0_DEVICE_ID    //�����豸ID
#define UART_INT_IRQ_ID    XPAR_XUARTPS_0_INTR          //�����ж�ID

XScuGic Intc;              //�жϿ�������������ʵ��
XUartPs Uart_Ps;           //������������ʵ��

u32 recvdata;
u8  rxbuf[1024];
int fcPortID;

int uart_init(XUartPs* uart_ps);
void uart_intr_handler(void *call_back_ref);
void uart_plEmioControl(void);

#endif /* SRC_UART_H_ */
