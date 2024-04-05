/*
 * IrqConfig.c
 *
 *  Created on: 2023��2��17��
 *      Author: 15993
 */
#include <irqprocess.h>
#include "xscugic.h"
#include "sleep.h"
#include "xil_printf.h"
#include <stdio.h>
#include "timerInit.h"
#include "ls1553.h"
#include "common.h"
#include "uart.h"
/***************************************************/
//�жϳ�ʼ��
int irq_init(void)
{
	scugic_cfg_ptr = XScuGic_LookupConfig(SCUGIC_ID);
    XScuGic_CfgInitialize(&scugic_inst, scugic_cfg_ptr, scugic_cfg_ptr->CpuBaseAddress);
    /************************************************************************/
	//�ж�ID:61 rt_addr
	//�����ж����ȼ��ʹ�������(�ߵ�ƽ����)
	XScuGic_SetPriorityTriggerType(&scugic_inst, RtAddr_ID, 0xA8, 0x3);
	//�����ж�ID���жϴ�����
	XScuGic_Connect(&scugic_inst, RtAddr_ID, rtAddr_handler, &scugic_inst);
	//ʹ���ж�
	XScuGic_Enable(&scugic_inst, RtAddr_ID);

	//�ж�ID:62 axi_gpio
	//�����ж����ȼ��ʹ�������(�ߵ�ƽ����)
	XScuGic_SetPriorityTriggerType(&scugic_inst, GPIO_ID, 0xAc, 0x3);
	//�����ж�ID���жϴ�����
	XScuGic_Connect(&scugic_inst, GPIO_ID, axi_gpio_handler, &scugic_inst);
	//ʹ���ж�
	XScuGic_Enable(&scugic_inst, GPIO_ID);

	//�ж�ID:63 rt_message
	//�����ж����ȼ��ʹ�������(�ߵ�ƽ����)
	XScuGic_SetPriorityTriggerType(&scugic_inst, Rt_Message_ID, 0xA4, 0x3);
	//�����ж�ID���жϴ�����
	XScuGic_Connect(&scugic_inst, Rt_Message_ID, rtMessage_handler, &scugic_inst);
	//ʹ���ж�
	XScuGic_Enable(&scugic_inst, Rt_Message_ID);

    //�ж�ID:64 adc
    //�����ж����ȼ��ʹ�������(�ߵ�ƽ����)
    XScuGic_SetPriorityTriggerType(&scugic_inst, Adc_ID, 0xA0, 0x03);
    //�����ж�ID���жϴ�����
    XScuGic_Connect(&scugic_inst, Adc_ID, adcIrq_handler, &scugic_inst);
    //ʹ���ж�
    XScuGic_Enable(&scugic_inst, Adc_ID);

	//���ö�ʱ���ж�
	XScuGic_Connect(&scugic_inst, TIMER_IRPT_INTR,(Xil_ExceptionHandler)timer_intr_handler, &Timer);
	//ʹ��timerI	rq�ж�
	XScuGic_Enable(&scugic_inst, TIMER_IRPT_INTR);
	XScuTimer_EnableInterrupt(&Timer); //ʹ�ܶ�ʱ���ж�

    //Ϊ�ж������жϴ�����
    XScuGic_Connect(&scugic_inst, UART_INT_IRQ_ID,(Xil_ExceptionHandler) uart_intr_handler,&Uart_Ps);
    //����UART���жϴ�����ʽ
    XUartPs_SetInterruptMask(&Uart_Ps, XUARTPS_IXR_RXOVR);
    //ʹ��GIC�еĴ����ж�
    XScuGic_Enable(&scugic_inst, UART_INT_IRQ_ID);
    /************************************************************************/
    //���ò����ж��쳣������
    Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler, &scugic_inst);
    Xil_ExceptionEnable();

    return XST_SUCCESS;
}

//�ж��ۺϴ���
void irqProcess(void)
{
	if(irqFlag.adc == 1){
		irqFlag.adc = 0;		//����жϱ�־λ
		adcIrqProcess();
	}
	if(irqFlag.axiGpio == 1){
		irqFlag.axiGpio = 0;	//����жϱ�־λ
		axiGpioIrqProcess();
	}
	if(irqFlag.rtAddr == 1){
		irqFlag.rtAddr = 0;		//����жϱ�־λ
		rtAddrIrqProcess();
	}
	if(irqFlag.rtMessage == 1){
		irqFlag.rtMessage = 0;	//����жϱ�־λ
		rtMessageIrqProcess();
	}
	if(irqFlag.uart == 1){
		irqFlag.uart = 0;
    	uart_plEmioControl();
	}
	if(irqFlag.timer == 1){
		irqFlag.timer = 0;
		timerProcess();
	}
}

void axiGpioIrqProcess(void)
{
	int gpio_value;
	print("\naxi_gpio�жϼ�ֵ��ȡ\n");
	gpio_value = ReadReg(axiGpioRegBaseAddr+0x0c);	  						//axi_gpio״̬�Ĵ������ݶ�ȡ

	printf("gpio_value:%0x\n",gpio_value);
	gpio_value&= 0x00000001;

	if(gpio_value == 1)printf("small bomb\n");								//������д��װ�������У�����
	else if(gpio_value == 0)printf("big bomb\n");
}

void rtAddrIrqProcess(void)
{
	int rt_addr,rt_start;
	print("\nRT��ַ�жϼĴ���\n");
	rt_addr = ReadReg(rtAddrRegBaseAddr+0x04);								//rt_addr״̬�Ĵ������ݶ�ȡ
	printf("originRt_addr:%0x\n",rt_addr);
	rt_addr&= 0x0000001f;
	printf("rt_addr:%0x\n",rt_addr);										//����в��� װ��������

	rt_start = 0x20;    													//RT����rt ok
	rt_addr |= rt_start;													//ָ��ϲ�
	printf("writeRtAddr:%0x\r\n",rt_addr);
	WriteReg(0x40000000+0x10,rt_addr);                  					//д�����װ���Ĵ���
}

void rtMessageIrqProcess(void)
{
	int 	rt_message;
	int 	length,type,subaddress;					  						//��Ϣ���ȣ����ͣ��ӵ�ַ
	char	commandSelect; 			 										//0:BC-RT,1:RT-BC,2:VectorCommand
	u32 	rtRxBuf[100];
	int 	i = 0;
	int     cmdStatus = 0;

	print("\nRT��Ϣ�жϼĴ���\n");
	rt_message 	= ReadReg(rtMessageRegBaseAddr+0x14);						//rt_message״̬�Ĵ������ݶ�ȡ
	cmdStatus   = rt_message&0xffff0000;
	printf("rt_messageOrig:%0x --- cmdStatus:%0x\n",rt_message,cmdStatus>>16);
	rt_message &= 0x00001fff;
	printf("rt_message:%0x\n",rt_message);


	length 		= rt_message&0x0000003f;
	subaddress  = (rt_message>>6)&0x0000001f;
	type 		= (rt_message>>11)&0x00000007;

	printf("length:%0x  subAddress:%0x  type:%0x\n",length,subaddress,type);
	//��Ϣ����ȷ��
	switch(type)
	{
		case 01:{
			printf("BC-RT:%0x\n",type);										 //BC-RT���ݽ���
			commandSelect = 1;
		}break;
		case 02:{
			printf("RT-BC:%0x\n",type);										 //RT-BC���ݷ��ͽ���
			commandSelect = 2;
		}break;
		case 04:{
			printf("Send Vector Mode Command:%0x\n",type);
			commandSelect = 3;
		}break;
		default:break;
	}

	//BC-RT���ݽ���
	if(commandSelect == 1){
		rt_RxTx_DataRead(rtRxBuff.BaseAddress+subaddress*128,rtRxBuf,length);//���ջ�����д����Ϣ  ��A8

		printf("rt recvd length:%d --- data:",length);
		for(i=0;i<length;i++)printf("%lx ",rtRxBuf[i]);
		memset((char*)rtRxBuf,0,length);
		printf("\r\n");

		if(subaddress == 6){
			if(rtRxBuf[3] == 0x00ff){
				lspd1553a6.batteryJhJg   =  0x00ff;
				lspd1553a6.invalidValueL &= 0xd0;					//bit5��6���ֽ�����Ч
				rt2bc_Send((int *)&lspd1553a6,2,sizeof(lspd1553a6)/4);
				regValueSet(plEmio,8,1);							//��ؼ���bit8��1
				regValueSet(plEmio,10,0);							//��ؼ����ѹ����bit10��0
			}
			if(rtRxBuf[27] == 0x00ff){
				lspd1553a6.weaponZjJg    =  0x00ff;
				lspd1553a6.invalidValueL &= 0xe0;					//bit4��5���ֽ�����Ч
				rt2bc_Send((int *)&lspd1553a6,2,sizeof(lspd1553a6)/4);
			}
			if(rtRxBuf[28] == 0x00ff){
				lspd1553a6.gd3JcJg       = 0x00ff;
				lspd1553a6.invalidValueL &= 0xb0;					//bit6��7���ֽ�����Ч
				rt2bc_Send((int *)&lspd1553a6,2,sizeof(lspd1553a6)/4);
			}
			if(rtRxBuf[29] == 0x00ff){
				regValueSet(plEmio,7,1);
				regValueSet(plEmio,9,1); 							//��������ѹ����bit11��0
				regValueSet(plEmio,11,0);							//�������bit9��1
			}
		}

		if(subaddress == 8){
			rt2bc_Send((int *)rtRxBuf,10,length);					//ʸ������λ ����RT-BC
		}
		commandSelect = 0;
	}
	//RT-BC���ݷ������
	else if(commandSelect == 2){
		printf("rt-bc data send completed\r\n");
		regValueSet(vectorCtl,subaddress,0);						//��λʸ��������  0~15λ bit[N]��ӦN��ŵķ����ӵ�ַ�������ݸ���
		commandSelect = 0;
	}
	//modeCode---vectorCtrol
	else if(commandSelect == 3){
		commandSelect = 0;
	}
}

void adcIrqProcess(void)
{
	int   status = 0,status1 = 0;
	int   numstatus = 0,count = 0;
	float amplitude[9];
	int   pulseWidth[9];
	int   TestValue = 0;
	char  changeFlag = 0;
	/***********************************************/
	//ͨ�Ų��ԼĴ�����TestReg��
	TestValue = ReadReg(adcRegBaseAddr+23*4);					//��ȡͨ�Ų��ԼĴ�����ֵ 23
	if(TestValue != 0x5a5a5a5a){
		printf("tongXinReg read error\r\n");
		return;
	}
	/***********************************************/
	ad_value[0] = ReadReg(adcRegBaseAddr+22*4);					//��ȡAD�жϼĴ�����ֵ 22
	printf("\nAdc�жϼĴ��� value:0x%0x --- flagCount:%d\r\n",ad_value[0],irqFlag.adc);
	if(irqFilter.adcIrqFilter != ad_value[0]){
		irqFilter.adcIrqFilter = ad_value[0];
	}else {
		printf("error irqReg filter\r\n");
		return;
	}/*******************************/
	status    = ad_value[0]&0x000003ff;
	status1   = ad_value[0]&0x000ffc00;
	numstatus = ad_value[0]&0x00f00000;
	/***********************************************/
	if((numstatus&0x00100000) == 00100000){
		adcValue.gd23JgTime = ReadReg(adcRegBaseAddr+18*4);
		printf("gd23JgTime:%d\r\n",adcValue.gd23JgTime);
		changeFlag = 1;
	}
	if((numstatus&0x00200000) == 0x00200000){
		adcValue.noReGd12JgTime = ReadReg(adcRegBaseAddr+19*4);
		printf("noReGd12JgTime:%d\r\n",adcValue.noReGd12JgTime);
		changeFlag = 1;
	}
	if((numstatus&0x00400000) == 0x00400000){
		adcValue.noReGd23JgTime = ReadReg(adcRegBaseAddr+20*4);
		printf("noReGd23JgTime:%d\r\n",adcValue.noReGd23JgTime);
		changeFlag = 1;
	}
	if((numstatus&0x00800000) == 0x00800000){
		adcValue.noReGd34JgTime = ReadReg(adcRegBaseAddr+21*4);
		printf("noReGd34JgTime:%d\r\n",adcValue.noReGd34JgTime);
		changeFlag = 1;
	}
	/***********************************************/
	if((status&0x0001) == 0x0001){
		printf("Bit0:����1�ж�����\r\n");
		ad_value[2] 	= ReadReg(adcRegBaseAddr+1*4);
		pulseWidth[0] 	= ad_value[2]&0x0fffffff;			//������28bit
		printf("pulseWidth:%d ms\n",pulseWidth[0]);			//��д��װ������������
		changeFlag = 1;
	}

	if((status1&0x0400) == 0x0400){
		printf("Bit0:����1�жϵ�ѹ\r\n");
		ad_value[1] 	= ReadReg(adcRegBaseAddr+0*4);
		ad_value[1] 	= ad_value[1]&0x00007fff;			//������15bit,��16bitΪ����λ,����Ĭ��Ϊ��
		amplitude[0]  	= 40.96*21*ad_value[1]/32768;
		adcValue.gd1Vot 			= amplitude[0]*1000;
		printf("gd1_amplitude:%.2f\n",amplitude[0]);		//��д��װ������������
	}

	if((status&0x0002) == 0x0002){
		printf("Bit1:����2�ж�����\r\n");
		ad_value[4] 	= ReadReg(adcRegBaseAddr+3*4);
		pulseWidth[1] 	= ad_value[4]&0x0fffffff;			//������28bit
		adcValue.gd2PulseWidth 	 = pulseWidth[1];
		printf("pulseWidth:%d ms\n",pulseWidth[1]);			//��д��װ������������
		changeFlag = 1;
	}

	if((status1&0x0800) == 0x0800){
		printf("Bit1:����2�жϵ�ѹ\r\n");
		ad_value[3] 	= ReadReg(adcRegBaseAddr+2*4);
		ad_value[3] 	= ad_value[3]&0x00000fff;			//������12bit
		amplitude[1]  	= ad_value[3]*(0.02563);			//5*21*adc/4096
		adcValue.gd2Vot 		= amplitude[1]*1000;
		printf("amplitude:%.2f\n",amplitude[1]);			//��д��װ������������

		if(adcValue.gd2Vot>25&&adcValue.gd2Vot<=35){
			lspd1553a6.weaponZjJg 	 =  0x00ff;
			lspd1553a6.invalidValueL &= 0xe0;				//bit4��5���ֽ�����Ч
			rt2bc_Send((int *)&lspd1553a6,2,sizeof(lspd1553a6)/4);
		}

		count++;
		adcValue.gd2Num = count;
		changeFlag = 1;
	}

	if((status&0x0004) == 0x0004){
		printf("Bit2:����3�ж�����\r\n");
		ad_value[6] 	= ReadReg(adcRegBaseAddr+5*4);
		pulseWidth[2] 	= ad_value[6]&0x0fffffff;			//������28bit
		adcValue.gd3PulseWidth 	 = pulseWidth[2];
		printf("pulseWidth:%d ms\n",pulseWidth[2]);			//��д��װ������������
		changeFlag = 1;
	}

	if((status1&0x1000) == 0x1000){
		printf("Bit2:����3�жϵ�ѹ\r\n");
		ad_value[5] 	= ReadReg(adcRegBaseAddr+4*4);
		ad_value[5] 	= ad_value[5]&0x00000fff;			//������12bit
		amplitude[2]  	= ad_value[5]*(0.02563);
		adcValue.gd3Vot 		= amplitude[2]*1000;
		printf("amplitude:%.2f\n",amplitude[2]);			//��д��װ������������

		if(adcValue.gd3Vot>25&&adcValue.gd3Vot<=35){
			lspd1553a6.gd3JcJg 	     =  0x00ff;
			lspd1553a6.invalidValueL &= 0xb0;				//bit6��7���ֽ�����Ч
			rt2bc_Send((int *)&lspd1553a6,2,sizeof(lspd1553a6)/4);
		}

		count++;
		adcValue.gd3Num = count;
		changeFlag = 1;
	}

	if((status&0x0008) == 0x0008){
		printf("Bit3:��������ж�����\r\n");					//VCC5
		ad_value[8] 	= ReadReg(adcRegBaseAddr+7*4);
		pulseWidth[3] 	= ad_value[8]&0x0fffffff;			//������28bit
		adcValue.abjcPulseWidth  = pulseWidth[3];
		printf("pulseWidth:%d ms\n",pulseWidth[3]);
		changeFlag = 1;
	}

	if((status1&0x2000) == 0x2000){
		printf("Bit3:��������жϵ�ѹ\r\n");
		ad_value[7] 	= ReadReg(adcRegBaseAddr+6*4);
		ad_value[7] 	= ad_value[7]&0x00000fff;			//������12bit
		amplitude[3]  	= ad_value[7]*(0.02563);
		adcValue.abjcVot 		= amplitude[3]*1000;
		printf("amplitude:%.2f\n",amplitude[3]);

		if(adcValue.abjcVot>25&&adcValue.abjcVot<=35){
			regValueSet(plEmio,4,1);							//bit4 1:���������0:����δ���
			regValueSet(plEmio,5,1);							//bit5 1:�����ָ���0:����δ�ָ�
		}

		count++;
		adcValue.abjcNum = count;
		changeFlag = 1;
	}

	if((status&0x0010) == 0x0010){
		printf("Bit4:�����ָ��ж�����\r\n");
		ad_value[10] 	= ReadReg(adcRegBaseAddr+9*4);
		pulseWidth[4] 	= ad_value[10]&0x0fffffff;			//������28bit
		printf("pulseWidth:%d ms\n",pulseWidth[4]);
		changeFlag = 1;
	}

	if((status1&0x4000) == 0x4000){
		printf("Bit4:�����ָ��жϵ�ѹ\r\n");					//VCC5
		ad_value[9]  	= ReadReg(adcRegBaseAddr+8*4);
		ad_value[9]  	= ad_value[9]&0x00000fff;			//������12bit
		amplitude[4]  	= ad_value[9]*(0.02563);
		adcValue.abhfVot 		= amplitude[4]*1000;
		printf("amplitude:%.2f\n",amplitude[4]);

		if(adcValue.abhfVot>25&&adcValue.abhfVot<=35){
			regValueSet(plEmio,5,0);							//bit5 1:�����ָ���0:����δ�ָ�
			regValueSet(plEmio,4,0);							//bit5 1:�����ָ���0:����δ�ָ�
		}
	}

	if((status&0x0020) == 0x0020){
		printf("Bit5:Ͳ�������ж�����\r\n");					//VCC4
		ad_value[12] 	= ReadReg(adcRegBaseAddr+11*4);
		pulseWidth[5] 	= ad_value[12]&0x0fffffff;			//������28bit
		adcValue.tdjsPulseWidth  = pulseWidth[5];
		printf("pulseWidth:%d ms\n",pulseWidth[5]);
		changeFlag = 1;
	}

	if((status1&0x8000) == 0x8000){
		printf("Bit5:Ͳ�������жϵ�ѹ\r\n");
		ad_value[11] 	= ReadReg(adcRegBaseAddr+10*4);
		ad_value[11] 	= ad_value[11]&0x00000fff;			//������12bit
		amplitude[5]  	= ad_value[11]*(0.02563);
		adcValue.tdjsVot 		= amplitude[5]*1000;
		printf("amplitude:%.2f\n",amplitude[5]);

		if(adcValue.tdjsVot>25&&adcValue.tdjsVot<=35){
			regValueSet(plEmio,2,1);						//bit2 2:Ͳ��������0:Ͳ��δ����
			regValueSet(plEmio,1,1);						//bit1 1:Ͳ��������0:Ͳ������
		}

		count++;
		adcValue.tdjsNum = count;
		changeFlag = 1;
	}

	if((status&0x0040) == 0x0040){
		printf("Bit6:Ͳ�������ж�����\r\n");
		ad_value[14] 	= ReadReg(adcRegBaseAddr+13*4);
		pulseWidth[6] 	= ad_value[14]&0x0fffffff;			//������28bit
		printf("pulseWidth:%d ms\n",pulseWidth[6]);
		changeFlag = 1;
	}

	if((status1&0x10000) == 0x10000){
		printf("Bit6:Ͳ�������жϵ�ѹ\r\n");					//VCC4
		ad_value[13] 	= ReadReg(adcRegBaseAddr+12*4);
		ad_value[13] 	= ad_value[13]&0x00000fff;			//������12bit
		amplitude[6]  	= ad_value[13]*(0.02563);
		adcValue.tdsdVot 	  = amplitude[6]*1000;
		printf("amplitude:%.2f\r\n",amplitude[6]);

		if(adcValue.tdsdVot>25&&adcValue.tdsdVot<=35){
			regValueSet(plEmio,2,0);						//bit2 2:Ͳ��������0:Ͳ��δ����
			regValueSet(plEmio,1,0);						//bit1 1:Ͳ��������0:Ͳ������
		}
	}

	if((status&0x0080) == 0x0080){
		printf("Bit7:��ؼ����ж�����\r\n");
		ad_value[16] 	= ReadReg(adcRegBaseAddr+15*4);
		pulseWidth[7] 	= ad_value[16]&0x0fffffff;			//������28bit
		adcValue.dcjhPulseWidth  = pulseWidth[7];
		printf("pulseWidth:%d ms\n",pulseWidth[7]);			//��д��װ������������

		regValueSet(plEmio,10,0);							//��ؼ����ѹ����bit10��0
		changeFlag = 1;
	}

	if((status1&0x20000) == 0x20000){
		printf("Bit7:��ؼ����жϵ�ѹ\r\n");					//VCC6
		ad_value[15] 	= ReadReg(adcRegBaseAddr+14*4);
		ad_value[15] 	= ad_value[15]&0x00000fff;			//������12bit
		amplitude[7]  	= ad_value[15]*(0.02563);
		adcValue.dcjhVot 		= amplitude[7]*1000;
		printf("amplitude:%.2f\n",amplitude[7]);			//��д��װ������������

		if(adcValue.dcjhVot>25&&adcValue.dcjhVot<=35){
			lspd1553a6.batteryJhJg   =  0x00ff;
			lspd1553a6.invalidValueL &= 0xd0;				//bit5��6���ֽ�����Ч
			rt2bc_Send((int *)&lspd1553a6,2,sizeof(lspd1553a6)/4);
			regValueSet(plEmio,8,1);						//��ؼ���bit8��1
			regValueSet(plEmio,10,1);						//��ؼ����ѹ����bit10��1
		}

		count++;
		adcValue.dcjhNum = count;
		changeFlag = 1;
	}

	if((status&0x0100) == 0x0100){
		printf("Bit8:Ͳ��������ж�����\r\n");
		ad_value[18] 	= ReadReg(adcRegBaseAddr+17*4);
		pulseWidth[8] 	= ad_value[18]&0x0fffffff;			//������28bit
		adcValue.dddhPulseWidth  = pulseWidth[8];
		printf("pulseWidth:%d ms\n",pulseWidth[8]);
			
		adcValue.gd23JgTime = ReadReg(adcRegBaseAddr+18*4);
		//printf("gd23JgTime:%d\r\n",adcValue.gd23JgTime);
		adcValue.noReGd12JgTime = ReadReg(adcRegBaseAddr+19*4);
		//printf("noReGd12JgTime:%d\r\n",adcValue.noReGd12JgTime);
		adcValue.noReGd23JgTime = ReadReg(adcRegBaseAddr+20*4);
		//printf("noReGd23JgTime:%d\r\n",adcValue.noReGd23JgTime);
		adcValue.noReGd34JgTime = ReadReg(adcRegBaseAddr+21*4);
		//printf("noReGd34JgTime:%d\r\n",adcValue.noReGd34JgTime);
		
		regValueSet(plEmio,11,0);							//�������bit11��0
		changeFlag = 1;
		regValueSet(vectorCtl,29,1);						//д��ʸ���֣������ӵ�ַ29��RT-BC����
	}

	if((status1&0x40000) == 0x40000){
		printf("Bit8:Ͳ��������жϵ�ѹ\r\n");				//VCC7
		ad_value[17] 	 = ReadReg(adcRegBaseAddr+16*4);
		ad_value[17] 	 = ad_value[17]&0x00000fff;			//������12bit
		amplitude[8]  	 = ad_value[17]*(0.02563);
		adcValue.dddhVot = amplitude[8]*1000;
		printf("pulseWidth:%.2f\n",amplitude[8]);

		if(adcValue.dddhVot>25&&adcValue.dddhVot<=35){
			regValueSet(plEmio,7,1);						//bit7����1�պ�
			regValueSet(plEmio,9,1); 						//��������ѹ����bit11��0
			regValueSet(plEmio,11,1);						//�������bit11��0
		}

		count++;
		adcValue.dddhNum = count;
		changeFlag = 1;
	}

	//�Ĵ���ֵ�����ı䣬�����ݸ��������ͻ�����
	if(changeFlag == 1){
		adcValue.checksum = check_GJB1188A((unsigned int *)&adcValue,sizeof(adcValue)/4);
		rt_RxTx_DataRead(rtTxBuff.BaseAddress+29*128,(u32 *)&adcValue,sizeof(adcValue)/4);
		changeFlag = 0;
	}
}

//rtAddr�ж�
void rtAddr_handler(void *CallbackRef)
{
	irqFlag.rtAddr = 1;
}

//ADC�����ж�
void adcIrq_handler(void *CallbackRef)
{
	irqFlag.adc = 1;
}

//����1553�ж�
void fc1553Irq_handler(void *CallbackRef)
{
	irqFlag.fc1553 = 1;
}

//PL�� AXI GPIO �жϷ���(����)����  axi_gpio
void axi_gpio_handler(void *CallbackRef)
{
	irqFlag.axiGpio = 1;
}

//����1553�жϴ���
void rtMessage_handler(void *CallbackRef)
{
	irqFlag.rtMessage = 1;
}

