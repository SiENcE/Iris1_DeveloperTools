// HuePickerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "HuePickerDlg.h"


// CHuePickerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHuePickerDlg, CDialog)
CHuePickerDlg::CHuePickerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuePickerDlg::IDD, pParent)
{
}

CHuePickerDlg::~CHuePickerDlg()
{
}

void CHuePickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHuePickerDlg, CDialog)
END_MESSAGE_MAP()


// CHuePickerDlg 메시지 처리기입니다.
