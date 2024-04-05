/*
 * common.c
 *
 *  Created on: 2023年3月2日
 *      Author: 15993
 */

#include "string.h"
#include "xil_printf.h"
#include "platform.h"
#include <stdio.h>
#include "common.h"
#include "xil_io.h"

/************************************************/
void timerSend(u32 *buf)
{
	int i=0;
	for(i=0;i<19;i++)WriteReg(rtTxBuff.BaseAddress+1*64,buf[i]);
}

u32 WriteReg(u32 addr, u32 data)
{
	Xil_Out32(addr,data);
	return 0;
}

u32 ReadReg(u32 addr)
{
	return Xil_In32(addr);
}

u32 checkSum(u32* data,int length)
{
	u32 value = 0;
	int i = 0;

	for(i=0;i<length-1;i++){
		value+=data[i];
	}printf("checksumValue:%lx\r\n",value&0xffff);

	i=0;
	return value&0xffff;
}

unsigned short  _1188A_code(unsigned char *data_add, unsigned char len_data)
{
	unsigned char i=0,len_word=0;
	unsigned short int word[100] = {0x0000};
	unsigned short int crc_value=0x0000;
	len_word = len_data;
	for (i = 0; i < len_word; i++)
	{

		word[i]=(data_add[2*i]<<8)+(data_add[2*i+1]);				//两个字节合成一个字
		word[i]=word[i]>> (i%16) | word[i]<< (16 - (i%16));			//对数据字进行循环移位
		crc_value = crc_value ^ word[i];							//求循环移位后的校验和字

	}
	crc_value = (crc_value << (len_word%16)) | (crc_value >> (16-(len_word%16)));//对上个结果进行反向循环移位
	return crc_value;
}

unsigned short check_GJB1188A(unsigned int *buf, unsigned char length)
{
    unsigned char i=0;
    unsigned short int crc_value=0x0000;
    unsigned int data[1024];
    length = length - 1;
    for (i = 0; i < length; i++)
    {
    	data[i]=buf[i];
        data[i]=data[i]>> (i%16) | data[i]<< (16 - (i%16));
        crc_value = crc_value ^ data[i];
    }
    crc_value = (crc_value << (length%16)) | (crc_value >> (16-(length%16)));
    return crc_value;
}

uint8_t hex_2_char(uint8_t *src)
{
    uint8_t desc;

    if((*src >= 0) && (*src <= 9))
        desc = *src + 0x30;
    else if((*src >= 0xA) && (*src <= 0xF))
        desc = *src + 0x37;

    return desc;
}

int char_2_hex(uint8_t *src,int length)
{
    uint8_t desc;
    int     value = 0;
    for(int i=0;i<length;i++){
		if((src[i] >= '0') && (src[i] <= '9'))
			desc = src[i] - 0x30;
		else if((src[i] >= 'a') && (src[i] <= 'f'))
			desc = src[i] - 0x57;
		else if((src[i] >= 'A') && (src[i] <= 'F'))
			desc = src[i] - 0x37;
		value = (value<<4)|desc;
    }
    return value;
}
/************************************************
 * func:set plGpio&vector level status
 * para:
 * 		addr : plGpio&vector reg addr
 * 		bit  : select io pin
 * 		level: set level to 0/1
 * ***********************************************/
void regValueSet(u32 addr,int bit,int level)
{
	u32 InitValue = 0;
	if(addr == plEmio)InitValue = plIoInitValue;
	else if(addr == vectorCtl)InitValue = vectorInitValue;
	if(level == 0){
		switch (bit)
		{
			case 0:
				InitValue&=0xfffffffe;
				break;
			case 1:
				InitValue&=0xfffffffd;
				break;
			case 2:
				InitValue&=0xfffffffb;
				break;
			case 3:
				InitValue&=0xfffffff7;
				break;
			case 4:
				InitValue&=0xffffffef;
				break;
			case 5:
				InitValue&=0xffffffdf;
				break;
			case 6:
				InitValue&=0xffffffbf;
				break;
			case 7:
				InitValue&=0xffffff7f;
				break;
			case 8:
				InitValue&=0xfffffeff;
				break;
			case 9:
				InitValue&=0xfffffdff;
				break;
			case 10:
				InitValue&=0xfffffbff;
				break;
			case 11:
				InitValue&=0xfffff7ff;
				break;
			case 12:
				InitValue&=0xffffefff;
				break;
			case 13:
				InitValue&=0xffffdfff;
				break;
			case 14:
				InitValue&=0xffffbfff;
				break;
			case 15:
				InitValue&=0xffff7fff;
				break;
			case 16:
				InitValue&=0xfffeffff;
				break;
			case 17:
				InitValue&=0xfffdffff;
				break;
			case 18:
				InitValue&=0xfffbffff;
				break;
			case 19:
				InitValue&=0xfff7ffff;
				break;
			case 20:
				InitValue&=0xffefffff;
				break;
			case 21:
				InitValue&=0xffdfffff;
				break;
			case 22:
				InitValue&=0xffbfffff;
				break;
			case 23:
				InitValue&=0xff7fffff;
				break;
			case 24:
				InitValue&=0xfeffffff;
				break;
			case 25:
				InitValue&=0xfdffffff;
				break;
			case 26:
				InitValue&=0xfbffffff;
				break;
			case 27:
				InitValue&=0xf7ffffff;
				break;
			case 28:
				InitValue&=0xefffffff;
				break;
			case 29:
				InitValue&=0xdfffffff;
				break;
			case 30:
				InitValue&=0xbfffffff;
				break;
			case 31:
				InitValue&=0x7fffffff;
				break;
			default:break;
		}
		WriteReg(addr,InitValue);
		if(addr == plEmio)plIoInitValue = InitValue;
		else if(addr == vectorCtl)vectorInitValue =InitValue;
		printf("setAddr:%lx --- setValue:0x%lx\r\n",addr,InitValue);
	}else if(level == 1){
		switch (bit)
		{
			case 0:
				InitValue|=0x00000001;
				break;
			case 1:
				InitValue|=0x00000002;
				break;
			case 2:
				InitValue|=0x00000004;
				break;
			case 3:
				InitValue|=0x00000008;
				break;
			case 4:
				InitValue|=0x00000010;
				break;
			case 5:
				InitValue|=0x00000020;
				break;
			case 6:
				InitValue|=0x00000040;
				break;
			case 7:
				InitValue|=0x00000080;
				break;
			case 8:
				InitValue|=0x00000100;
				break;
			case 9:
				InitValue|=0x00000200;
				break;
			case 10:
				InitValue|=0x00000400;
				break;
			case 11:
				InitValue|=0x00000800;
				break;
			case 12:
				InitValue|=0x00001000;
				break;
			case 13:
				InitValue|=0x00002000;
				break;
			case 14:
				InitValue|=0x00004000;
				break;
			case 15:
				InitValue|=0x00008000;
				break;
			case 16:
				InitValue|=0x00010000;
				break;
			case 17:
				InitValue|=0x00020000;
				break;
			case 18:
				InitValue|=0x00040000;
				break;
			case 19:
				InitValue|=0x00080000;
				break;
			case 20:
				InitValue|=0x00100000;
				break;
			case 21:
				InitValue|=0x00200000;
				break;
			case 22:
				InitValue|=0x00400000;
				break;
			case 23:
				InitValue|=0x00800000;
				break;
			case 24:
				InitValue|=0x01000000;
				break;
			case 25:
				InitValue|=0x02000000;
				break;
			case 26:
				InitValue|=0x04000000;
				break;
			case 27:
				InitValue|=0x08000000;
				break;
			case 28:
				InitValue|=0x10000000;
				break;
			case 29:
				InitValue|=0x20000000;
				break;
			case 30:
				InitValue|=0x40000000;
				break;
			case 31:
				InitValue|=0x80000000;
				break;
			default:break;
		}
		WriteReg(addr,InitValue);
		if(addr == plEmio)plIoInitValue = InitValue;
		else if(addr == vectorCtl)vectorInitValue =InitValue;
		printf("setAddr:%lx --- setValue:0x%lx\r\n",addr,InitValue);
	}

}

void axiGpioTestScope(void)
{
	regValueSet(plEmio,0,1);//大小弹
	regValueSet(plEmio,0,0);

	regValueSet(plEmio,1,1);//筒弹解锁
	regValueSet(plEmio,1,0);

	regValueSet(plEmio,2,1);//筒弹锁定
	regValueSet(plEmio,2,0);

	regValueSet(plEmio,3,1);//自闭合
	regValueSet(plEmio,3,0);

	regValueSet(plEmio,4,1);//安保解除
	regValueSet(plEmio,4,0);

	regValueSet(plEmio,5,1);//安保恢复
	regValueSet(plEmio,5,0);

	regValueSet(plEmio,6,1);//联锁2
	regValueSet(plEmio,6,0);

	regValueSet(plEmio,7,1);//联锁1
	regValueSet(plEmio,7,0);
}
