// DynPropPicturePage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include ".\GumpPicturePropertyPage.h"
#include "GumpPicture.h"
#include "GumpEditorDoc.h"


// CGumpPicturePropertyPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGumpPicturePropertyPage, CDiagramPropertyPage)

CGumpPicturePropertyPage::CGumpPicturePropertyPage(CWnd* pParent)
	: CDiagramPropertyPage(CGumpPicturePropertyPage::IDD)
	, m_strGumpID("0")
{
}

CGumpPicturePropertyPage::~CGumpPicturePropertyPage()
{
}

void CGumpPicturePropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CDiagramPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GUMP_ID, m_strGumpID);
	DDX_Control(pDX, IDC_GUMP_SPIN, m_spinGump);
}


BEGIN_MESSAGE_MAP(CGumpPicturePropertyPage, CDiagramPropertyPage)
END_MESSAGE_MAP()


// CGumpPicturePropertyPage 메시지 처리기입니다.


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
		CGumpPicture* pPicture = (CGumpPicture*)GetEntity();
		m_strGumpID = GfxXtoA(pPicture->GetGumpID());
		UpdateData(FALSE);
	}
}

void CGumpPicturePropertyPage::ApplyValues()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (GetSafeHwnd() && GetEntity())
	{
		UpdateData();
		CGumpPicture* pPicture = (CGumpPicture*)GetEntity();

		int iGumpID = GfxAtoX(m_strGumpID);

		if (pPicture->GetGumpID() != iGumpID) {	
			ASSERT(GfxGetGumpDocument());
			CGumpPtr pGump = GfxGetGumpDocument()->LoadGump(iGumpID);
			if (!pGump) return;
			pPicture->SetGump(pGump);
			Redraw();
		} 
					
	}
}

void CGumpPicturePropertyPage::ResetValues()
{
	if( GetSafeHwnd() && GetEntity() )
	{	
		UpdateData();
		CGumpPicture* pPicture = (CGumpPicture*)GetEntity();

		pPicture->SetGump(pPicture->GetGump());
			
		Redraw();			
	}	
}


