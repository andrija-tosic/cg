
// I_Kolokvijum_2016.h : main header file for the I_Kolokvijum_2016 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIKolokvijum2016App:
// See I_Kolokvijum_2016.cpp for the implementation of this class
//

class CIKolokvijum2016App : public CWinApp
{
public:
	CIKolokvijum2016App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIKolokvijum2016App theApp;
