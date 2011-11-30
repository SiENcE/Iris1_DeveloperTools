// TestPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "TestPage.h"


// CTestPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTestPage, CPropertyPage)
CTestPage::CTestPage()
	: CPropertyPage(CTestPage::IDD)
{
}

CTestPage::~CTestPage()
{
}

void CTestPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestPage, CPropertyPage)
END_MESSAGE_MAP()


// CTestPage 메시지 처리기입니다.
