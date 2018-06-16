#ifndef _MYWINDOW_H_
#define _MYWINDOW_H_

/*Include*/
#include "w32Window.h"
#include "DxRender.h"

/*Some Typedef*/
class CMyWindow : public CW32Window {
public:
	DXRender			renderer;
	static CMyWindow	*cela;
	char				m_EEprom[10240];
	char				m_Data[10240];
	char				m_DataUnc[10240];

	int					m_cptSrc;
	int					m_cptDst;

	CMyWindow():CW32Window(){cela= this;}
	virtual void	onFrame();
	virtual void	onInit();
	virtual void	onExit();
};
#endif // _MYWINDOW_H_