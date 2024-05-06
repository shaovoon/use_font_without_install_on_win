// TestGetFontFamilyNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestGetFontFamilyName.h"
#include "TestGetFontFamilyNameDlg.h"
#include "../TTFNameLibrary/TTF.h"
#include "../TTFNameLibrary/TTC.h"
#include <sstream>
#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestGetFontFamilyNameDlg dialog




CTestGetFontFamilyNameDlg::CTestGetFontFamilyNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestGetFontFamilyNameDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestGetFontFamilyNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_FILE, m_edtFile);
	DDX_Control(pDX, IDC_EDT_INFO, m_edtInfo);
}

BEGIN_MESSAGE_MAP(CTestGetFontFamilyNameDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CTestGetFontFamilyNameDlg::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_PARSE, &CTestGetFontFamilyNameDlg::OnBnClickedBtnParse)
END_MESSAGE_MAP()


// CTestGetFontFamilyNameDlg message handlers

BOOL CTestGetFontFamilyNameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestGetFontFamilyNameDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestGetFontFamilyNameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestGetFontFamilyNameDlg::OnBnClickedBtnBrowse()
{
	wchar_t szFilters[]=
		L"TTF Font Files (*.ttf)|*.ttf|OTF Font Files (*.otf)|*.otf|TTC Font Files (*.ttc)|*.ttc|All Files (*.*)|*.*||";

	CFileDialog fileDlg (TRUE, L"ttf", NULL,
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY|OFN_EXPLORER, szFilters, this);
	if( fileDlg.DoModal ()==IDOK )
	{
		m_edtFile.SetWindowText(fileDlg.GetPathName());
		m_szExt=fileDlg.GetFileExt();
		m_szExt.MakeLower();
		OnBnClickedBtnParse();
	}
}

void CTestGetFontFamilyNameDlg::OnBnClickedBtnParse()
{
	CString szFile;
	m_edtFile.GetWindowText(szFile);
	if(szFile.IsEmpty())
	{
		MessageBox(L"Font file is not specified!", L"Error", MB_ICONERROR);
		return;
	}
	if(FALSE==PathFileExists(szFile))
	{
		MessageBox(L"Font file does not exist!", L"Error", MB_ICONERROR);
		return;
	}

	if(m_szExt=="ttf"||m_szExt=="otf")
	{
		TTF ttf;
		if( false == ttf.Parse((LPCWSTR)(szFile)) )
		{
			MessageBox(L"Font file cannot be parsed!", L"Error", MB_ICONERROR);
			return;
		}
		else
		{
			std::wostringstream stream;
			stream<<L"FontName : "<<ttf.GetFontName()<<L"\r\n\r\n";
			stream<<L"Copyright : "<<ttf.GetCopyright()<<L"\r\n\r\n";
			stream<<L"FontFamilyName : "<<ttf.GetFontFamilyName()<<L"\r\n\r\n";
			stream<<L"FontSubFamilyName : "<<ttf.GetFontSubFamilyName()<<L"\r\n\r\n";
			stream<<L"FontID : "<<ttf.GetFontID()<<L"\r\n\r\n";
			stream<<L"Version : "<<ttf.GetVersion()<<L"\r\n\r\n";
			stream<<L"PostScriptName : "<<ttf.GetPostScriptName()<<L"\r\n\r\n";
			stream<<L"Trademark : "<<ttf.GetTrademark()<<L"\r\n\r\n";

			m_edtInfo.SetWindowText(stream.str().c_str());
		}
	}
	if(m_szExt=="ttc")
	{
		TTC ttc;
		if( false == ttc.Parse((LPCWSTR)(szFile)) )
		{
			MessageBox(L"Font file cannot be parsed!", L"Error", MB_ICONERROR);
			return;
		}
		else
		{
			CString szStr=L"";
			for(size_t i=0; i<ttc.Size(); ++i)
			{
				std::wostringstream stream;
				stream<<L"FontName : "<<ttc.GetFontName(i)<<L"\r\n\r\n";
				stream<<L"Copyright : "<<ttc.GetCopyright(i)<<L"\r\n\r\n";
				stream<<L"FontFamilyName : "<<ttc.GetFontFamilyName(i)<<L"\r\n\r\n";
				stream<<L"FontSubFamilyName : "<<ttc.GetFontSubFamilyName(i)<<L"\r\n\r\n";
				stream<<L"FontID : "<<ttc.GetFontID(i)<<L"\r\n\r\n";
				stream<<L"Version : "<<ttc.GetVersion(i)<<L"\r\n\r\n";
				stream<<L"PostScriptName : "<<ttc.GetPostScriptName(i)<<L"\r\n\r\n";
				stream<<L"Trademark : "<<ttc.GetTrademark(i)<<L"\r\n\r\n";

				if(i!=ttc.Size()-1)
					stream<<L"=========================================="<<L"\r\n\r\n";

				szStr += stream.str().c_str();
			}
			m_edtInfo.SetWindowText(szStr);
		}
	}
}
