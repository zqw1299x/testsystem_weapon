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
* @名称			: drvSpInit
* @返回值			: 成功返回0，失败时返回-1。
* @说明			: 系统初始化
* @日期			: 2021/04/16
\************************************************************************/
int drvSpInit(void);

/************************************************************************\
* @名称			: drvSpReadReg
* @param[输入]	: u32RegAddress:读寄存器地址
* @param[输出]	: pu32RegValue:读数据
* @返回值			: 无
* @说明			: 系统读寄存器。
* @日期			: 2021/04/16
\************************************************************************/
void drvSpReadReg(int devHd, unsigned int u32RegAddress, unsigned int *pu32RegValue);


/************************************************************************\
* @名称			: drvSpWriteReg
* @param[输入]	: u32RegAddress:写寄存器地址。
* @param[输入]	: u32RegValue:写入寄存器值。
* @返回值			: 无
* @说明			: 系统写寄存器。
* @日期			: 2021/04/16
\************************************************************************/
void drvSpWriteReg(int devHd, unsigned int u32RegAddress,unsigned int u32RegValue);


/************************************************************************\
* @名称			: drvSpReadMem
* @param[输入]	: u32MemoryAddress:读数据起始地址。
* @param[输出]	: pvBuffer:读数据缓冲区指针。
* @param[输入]	: u32Length:读数据长度。
* @返回值			: 无
* @说明			: 读一段连续内存数据
* @日期			: 2021/04/16
\************************************************************************/
void drvSpReadMem(int devHd, unsigned int u32MemoryAddress, void *pvBuffer, unsigned int u32Length);


/************************************************************************\
* @名称			: drvSpWriteMem
* @param[输入]	: u32MemoryAddress:写数据起始地址。
* @param[输入]	: pvBuffer:读写数据缓冲区指针。
* @param[输入]	: u32Length:写数据长度。
* @返回值			: 无
* @说明			: 写一段连续内存数据
* @日期			: 2021/04/16
\************************************************************************/
void drvSpWriteMem(int devHd, unsigned int u32MemoryAddress, void *pvBuffer, unsigned int u32Length);



/************************************************************************\
* @名称			: drvSpDebugInit
* @param[输入]	: u8DebugLevel:打印信息等级。
* @返回值			: 成功返回0，失败时返回-1。
* @说明			: 系统打印信息初始化。
* @日期			: 2021/04/16
\************************************************************************/
int drvSpDebugInit(unsigned char u8DebugLevel);


/************************************************************************\
* @名称			: drvSpDebugInit
* @param[输入]	: u8Level:打印信息等级。
* @param[输入]	: pcs8Format:打印格式。
* @返回值			: 成功返回0，失败时返回-1。
* @说明			: 打印调试信息。
* @日期			: 2021/04/16
\************************************************************************/
void drvSpDebugPrintf(unsigned char u8Level, const char *pcs8Format, ...);

#endif
