#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpCustomPropertyPage.h"
#include "GumpCustom.h"


IMPLEMENT_DYNAMIC(CGumpCustomPropertyPage, CPropertyPage)
CGumpCustomPropertyPage::CGumpCustomPropertyPage()
	: CDiagramPropertyPage(CGumpCustomPropertyPage::IDD)
	, m_strCustom(_T(""))
{
}

CGumpCustomPropertyPage::~CGumpCustomPropertyPage()
{
}

void CGumpCustomPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CUSTOM_EDIT, m_strCustom);
}


BEGIN_MESSAGE_MAP(CGumpCustomPropertyPage, CDiagramPropertyPage)
END_MESSAGE_MAP()



BOOL CGumpCustomPropertyPage::OnInitDialog()
{ 
	CDiagramPropertyPage::OnInitDialog();

	return TRUE;  
}


void CGumpCustomPropertyPage::SetValues()
{
	if (GetSafeHwnd() && GetEntity())
	{
		CGumpCustom* pCustom = dynamic_cast<CGumpCustom*>(GetEntity());
		if (!pCustom) return;

		m_strCustom = pCustom->GetCustom();
		
		UpdateData(FALSE);
	}
}

void CGumpCustomPropertyPage::ApplyValues()
{
	if (GetSafeHwnd() && GetEntity())
	{
		UpdateData();
		CGumpCustom* pCustom = dynamic_cast<CGumpCustom*>(GetEntity());
		if (!pCustom) return;

		pCustom->SetCustom(m_strCustom);

		Redraw();
	}
}



