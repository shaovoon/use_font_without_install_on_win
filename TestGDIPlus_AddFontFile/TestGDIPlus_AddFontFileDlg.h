// TestGDIPlus_AddFontFileDlg.h : header file
//

#pragma once

// CTestGDIPlus_AddFontFileDlg dialog
class CTestGDIPlus_AddFontFileDlg : public CDialog
{
// Construction
public:
	CTestGDIPlus_AddFontFileDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTGDIPLUS_ADDFONTFILE_DIALOG };

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
