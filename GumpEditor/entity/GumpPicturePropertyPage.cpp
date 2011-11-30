#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpPicturePropertyPage.h"
#include "GumpPicture.h"
#include "GumpEditorDoc.h"

IMPLEMENT_DYNAMIC(CGumpPicturePropertyPage, CDiagramPropertyPage)

CGumpPicturePropertyPage::CGumpPicturePropertyPage()
	: CDiagramPropertyPage(CGumpPicturePropertyPage::IDD)
	, m_strGumpID("0")
	, m_iType(0)
{
}

CGumpPicturePropertyPage::~CGumpPicturePropertyPage()
{
}

void CGumpPicturePropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CDiagramPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLOR, m_btnColor);
	DDX_Text(pDX, IDC_GUMP_ID, m_strGumpID);
	DDX_Control(pDX, IDC_GUMP_SPIN, m_spinGump);
	DDX_CBIndex(pDX, IDC_TYPE_COMBO, m_iType);
}


BEGIN_MESSAGE_MAP(CGumpPicturePropertyPage, CDiagramPropertyPage)
	ON_CBN_SELCHANGE(IDC_TYPE_COMBO, OnCbnSelchangeTypeCombo)
END_MESSAGE_MAP()


BOOL CGumpPicturePropertyPage::OnInitDialog()
{
	CDiagramPropertyPage::OnInitDialog();

	m_spinGump.SetRange32(0,0xffff);

	return TRUE; 
}

void CGumpPicturePropertyPage::SetValues()
{
	if( GetSafeHwnd() && GetEntity())
	{
		CGumpPicture* pPicture = dynamic_cast<CGumpPicture*>(GetEntity());
		if (!pPicture) return;

		m_strGumpID = GfxXtoA(pPicture->GetGumpID());
		m_iType = pPicture->GetPictureType();
		m_btnColor.SetHueId(pPicture->GetHueId());
		
		UpdateData(FALSE);
	}
}

void CGumpPicturePropertyPage::ApplyValues()
{
	if (GetSafeHwnd() && GetEntity())
	{
		UpdateData();
		CGumpPicture* pPicture = dynamic_cast<CGumpPicture*>(GetEntity());
		if (!pPicture) return;

		bool bRedraw = false;
		int iGumpID = GfxAtoX(m_strGumpID);
		DWORD hueId = m_btnColor.GetHueId();

		if (pPicture->GetGumpID() != iGumpID) 
		{	
			ASSERT(GfxGetGumpDocument());
			CGumpPtr pGump = GfxGetGumpDocument()->GetGump(iGumpID);
			if (!pGump) return;
			pPicture->SetGump(pGump);
			bRedraw = CGumpPicture::GUMP == m_iType;
		} 

		if (pPicture->GetPictureType() != m_iType)
		{
			pPicture->SetPictureType((CGumpPicture::TYPE)m_iType);
			bRedraw = CGumpPicture::GUMP != m_iType;
		}

		if (pPicture->GetHueId() != hueId && m_iType)
		{
			pPicture->SetHueId(hueId);
			bRedraw = CGumpPicture::GUMP != m_iType;
		}

		if (bRedraw) Redraw();
	}
}

void CGumpPicturePropertyPage::ResetValues()
{
	if (GetSafeHwnd() && GetEntity())
	{	
		UpdateData();
		CGumpPicture* pPicture = dynamic_cast<CGumpPicture*>(GetEntity());
		if (!pPicture) return;

		pPicture->SetGump(pPicture->GetGump());
			
		Redraw();			
	}	
}



void CGumpPicturePropertyPage::OnCbnSelchangeTypeCombo()
{
}
