/*****************************************************************************/
/*             COPYRIGHT    TASSON     CO.,LTD                               */
/*****************************************************************************/
/**
* @fileName:calcCheckSum.h
*
*
* @note:校验和计算头文件。
*
* None.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00  xx   07/22/20 First release
*
*
* </pre>
*
******************************************************************************/

#ifndef CALCCHECKSUM_H
#define CALCCHECKSUM_H

#include <stdio.h>

unsigned char AppCalculateCRC8(unsigned char * pucData, unsigned int iLength);

unsigned int AppCalculateCRC32(unsigned char * pucData, unsigned int iLength);

unsigned int AppCalcCKS(unsigned char * pucData, unsigned int iLength);     

unsigned long AppCRC32Value(int i);

unsigned long AppCalculateBlockCRC32(unsigned long ulCount, unsigned char *ucBuffer);

#endif // CALCCHECKSUM_H
