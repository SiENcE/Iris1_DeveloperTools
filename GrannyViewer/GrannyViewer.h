// GrannyViewer.h : GrannyViewer 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"       // 주 기호


// CGrannyViewerApp:
// 이 클래스의 구현에 대해서는 GrannyViewer.cpp을 참조하십시오.
//


class CGrannyViewerApp : public CWinApp
{
public:
	CGrannyViewerApp();

// 재정의
public:
	virtual BOOL InitInstance();

	//int m_iModelID;
	//int GetOpenModelID() { return m_iModelID; }
	//void SetOpenModelID(int id) { m_iModelID = id; }

	void ReadConfig();
	void SaveConfig();

// 구현
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
public:
	afx_msg void OnLogSetuplog();
};

extern CGrannyViewerApp theApp;
