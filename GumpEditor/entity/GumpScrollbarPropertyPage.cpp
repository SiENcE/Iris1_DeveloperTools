#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpEditorDoc.h"
#include "GumpScrollbarPropertyPage.h"
#include "GumpScrollbar.h"


IMPLEMENT_DYNAMIC(CGumpScrollbarPropertyPage, CDiagramPropertyPage)
CGumpScrollbarPropertyPage::CGumpScrollbarPropertyPage()
	: CDiagramPropertyPage(CGumpScrollbarPropertyPage::IDD)
	, m_iMin(0)
	, m_iMax(100)
	, m_iPos(0)
	, m_bVertical(FALSE)
	, m_bUseArrow(FALSE)
{
}

CGumpScrollbarPropertyPage::~CGumpScrollbarPropertyPage()
{
}

void CGumpScrollbarPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CDiagramPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MIN, m_iMin);
	DDX_Text(pDX, IDC_MAX, m_iMax);
	DDX_Text(pDX, IDC_POS, m_iPos);
	DDX_Text(pDX, IDC_GUMP_ID,  m_strGumpID[TRACK]);
	DDX_Text(pDX, IDC_GUMP_ID2, m_strGumpID[THUMB]);
	DDX_Text(pDX, IDC_GUMP_ID3, m_strGumpID[LTUP_NORMAL]);
	DDX_Text(pDX, IDC_GUMP_ID4, m_strGumpID[LTUP_HOVER]);
	DDX_Text(pDX, IDC_GUMP_ID5, m_strGumpID[LTUP_PRESSED]);
	DDX_Text(pDX, IDC_GUMP_ID6, m_strGumpID[RTDN_NORMAL]);
	DDX_Text(pDX, IDC_GUMP_ID7, m_strGumpID[RTDN_HOVER]);
	DDX_Text(pDX, IDC_GUMP_ID8, m_strGumpID[RTDN_PRESSED]);
	DDX_Control(pDX, IDC_SPIN3, m_spinMin);
	DDX_Control(pDX, IDC_SPIN6, m_spinMax);
	DDX_Control(pDX, IDC_SPIN9, m_spinPos);
	DDX_Control(pDX, IDC_SPIN1,  m_spinGumpID[TRACK]);
	DDX_Control(pDX, IDC_SPIN2,  m_spinGumpID[THUMB]);
	DDX_Control(pDX, IDC_SPIN10, m_spinGumpID[LTUP_NORMAL]);
	DDX_Control(pDX, IDC_SPIN12, m_spinGumpID[LTUP_HOVER]);
	DDX_Control(pDX, IDC_SPIN13, m_spinGumpID[LTUP_PRESSED]);
	DDX_Control(pDX, IDC_SPIN14, m_spinGumpID[RTDN_NORMAL]);
	DDX_Control(pDX, IDC_SPIN15, m_spinGumpID[RTDN_HOVER]);
	DDX_Control(pDX, IDC_SPIN16, m_spinGumpID[RTDN_PRESSED]);
	DDX_Check(pDX, IDC_VERTICAL, m_bVertical);
	DDX_Check(pDX, IDC_ARROWBTN, m_bUseArrow);
}


BEGIN_MESSAGE_MAP(CGumpScrollbarPropertyPage, CDiagramPropertyPage)
END_MESSAGE_MAP()


void CGumpScrollbarPropertyPage::SetValues()
{
	if (GetSafeHwnd() && GetEntity())
	{		
		CGumpScrollbar* pScrollbar = dynamic_cast<CGumpScrollbar*>(GetEntity()); ASSERT(pScrollbar);
		if (!pScrollbar) return;

		for (int i = 0; i < NUM_PART; i++)
			m_strGumpID[i] = GfxXtoA(pScrollbar->GetGumpID(CGumpScrollbar::PART(i)));

		pScrollbar->GetRange(m_iMin, m_iMax);
		m_iPos = pScrollbar->GetPos();
		m_bVertical = pScrollbar->IsVertical();
		m_bUseArrow = pScrollbar->IsUseArrowButton();
		
		UpdateData(FALSE);
	}
}

void CGumpScrollbarPropertyPage::ApplyValues()
{
	if (GetSafeHwnd() && GetEntity())
	{
		UpdateData();

		CGumpEditorDoc* pDoc = GfxGetGumpDocument();
		CGumpScrollbar* pScrollbar = dynamic_cast<CGumpScrollbar*>(GetEntity()); ASSERT(pScrollbar);
		if (!pScrollbar) return;

		CGumpPtr pGump;

		int iGumpID1 = -1, iGumpID2 = -1;
		for (int i = 0; i < NUM_PART; i++) {
			iGumpID1 = GfxAtoX(m_strGumpID[i]);
			iGumpID2 = pScrollbar->GetGumpID(CGumpScrollbar::PART(i));

			if (iGumpID1 != iGumpID2  && (pGump = pDoc->GetGump(iGumpID1))) 
				pScrollbar->SetGump(CGumpScrollbar::PART(i), pGump);
		}
		
		pScrollbar->SetRange(m_iMin, m_iMax);
		pScrollbar->SetPos(m_iPos);
		pScrollbar->SetVertical(m_bVertical);
		pScrollbar->SetUseArrowButton(m_bUseArrow);

		Redraw();
	}
}

void CGumpScrollbarPropertyPage::ResetValues()
{
	if (GetSafeHwnd() && GetEntity())
	{	
		UpdateData();
		CGumpScrollbar* pScrollbar = dynamic_cast<CGumpScrollbar*>(GetEntity()); ASSERT(pScrollbar);
		if (!pScrollbar) return;

		pScrollbar->SetGump(CGumpScrollbar::TRACK, pScrollbar->GetGump(CGumpScrollbar::TRACK));
			
		Redraw();			
	}	
}


BOOL CGumpScrollbarPropertyPage::OnInitDialog()
{
	CDiagramPropertyPage::OnInitDialog();

	m_spinMin.SetRange32(0,0xffff);
	m_spinMax.SetRange32(0,0xffff);
	m_spinPos.SetRange32(0,0xffff);

	for (int i = 0; i < NUM_PART; i++) 
		m_spinGumpID[i].SetRange32(0,0xffff);

	return TRUE;  
}

