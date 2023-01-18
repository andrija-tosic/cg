
// I_Kolokvijum_2015.h : main header file for the I_Kolokvijum_2015 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIKolokvijum2015App:
// See I_Kolokvijum_2015.cpp for the implementation of this class
//

class CIKolokvijum2015App : public CWinApp
{
public:
	CIKolokvijum2015App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIKolokvijum2015App theApp;
