/*
 * IrqConfig.h
 *
 *  Created on: 2023��2��17��
 *      Author: 15993
 */

#ifndef SRC_IRQPROCESS_H_
#define SRC_IRQPROCESS_H_

#include "xscutimer.h"
#include "xscugic.h"
/*********************************************/
//ȫ�ֱ���
XScuGic 		 					scugic_inst;				//�жϿ�����  ����ʵ��
XScuGic_Config   					*scugic_cfg_ptr; 			//�жϿ�����  ������Ϣ

#define SCUGIC_ID					XPAR_SCUGIC_0_DEVICE_ID		//�жϿ����� ID
#define RtAddr_ID 					62							//rt��ַ�ж� ID
#define GPIO_ID 					63							//gpio�ж� ID
#define Rt_Message_ID 				64							//rtMessage�ж�ID
#define Adc_ID 						65							//adc�ж�ID

#define rtMessageRegBaseAddr		0x40000000					//rtMessage״̬�Ĵ�������ַ
#define rtAddrRegBaseAddr			0x40003000					//rtAddr״̬�Ĵ�������ַ
#define adcRegBaseAddr				0x40001000					//ADC״̬�Ĵ�������ַ
#define axiGpioRegBaseAddr			0x40002000					//axiGpio״̬�Ĵ�������ַ

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
