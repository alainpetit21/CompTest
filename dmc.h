#ifndef _DMC_H_
#define _DMC_H_

extern int DMC_Compress(unsigned char* p_pDst, unsigned char* p_pSrc, int pSrcLen);
extern void DMC_Decompress(unsigned char* p_pDst, unsigned char* p_pSrc);


#endif /*_DMC_H_*/