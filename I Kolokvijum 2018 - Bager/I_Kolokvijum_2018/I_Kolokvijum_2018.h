
// I_Kolokvijum_2018.h : main header file for the I_Kolokvijum_2018 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIKolokvijum2018App:
// See I_Kolokvijum_2018.cpp for the implementation of this class
//

class CIKolokvijum2018App : public CWinApp
{
public:
	CIKolokvijum2018App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIKolokvijum2018App theApp;
