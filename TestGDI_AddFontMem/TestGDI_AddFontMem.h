// TestGDI_AddFontMem.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTestGDI_AddFontMemApp:
// See TestGDI_AddFontMem.cpp for the implementation of this class
//

class CTestGDI_AddFontMemApp : public CWinApp
{
public:
	CTestGDI_AddFontMemApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTestGDI_AddFontMemApp theApp;