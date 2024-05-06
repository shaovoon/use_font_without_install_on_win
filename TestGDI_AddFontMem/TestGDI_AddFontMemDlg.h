// TestGDI_AddFontMemDlg.h : header file
//

#pragma once
#include "../TTFNameLibrary/TTF.h"


// CTestGDI_AddFontMemDlg dialog
class CTestGDI_AddFontMemDlg : public CDialog
{
// Construction
public:
	CTestGDI_AddFontMemDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTGDI_ADDFONTMEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	HANDLE m_fonthandle;
	TTF m_Ttf;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};
