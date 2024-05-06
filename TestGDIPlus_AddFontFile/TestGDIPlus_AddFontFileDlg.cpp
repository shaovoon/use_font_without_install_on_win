// TestGDIPlus_AddFontFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestGDIPlus_AddFontFile.h"
#include "TestGDIPlus_AddFontFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestGDIPlus_AddFontFileDlg dialog




CTestGDIPlus_AddFontFileDlg::CTestGDIPlus_AddFontFileDlg(CWnd* pParent /*=NULL*/)
	: 
	CDialog(CTestGDIPlus_AddFontFileDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestGDIPlus_AddFontFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestGDIPlus_AddFontFileDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CTestGDIPlus_AddFontFileDlg message handlers

BOOL CTestGDIPlus_AddFontFileDlg::OnInitDialog()
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

	CString szFontFile = szExePath + L"SkiCargo.ttf";

	Gdiplus::Status nResults = m_fontcollection.AddFontFile(szFontFile);

	if(nResults!=Gdiplus::Ok)
	{
		MessageBox(L"Font add fails", L"Error");
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestGDIPlus_AddFontFileDlg::OnPaint()
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

		using namespace Gdiplus;
		Graphics graphics(dc.GetSafeHdc());
		graphics.SetSmoothingMode(SmoothingModeAntiAlias);
		graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
		SolidBrush brush(Color(0,0,0));
		//Font font(pDC->GetSafeHdc(), &lf);
		FontFamily fontFamily;
		int nNumFound=0;
		m_fontcollection.GetFamilies(1,&fontFamily,&nNumFound);

		if(nNumFound>0)
		{
			Font font(&fontFamily,28,FontStyleRegular,UnitPixel);

			StringFormat strformat;
			wchar_t buf[] = L"The quick brown fox jumps over the lazy dog!";
			graphics.DrawString(buf,wcslen(buf),&font,PointF(10.0f,10.0f),&strformat,&brush);
		}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestGDIPlus_AddFontFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

