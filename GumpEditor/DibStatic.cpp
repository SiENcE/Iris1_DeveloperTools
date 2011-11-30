// DibStatic.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "DibStatic.h"
#include ".\dibstatic.h"


// CDibStatic

IMPLEMENT_DYNAMIC(CDibStatic, CStatic)
CDibStatic::CDibStatic(CGumpPtr pDib) : m_pDib(pDib)
{
}

CDibStatic::~CDibStatic()
{
}


BEGIN_MESSAGE_MAP(CDibStatic, CStatic)
	ON_WM_DRAWITEM()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CDibStatic 메시지 처리기입니다.


BOOL CDibStatic::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CRect rc(rect);
	if (m_pDib)
	{
		CSize size = m_pDib->GetDimensions();
		rc.right  = rc.left + size.cx;
		rc.bottom = rc.top + size.cy;
	}

	return CStatic::Create(lpszText, dwStyle, rc, pParentWnd, nID);
}

void CDibStatic::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	//CStatic::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDibStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CStatic::OnPaint()을(를) 호출하지 마십시오.
	if (!m_pDib) return;

	m_pDib->Draw(&dc, CPoint(0,0));
	//m_pDib->DrawTransparent(&dc,CPoint(0,0),RGB(255,255,255));
}

int CDibStatic::GetGumpID(void)
{
	if (m_pDib) return m_pDib->GetGumpID();
	
	return -1;
}

void CDibStatic::SetGump(CGumpPtr pDib)
{
	ASSERT(pDib);
	m_pDib = pDib;

	CRect rc;
	GetWindowRect(rc);
	GetParent()->ScreenToClient(rc);

	CSize size = m_pDib->GetDimensions();
	rc.right  = rc.left + size.cx;
	rc.bottom = rc.top + size.cy;
	
	MoveWindow(rc);
}
