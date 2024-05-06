// TestGDIPlus_AddMemFontDlg.h : header file
//

#pragma once


// CTestGDIPlus_AddMemFontDlg dialog
class CTestGDIPlus_AddMemFontDlg : public CDialog
{
// Construction
public:
	CTestGDIPlus_AddMemFontDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTGDIPLUS_ADDMEMFONT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	Gdiplus::PrivateFontCollection m_fontcollection;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
