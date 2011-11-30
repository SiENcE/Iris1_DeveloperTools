// GumpEntityPropertyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpEntityPropertyPage.h"


// CGumpEntityPropertyDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGumpEntityPropertyDlg, CDialog)
CGumpEntityPropertyDlg::CGumpEntityPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGumpEntityPropertyDlg::IDD, pParent)
{
}

CGumpEntityPropertyDlg::~CGumpEntityPropertyDlg()
{
}

void CGumpEntityPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGumpEntityPropertyDlg, CDialog)
END_MESSAGE_MAP()


// CGumpEntityPropertyDlg 메시지 처리기입니다.
