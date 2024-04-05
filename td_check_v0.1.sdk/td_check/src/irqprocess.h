/*
 * IrqConfig.h
 *
 *  Created on: 2023年2月17日
 *      Author: 15993
 */

#ifndef SRC_IRQPROCESS_H_
#define SRC_IRQPROCESS_H_

#include "xscutimer.h"
#include "xscugic.h"
/*********************************************/
//全局变量
XScuGic 		 					scugic_inst;				//中断控制器  驱动实例
XScuGic_Config   					*scugic_cfg_ptr; 			//中断控制器  配置信息

#define SCUGIC_ID					XPAR_SCUGIC_0_DEVICE_ID		//中断控制器 ID
#define RtAddr_ID 					62							//rt地址中断 ID
#define GPIO_ID 					63							//gpio中断 ID
#define Rt_Message_ID 				64							//rtMessage中断ID
#define Adc_ID 						65							//adc中断ID

#define rtMessageRegBaseAddr		0x40000000					//rtMessage状态寄存器基地址
#define rtAddrRegBaseAddr			0x40003000					//rtAddr状态寄存器基地址
#define adcRegBaseAddr				0x40001000					//ADC状态寄存器基地址
#define axiGpioRegBaseAddr			0x40002000					//axiGpio状态寄存器基地址

int ad_value[19];

typedef struct{
	u32 adcIrqFilter;
	u32 rtAddrIrqFilter;
	u32 rtMessageIrqFilter;
	u32 axiGpioIrqFilter;
}IrqFilter;
IrqFilter irqFilter;


typedef struct {
	int adc;
	int rtAddr;
	int rtMessage;
	int axiGpio;
	int fc1553;
	int timer;
	int uart;
}IrqFlag;
IrqFlag irqFlag;


int  irq_init(void);
void rtAddr_handler(void *CallbackRef);
void axi_gpio_handler(void *CallbackRef);
void rtMessage_handler(void *CallbackRef);
void adcIrq_handler(void *CallbackRef);
void fc1553Irq_handler(void *CallbackRef);
void adcIrqProcess(void);
void rtMessageIrqProcess(void);
void rtAddrIrqProcess(void);
void axiGpioIrqProcess(void);
void irqProcess(void);

#endif /* SRC_IRQPROCESS_H_ */
