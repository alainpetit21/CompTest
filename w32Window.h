
#ifndef _W32WINDOW_H_
#define _W32WINDOW_H_

/*Include*/
#define WIN32_LEAN_AND_MEAN	/*For the rest of the w32*/
#include <windows.h>		/*For the rest of the w32*/
#include <windowsx.h>		/*For the rest of the w32*/

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef char s8;
typedef short s16;
typedef long s32;

/*Some Typedef*/
class CW32Window{
public:
	void			Init(int nW, int nH, int nD, int nFS, char* appName, char* appTitle);
	void			Run();
	void			Exit();
	virtual void	onFrame(){};
	virtual void	onInit(){};
	virtual void	onExit(){};

	unsigned int	m_nWidth;
	unsigned int	m_nHeight;
	unsigned int	m_nDepth;
	BOOL			m_bFs;
	BOOL			m_bOk;
	char			m_szWindowTitle[128];
	char			m_szAppName[128];
	WNDPROC			m_procHandler;
	HINSTANCE		m_hInstance;
	HWND			m_hWnd;
};

extern	int WINAPI		WinMainThread(HINSTANCE hInstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow);
extern long FAR PASCAL	WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/*==============================================================================================================*/
/*==============================================================================================================*/
#undef PROFILER_INC
#undef PROFILER_DEC
#undef PROFILER_SEE
#undef PROFILER_INIT
#undef PROFILER_BEFORE
#undef PROFILER_AFTER
  #if defined(_POCKET_PC)
    #define PROFILER_INC
    #define PROFILER_DEC	\
      __int64 freq;				\
      __int64 timePrev;		\
      __int64 time;				\
      wchar_t  buf[64];														      
    #define PROFILER_SEE	\
      extern __int64 freq;				\
      extern __int64 timePrev;		\
      extern __int64 time;				\
      extern wchar_t  buf[64];														      
   #define PROFILER_INIT					\
    ::QueryPerformanceFrequency(	\
      (LARGE_INTEGER*)&freq);			\
    buf[0]='\0';
  #define PROFILER_BEFORE				\
    ::QueryPerformanceCounter(	\
      (LARGE_INTEGER*)&time);		\
    timePrev= time;
  #define PROFILER_AFTER(string, returnLine)\
    ::QueryPerformanceCounter(							\
      (LARGE_INTEGER*)&time);								\
    swprintf(buf,_T"%s%d%s", string,				\
      (uint)(time-timePrev), returnLine);		\
    OutputDebugString(buf);
  #elif defined(WIN32)
    #define PROFILER_INC
    #define PROFILER_DEC	\
      __int64 freq;				\
      __int64 timePrev;		\
      __int64 time;				\
      char  buf[64];														      
    #define PROFILER_SEE	\
      extern __int64 freq;				\
      extern __int64 timePrev;		\
      extern __int64 time;				\
      extern char  buf[64];														      
   #define PROFILER_INIT					\
    ::QueryPerformanceFrequency(	\
      (LARGE_INTEGER*)&freq);			\
    buf[0]='\0';
  #define PROFILER_BEFORE				\
    ::QueryPerformanceCounter(	\
      (LARGE_INTEGER*)&time);		\
    timePrev= time;
  #define PROFILER_AFTER(string, returnLine)\
    ::QueryPerformanceCounter(							\
      (LARGE_INTEGER*)&time);								\
    printf("%s%d%s", string,				\
      (unsigned int)(time-timePrev), returnLine);		\
//    OutputDebugString(buf);
  #else
	  #define PROFILER_INC
	  #define PROFILER_DEC
	  #define PROFILER_INIT
	  #define PROFILER_BEFORE
	  #define PROFILER_AFTER(string, returnLine)
  #endif	//	_POCKET_PC
/*==============================================================================================================*/
/*==============================================================================================================*/

#endif // _W32WINDOW_H_
