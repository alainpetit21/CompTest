#include <malloc.h>
#include <memory.h>
#include <stdio.h>

typedef struct nnn {
	float		count[2];
	struct nnn	*next[2];
}node;

static node *p;
static node nodes[256][256];
int nbNodeNeeded;

float	DMC_Redict();
void	DMC_Init(int memsize);
void	DMC_Update(int b);
void	DMC_Flush();
void	DMC_Reset();

int memsize = 0x1000000;

int
DMC_Compress(unsigned char* p_pDst, unsigned char* p_pSrc, int pSrcLen)
{
int				mid;
int				c;
int				i;
int				bit;
int				max			= 0x1000000;
int				min			= 0;
int				inbytes		= 0;
int				outbytes	= 3;
int				pout		= 3;
unsigned char	*pSrc		= p_pSrc;
unsigned char	*pDst		= p_pDst;

   
	DMC_Init(memsize);
	nbNodeNeeded= 0;

	while(1){
		c = *pSrc++;
		if(pSrc > (&p_pSrc[pSrcLen])) {
			min= max-1;
			break;
		}
		for(i=0;i<8;i++){
			bit = (c << i) & 0x80;
			mid = min + (max-min-1) * DMC_Redict();
			DMC_Update(bit != 0);
		
			if(mid == min)
				mid++;
			if(mid == (max-1)) 
				mid--;

			if(bit) { 
				min = mid;
			}else{
				max = mid;
			}
		
			while((max-min) < 256) {
				if(bit)
					max--;

				*pDst++= (min >> 16);
				outbytes++;
				min= (min << 8) & 0xffff00;
				max= ((max << 8) & 0xffff00);

				if(min >= max) 
					max = 0x1000000;
			}
		}
		if(!(++inbytes & 0xff)){
			if (outbytes - pout > 256) {
				DMC_Flush();
			}
			pout = outbytes;
		}
	}

	*pDst++= (min>>16);
	*pDst++= ((min>>8) & 0xff);
	*pDst++= (min & 0x00ff);

	return pout;
}

void
DMC_Decompress(unsigned char* p_pDst, unsigned char* p_pSrc)
{
int				mid;
int				val;
int				i;
int				bit;
int				c;
int				max			= 0x1000000;
int				min			= 0;
int				inbytes		= 3;
int				pin			= 3;
int				outbytes	= 0;
unsigned char	*pSrc		= p_pSrc;
unsigned char	*pDst		= p_pDst;

	DMC_Init(memsize);
	nbNodeNeeded= 0;

	val = (*pSrc++)<<16;
	val += (*pSrc++)<<8;
	val += (*pSrc++);

	while(1){
		c= 0;

		if(val == (max-1)){
			break;
		}

		for(i=0;i<8;i++){
			mid= min + (max-min-1) * DMC_Redict();

			if(mid == min) 
				mid++;
			if(mid == (max-1)) 
				mid--;
			if(val >= mid){
				bit = 1;
				min = mid;
			}else{
				bit = 0;
				max = mid;
			}

			DMC_Update(bit != 0);
			
			c= c + c + bit;
			while((max-min) < 256){
				if(bit)
					max--;

				inbytes++;
				val = (val << 8) & 0xffff00 | ((*pSrc++) & 0xff);
				min = (min << 8) & 0xffff00;
				max = ((max << 8) & 0xffff00 ) ;

				if(min >= max) 
					max= 0x1000000;
			}
		}

		*pDst++= c;
		
		if(!(++outbytes & 0xff)){
			if(inbytes - pin > 256){
				DMC_Flush();
			}
			pin = inbytes;
		}
	}
}

void 
DMC_Init(int memsize)
{
	DMC_Flush();
}

void
DMC_Flush()
{
int i,j;

   for(j=0;j<256;j++){
      for(i=0;i<127;i++){
         nodes[j][i].count[0]	= 0.2;
         nodes[j][i].count[1]	= 0.2;
         nodes[j][i].next[0]	= &nodes[j][2*i + 1];
         nodes[j][i].next[1]	= &nodes[j][2*i + 2];
      }
      for (i=127;i<255;i++) {
         nodes[j][i].count[0]	= 0.2;
         nodes[j][i].count[1]	= 0.2;
         nodes[j][i].next[0]	= &nodes[i+1][0];
         nodes[j][i].next[1]	= &nodes[i-127][0];
      }
   }

   DMC_Reset();
}

void
DMC_Reset()
{
   p = &nodes[0][0];
}

float 
DMC_Redict()
{
   return(p->count[0] / (p->count[0] + p->count[1]));
}

void
DMC_Update(int b)
{
float r;

   p->count[b]++;
   p = p->next[b];
	++nbNodeNeeded;

}


/*

#include <stdio.h>

float predict();
int pinit();
int pupdate();

int memsize = 0x1000000;

main(argc,argv)
   int argc;
   char *argv[];
{
   if (argc == 3 && isdigit(*argv[2])) sscanf(argv[2],"%d",&memsize);
   if (argc >= 2 && *argv[1] == 'c') comp();
   else if (argc >= 2 && *argv[1] == 'e') exp();
   else {
      fprintf(stderr,"usage:  dmc [ce] memsize <infile >outfile\n");
      exit(1);
   }
   return 0;
}

comp(){
   int max = 0x1000000,
       min = 0,
       mid,
       c,i,
       inbytes = 0, 
       outbytes =3,
       pout = 3,
       bit;
   
   pinit(memsize);
   
   for(;;){
      c = getchar();
      if (c == EOF) {
         min = max-1;
         fprintf(stderr,"compress done: bytes in %d, bytes out %d, ratio %f\n",
                         inbytes,outbytes,(float)outbytes/inbytes);
         break;
      }
      for (i=0;i<8;i++){
         bit = (c << i) & 0x80;
         mid = min + (max-min-1) * predict();
         pupdate(bit != 0);
         if (mid == min) mid++;
         if (mid == (max-1)) mid--;
   
         if (bit) { 
            min = mid;
         } else {
            max = mid;
         }
         while ((max-min) < 256) {
            if(bit)max--;
            putchar(min >> 16);
            outbytes++;
            min = (min << 8) & 0xffff00;
            max = ((max << 8) & 0xffff00 ) ;
            if (min >= max) max = 0x1000000;
         }
      }
      if(!(++inbytes & 0xff)){
         if(!(inbytes & 0xffff)){
               fprintf(stderr,
                       "compressing... bytes in %d, bytes out %d, ratio %f\r",
                       inbytes,outbytes,(float)outbytes/inbytes);
         }
         if (outbytes - pout > 256) {
            pflush();
         }
         pout = outbytes;
      }
   }
   putchar(min>>16);
   putchar((min>>8) & 0xff);
   putchar(min & 0x00ff);
}


exp(){
   int max = 0x1000000,
       min = 0,
       mid,
       val,
       i,
       inbytes=3,
       pin=3,
       outbytes=0,
       bit,
       c;
   
   pinit(memsize);
   
   val = getchar()<<16;
   val += getchar()<<8;
   val += getchar();
   while(1) {
      c = 0;
      if (val == (max-1)) {
         fprintf(stderr,"expand: input %d output %d\n",inbytes,outbytes);
         break;
      }
      for (i=0;i<8;i++){
         mid = min + (max-min-1)*predict();
         if (mid == min) mid++;
         if (mid == (max-1)) mid--;
         if (val >= mid) {
            bit = 1;
            min = mid;
         } else {
            bit = 0;
            max = mid;
         }
         pupdate(bit != 0);
         c = c + c + bit;
         while ((max-min) < 256) {
            if(bit)max--;
            inbytes++;
            val = (val << 8) & 0xffff00 | (getchar()& 0xff);
            min = (min << 8) & 0xffff00;
            max = ((max << 8) & 0xffff00 ) ;
            if (min >= max) max = 0x1000000;
         }
      }
      putchar(c);
      if(!(++outbytes & 0xff)){
         if (inbytes - pin > 256) {
            pflush();
         }
         pin = inbytes;
      }
   }
}

typedef struct nnn {
           float count[2];
           struct nnn    *next[2];
} node;

static int threshold = 2, bigthresh = 2; 

static node *p, *new, nodes[256][256];

static node *nodebuf;
static node *nodemaxp;
static node *nodesptr;

#include <malloc.h>

pinit(memsize)
   int memsize;
{
   fprintf(stderr,"using %d bytes of predictor memory\n",memsize);
   nodebuf = (node *) malloc (memsize);
   if (nodebuf == (node *) NULL) {
      fprintf(stderr,"memory alloc failed; try smaller predictor memory\n");
      exit(1);
   }
   nodemaxp = nodebuf + (memsize/sizeof(node)) - 20;
   pflush();
}

pflush(){
   int i,j;
   for (j=0;j<256;j++){
      for (i=0;i<127;i++) {
         nodes[j][i].count[0] = 0.2;
         nodes[j][i].count[1] = 0.2;
         nodes[j][i].next[0] = &nodes[j][2*i + 1];
         nodes[j][i].next[1] = &nodes[j][2*i + 2];
      }
      for (i=127;i<255;i++) {
         nodes[j][i].count[0] = 0.2;
         nodes[j][i].count[1] = 0.2;
         nodes[j][i].next[0] = &nodes[i+1][0];
         nodes[j][i].next[1] = &nodes[i-127][0];
      }
   }
   nodesptr = nodebuf;
   preset();
}

preset(){
   p = &nodes[0][0];
}

float predict(){
   return   p->count[0] / (p->count[0] + p->count[1]);
}

pupdate(b)
   int b;
{
   float r;
   if (p->count[b] >= threshold &&
      p->next[b]->count[0]+p->next[b]->count[1]
       >= bigthresh + p->count[b]){
      new = nodesptr++;
      p->next[b]->count[0] -= new->count[0] =
         p->next[b]->count[0] * 
         (r = p->count[b]/(p->next[b]->count[1]+p->next[b]->count[0]));
      p->next[b]->count[1] -= new->count[1] =
         p->next[b]->count[1] * r;
      new->next[0] = p->next[b]->next[0];
      new->next[1] = p->next[b]->next[1];
      p->next[b] = new;
   }
   p->count[b]++;
   p = p->next[b];
   if (nodesptr > nodemaxp){
      fprintf(stderr,"flushing ...\n");
      pflush();
   }
}
*/