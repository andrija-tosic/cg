
// IND_18015.h : main header file for the IND_18015 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIND18015App:
// See IND_18015.cpp for the implementation of this class
//

class CIND18015App : public CWinApp
{
public:
	CIND18015App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIND18015App theApp;
