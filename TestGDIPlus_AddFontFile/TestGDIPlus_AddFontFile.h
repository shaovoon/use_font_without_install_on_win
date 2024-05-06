// TestGDIPlus_AddFontFile.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTestGDIPlus_AddFontFileApp:
// See TestGDIPlus_AddFontFile.cpp for the implementation of this class
//

class CTestGDIPlus_AddFontFileApp : public CWinApp
{
public:
	CTestGDIPlus_AddFontFileApp();
	~CTestGDIPlus_AddFontFileApp();

private:
	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTestGDIPlus_AddFontFileApp theApp;