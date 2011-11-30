// GumpPropertyPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include ".\gumppropertypage.h"
#include "gumpentity.h"




// CGumpPropertyPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGumpPropertyPage, CDiagramPropertyPage)
CGumpPropertyPage::CGumpPropertyPage()
	: CDiagramPropertyPage(CGumpPropertyPage::IDD)
	, m_xPos(0)
	, m_yPos(0)
	, m_iWidth(0)
	, m_iHeight(0)
	, m_iAlpha(0)
	, m_strEvClick(_T(""))
	, m_strEvClose(_T(""))
	, m_strEvMouseUp(_T(""))
	, m_strEvMouseDown(_T(""))
	, m_strEvKeyPressed(_T(""))
	, m_strName(_T(""))
	, m_strType(_T(""))
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
	DDX_Text(pDX, IDC_EVENT_ONCLICK, m_strEvClick);
	DDX_Text(pDX, IDC_EVENT_ONCLOSE, m_strEvClose);
	DDX_Text(pDX, IDC_EVENT_ONMOUSEUP, m_strEvMouseUp);
	DDX_Text(pDX, IDC_EVENT_ONMOUSEDOWN, m_strEvMouseDown);
	DDX_Text(pDX, IDC_EVENT_ONKEYPRESSED, m_strEvKeyPressed);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_TYPE, m_strType);
}


BEGIN_MESSAGE_MAP(CGumpPropertyPage, CDiagramPropertyPage)
END_MESSAGE_MAP()


// CGumpPropertyPage 메시지 처리기입니다.

BOOL CGumpPropertyPage::OnInitDialog()
{
	CDiagramPropertyPage::OnInitDialog();

	m_spinX.SetRange32(-0xffff,0xffff);
	m_spinY.SetRange32(-0xffff,0xffff);
	m_spinW.SetRange32(0,0xffff);
	m_spinH.SetRange32(0,0xffff);
	m_spinAlpha.SetRange32(0,0xffff);

	CGumpEntity* pEntity= (CGumpEntity*)GetEntity();
	if (pEntity && pEntity->IsFixedSize()) {
		GetDlgItem(IDC_W)->SendMessage(EM_SETREADONLY,1);
		GetDlgItem(IDC_H)->SendMessage(EM_SETREADONLY,1);
		m_spinW.EnableWindow(FALSE);
		m_spinH.EnableWindow(FALSE);
	}

	return TRUE; 
}

void CGumpPropertyPage::SetValues()
{
	if (GetSafeHwnd() && GetEntity()) {		
		CGumpEntity* pEntity= (CGumpEntity*)GetEntity();

		m_strName = pEntity->GetName();
		m_strType = pEntity->GetType();
		m_iAlpha = pEntity->GetAlpha();
		
		pEntity->GetEventHandler(m_strEvClick, m_strEvClose, m_strEvMouseUp, m_strEvMouseDown, m_strEvKeyPressed);

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

		CGumpEntity* pEntity = (CGumpEntity*)GetEntity();

		pEntity->SetName(m_strName);
		pEntity->SetAlpha(m_iAlpha);

		pEntity->SetEventHandler(m_strEvClick, m_strEvClose, m_strEvMouseUp, m_strEvMouseDown, m_strEvKeyPressed);

		pEntity->SetRect(m_xPos, m_yPos, m_xPos + m_iWidth, m_yPos + m_iHeight);

		Redraw();
	}	
}

void CGumpPropertyPage::ResetValues() 
{

}