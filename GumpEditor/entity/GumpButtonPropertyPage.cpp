#include "stdafx.h"
#include "GumpEditor.h"
#include ".\gumpbuttonpropertypage.h"
#include "GumpEditorDoc.h"
#include "GumpButton.h"


IMPLEMENT_DYNAMIC(CGumpButtonPropertyPage, CDiagramPropertyPage)
CGumpButtonPropertyPage::CGumpButtonPropertyPage(CWnd* pParent)
	: CDiagramPropertyPage(CGumpButtonPropertyPage::IDD)
	, m_bChecked(FALSE)
	, m_bGrouped(FALSE)
	, m_iType(0)
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
	DDX_Control(pDX, IDC_TYPE_COMBO2, m_strType);
	DDX_Check(pDX, IDC_CHECKED_CHECK, m_bChecked);
	DDX_Check(pDX, IDC_GROUP_CHECK, m_bGrouped);
	DDX_CBIndex(pDX, IDC_TYPE_COMBO2, m_iType);
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

	if (GetSafeHwnd() && GetEntity()) {		
		CGumpButton* pButton = (CGumpButton*)GetEntity();

		CGumpPtr pNormal, pHover, pPressed;
		pButton->GetGump(pNormal, pHover, pPressed);
		
		//ASSERT(pNormal && pHover && pPressed);
		m_btnTest.SetBitmap(pNormal, pHover, pPressed);

		m_strNormalID = GfxXtoA(pNormal->GetGumpID());
		m_strHoverID = GfxXtoA(pHover->GetGumpID());
		m_strPressedID = GfxXtoA(pPressed->GetGumpID());

		m_iType = pButton->GetButtonType();
		m_bChecked = pButton->IsChecked();
		m_bGrouped = pButton->IsGrouped();

		UpdateData(FALSE);
	}
}

void CGumpButtonPropertyPage::ApplyValues()
{
	if (GetSafeHwnd() && GetEntity()) {
		UpdateData();

		CGumpEditorDoc* pDoc = GfxGetGumpDocument();
		CGumpButton* pButton = (CGumpButton*)GetEntity();

		int iNormalID =GfxAtoX(m_strNormalID);
		int iHoverID  =GfxAtoX(m_strHoverID);
		int iPressedID=GfxAtoX(m_strPressedID);

		CGumpPtr pNormal, pHover, pPressed, pGump;
		pButton->GetGump(pNormal, pHover, pPressed);

		if (pNormal->GetGumpID() != iNormalID && (pGump = pDoc->GetGump(iNormalID))) {
			pButton->SetGump(CGumpButton::NORMAL, pGump);
			m_btnTest.SetGump(CHoverButton::NORMAL, pGump);
		}
		if (pHover->GetGumpID() != iHoverID && (pGump = pDoc->GetGump(iHoverID))) {
			pButton->SetGump(CGumpButton::HOVER, pGump);
			m_btnTest.SetGump(CHoverButton::HOVER, pGump);
		}
		if (pPressed->GetGumpID() != iPressedID && (pGump = pDoc->GetGump(iPressedID))) {
			pButton->SetGump(CGumpButton::PRESSED, pGump);
			m_btnTest.SetGump(CHoverButton::PRESSED, pGump);
		}

		pButton->SetChecked(m_bChecked);
		pButton->SetGrouped(m_bGrouped);
		pButton->SetButtonType((CGumpButton::TYPE)m_iType);
		
		Redraw();
	}
}

