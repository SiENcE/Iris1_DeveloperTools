// GumpEditor.h : main header file for the GumpEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CGumpEditorApp:
// See GumpEditor.cpp for the implementation of this class
//

class CGumpEditorApp : public CWinApp
{
public:
	CGumpEditorApp();


// @cmember find strings
   CStringArray m_strFindstrings;
// @cmember replace strings
   CStringArray m_strReplacestrings;
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGumpEditorApp theApp;

