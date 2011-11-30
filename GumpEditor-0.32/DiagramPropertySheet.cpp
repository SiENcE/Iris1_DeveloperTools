// DiagramSheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include ".\diagrampropertypage.h"
#include ".\diagrampropertysheet.h"


// CDiagramPropertySheet

IMPLEMENT_DYNAMIC(CDiagramPropertySheet, CPropertySheet)

CDiagramPropertySheet::CDiagramPropertySheet() :CPropertySheet("Control Properties", NULL, 0)
{
}

CDiagramPropertySheet::~CDiagramPropertySheet()
{
}

#define IDC_APPLY_BUTTON 100
#define IDC_RESET_BUTTON 101

BEGIN_MESSAGE_MAP(CDiagramPropertySheet, CPropertySheet)
	ON_COMMAND(IDC_APPLY_BUTTON, OnApply)
	ON_COMMAND(IDC_RESET_BUTTON, OnReset)
END_MESSAGE_MAP()


BOOL CDiagramPropertySheet::OnInitDialog()
{

	CPropertySheet::OnInitDialog();

	ShowWindow(SW_HIDE);

    RECT rc;
    GetWindowRect (&rc);
    // Increase the height of the CPropertySheet by 30
    rc.bottom += 30;
    // Resize the CPropertySheet
    MoveWindow (rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
    // Convert to client coordinates
    ScreenToClient (&rc);
    // m_Button is of type CButton and is a member of CMySheet
    m_btnApply.Create ("&Apply", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                        CRect (rc.right-60, rc.bottom-30, rc.right-10, rc.bottom-8),
                        this, IDC_APPLY_BUTTON);
	m_btnApply.SetFont(GetFont());

	m_btnReset.Create ("&Reset", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                        CRect (rc.right-120, rc.bottom-30, rc.right-70, rc.bottom-8),
                        this, IDC_RESET_BUTTON);
	m_btnReset.SetFont(GetFont());

	for (int i =GetPageCount()-1; i >=0 ; i--) {
		CDiagramPropertyPage* page = (CDiagramPropertyPage*)GetPage(i);
		page->SetRedrawWnd(GetParent());
		SetActivePage(page);
	}

	ShowWindow(SW_SHOW);

	return TRUE;	
}

void CDiagramPropertySheet::SetValues(void)
{
	for (int i =GetPageCount()-1; i >=0 ; i--) {
		CDiagramPropertyPage* page = (CDiagramPropertyPage*)GetPage(i);
		page->SetValues();
	}
}

void CDiagramPropertySheet::ApplyValues()
{
	for (int i =GetPageCount()-1; i >=0 ; i--) {
		CDiagramPropertyPage* page = (CDiagramPropertyPage*)GetPage(i);
		page->ApplyValues();
	}
}

void CDiagramPropertySheet::ResetValues()
{
	for (int i =GetPageCount()-1; i >=0 ; i--) {
		CDiagramPropertyPage* page = (CDiagramPropertyPage*)GetPage(i);
		page->ResetValues();
	}
}


void CDiagramPropertySheet::OnApply()
{
	ApplyValues();
}

void CDiagramPropertySheet::OnReset()
{
	ResetValues();
}
BOOL CDiagramPropertySheet::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
		ShowWindow(SW_HIDE);
		return TRUE;
	}

	return CPropertySheet::PreTranslateMessage(pMsg);
}
