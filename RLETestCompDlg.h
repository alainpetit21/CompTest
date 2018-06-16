// RLETestCompDlg.h : header file
//

#if !defined(AFX_RLETESTCOMPDLG_H__357C9181_EB22_46D0_95EE_153D522E988B__INCLUDED_)
#define AFX_RLETESTCOMPDLG_H__357C9181_EB22_46D0_95EE_153D522E988B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRLETestCompDlg dialog

class CRLETestCompDlg : public CDialog
{
// Construction
public:
	static CRLETestCompDlg *cela;

	CRLETestCompDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRLETestCompDlg)
	enum { IDD = IDD_RLETESTCOMP_DIALOG };
	CString	m_edit;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRLETestCompDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRLETestCompDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RLETESTCOMPDLG_H__357C9181_EB22_46D0_95EE_153D522E988B__INCLUDED_)
