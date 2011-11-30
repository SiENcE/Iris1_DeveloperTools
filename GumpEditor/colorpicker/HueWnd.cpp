#include "stdafx.h"
#include "../GumpEditor.h"
#include "../GumpEditorDoc.h"
#include "HueWnd.h"
#include ".\huewnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CHueWnd::CHueWnd()
{
	CGumpEditorDoc* pDoc = GfxGetGumpDocument();
	m_pHueLoader = pDoc->GetHueLoader();

	m_ptCursor=CPoint(0,0);
	m_iColorSize = 16;
	
	m_iTotalSize = m_pHueLoader->getSize();
	m_sizePage = CSize(20,16);
	m_sizeTotal = CSize(m_sizePage.cx, m_iTotalSize/m_sizePage.cx + (m_iTotalSize % m_sizePage.cx ? 1 : 0));
	m_rectColor=CRect(0,0,m_iColorSize,m_iColorSize);
}

CHueWnd::~CHueWnd()
{
}


BEGIN_MESSAGE_MAP(CHueWnd, CWnd)
	//{{AFX_MSG_MAP(CHueWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHueWnd message handlers


void CHueWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(rect);

#if 0
	
	for (int x=0; x < m_sizeTotal.cx;  x++)
	{
		dc.MoveTo(x*m_iColorSize,0);
		dc.LineTo(x*m_iColorSize,rect.bottom);
	}
	
	for (int y=0; y < m_sizeTotal.cy; y++)
	{
		dc.MoveTo(0,y*m_iColorSize);
		dc.LineTo(rect.right,y*m_iColorSize);
	}
#endif

	CPoint ptOrg(0, GetScrollPos(SB_VERT) * m_iColorSize);
	
	dc.SetMapMode(MM_TEXT);
	dc.SetViewportOrg(-ptOrg);

	ASSERT(m_pHueLoader);


	int id=1;
	for (int y=0; y < m_sizeTotal.cy; y++)
	for (int x=0; x < m_sizeTotal.cx; x++)
	{
		CPoint pt = ColorToClient(CPoint(x,y));
		dc.FillSolidRect(m_rectColor+pt, m_pHueLoader->getColor(id++));

		//if (id>200) return;
	}

	rect = m_rectColor;
	rect += ColorToClient(m_ptCursor); 
	dc.FillSolidRect(rect, 0x0);
	rect.DeflateRect(1,1,1,1);
	dc.FillSolidRect(rect, 0xffffff);
	rect.DeflateRect(1,1,1,1);
	dc.FillSolidRect(rect, GetColor(m_ptCursor));

	// Do not call CWnd::OnPaint() for painting messages
}

BOOL CHueWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//CRect rect;
	//GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(255,255,255));

	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

BOOL CHueWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	cs.cx = m_sizeTotal.cx * m_iColorSize + GetSystemMetrics(SM_CYVSCROLL);
	cs.cy = cs.cy / m_iColorSize * m_iColorSize;

	return CWnd::PreCreateWindow(cs);
}

int CHueWnd::OnCreate(LPCREATESTRUCT lpCS) 
{
	if (CWnd::OnCreate(lpCS) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	m_sizePage = CSize(m_sizeTotal.cx,lpCS->cy / m_iColorSize);
	
	int iMaxPos = m_sizeTotal.cy - m_sizePage.cy;
	SetScrollRange(SB_VERT, 0, iMaxPos);
	
	return 0;
}



void CHueWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}


void CHueWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int yPos = GetScrollPos(SB_VERT);
	int yOrg = yPos;

	switch (nSBCode)
	{
		case SB_LINEUP :
			yPos--;
			break;
		case SB_LINEDOWN :
			yPos++;
			break;
		case SB_PAGEUP :
			yPos -= m_sizePage.cy;
			break;
		case SB_PAGEDOWN :
			yPos += m_sizePage.cy;
			break;
		case SB_THUMBTRACK :
			yPos = nPos;
			break;
	}

	SetScrollPos(SB_VERT, yPos);

	if (yOrg != yPos) InvalidateRect(NULL);
}


void CHueWnd::SelectHue(CPoint point)
{
	CPoint ptOrg = m_ptCursor;
	CPoint ptTmp = ClientToColor(point);
	
	if (ptTmp == ptOrg || ptTmp.x >= m_sizeTotal.cx || ptTmp.y >= m_sizeTotal.cy) return;
	if (ptTmp.x + ptTmp.y * m_sizePage.cx >= m_iTotalSize) return;

	m_ptCursor = ptTmp;

	int yPos = GetScrollPos(SB_VERT);
	
	CPoint ptOld = ColorToClient(CPoint(ptOrg.x, ptOrg.y - yPos));
	CPoint ptNew = ColorToClient(CPoint(m_ptCursor.x, m_ptCursor.y - yPos));

	if (m_ptCursor.y < yPos || m_ptCursor.y - yPos >= m_sizePage.cy) {
		SetScrollPos(SB_VERT, m_ptCursor.y);
		InvalidateRect(NULL);
	} else {
		InvalidateRect(m_rectColor+ptOld);
		InvalidateRect(m_rectColor+ptNew);
	}


	CWnd* pParent = GetParent();
	if (pParent) pParent->SendMessage(WM_HUE_CHANGED, GetCurrentHueId(), GetCurrentColor());
}

void CHueWnd::SelectHue(DWORD hueId)
{
	if (hueId==0 || hueId > m_iTotalSize) return;

	hueId--;
	int yPos = GetScrollPos(SB_VERT);
	CPoint pt(hueId % m_sizePage.cx, hueId / m_sizePage.cx - yPos);

	SelectHue(ColorToClient(pt));
}

void CHueWnd::SelectNearHue(COLORREF color)
{
}

COLORREF CHueWnd::GetColor(CPoint pt)
{
	if (m_pHueLoader)
		return m_pHueLoader->getColor(pt.x + pt.y * m_sizePage.cx+1);
	
	return 0;
}


void CHueWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	SelectHue(point);
	SetCapture();
	
	CWnd::OnLButtonDown(nFlags, point);
}


void CHueWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetCapture()==this) SelectHue(point);

	CWnd::OnMouseMove(nFlags, point);
}

void CHueWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (GetCapture()==this) ReleaseCapture();

	CWnd::OnLButtonUp(nFlags, point);
}
