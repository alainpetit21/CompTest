#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include "lbe.h"

typedef struct CompressionData
{
  unsigned char a;
  long int size;
  int tabno;
  int code[17];
};

CompressionData data[256];


unsigned char	r; 
unsigned char	w;
long			siz; 
int				buf[8]; 
char			ftc[40];
char			cf[40];
long			outByte; 

void	LBEI_Q2Sort(int head,int tail);
void	LBEI_Q2Sort1(int head,int tail);
void	LBEI_Xtract(unsigned char* p_pDst, unsigned char* p_pSrc, int pSrcLen);
void	LBEI_AssignTable(unsigned char* p_pDst, unsigned char* p_pSrc, int pSrcLen);
void	LBEI_CodeGen(unsigned char* p_pDst, unsigned char* p_pSrc, int pSrcLen);
void	LBEI_CodeSubs(unsigned char* p_pDst, unsigned char* p_pSrc, int pSrcLen);
int		LBEI_BinSearch(char c);


void 
LBEI_Xtract(unsigned char* p_pDst, unsigned char* p_pSrc, int pSrcLen)
{
CompressionData a[256];
int				sto[256];
int				m;
int				c		= 0;
unsigned char	*pSrc	= p_pSrc;
unsigned char	*pDst	= p_pDst;

	siz=1;

	r= *pSrc++;
	a[int(r)].a= r;

	for(int i=0;i<256;i++){
		a[i].size =0;
	}
	
	m			= int(r);
	a[m].size	= 1;  
	sto[c]		= int(r);
    c++;

	while(pSrc < (p_pSrc + pSrcLen)){
		r= *pSrc++;
		m =int(r);
	
		if(a[m].size!=0){
			a[m].size++;
		}else{
			a[m].a		= r;
			a[m].size	= 1;
			sto[c]		= int(r);
			c++;
			siz++;
		}
	}
	for( i=0;i<siz;i++){
		memcpy(&data[i], &a[sto[i]], sizeof(CompressionData));
	}

	LBEI_Q2Sort(0,siz-1);
}

void 
LBEI_AssignTable(unsigned char* p_pDst, unsigned char* p_pSrc, int pSrcLen)
{
int				row[10];
int				element	=0;
int				notab	=0;
int				ctab	=0;
int				j		=0;
unsigned char	*pSrc	= p_pSrc;
unsigned char	*pDst	= p_pDst;


	for(int k=0;k < 10;k++){
		row[k]=0;
	}

	for(int i=0;j < siz;i++){
		if(element < row[ctab]+2){
			data[j].tabno= ctab;
			element++;
			j++;
		}else{
			row[ctab]++;

			if(ctab<notab){
				ctab++;
				element=0;
				continue;
			}
			if(ctab==notab){
				if(row[ctab]<=2){
					ctab=0;
					element=0;
				}else{
					ctab++;notab++;
					element=0;
				}
			}
		}
	}

	*((unsigned long*)pDst)= siz;
	pDst+= 4;

	for(i= 0; i < siz; i++){
		*pDst++= data[i].a;
	}
}

void 
LBEI_CodeGen(unsigned char* p_pDst, unsigned char* p_pSrc, int pSrcLen)
{
int rows[20];
int id;
int i;
int m		=0;
int n		=0;
int track1	=1;
int track2	=0;

	for(i= 0; i < 20; i++){
		rows[i]=1;
	}

	while(m < siz){
		i=0;

		for(int j= track1;j>=0&&m<siz;j--){    
			n=0;

			for(int h=0;h<track2;h++){
				data[m].code[n]=1; 
				n++;
				data[m].code[n]=1; 
				n++;
			}

			for(int k=0;k<i;k++){
				data[m].code[n]=0;
				n++;
			}

			data[m].code[n]=1;
			n++;
			
			for(int l=0;l<j;l++){
				data[m].code[n]=0;
				n++;
			}

			data[m].code[n]=1; 
			n++;
		
			data[m].code[n]=2;

			i++;
			m++;

			if(data[m].tabno!=track2){
				rows[track2]++;track2=data[m].tabno;j=rows[track2]+1;i=0;//track1=0;
			}
		}
	
		if(m<9){
			rows[track2]++;
		}

		track1++;
	}
}

void 
LBEI_CodeSubs(unsigned char* p_pDst, unsigned char* p_pSrc, int pSrcLen)
{
CompressionData temp;
unsigned char	w;
int				n;
int				dbuf[8];
int				track;
int				track1;
int				track2	= 0;    
unsigned char	*pSrc	= p_pSrc;
unsigned char	*pDst	= p_pDst;


	LBEI_Q2Sort1(0,siz-1);

	r= *pSrc++;

	while(pSrc < (p_pSrc + pSrcLen)){

		track= LBEI_BinSearch(r);

		for(n=0; data[track].code[n] !=2 ; n++){
			buf[track2]= data[track].code[n];
			track2++;

			if(track2==8){
				register int wr=0;
				register int bc;
				
				for(bc=0; bc<8; bc++){
					if(buf[bc]){
						wr= wr + (buf[bc]<<bc);
					}
				}
				
				w = char(wr);
				*pDst++= w;
				++outByte;

				track2=0;
			}
		}
		
		r= *pSrc++;
	}

	if(track2!=0){
		register int wr=0;
		
		for(int i=0;i<track2;i++){
			if(buf[i]){
				wr = wr + (buf[i]<<i);
			}
		}

		w = char(wr);
		*pDst++= w;
		++outByte;
	}
}

int 
LBEI_BinSearch(char c)
{
int beg;
int mid;
int end;
int i;

	beg	= 0;
	end	= siz;
	mid	= siz/2;

	for(i=0; (i < siz) && (data[mid].a != c); i++){
		if(data[mid].a > c){
			end= mid;
			mid= (beg+end)/2;
		}else{
			beg= mid;
			mid= (beg+end)/2;
		}
	}
	
    if(data[mid].a==c){
		return(mid);
    }else{
		return(-1);
	}
}

void 
LBEI_Q2Sort(int head,int tail)
{
CompressionData a;
int				in,
int				h,
int				t;
int				d;

	h=head;
	t=tail;

	while(1){
		while((data[head].size >= data[tail].size) && (head<tail))
			head++;
		while((data[tail].size <= data[head].size) && (tail>head))
			tail--;
		if(head>=tail)
			break;
		a			= data[head];
		data[head]	= data[tail];
		data[tail]	= a;

	}
	
	d= t-h;
	
	if(d < 1){
		return;
	}else if(d==1){
		if(data[t].size>data[h].size){
			a		= data[h];
			data[h]	= data[t];
			data[t]	= a;
		}
		return;
	}
	LBEI_Q2Sort(h,head-1);
	LBEI_Q2Sort(tail,t);
}

void 
LBEI_Q2Sort1(int head,int tail)
{
CompressionData a;
int				in,
int				h,
int				t;
int				d;

	while(1){
		while(data[head].a<=data[tail].a && head<tail)
			head++;
		while(data[tail].a>=data[head].a && tail>head)
			tail--;
		if(head>=tail)
			break;

		a			=data[head];
		data[head]	=data[tail];
		data[tail]	=a;

	}

	d= t-h;
	if(d < 1){
		return;
	}else if(d == 1){
		if(data[t].a<data[h].a){
			a		=data[h];
			data[h]	=data[t];
			data[t]	=a;
		}
		return;
	}
	LBEI_Q2Sort1(h,head-1);
	LBEI_Q2Sort1(tail,t);
}

int
LBE_Compress(unsigned char* p_pDst, unsigned char* p_pSrc, int pSrcLen)
{
	outByte= 0;
	LBEI_Xtract		(p_pDst, p_pSrc, pSrcLen);
	LBEI_AssignTable(p_pDst, p_pSrc, pSrcLen);
	LBEI_CodeGen	(p_pDst, p_pSrc, pSrcLen);
	LBEI_CodeSubs	(p_pDst, p_pSrc, pSrcLen);
	return 0;
}

void
LBE_Decompress(unsigned char* p_pDst, unsigned char* p_pSrc)
{
}

