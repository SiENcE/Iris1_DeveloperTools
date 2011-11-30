// GumpEditPropertyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include ".\GumpEditPropertyPage.h"
#include "GumpEditorDoc.h"
#include "GumpEdit.h"
#include "iris/config.h"


IMPLEMENT_DYNAMIC(CGumpEditPropertyPage, CDiagramPropertyPage)

CGumpEditPropertyPage::CGumpEditPropertyPage(CWnd* pParent)
	: CDiagramPropertyPage(CGumpEditPropertyPage::IDD)
	, m_bPassword(FALSE)
{
}

CGumpEditPropertyPage::~CGumpEditPropertyPage()
{
}

void CGumpEditPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_PASSWORD_CHECK, m_bPassword);
}


BEGIN_MESSAGE_MAP(CGumpEditPropertyPage, CDiagramPropertyPage)
END_MESSAGE_MAP()


BOOL CGumpEditPropertyPage::OnInitDialog()
{ 
	CDiagramPropertyPage::OnInitDialog();

	return TRUE;  
}


void CGumpEditPropertyPage::SetValues()
{
	if( GetSafeHwnd() && GetEntity() )
	{
		CGumpEdit* pEdit= (CGumpEdit*)GetEntity();

		m_bPassword = pEdit->IsPasswordMode();
		
		UpdateData(FALSE);
	}
}

void CGumpEditPropertyPage::ApplyValues()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( GetSafeHwnd() && GetEntity() )
	{
		UpdateData();
		CGumpEdit* pEdit = (CGumpEdit*)GetEntity();

		pEdit->SetPasswordMode(m_bPassword);
		
		Redraw();
	}
}

