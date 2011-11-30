// DynCtrlTypeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpControlTypeDlg.h"
#include ".\gumpcontroltypedlg.h"

// CGumpControlTypeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGumpControlTypeDlg, CDialog)
CGumpControlTypeDlg::CGumpControlTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGumpControlTypeDlg::IDD, pParent)
	, m_nControlType(0)
{
}

CGumpControlTypeDlg::~CGumpControlTypeDlg()
{
}

void CGumpControlTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nControlType);
}


BEGIN_MESSAGE_MAP(CGumpControlTypeDlg, CDialog)
END_MESSAGE_MAP()


// CGumpControlTypeDlg 메시지 처리기입니다.


