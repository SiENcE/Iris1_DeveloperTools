// DynPropBorderPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include ".\GumpBorderPropertyPage.h"
#include "GumpEditorDoc.h"
#include "GumpBorder.h"


// CGumpBorderPropertyPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGumpBorderPropertyPage, CDiagramPropertyPage)

CGumpBorderPropertyPage::CGumpBorderPropertyPage()
	: CDiagramPropertyPage(CGumpBorderPropertyPage::IDD)
	, m_strGumpID("0")
{
}

CGumpBorderPropertyPage::~CGumpBorderPropertyPage()
{
}

void CGumpBorderPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CDiagramPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GUMP_ID, m_strGumpID);
	DDX_Control(pDX, IDC_SPIN1, m_spinCtrl);
}


BEGIN_MESSAGE_MAP(CGumpBorderPropertyPage, CDiagramPropertyPage)
END_MESSAGE_MAP()


BOOL CGumpBorderPropertyPage::OnInitDialog()
{
	CDiagramPropertyPage::OnInitDialog();
	
	m_spinCtrl.SetRange32(0,0xffff);

	return TRUE; 
}

void CGumpBorderPropertyPage::SetValues()
{

	if( m_hWnd )
	{
		if( GetEntity() )
		{
			CGumpBorder* pBorder = (CGumpBorder*)GetEntity();
			m_strGumpID = GfxXtoA(pBorder->GetGumpID());
			UpdateData(FALSE);		
		}
	}
}

void CGumpBorderPropertyPage::ApplyValues()
{
	if( GetSafeHwnd() && GetEntity() )
	{
		UpdateData();
		CGumpBorder* pBorder = (CGumpBorder*)GetEntity();
		CGumpEditorDoc* pDoc = GfxGetGumpDocument();
		int iGumpID = GfxAtoX(m_strGumpID);

		if (pBorder->GetGumpID() != iGumpID) {	
			CGumpPtr pGump = pDoc->LoadGump(iGumpID);
			if (!pGump) return;
			pBorder->SetGump(pGump);
		}
					
		Redraw();
	
	}	
}

void CGumpBorderPropertyPage::ResetValues()
{
	CGumpBorder* pBorder = (CGumpBorder*)GetEntity();
	CGumpPtr pGump = pBorder->GetGump();
	if (!pGump) return;
	pBorder->SetGump(pGump);
	UpdateData(FALSE);
	Redraw();
}
