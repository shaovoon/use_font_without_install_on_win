// TestGDIPlus_AddMemFont.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTestGDIPlus_AddMemFontApp:
// See TestGDIPlus_AddMemFont.cpp for the implementation of this class
//

class CTestGDIPlus_AddMemFontApp : public CWinApp
{
public:
	CTestGDIPlus_AddMemFontApp();
	~CTestGDIPlus_AddMemFontApp();

private:
	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTestGDIPlus_AddMemFontApp theApp;