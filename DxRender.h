#ifndef _DX_RENDER_H_
#define _DX_RENDER_H_

#include <ddraw.h>
#include <math.h>


class DXRender
{
public:
	DXRender():m_bInitiated(FALSE){};
	s32					Init					(s32 width, s32 height);
	s32					Exit					(u8 bReleaseDD);
	IDirectDrawSurface*	GetFrontBuffer			(void);
	IDirectDrawSurface*	GetBackBuffer			(void);
	s32					NewBitmap				(IDirectDrawSurface **ppBitmap, u32 nWidth, u32 nHeight);
	s32					NewBitmapSystem			(IDirectDrawSurface **ppBitmap, u32 nWidth, u32 nHeight);
	s32					FreeBitmap				(IDirectDrawSurface **ppBitmap);
	s32					GetWidth				(IDirectDrawSurface *pBitmap);
	s32					GetHeight				(IDirectDrawSurface *pBitmap);
	s32					PutBitmap_i				(IDirectDrawSurface *pDestBitmap, s32 nDx, s32 nDy, s32 nDw, s32 nDh, s32 nMode, 
												 IDirectDrawSurface *pSrcBitmap, s32 nSx, s32 nSy, s32 nSw, s32 nSh);
	IDirectDrawSurface* BeginScene				(void);
	void				EndScene				(IDirectDrawSurface** p_bit);
	s32					FillBitmap				(IDirectDrawSurface *pBitmap, s32 lColor);
	s32					FillRect_i				(IDirectDrawSurface *pBitmap, s32 left, s32 top, s32 right, s32 bottom, s32 lColor);
	void				DrawRect_i				(IDirectDrawSurface *pBitmap, s32 left, s32 top, s32 right, s32 bottom, s32 lColor);
	u16					GetPixel_i				(IDirectDrawSurface *pBitmap, u32 nX, u32 nY);
	void				PutPixel_i				(IDirectDrawSurface *pBitmap, u32 nX, u32 nY, s32 lColor);
	u16*				LockSurface				(IDirectDrawSurface *pBitmap);
	u16*				LockBB					(void);
	void				PutPixel_locked			(u32 *pImage, u32 nX, u32 nY, s32 lColor);
	void				PutPixel32_locked		(u32 *pImage, u32 nX, u32 nY, s32 lColor);
	void				PutPixel16_locked		(u16 *pImage, u32 nX, u32 nY, s32 lColor);
	void				DrawLine_locked			(u32 *pImage, s32 X0, s32 Y0, s32 X1, s32 Y1, s32 width, s32 lColor);
	void				DrawLineOctant0			(u32 *pImage, s32 lColor, u32 X0, u32 Y0, u32 DeltaX, u32 DeltaY, s32 XDirection);
	void				DrawLineOctant1			(u32 *pImage, s32 lColor, u32 X0, u32 Y0, u32 DeltaX, u32 DeltaY, s32 XDirection);
	void				DrawCircle_locked		(u32 *pImage, s32 p_ptX, s32 p_ptY, s32 radius, s32 lColor);
	void				UnLockSurface			(IDirectDrawSurface *pBitmap, u16 *pImage);
	void				UnLockBB				(u16 *pImage);
	void				DrawLine				(IDirectDrawSurface *pBitmap, float fX1, float fY1, float fX2, float fY2, s32 lColor);
	u8					BitmapDoClipping		(s32& nDx, s32& nDy, s32& nDw, s32& nDh, s32& nSx, s32& nSy, s32& nSw, s32& nSh);
	IDirectDrawClipper*	AttachClipper			(IDirectDrawSurface* surface, s32 RectCount, LPRECT ClipList);
	DWORD				DDColorMatch			(IDirectDrawSurface *pdds, COLORREF rgb);
	HRESULT				DDSetColorKey			(IDirectDrawSurface *pdds, COLORREF rgb);

	RECT				RendererRect;
	RECT				ViewRect;
	DDBLTFX				m_blitFX;
	DDSURFACEDESC		m_ddsd;
	DDSCAPS				m_ddscaps;
	DDPIXELFORMAT		m_ddpf;
	HWND				m_frameHWnd;
	IDirectDrawSurface*	m_pBB;
	IDirectDrawSurface*	m_pFB;
	IDirectDraw*		m_pDD;
	IDirectDrawClipper*	m_pClipper;
	IDirectDrawClipper*	m_pWindowClipper;
	s32					m_pixelFormat;
	s32					m_windowClientX0;
	s32					m_windowClientY0;
	s32					m_bFS;
	s32					m_nWidth, m_nHeight;
	BOOL				m_bInitiated;
};
#endif // _DX_RENDER_H_