// GumpView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpView.h"
#include ".\gumpview.h"

// CGumpView

IMPLEMENT_DYNCREATE(CGumpView, CView)

CGumpView::CGumpView()
{
}

CGumpView::~CGumpView()
{
}

BEGIN_MESSAGE_MAP(CGumpView, CView)
END_MESSAGE_MAP()


// CGumpView 그리기입니다.

void CGumpView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
	if (!m_pDib) return;

	CRect rect;
	GetClientRect(rect);

	CSize sizeWnd = rect.Size();
	CSize sizeDib = m_pDib->GetDimensions();

	if (sizeDib.cx <= 0 || sizeDib.cy <= 0) return;
	
	double ratio = min(float(sizeWnd.cx) / sizeDib.cx, float(sizeWnd.cy) / sizeDib.cy);

	if (ratio < 1.0) {
		sizeDib = CSize(int(sizeDib.cx * ratio), int(sizeDib.cy * ratio));
	}

	CPoint point;
	point.x = max(0, sizeWnd.cx /2 - sizeDib.cx/2);
	point.y = max(0, sizeWnd.cy /2 - sizeDib.cy/2);

	CRect rcBound(point.x-1, point.y-1, point.x+sizeDib.cx+1, point.y+sizeDib.cy+1);
	
	CPen pen(PS_DOT, 1, RGB(128,128,128));
	CPen* pOld = pDC->SelectObject(&pen);
	pDC->Rectangle(rcBound);
	pDC->SelectObject(pOld);

	m_pDib->Draw(pDC, point, sizeDib);
}


// CGumpView 진단입니다.

#ifdef _DEBUG
void CGumpView::AssertValid() const
{
	CView::AssertValid();
}

void CGumpView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CGumpView 메시지 처리기입니다.

void CGumpView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

void CGumpView::SetGump(CGumpPtr pDib)
{
	m_pDib = pDib;
	Invalidate();
}
