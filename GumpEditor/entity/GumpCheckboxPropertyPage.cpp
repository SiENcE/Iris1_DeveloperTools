// DynPropCheckboxPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include ".\gumpbuttonpropertypage.h"
#include "GumpEditorDoc.h"
#include "GumpCheckbox.h"


// CGumpCheckboxPropertyPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGumpCheckboxPropertyPage, CDiagramPropertyPage)
CGumpCheckboxPropertyPage::CGumpCheckboxPropertyPage(CWnd* pParent)
	: CDiagramPropertyPage(CGumpCheckboxPropertyPage::IDD)
	, m_bChecked(FALSE)
{
}

CGumpCheckboxPropertyPage::~CGumpCheckboxPropertyPage()
{
}

void CGumpCheckboxPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CDiagramPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GUMP_ID, m_strNormalID);
	DDX_Text(pDX, IDC_GUMP_ID2, m_strCheckedID);

	DDX_Control(pDX, IDC_BUTTON2, m_btnTest);
	DDX_Control(pDX, IDC_SPIN1, m_spinNormal);
	DDX_Control(pDX, IDC_SPIN2, m_spinChecked);

	DDX_Check(pDX, IDC_CHECK3, m_bChecked);
}


BEGIN_MESSAGE_MAP(CGumpCheckboxPropertyPage, CDiagramPropertyPage)
END_MESSAGE_MAP()


// CGumpCheckboxPropertyPage 메시지 처리기입니다.

void CGumpCheckboxPropertyPage::OnOK()
{	
	CDiagramPropertyPage::OnOK();
}

BOOL CGumpCheckboxPropertyPage::OnInitDialog()
{
	CDiagramPropertyPage::OnInitDialog();

	m_spinNormal.SetRange32(0,0xffff);
	m_spinChecked.SetRange32(0,0xffff);

	m_btnTest.SetCheckbox(TRUE);
	return TRUE; 
}

void CGumpCheckboxPropertyPage::SetValues()
{
	if (GetSafeHwnd() && GetEntity())
	{
		CGumpCheckbox* pCheckbox = (CGumpCheckbox*)GetEntity();

		m_bChecked = pCheckbox->IsChecked();

		CGumpPtr pNormal, pChecked;
		pCheckbox->GetGump(pNormal, pChecked);
		
		ASSERT(pNormal && pChecked);
		m_btnTest.SetBitmap(pNormal, pNormal, pChecked);

		m_strNormalID = GfxXtoA(pNormal->GetGumpID());
		m_strCheckedID = GfxXtoA(pChecked->GetGumpID());

		UpdateData(FALSE);
	}
}

void CGumpCheckboxPropertyPage::ApplyValues()
{
	if (GetSafeHwnd() && GetEntity())
	{
		UpdateData();

		CGumpEditorDoc* pDoc = GfxGetGumpDocument(); ASSERT(pDoc);
		CGumpCheckbox* pCheckbox = (CGumpCheckbox*)GetEntity();

		pCheckbox->SetCheck(m_bChecked);

		int iNormalID =GfxAtoX(m_strNormalID);
		int iCheckedID  =GfxAtoX(m_strCheckedID);

		CGumpPtr pNormal, pChecked, pPressed, pGump;
		pCheckbox->GetGump(pNormal, pChecked);

		if (pNormal->GetGumpID() != iNormalID 
			&& (pGump = pDoc->GetGump(iNormalID))) 
		{
			pCheckbox->SetGump(CGumpCheckbox::NORMAL, pGump);
			m_btnTest.SetGump(CHoverButton::NORMAL, pGump);
		}
		if (pChecked->GetGumpID() != iCheckedID
			&& (pGump = pDoc->GetGump(iCheckedID))) 
		{
			pCheckbox->SetGump(CGumpCheckbox::CHECKED, pGump);
			m_btnTest.SetGump(CHoverButton::PRESSED, pGump);
		}

		Redraw();	
	}	
}


