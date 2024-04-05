/*
 * common.h
 *
 *  Created on: 2023��3��2��
 *      Author: 15993
 */

#ifndef SRC_COMMON_H_
#define SRC_COMMON_H_

u32 plIoInitValue;
u32 vectorInitValue;

#define plEmio 		0x40002000+0x08		//axiGpio�Ĵ�����ַ
#define vectorCtl 	0x40000000+0x0c 	//ʸ���ֿ��ƼĴ�����ַ

//rt_message����װ����ؽṹ��
typedef struct {

	int BaseAddress;
	int value[30];

} RT_RxBuff;
RT_RxBuff rtRxBuff;

typedef struct {

	int BaseAddress;
	int value[32];

} RT_TxBuff;
RT_TxBuff rtTxBuff;

void regValueSet(u32 addr,int bit,int level);
u32 checkSum(u32* data,int length);
u32  WriteReg(u32 addr, u32 data);
u32  ReadReg(u32 addr);
void axiGpioTestScope(void);
unsigned short  _1188A_code(unsigned char *data_add, unsigned char len_data);
unsigned short check_GJB1188A(unsigned int *buf, unsigned char length);
int char_2_hex(uint8_t *src,int length);
uint8_t hex_2_char(uint8_t *src);

#endif /* SRC_COMMON_H_ */
