// TestGDI_AddFontRes.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTestGDI_AddFontResApp:
// See TestGDI_AddFontRes.cpp for the implementation of this class
//

class CTestGDI_AddFontResApp : public CWinApp
{
public:
	CTestGDI_AddFontResApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTestGDI_AddFontResApp theApp;