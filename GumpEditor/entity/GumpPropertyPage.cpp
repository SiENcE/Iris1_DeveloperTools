// GumpPropertyPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpEditorDoc.h"
#include "gumppropertypage.h"
#include "gumpentity.h"

IMPLEMENT_DYNAMIC(CGumpPropertyPage, CDiagramPropertyPage)
CGumpPropertyPage::CGumpPropertyPage()
	: CDiagramPropertyPage(CGumpPropertyPage::IDD)
	, m_xPos(0)
	, m_yPos(0)
	, m_iWidth(0)
	, m_iHeight(0)
	, m_iAlpha(0)
	, m_strName(_T(""))
	, m_strType(_T(""))
	, m_strEvHandler(_T(""))
	, m_strEvType(_T(""))
	, m_bVisible(FALSE)
	, m_bDisable(FALSE)
{
}

CGumpPropertyPage::~CGumpPropertyPage()
{
}

void CGumpPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CDiagramPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ALPHA, m_iAlpha);
	DDX_Control(pDX, IDC_ALPHASPIN, m_spinAlpha);
	DDX_Control(pDX, IDC_XSPIN, m_spinX);
	DDX_Control(pDX, IDC_YSPIN, m_spinY);
	DDX_Control(pDX, IDC_WSPIN, m_spinW);
	DDX_Control(pDX, IDC_HSPIN, m_spinH);
	DDX_Text(pDX, IDC_XPOS, m_xPos);
	DDX_Text(pDX, IDC_YPOS, m_yPos);
	DDX_Text(pDX, IDC_W, m_iWidth);
	DDX_Text(pDX, IDC_H, m_iHeight);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_TYPE, m_strType);
	DDX_Text(pDX, IDC_EVENT_HANDLER, m_strEvHandler);
	DDX_Text(pDX, IDC_EVENT_TYPE, m_strEvType);
	DDX_Check(pDX, IDC_VISIBLE_CHECK, m_bVisible);
	DDX_Check(pDX, IDC_DISABLE_CHECK, m_bDisable);
}


BEGIN_MESSAGE_MAP(CGumpPropertyPage, CDiagramPropertyPage)
	ON_BN_CLICKED(IDC_CODE_BUTTON, OnBnClickedCodeButton)
END_MESSAGE_MAP()


BOOL CGumpPropertyPage::OnInitDialog()
{
	CDiagramPropertyPage::OnInitDialog();

	m_spinX.SetRange32(-0xffff,0xffff);
	m_spinY.SetRange32(-0xffff,0xffff);
	m_spinW.SetRange32(0,0xffff);
	m_spinH.SetRange32(0,0xffff);
	m_spinAlpha.SetRange32(0,0xffff);

#if 0
	CGumpEntity* pEntity= dynamic_cast<CGumpEntity*>(GetEntity());
	if (pEntity && pEntity->IsFixedSize()) {
		GetDlgItem(IDC_W)->SendMessage(EM_SETREADONLY,1);
		GetDlgItem(IDC_H)->SendMessage(EM_SETREADONLY,1);
		m_spinW.EnableWindow(FALSE);
		m_spinH.EnableWindow(FALSE);
	}
#endif

	return TRUE; 
}

void CGumpPropertyPage::SetValues()
{
	if (GetSafeHwnd() && GetEntity()) {		
		CGumpEntity* pEntity= dynamic_cast<CGumpEntity*>(GetEntity());
		if (!pEntity) return;

		m_strName = pEntity->GetName();
		m_strType = pEntity->GetType();
		m_iAlpha = pEntity->GetAlpha();

		m_bVisible = pEntity->IsSetFlags(CGumpEntity::VISIBLE);
		m_bDisable = pEntity->IsSetFlags(CGumpEntity::DISALBE);
		
		m_strEvType = pEntity->GetEventTypeName();
		m_strEvHandler = pEntity->GetEventHandler();

		CRect rect = pEntity->GetRect();
		m_xPos = rect.left;
		m_yPos = rect.top;
		m_iWidth = rect.Width();
		m_iHeight = rect.Height();

		UpdateData(FALSE);
	}
}

void CGumpPropertyPage::ApplyValues()
{
	if (GetSafeHwnd() && GetEntity()) {	
		UpdateData();

		CGumpEntity* pEntity = dynamic_cast<CGumpEntity*>(GetEntity());
		if (!pEntity) return;

		pEntity->SetName(m_strName);
		pEntity->SetAlpha(m_iAlpha);

		int iFlags = pEntity->GetFlags();
		iFlags |= m_bVisible ? CGumpEntity::VISIBLE : 0;
		iFlags |= m_bDisable ? CGumpEntity::DISALBE: 0;
		pEntity->SetFlags(iFlags);

		pEntity->SetEventHandler(m_strEvHandler);

		pEntity->SetRect(m_xPos, m_yPos, m_xPos + m_iWidth, m_yPos + m_iHeight);

		Redraw();
	}	
}

void CGumpPropertyPage::ResetValues() 
{

}

void CGumpPropertyPage::OnBnClickedCodeButton()
{
	CGumpEditorDoc* pDoc = GfxGetGumpDocument();
	if (pDoc) pDoc->ShowCode(TRUE, GetEntity());
}
