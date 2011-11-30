// DynPropButtonPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include ".\gumpbuttonpropertypage.h"
#include "GumpEditorDoc.h"
#include "GumpButton.h"


// CGumpButtonPropertyPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGumpButtonPropertyPage, CDiagramPropertyPage)
CGumpButtonPropertyPage::CGumpButtonPropertyPage(CWnd* pParent)
	: CDiagramPropertyPage(CGumpButtonPropertyPage::IDD)
{
}

CGumpButtonPropertyPage::~CGumpButtonPropertyPage()
{
}

void CGumpButtonPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CDiagramPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GUMP_ID, m_strNormalID);
	DDX_Text(pDX, IDC_GUMP_ID2, m_strHoverID);
	DDX_Text(pDX, IDC_GUMP_ID3, m_strPressedID);
	DDX_Control(pDX, IDC_BUTTON2, m_btnTest);
	DDX_Control(pDX, IDC_SPIN1, m_spinNormal);
	DDX_Control(pDX, IDC_SPIN2, m_spinHover);
	DDX_Control(pDX, IDC_SPIN3, m_spinPressed);
}


BEGIN_MESSAGE_MAP(CGumpButtonPropertyPage, CDiagramPropertyPage)
END_MESSAGE_MAP()



BOOL CGumpButtonPropertyPage::OnInitDialog()
{
	CDiagramPropertyPage::OnInitDialog();

	m_spinNormal.SetRange32(0,0xffff);
	m_spinHover.SetRange32(0,0xffff);
	m_spinPressed.SetRange32(0,0xffff);

	return TRUE; 
}

void CGumpButtonPropertyPage::SetValues()
{

	if( GetSafeHwnd() && GetEntity() )
	{		
		CGumpButton* pButton = (CGumpButton*)GetEntity();

		CGumpPtr pNormal, pHover, pPressed;
		pButton->GetGump(pNormal, pHover, pPressed);
		
		//ASSERT(pNormal && pHover && pPressed);
		m_btnTest.SetBitmap(pNormal, pHover, pPressed);

		m_strNormalID = GfxXtoA(pNormal->GetGumpID());
		m_strHoverID = GfxXtoA(pHover->GetGumpID());
		m_strPressedID = GfxXtoA(pPressed->GetGumpID());

		UpdateData(FALSE);
	}
}

void CGumpButtonPropertyPage::ApplyValues()
{
	if( GetSafeHwnd() && GetEntity() )
	{
		UpdateData();

		CGumpEditorDoc* pDoc = GfxGetGumpDocument();
		CGumpButton* pButton = (CGumpButton*)GetEntity();

		int iNormalID =GfxAtoX(m_strNormalID);
		int iHoverID  =GfxAtoX(m_strHoverID);
		int iPressedID=GfxAtoX(m_strPressedID);

		CGumpPtr pNormal, pHover, pPressed, pGump;
		pButton->GetGump(pNormal, pHover, pPressed);

		if (pNormal->GetGumpID() != iNormalID 
			&& (pGump = pDoc->LoadGump(iNormalID))) 
		{
			pButton->SetGump(CGumpButton::NORMAL, pGump);
			m_btnTest.SetGump(CHoverButton::NORMAL, pGump);
		}
		if (pHover->GetGumpID() != iHoverID
			&& (pGump = pDoc->LoadGump(iHoverID))) 
		{
			pButton->SetGump(CGumpButton::HOVER, pGump);
			m_btnTest.SetGump(CHoverButton::HOVER, pGump);
		}
		if (pPressed->GetGumpID() != iPressedID
			&& (pGump = pDoc->LoadGump(iPressedID))) 
		{
			pButton->SetGump(CGumpButton::PRESSED, pGump);
			m_btnTest.SetGump(CHoverButton::PRESSED, pGump);
		}


		Redraw();
	}
}

