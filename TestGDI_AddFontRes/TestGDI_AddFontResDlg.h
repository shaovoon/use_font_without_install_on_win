// TestGDI_AddFontResDlg.h : header file
//

#pragma once
#include "../TTFNameLibrary/TTF.h"


// CTestGDI_AddFontResDlg dialog
class CTestGDI_AddFontResDlg : public CDialog
{
// Construction
public:
	CTestGDI_AddFontResDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTGDI_ADDFONTRES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	int m_nResults;
	CString m_szFontFile;
	TTF m_Ttf;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};
