#include "StdAfx.h"
#include "GumpEditor.h"
#include "StdGrfx.h"
#include ".\gumpBorder.h"
#include "GumpEditorDoc.h"


CGumpBorder::CGumpBorder(CGumpPtr pGump) : CGumpPicture(NULL)
{
	SetGump(pGump);

	SetTitle("border");
	SetType("border");
	
	CString strName;
	strName.Format("border_%x", pGump ? pGump->GetGumpID() : 0);
	SetName(strName);

	//AddPropertyPage( &m_page );
}

CGumpBorder::~CGumpBorder(void)
{
}

CDiagramEntity* CGumpBorder::Clone()
{
	CGumpBorder* obj = new CGumpBorder(m_pGumpB[LT]);
	obj->Copy( this );
	return obj;

}

void CGumpBorder::Draw( CDC* dc, CRect rect )
{
	if (!m_pGumpB[LT]) {
		CGumpEntity::Draw(dc,rect);
		return;
	}

	double zoom = GetZoom();

	int img_x[4],img_y[4];

	img_x[0] = rect.left;
	img_x[1] = max(rect.left, rect.left  + m_sizeGumpB[LT].cx * zoom);
	img_x[2] = min(rect.right, rect.right - m_sizeGumpB[RT].cx * zoom);
	img_x[3] = rect.right;

	img_y[0] = rect.top;
	img_y[1] = max(rect.top, rect.top    + m_sizeGumpB[LT].cy * zoom);
	img_y[2] = min(rect.bottom, rect.bottom - m_sizeGumpB[LB].cy * zoom);
	img_y[3] = rect.bottom;

	if (m_pGumpB[LT]) m_pGumpB[LT]->DrawGump(dc, CPoint(img_x[0],img_y[0]), zoom);
	if (m_pGumpB[RT]) m_pGumpB[RT]->DrawGump(dc, CPoint(img_x[2],img_y[0]), zoom);
	if (m_pGumpB[LB]) m_pGumpB[LB]->DrawGump(dc, CPoint(img_x[0],img_y[2]), zoom);
	if (m_pGumpB[RB]) m_pGumpB[RB]->DrawGump(dc, CPoint(img_x[2],img_y[2]), zoom);
		
	if (m_pGumpB[TOP]) m_pGumpB[TOP]->DrawGump(dc, CRect(img_x[1],img_y[0],img_x[2],img_y[1]), zoom);
	if (m_pGumpB[LC])  m_pGumpB[LC]->DrawGump(dc, CRect(img_x[0],img_y[1],img_x[1],img_y[2]), zoom);
	if (m_pGumpB[RC])  m_pGumpB[RC]->DrawGump(dc, CRect(img_x[2],img_y[1],img_x[3],img_y[2]), zoom);
	if (m_pGumpB[BOTTOM]) m_pGumpB[BOTTOM]->DrawGump(dc, CRect(img_x[1],img_y[2],img_x[2],img_y[3]), zoom);

	if (m_pGumpB[CENTER]) m_pGumpB[CENTER]->DrawGump(dc, CRect(img_x[1],img_y[1],img_x[2],img_y[2]), zoom);
}

CGumpEntity* CGumpBorder::CreateFromNode( XML::Node* node )
{

	CGumpBorder* obj = new CGumpBorder(NULL);
	if (!obj->FromString( node ))
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

int CGumpBorder::GetGumpID(void) const
{
	return m_pGumpB[LT] ? m_pGumpB[LT]->GetGumpID() : -1;
}

void CGumpBorder::SetGump(CGumpPtr pGump, bool bUpdateRect)
{
//	ASSERT(pGump);
	for (int i = 0; i < NUM_PART; i++) { 
		m_pGumpB[i] = NULL;
		m_sizeGumpB[i].SetSize(0,0);
	}
	
	if (!pGump) return;
	int iGumpID = pGump->GetGumpID();

	int iWidth = 0, iHeight = 0;
	CGumpEditorDoc* pDoc = GfxGetGumpDocument();
	m_pGumpB[LT] = pGump;
	m_sizeGumpB[LT] = pGump->GetDimensions();
	for (int i = 1; i < NUM_PART; i++) {
		m_pGumpB[i] = pDoc->GetGump(iGumpID+i);
		if (!m_pGumpB[i]) continue;
		m_sizeGumpB[i] = m_pGumpB[i]->GetDimensions();
		if (i < LC) iWidth += m_sizeGumpB[i].cx;
		if (i % LC == 0) iHeight += m_sizeGumpB[i].cy;
	}

	if (!bUpdateRect) return;
	
	CRect rect = GetRect();
	SetRect(rect.left,rect.top,rect.left+iWidth,rect.top+iHeight);
}
