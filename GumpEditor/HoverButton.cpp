// HoverButton.cpp : implementation file
//

#include "stdafx.h"
#include "HoverButton.h"
#include ".\hoverbutton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoverButton

CHoverButton::CHoverButton()
{
	m_bHover = FALSE;
	m_bTracking = FALSE;
	m_bClick = FALSE;
	m_bCheckbox = FALSE;
}

CHoverButton::~CHoverButton()
{
}

IMPLEMENT_DYNAMIC(CHoverButton, CButton)

BEGIN_MESSAGE_MAP(CHoverButton, CButton)
	//{{AFX_MSG_MAP(CHoverButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT(BN_CLICKED, OnBnClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////
 //	CHoverButton message handlers
		
void CHoverButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	//	TODO: Add your message handler code here and/or call default

	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	CButton::OnMouseMove(nFlags, point);
}

BOOL CHoverButton::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);		
	return CButton::PreTranslateMessage(pMsg);
}

// Set the tooltip with a string resource
void CHoverButton::SetToolTipText(int nId, BOOL bActivate)
{
	CString sText;

	// load string resource
	sText.LoadString(nId);
	// If string resource is not empty
	if (sText.IsEmpty() == FALSE) SetToolTipText(&sText, bActivate);

}

// Set the tooltip with a CString
void CHoverButton::SetToolTipText(CString *spText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (spText == NULL) return;

	// Initialize ToolTip
	InitToolTip();

	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, (LPCTSTR)*spText, rectBtn, 1);
	}

	// Set text for tooltip
	m_ToolTip.UpdateTipText((LPCTSTR)*spText, this, 1);
	m_ToolTip.Activate(bActivate);
}

void CHoverButton::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this);
		// Create inactive
		m_ToolTip.Activate(FALSE);
	}
} // End of InitToolTip

// Activate the tooltip
void CHoverButton::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
	if (m_ToolTip.GetToolCount() == 0) return;

	// Activate tooltip
	m_ToolTip.Activate(bActivate);
} // End of EnableTooltip

void CHoverButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);

	CPoint point(0,0);	

	CGumpPtr pDib = m_pDib[NORMAL];

	UINT state = lpDrawItemStruct->itemState;

	if (!IsCheckbox()) {
		if (state & ODS_DISABLED) pDib = m_pDib[NORMAL];
		else if (state & ODS_SELECTED) pDib = m_pDib[PRESSED];
		else if (m_bHover) pDib = m_pDib[HOVER];
	} else {
		if (m_bClick) pDib = m_pDib[PRESSED];
	}

	if (pDib) 
		pDib->DrawTransparent(pDC, CPoint(0,0), RGB(255,255,255));

	CString strText;
	GetWindowText(strText);

	CRect rcItem(lpDrawItemStruct->rcItem);

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		rcItem += CPoint(2,2);

	SetTextColor(lpDrawItemStruct->hDC, m_bHover ? RGB(255,255,0) : RGB(255,255,255));
	DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), 
		&rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// Load a bitmap from the resources in the button, the bitmap has to have 3 buttonsstates next to each other: Up/Down/Hover
BOOL CHoverButton::LoadBitmap(LPCTSTR szNormal, LPCTSTR szHover, LPCTSTR szPressed)
{
	if (!m_pDib[NORMAL]) m_pDib[NORMAL] = new CDibGump;
	if (!m_pDib[NORMAL]->Read(szNormal)) return FALSE;

	if (!m_pDib[HOVER]) m_pDib[HOVER] = new CDibGump;
	if (!m_pDib[HOVER]->Read(szHover)) return FALSE;

	if (!m_pDib[PRESSED]) m_pDib[PRESSED] = new CDibGump;
	if (!m_pDib[PRESSED]->Read(szPressed)) return FALSE;
	
	m_ButtonSize = m_pDib[NORMAL]->GetDimensions();

	SetWindowPos( NULL, 0,0, m_ButtonSize.cx,m_ButtonSize.cy, SWP_NOMOVE | SWP_NOOWNERZORDER);

	return TRUE;
} 

BOOL CHoverButton::SetBitmap(CGumpPtr pNormal, CGumpPtr pHover, CGumpPtr pPressed)
{
	if (!pNormal || !pHover || !pPressed) return FALSE;

	m_pDib[NORMAL] = pNormal;
	m_pDib[HOVER]  = pHover;
	m_pDib[PRESSED] = pPressed;

	m_ButtonSize = m_pDib[NORMAL]->GetDimensions();

	SetWindowPos( NULL, 0,0, m_ButtonSize.cx,m_ButtonSize.cy, SWP_NOMOVE | SWP_NOOWNERZORDER);

	return TRUE;
}

LRESULT CHoverButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	// TODO: Add your message handler code here and/or call default
	m_bHover=TRUE;
	Invalidate();
	return 0;
}


LRESULT CHoverButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover=FALSE;
	Invalidate();
	return 0;
}

void CHoverButton::GetBitmap(CGumpPtr& pNormal, CGumpPtr& pHover, CGumpPtr& pPressed)
{
	pNormal = m_pDib[NORMAL];
	pHover = m_pDib[HOVER];
	pPressed = m_pDib[PRESSED];
}

CGumpPtr CHoverButton::GetGump(STATE state)
{
	CGumpPtr pDib;
	
	if (NORMAL==state) pDib = m_pDib[NORMAL];
	else if (HOVER==state) pDib = m_pDib[HOVER];
	else if (PRESSED==state) pDib = m_pDib[PRESSED];
	
	return pDib;
}


int CHoverButton::GetGumpID(STATE state)
{
	CGumpPtr pDib = GetGump(state);
	if (pDib) return pDib->GetGumpID();

	return -1;
}

bool CHoverButton::SetGump(STATE state, CGumpPtr pDib)
{
	if (!pDib) return false;
	
	m_pDib[state] = pDib;
	if (state == NORMAL) {
		m_ButtonSize = m_pDib[NORMAL]->GetDimensions();
		SetWindowPos( NULL, 0,0, m_ButtonSize.cx,m_ButtonSize.cy, SWP_NOMOVE | SWP_NOOWNERZORDER);
	}

	return true;
}


void CHoverButton::OnBnClicked()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (IsCheckbox()) {
		m_bClick = !m_bClick;
		Invalidate();
	}
}
