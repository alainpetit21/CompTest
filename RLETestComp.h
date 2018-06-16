// RLETestComp.h : main header file for the RLETESTCOMP application
//

#if !defined(AFX_RLETESTCOMP_H__FBBE8152_16CE_47CA_83C6_2A9FDF0C9889__INCLUDED_)
#define AFX_RLETESTCOMP_H__FBBE8152_16CE_47CA_83C6_2A9FDF0C9889__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRLETestCompApp:
// See RLETestComp.cpp for the implementation of this class
//

class CRLETestCompApp : public CWinApp
{
public:
	CRLETestCompApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRLETestCompApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRLETestCompApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RLETESTCOMP_H__FBBE8152_16CE_47CA_83C6_2A9FDF0C9889__INCLUDED_)
