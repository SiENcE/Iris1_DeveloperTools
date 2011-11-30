#include "StdAfx.h"
#include ".\dibgump.h"


CDibGump::CDibGump(int iGumpID, CSize sizeDib, LPBYTE pData) 
: CDib(sizeDib, 32) 
{
	m_iGumpID = iGumpID;
	
	m_lpImage = pData;
	m_nImageAlloc = CDib::crtAlloc;
}


CDibGump::~CDibGump(void)
{
}

void CDibGump::DrawGump(CDC *dc, CPoint pt, double zoom)
{
	CSize size = GetDimensions();
	size.cx *= zoom; size.cy *= zoom;
	DrawTransparent(dc, pt, RGB(255,255,255), size);
}

void CDibGump::DrawGump(CDC *dc, CRect rect, double zoom)
{	
	CSize sizeDib = GetDimensions();
	CSize sizeDst = rect.Size(), sizeSrc;
	CSize sizeOrg = sizeDib;
	
	sizeDst.cx *= zoom; sizeDst.cy *= zoom;
	sizeDib.cx *= zoom; sizeDib.cy *= zoom;

	for (int y=rect.top;  y < rect.bottom; y += sizeDib.cy) {
		sizeDst.cy = min(sizeDib.cy, rect.bottom-y);
		for (int x=rect.left; x < rect.right;  x += sizeDib.cx) {
			sizeDst.cx = min(sizeDib.cx, rect.right-x);
			sizeSrc.cx = min(sizeDst.cx, sizeOrg.cx);
			sizeSrc.cy = min(sizeDst.cy, sizeOrg.cy);
			DrawTransparent(dc, CPoint(x,y),RGB(255,255,255), sizeDst, 
				CPoint(0,0), sizeSrc);
		}
	}
}
