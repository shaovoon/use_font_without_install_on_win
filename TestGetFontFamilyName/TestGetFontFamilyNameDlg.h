// TestGetFontFamilyNameDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CTestGetFontFamilyNameDlg dialog
class CTestGetFontFamilyNameDlg : public CDialog
{
// Construction
public:
	CTestGetFontFamilyNameDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTGETFONTFAMILYNAME_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CString m_szExt;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtFile;
	CEdit m_edtInfo;
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnParse();
};
