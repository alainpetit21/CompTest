#include "Mywindow.h"
#include <stdio.h>
#include "dmc.h"
#include "lbe.h"
#include "arithm.h"
PROFILER_SEE;

CMyWindow	*CMyWindow::cela;

static inline void
EnhancedQuickSortBrute(u8 *a, int lo, int hi)
{
u8	T;

	if ((hi-lo) == 1){
		if(a[hi] > a[lo]){
			T = a[lo];
			a[lo] = a[hi];
			a[hi] = T;
		}
	}
	if ((hi-lo) == 2){
		s8 pmin = (a[lo] > a[lo+1])? lo : lo+1;
		pmin = (a[pmin] > a[lo+2])? pmin : lo+2;

		if (pmin != lo){
			T = a[lo];
			a[lo] = a[pmin];
			a[pmin] = T;
		}
		EnhancedQuickSortBrute(a, lo+1, hi);
	}
	if ((hi-lo) == 3){
		s8 pmin = (a[lo] > a[lo+1])? lo : lo+1;
		pmin = (a[pmin] > a[lo+2])? pmin : lo+2;
		pmin = (a[pmin] > a[lo+3])? pmin : lo+3;
		if (pmin != lo){
			T = a[lo];
			a[lo] = a[pmin];
			a[pmin] = T;
		}
		{
			s8 pmax = (a[hi] < a[hi-1])? hi : hi-1;
			pmax = (a[hi] < a[hi-2])? hi : hi-2;
			if (pmax != hi){
				T = a[hi];
				a[hi] = a[pmax];
				a[pmax] = T;
			}
		}
		EnhancedQuickSortBrute(a, lo+1, hi-1);
	}
}

static void
EnhancedQuickSort(u8 *a, int lo0, int hi0)
{
s8			lo		= lo0;
s8			hi		= hi0;
u8			pivot;
u8			T;

	if ((hi-lo) <= 3){
		EnhancedQuickSortBrute(a, lo, hi);
		return;
	}

	pivot = a[(lo + hi) >> 1];
	a[(lo + hi) >> 1] = a[hi];
	a[hi] = pivot;

	while( lo < hi ){
		while((lo < hi) && (a[lo] > pivot)){
			lo++;
		}
		while((lo < hi) && (pivot >= a[hi])){
			hi--;
		}
		if( lo < hi ){
			T = a[lo];
			a[lo] = a[hi];
			a[hi] = T;
		}
	}

	a[hi0] = a[hi];
	a[hi] = pivot;

	EnhancedQuickSort(a, lo0, lo-1);
	EnhancedQuickSort(a, hi+1, hi0);
}



void	
CMyWindow::onExit()
{
	renderer.Exit(1);
}

void	
CMyWindow::onInit()
{

	renderer.m_frameHWnd= m_hWnd;
	renderer.Init(m_nWidth, m_nHeight);

}
#define SIZE_SOURCE 560

void	
CMyWindow::onFrame()
{
int i;
	if(!renderer.m_bInitiated)	return;

	//Init 
	m_cptSrc= 0;
	m_cptDst= 0;
	
	//Rand Data
	for(i=0 ;i < SIZE_SOURCE;++i){
		m_Data[i]= i%256;
	}

//	EnhancedQuickSort((u8*)m_Data, 0, (SIZE_SOURCE)-1);

	m_cptDst= ARITHM_Compress((unsigned char*)m_EEprom, (unsigned char*)m_Data, SIZE_SOURCE);

	memset(m_DataUnc, 0x0, SIZE_SOURCE);

//	LBE_Decompress((unsigned char*)m_DataUnc, (unsigned char*)m_EEprom);
	
//	for(i=0 ;i < SIZE_SOURCE;++i){
//		if(m_Data[i] != m_DataUnc[i])
//			int otot=23;
//	}
	//Draw
	IDirectDrawSurface* bb= renderer.BeginScene();
	renderer.FillBitmap(bb, 0x0);

	unsigned short *pPixel= renderer.LockBB();
	
	renderer.DrawLine_locked((unsigned long*)pPixel, 0, 100, SIZE_SOURCE, 100, 1, 0x00FF0000);
	renderer.DrawLine_locked((unsigned long*)pPixel, 0, 101, 512, 101, 1, 0x0000FF00);
	for(i=2; i < 16; ++i){
		renderer.DrawLine_locked((unsigned long*)pPixel, 0, i+100, m_cptDst<512?m_cptDst:512, i+100, 1, 0x0000FF00);
	}
	if(m_cptDst > 512){
		for(i=0; i < 16; ++i){
			renderer.DrawLine_locked((unsigned long*)pPixel, 512, i+100, m_cptDst, i+100, 1, 0x00FF0000);
		}
	}
	renderer.UnLockBB(pPixel);
	renderer.EndScene(&bb);

}





















