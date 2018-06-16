#ifndef _ZIP_H_
#define _ZIP_H_

extern int ZIP_Compress(unsigned char* p_pDst, unsigned char* p_pSrc, int pSrcLen);
extern void ZIP_Decompress(unsigned char* p_pDst, unsigned char* p_pSrc);


#endif /*_ZIP_H_*/