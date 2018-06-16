/*   Multiply-free Arithmetic Coding Implementation - Compression version 0.0.0
 
 
     Copyright Feb. 1993
              
     Gordon V. Cormack  Feb. 1993
     University of Waterloo
     cormack@uwaterloo.ca
 
 
     All rights reserved.
 
     This code and the algorithms herein are the property of Gordon V. Cormack.
 
     Neither the code nor any algorithm herein may be included in any software,
     device, or process which is sold, exchanged for profit, or for which a 
     licence or royalty fee is charged.
 
     Permission is granted to use this code for educational, research, or
     commercial purposes, provided this notice is included, and provided this
     code is not used as described in the above paragraph.
 
*/
 
/* 
     This code uses a one-byte finite state predictor to drive an arithmetic
     coder for data compression.  It should give compression nearly identical
     to one-byte huffman coding.
 
     Find a better predictor, and you'll have a better compressor!

     It uses no floating point, no multiplies, no divides during compression.

     ***EOF is handled in a non-general way; (char)EOF is stuffed into the
     compressed stream.
 
*/
 
#define THRESH 0

#include <stdio.h>

int diff[0x10000];
int inc0tab [0x10000];
int inc1tab [0x10000];
int cnt[0x100];
int outByte;


#define INC0(x) pinc0tab[(unsigned int)(x)]
#define INC1(x) pinc1tab[(unsigned int)(x)]
#define DIFF(x) pdiff[(unsigned int)(x)]
#define CNT1(x) ((x&0xff)+1)
#define CNT0(x) ((x>>8)+1)

void init()
{
int i,j;

	for (i=0;i<256;i++){
		for (j=0;j<256;j++){
			if(i != 255)
				inc0tab[(i<<8)+j] = ((i+1)<<8) + j;
			else
				inc0tab[(i<<8)+j] = (127<<8) + (j>>1);
	
			if (j != 255)
				inc1tab[(i<<8)+j] = (i<<8) + j + 1;
			else
				inc1tab[(i<<8)+j] = ((i>>1)<<8) + 127;

			if (i < j) {
				diff[(i<<8)+j] = 175.0 * (i+1) / (i+j+2);
				
				if (!diff[(i<<8)+j]) 
					diff[(i<<8)+j]++;
			}else{
				diff[(i<<8)+j] = -175.0 * (j+1) / (i+j+2);
				
				if (!diff[(i<<8)+j]) 
					diff[(i<<8)+j]--;
			}
		}
	}
}

int 
ARITHM_Compress(unsigned char* p_pDst, unsigned char* p_pSrc, int pSrcLen)
{
register int			*pinc0tab;
register int			*pinc1tab;
register int			*pdiff;
register int			index;
register int			i;
register int			count	= -24;
register int			space	= 0xff;
register int			min		= 0;
register unsigned char	c		= 0;
register unsigned int	last	= 0;
unsigned char			*pSrc	= p_pSrc;
unsigned char			*pDst	= p_pDst;

	init();

	pinc0tab = inc0tab;
	pinc1tab = inc1tab;
	pdiff = diff;

	do{
		register mask;
		c = 0xff & *pSrc++;
		index = 1;
		
		for (mask = 0x80;mask;mask>>=1){
			register l = last  + index;
			register a = cnt[l];
			register x = DIFF(a);
				
			if (x>0){
				if (c & mask) { 
					space -=x;
					cnt[l] = INC1(a);
					index += index + 1;
				}else { 
					min += space-x;
					space = x;
					cnt[l] = INC0(a);
					index += index;
		        }
			}else{
				if (!(c & mask)) {
					space +=x; 
					cnt[l] = INC0(a);
					index += index;
				}else{ 
					min += space+x; 
					space = -x;
					cnt[l] = INC1(a);
					index += index+1;
				}
			}
			while ((space) < 128){
				space <<= 1;
				min <<= 1;
				if (!++count) {
					count = -8;
					*pDst++= (min >> 24);
					++outByte;
					min &= 0xffffff;
					if (0xffffff - min < space){
						space = 0xffffff - min;
					}
				}
			}
		}
	}while (pSrc < (p_pSrc + pSrcLen));

//	fprintf(stderr,"count %d min %x minshift %x\n",count&7,min,min<<(8-(count&0x7)));

	min <<= 8-(count&7);
	
	*pDst++= (min>>24);
	*pDst++= ((min>>16) & 0xff);
	*pDst++= ((min>>8) & 0xff);
	*pDst++= (min & 0x00ff);

	return outByte;
}

