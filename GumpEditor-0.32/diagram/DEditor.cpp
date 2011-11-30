// DEditor.cpp : implementation file
//

#include "stdafx.h"
#include "StdGrfx.h"
#include "DialogEditorDemo.h"
#include "DEditor.h"
#include ".\deditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDEditor

void CDEditor::DrawBackground( CDC* dc, CRect rect, double /*zoom*/ ) const
{
	CStdGrfx::drawframed3dBox( dc, rect );
}

void CDEditor::DrawGrid( CDC* dc, CRect /*rect*/, double zoom ) const
{
	COLORREF gridcol = GetGridColor();

	dc->SelectStockObject( BLACK_PEN );

	int stepx = GetVirtualSize().cx / GetGridSize().cx;
	int stepy = GetVirtualSize().cy / GetGridSize().cy;

	for( int x = 0 ; x <= stepx ; x++ )
		for( int y = 0; y <= stepy ; y++ )
			dc->SetPixel( round( ( double ) ( GetGridSize().cx * x ) * zoom ), round( ( double ) ( GetGridSize().cy * y ) * zoom ), gridcol );
}

bool CDEditor::AdjustRect(CRect& rect)
{
	CRect rcOld=rect;

	CPoint point = rect.TopLeft();
	CSize size = rect.Size();
	CSize sizeVt = GetVirtualSize();
	int restraint = GetRestraints();	

	if (RESTRAINT_NONE != restraint) {
		int left=0,top=0,right=sizeVt.cx,bottom=sizeVt.cy; // m_restraint == RESTRAINT_VIRTUAL
		if (restraint == RESTRAINT_MARGIN) {
			GetMargins(left,top,right,bottom);
			right = sizeVt.cx - right;
			bottom = sizeVt.cy - bottom;
		}
		point.x = min(max(left,point.x),right-size.cx);
		point.y = min(max(top,point.y),bottom-size.cy);
	}

	if (GetSnapToGrid()) {
		point.x = SnapX(point.x);
		point.y = SnapY(point.y);
	}

	rect.SetRect(point,point+size);

	return rcOld==rect;
}

BEGIN_MESSAGE_MAP(CDEditor, CDiagramEditor)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CDEditor::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDiagramEditor::OnLButtonDown(nFlags, point);
}

void CDEditor::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDiagramEditor::OnLButtonUp(nFlags, point);
}

void CDEditor::OnMouseMove(UINT nFlags, CPoint point)
{
	CDiagramEditor::OnMouseMove(nFlags, point);
}

