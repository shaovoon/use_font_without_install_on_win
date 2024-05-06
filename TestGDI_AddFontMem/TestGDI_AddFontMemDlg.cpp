// TestGDI_AddFontMemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestGDI_AddFontMem.h"
#include "TestGDI_AddFontMemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestGDI_AddFontMemDlg dialog




CTestGDI_AddFontMemDlg::CTestGDI_AddFontMemDlg(CWnd* pParent /*=NULL*/)
	: 
	CDialog(CTestGDI_AddFontMemDlg::IDD, pParent),
	m_fonthandle(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestGDI_AddFontMemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestGDI_AddFontMemDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTestGDI_AddFontMemDlg message handlers

BOOL CTestGDI_AddFontMemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	HINSTANCE hResInstance = AfxGetResourceHandle( );

	HRSRC res = FindResource(hResInstance,
		MAKEINTRESOURCE(IDR_MYFONT),L"BINARY");
	if (res) 
	{
		HGLOBAL mem = LoadResource(hResInstance, res);
		void *data = LockResource(mem);
		size_t len = SizeofResource(hResInstance, res);

		DWORD nFonts;
		m_fonthandle = AddFontMemResourceEx(
			data,       // font resource
			len,       // number of bytes in font resource 
			NULL,          // Reserved. Must be 0.
			&nFonts      // number of fonts installed
			);

		m_Ttf.Parse((BYTE*)(data), len);

		if(m_fonthandle==0)
		{
			MessageBox(L"Font add fails", L"Error");
		}
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestGDI_AddFontMemDlg::OnPaint()
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
HCURSOR CTestGDI_AddFontMemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestGDI_AddFontMemDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if(m_fonthandle)
	{
		BOOL b = RemoveFontMemResourceEx(m_fonthandle);
		if(b==0)
		{
			MessageBox(L"Font remove fails", L"Error");
		}
	}
}
