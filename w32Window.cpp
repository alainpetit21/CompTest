#ifndef _MT
#define _MT
#endif
#include <process.h>    /* _beginthread, _endthread */
#include "w32Window.h"

CW32Window *gCurrent;

PROFILER_DEC;

long FAR PASCAL 
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message){
		case WM_DESTROY:	PostQuitMessage(0);			return 0;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 1;
}

void
CW32Window::Init(int nW, int nH, int nD, int nFS, char* appName, char* appTitle)
{
	gCurrent		= this;
	m_bFs			= nFS;
	m_nDepth		= nD;
	m_nWidth		= nW;
	m_nHeight		= nH;
	m_bOk			= FALSE;

	strncpy(&m_szAppName[0], appName, 128);
	strncpy(&m_szWindowTitle[0], appTitle, 128);
	_beginthread ((void (__cdecl *)(void *))WinMainThread,0,NULL);
	while(!m_bOk);
	onInit();
}

void	
CW32Window::Exit()
{
	onExit();
}

void 
CW32Window::Run()
{
	MSG							msg;
	while(1){
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			if(msg.message == WM_QUIT){
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		onFrame();
	}
	Exit();
}

int WINAPI 
WinMainThread(HINSTANCE hInstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) 
{
	gCurrent->m_hInstance	= hInstance;
	gCurrent->m_procHandler	= (WNDPROC)WindowProc;

	WNDCLASS wc;

	if(gCurrent->m_hWnd= FindWindow(gCurrent->m_szAppName, gCurrent->m_szWindowTitle)){
		SetForegroundWindow(gCurrent->m_hWnd);
	}

	wc.style		= CS_HREDRAW | CS_VREDRAW ;
	wc.lpfnWndProc	= (WNDPROC)gCurrent->m_procHandler;
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hIcon		= 0;
	wc.hInstance	= gCurrent->m_hInstance;
	wc.hCursor		= 0;
	wc.hbrBackground= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName= gCurrent->m_szAppName;
	
	RegisterClass(&wc);

	gCurrent->m_hWnd= CreateWindow(gCurrent->m_szAppName, gCurrent->m_szWindowTitle, WS_VISIBLE, CW_USEDEFAULT,	
									CW_USEDEFAULT,	gCurrent->m_nWidth, gCurrent->m_nHeight, 0, 0, gCurrent->m_hInstance, 0);

	ShowWindow((HWND)gCurrent->m_hWnd, SW_SHOWNORMAL);
	UpdateWindow((HWND)gCurrent->m_hWnd);

	gCurrent->m_bOk	= TRUE;
	gCurrent->Run();
	return 0;
}
