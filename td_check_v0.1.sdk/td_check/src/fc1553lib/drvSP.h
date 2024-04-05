/**
*   COPYRIGHT (C) TASSON     CO.,LTD
*----------------------------------------------------------------------------
* @file drvSysPlatform.h
*
* @brief 
* Details
*----------------------------------------------------------------------------
* Change History :
* <Date>           | <Author> | <Description>
* @date 2017/5/16 |  gepeng  |  Create File
* 
*
*/
#ifndef DRVSP_H
#define DRVSP_H

/***************************** Include Files *********************************/
  
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define C_SP_DEBUG_LEVEL_LOWEST      (0)
#define C_SP_DEBUG_LEVEL_LOW         (1)
#define C_SP_DEBUG_LEVEL_NORMAL      (2)
#define C_SP_DEBUG_LEVEL_HIGH        (3)
#define C_SP_DEBUG_LEVEL_HIGHEST     (4)

#define C_SP_MAX_PRINT_STRING_LENGTH (256)
/************************** Variable Declaration *****************************/

#if 0
typedef char								s8;
typedef signed short                        s16;
typedef signed int                          s32;
typedef signed long long                    s64;
typedef unsigned char                       u8;
typedef unsigned short                      u16;
typedef unsigned int                        u32;
typedef unsigned long long                  u64;
typedef float                               f32;
typedef double                              f64;
#endif


#define C_SYS_NULL		((void *)0)
//#define NULL		((void *)0)

#ifndef FALSE
#define FALSE			(0)
#endif

#ifndef TRUE
#define TRUE			(1)
#endif

/************************** Function Prototypes ******************************/

/************************************************************************\
* @����			: drvSpInit
* @����ֵ			: �ɹ�����0��ʧ��ʱ����-1��
* @˵��			: ϵͳ��ʼ��
* @����			: 2021/04/16
\************************************************************************/
int drvSpInit(void);

/************************************************************************\
* @����			: drvSpReadReg
* @param[����]	: u32RegAddress:���Ĵ�����ַ
* @param[���]	: pu32RegValue:������
* @����ֵ			: ��
* @˵��			: ϵͳ���Ĵ�����
* @����			: 2021/04/16
\************************************************************************/
void drvSpReadReg(int devHd, unsigned int u32RegAddress, unsigned int *pu32RegValue);


/************************************************************************\
* @����			: drvSpWriteReg
* @param[����]	: u32RegAddress:д�Ĵ�����ַ��
* @param[����]	: u32RegValue:д��Ĵ���ֵ��
* @����ֵ			: ��
* @˵��			: ϵͳд�Ĵ�����
* @����			: 2021/04/16
\************************************************************************/
void drvSpWriteReg(int devHd, unsigned int u32RegAddress,unsigned int u32RegValue);


/************************************************************************\
* @����			: drvSpReadMem
* @param[����]	: u32MemoryAddress:��������ʼ��ַ��
* @param[���]	: pvBuffer:�����ݻ�����ָ�롣
* @param[����]	: u32Length:�����ݳ��ȡ�
* @����ֵ			: ��
* @˵��			: ��һ�������ڴ�����
* @����			: 2021/04/16
\************************************************************************/
void drvSpReadMem(int devHd, unsigned int u32MemoryAddress, void *pvBuffer, unsigned int u32Length);


/************************************************************************\
* @����			: drvSpWriteMem
* @param[����]	: u32MemoryAddress:д������ʼ��ַ��
* @param[����]	: pvBuffer:��д���ݻ�����ָ�롣
* @param[����]	: u32Length:д���ݳ��ȡ�
* @����ֵ			: ��
* @˵��			: дһ�������ڴ�����
* @����			: 2021/04/16
\************************************************************************/
void drvSpWriteMem(int devHd, unsigned int u32MemoryAddress, void *pvBuffer, unsigned int u32Length);



/************************************************************************\
* @����			: drvSpDebugInit
* @param[����]	: u8DebugLevel:��ӡ��Ϣ�ȼ���
* @����ֵ			: �ɹ�����0��ʧ��ʱ����-1��
* @˵��			: ϵͳ��ӡ��Ϣ��ʼ����
* @����			: 2021/04/16
\************************************************************************/
int drvSpDebugInit(unsigned char u8DebugLevel);


/************************************************************************\
* @����			: drvSpDebugInit
* @param[����]	: u8Level:��ӡ��Ϣ�ȼ���
* @param[����]	: pcs8Format:��ӡ��ʽ��
* @����ֵ			: �ɹ�����0��ʧ��ʱ����-1��
* @˵��			: ��ӡ������Ϣ��
* @����			: 2021/04/16
\************************************************************************/
void drvSpDebugPrintf(unsigned char u8Level, const char *pcs8Format, ...);

#endif
