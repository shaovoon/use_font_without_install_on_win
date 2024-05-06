// TestGDI_AddFontResDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestGDI_AddFontRes.h"
#include "TestGDI_AddFontResDlg.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestGDI_AddFontResDlg dialog




CTestGDI_AddFontResDlg::CTestGDI_AddFontResDlg(CWnd* pParent /*=NULL*/)
	: 
CDialog(CTestGDI_AddFontResDlg::IDD, pParent),
m_nResults(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestGDI_AddFontResDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestGDI_AddFontResDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTestGDI_AddFontResDlg message handlers

BOOL CTestGDI_AddFontResDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	wchar_t FullPath[MAX_PATH];
	memset( FullPath, 0, sizeof(FullPath) );
	CString szExePath;
	if (::GetModuleFileNameW( NULL, FullPath, sizeof(wchar_t)*MAX_PATH))
	{
		szExePath = FullPath;

		int pos = szExePath.ReverseFind( L'\\' );

		if( -1 != pos )
		{
			szExePath = szExePath.Left(pos+1);
		}
	}

	m_szFontFile = szExePath + L"SkiCargo.ttf";

	m_nResults = AddFontResourceEx(
		m_szFontFile, // font file name
		FR_PRIVATE,             // font characteristics
		NULL);

	m_Ttf.Parse((LPCWSTR)(m_szFontFile));

	if(m_nResults==0)
	{
		MessageBox(L"Font add fails", L"Error");
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestGDI_AddFontResDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();
		CPaintDC dc(this);
		CDC* pDC = &dc;

		pDC->SetBkMode(TRANSPARENT);

		LOGFONT lf;
		memset(&lf, 0, sizeof(lf));
		lf.lfHeight = -MulDiv(24, pDC->GetDeviceCaps(LOGPIXELSY), 72);
		lf.lfWeight = FW_NORMAL;
		lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
		//wcscpy_s(lf.lfFaceName, L"SkiCargo");
		wcscpy_s(lf.lfFaceName, m_Ttf.GetFontFamilyName().c_str());

		// create and select it
		CFont newFont;
		if (!newFont.CreateFontIndirect(&lf))
			return;
		CFont* pOldFont = pDC->SelectObject(&newFont);

		// use a path to record how the text was drawn
		wchar_t buf[] = _T("The quick brown fox jumps over the lazy dog!");
		pDC->TextOut( 10, 10, buf, wcslen(buf));

		// Put back the old font
		pDC->SelectObject(pOldFont);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestGDI_AddFontResDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestGDI_AddFontResDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if(m_nResults!=0)
	{
		BOOL b = RemoveFontResourceEx(
			m_szFontFile,  // name of font file
			FR_PRIVATE,            // font characteristics
			NULL            // Reserved.
			);

		if(b==0)
		{
			MessageBox(L"Font remove fails", L"Error");
		}

	}

}
