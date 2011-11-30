#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpSliderPropertyPage.h"
#include "GumpEditorDoc.h"
#include "GumpSlider.h"
#include ".\gumpsliderpropertypage.h"



IMPLEMENT_DYNAMIC(CGumpSliderPropertyPage, CDiagramPropertyPage)

CGumpSliderPropertyPage::CGumpSliderPropertyPage()
	: CDiagramPropertyPage(CGumpSliderPropertyPage::IDD)
	, m_iMin(0)
	, m_iMax(100)
	, m_iPos(0)
	, m_strTrackID(_T(""))
	, m_strThumbID(_T(""))
	, m_bVertical(FALSE)
{
}

CGumpSliderPropertyPage::~CGumpSliderPropertyPage()
{
}

void CGumpSliderPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CDiagramPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MIN, m_iMin);
	DDX_Text(pDX, IDC_MAX, m_iMax);
	DDX_Text(pDX, IDC_POS, m_iPos);
	DDX_Text(pDX, IDC_GUMP_ID, m_strTrackID);
	DDX_Text(pDX, IDC_GUMP_ID2, m_strThumbID);
	DDX_Control(pDX, IDC_SPIN1, m_spinTrack);
	DDX_Control(pDX, IDC_SPIN2, m_spinThumb);
	DDX_Control(pDX, IDC_SPIN3, m_spinMin);
	DDX_Control(pDX, IDC_SPIN6, m_spinMax);
	DDX_Control(pDX, IDC_SPIN9, m_spinPos);
	DDX_Check(pDX, IDC_VERTICAL, m_bVertical);
}


BEGIN_MESSAGE_MAP(CGumpSliderPropertyPage, CDiagramPropertyPage)
END_MESSAGE_MAP()


void CGumpSliderPropertyPage::SetValues()
{
	if (GetSafeHwnd() && GetEntity())
	{		
		CGumpSlider* pSlider = dynamic_cast<CGumpSlider*>(GetEntity()); ASSERT(pSlider);
		if (!pSlider) return;

		CGumpPtr pTrack, pThumb;
		pSlider->GetGump(pTrack, pThumb);
		
		ASSERT(pTrack && pThumb);

		m_strTrackID = GfxXtoA(pTrack->GetGumpID());
		m_strThumbID = GfxXtoA(pThumb->GetGumpID());

		pSlider->GetRange(m_iMin, m_iMax);
		m_iPos = pSlider->GetPos();
		m_bVertical = pSlider->IsVertical();
		
		UpdateData(FALSE);
	}
}

void CGumpSliderPropertyPage::ApplyValues()
{
	if (GetSafeHwnd() && GetEntity())
	{
		UpdateData();

		CGumpEditorDoc* pDoc = GfxGetGumpDocument();
		CGumpSlider* pSlider= dynamic_cast<CGumpSlider*>(GetEntity()); ASSERT(pSlider);
		if (!pSlider) return;

		int iTrackID = GfxAtoX(m_strTrackID);
		int iThumbID = GfxAtoX(m_strThumbID);

		CGumpPtr pTrack, pThumb, pGump;
		pSlider->GetGump(pTrack, pThumb);

		if (pTrack->GetGumpID() != iTrackID  && (pGump = pDoc->GetGump(iTrackID))) 
		{
			pSlider->SetGump(CGumpSlider::TRACK, pGump);
		}
		if (pThumb->GetGumpID() != iThumbID  && (pGump = pDoc->GetGump(iThumbID))) 
		{
			pSlider->SetGump(CGumpSlider::THUMB, pGump);
		}

		pSlider->SetRange(m_iMin, m_iMax);
		pSlider->SetPos(m_iPos);
		pSlider->SetVertical(m_bVertical);

		Redraw();
	}
}

void CGumpSliderPropertyPage::ResetValues()
{
	if (GetSafeHwnd() && GetEntity())
	{	
		UpdateData();
		CGumpSlider* pSlider = dynamic_cast<CGumpSlider*>(GetEntity()); ASSERT(pSlider);
		if (!pSlider) return;

		pSlider->SetGump(CGumpSlider::TRACK, pSlider->GetGump(CGumpSlider::TRACK));
			
		Redraw();			
	}	
}


BOOL CGumpSliderPropertyPage::OnInitDialog()
{
	CDiagramPropertyPage::OnInitDialog();

	m_spinMin.SetRange32(0,0xffff);
	m_spinMax.SetRange32(0,0xffff);
	m_spinPos.SetRange32(0,0xffff);
	m_spinTrack.SetRange32(0,0xffff);
	m_spinThumb.SetRange32(0,0xffff);

	return TRUE;  
}
