// DynPropRadioPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include ".\gumpbuttonpropertyPage.h"
//#include "GumpEditorDoc.h"
#include "GumpRadio.h"


// CGumpRadioPropertyPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGumpRadioPropertyPage, CDiagramPropertyPage)
CGumpRadioPropertyPage::CGumpRadioPropertyPage(CWnd* pParent)
	: CDiagramPropertyPage(CGumpRadioPropertyPage::IDD)
	, m_iGroupID(0)
{
}

CGumpRadioPropertyPage::~CGumpRadioPropertyPage()
{
}

void CGumpRadioPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CDiagramPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN, m_spinGroupID);
	DDX_Text(pDX, IDC_GROUP, m_iGroupID);
}


BEGIN_MESSAGE_MAP(CGumpRadioPropertyPage, CDiagramPropertyPage)
END_MESSAGE_MAP()


// CGumpRadioPropertyPage 메시지 처리기입니다.

void CGumpRadioPropertyPage::OnOK()
{	
	CDiagramPropertyPage::OnOK();
}

BOOL CGumpRadioPropertyPage::OnInitDialog()
{
	CDiagramPropertyPage::OnInitDialog();

	m_spinGroupID.SetRange32(0,0xffff);
	return TRUE; 
}

void CGumpRadioPropertyPage::SetValues()
{

	if( GetSafeHwnd() && GetEntity())
	{		
		CGumpRadio* pRadio = (CGumpRadio*)GetEntity();

		m_iGroupID = pRadio->GetGroupID();

		UpdateData(FALSE);
	}
}

void CGumpRadioPropertyPage::ApplyValues()
{
	if( GetSafeHwnd() && GetEntity())
	{
		UpdateData();
		CGumpRadio* pRadio = (CGumpRadio*)GetEntity();

		pRadio->SetGroupID(m_iGroupID);

		//Redraw();
	}
}

