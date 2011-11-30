// GumpEditPropertyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpEditPropertyPage.h"
#include "GumpEdit.h"



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
	if (GetSafeHwnd() && GetEntity())
	{
		CGumpEdit* pEdit= dynamic_cast<CGumpEdit*>(GetEntity());
		if (!pEdit) return;

		m_bPassword = pEdit->IsPasswordMode();
		
		UpdateData(FALSE);
	}
}

void CGumpEditPropertyPage::ApplyValues()
{
	if (GetSafeHwnd() && GetEntity())
	{
		UpdateData();
		CGumpEdit* pEdit= dynamic_cast<CGumpEdit*>(GetEntity());
		if (!pEdit) return;

		pEdit->SetPasswordMode(m_bPassword);
		
		Redraw();
	}
}



