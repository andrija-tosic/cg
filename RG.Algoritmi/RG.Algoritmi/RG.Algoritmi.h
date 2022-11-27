
// RG.Algoritmi.h : main header file for the RG.Algoritmi application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRGAlgoritmiApp:
// See RG.Algoritmi.cpp for the implementation of this class
//

class CRGAlgoritmiApp : public CWinApp
{
public:
	CRGAlgoritmiApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRGAlgoritmiApp theApp;
