#include "StdAfx.h"
#include "GumpEditor.h"
#include "StdGrfx.h"
#include ".\gumpslider.h"
#include "GumpEditorDoc.h"

CGumpSlider::CGumpSlider(CGumpPtr pTrack, CGumpPtr pThumb, bool bVertical, int iMin, int iMax, int iPos)
 : m_bVertical(bVertical), m_iMin(iMin), m_iMax(iMax), m_iPos(iPos)
{
	SetGump(pTrack, pThumb);

	SetTitle("slider");
	SetType("slider");
	
	CString strName;
	strName.Format("slider_%x", pTrack? pTrack->GetGumpID() : 0);
	SetName(strName);

	AddPropertyPage( &m_page );
}

CGumpSlider::~CGumpSlider(void)
{
}

CDiagramEntity* CGumpSlider::Clone()
{
	CGumpSlider* obj = new CGumpSlider(m_pGump[TRACK], m_pGump[THUMB], m_bVertical, m_iMin, m_iMax, m_iPos);
	obj->Copy( this );
	return obj;

}

void CGumpSlider::Draw( CDC* dc, CRect rect )
{
	if (!m_pGump[TRACK]) {
		CGumpEntity::Draw(dc,rect);
		return;
	}

	m_pGump[TRACK]->DrawGump(dc, rect, GetZoom());
	if (m_pGump[THUMB]) 
		m_pGump[THUMB]->DrawGump(dc, rect.TopLeft(), GetZoom());
}

CGumpEntity* CGumpSlider::CreateFromNode( XML::Node* node )
{
	CGumpSlider* obj = new CGumpSlider(NULL, NULL);
	if (!obj->FromString( node ))
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

BOOL CGumpSlider::FromString( XML::Node* node )
{
	if (!CGumpEntity::FromString(node)) return FALSE;

	int thumb=-1, track=-1, min=0, max=100, pos=0;
	bool vert = false;

	XML::Node* gump_node = node->findNode("gump");
	if (gump_node) { 
		gump_node->lookupAttribute("track", track);
		gump_node->lookupAttribute("thumb", thumb);
	}

	XML::Node* range_node = node->findNode("<range>");
	if (range_node) {
		range_node->lookupAttribute("min", min);
		range_node->lookupAttribute("max", max);
		range_node->lookupAttribute("pos", pos);
	}

	XML::Node* vert_node = node->findNode("<vertical>");
	if (vert_node) vert = vert_node->asBool();
	
	CGumpEditorDoc* pDoc = GfxGetGumpDocument(); ASSERT(pDoc);
	
	SetVertical(vert);
	SetRange(min, max);
	SetPos(max);
	SetGump(pDoc->GetGump(track), pDoc->GetGump(thumb));
	
	return TRUE;
}

CString CGumpSlider::GetString(BOOL bBegin) const
{
	CString ret, str;
	if (bBegin) {
		ret += CGumpEntity::GetString(TRUE) + "\n";
		str.Format(" <gump track='0x%X' thumb='0x%X'/>",
			GetGumpID(TRACK), GetGumpID(THUMB));
		ret += str;
		str.Format(" <range min='%d' max='%d' pos='%d'/>", 
			m_iMin, m_iMax, m_iPos);
		ret += str;
		str.Format(" <vertical>%d</vertical>", 
			m_bVertical);
		ret += str;
	} else {
		ret += CGumpEntity::GetString(FALSE);
	}
	
	return ret;
}

void CGumpSlider::SetVertical(bool bVertical)
{
	m_bVertical = bVertical;
}

void CGumpSlider::RecalSizes()
{
	if (!m_pGump[TRACK]) return;

	CRect rect = GetRect();
	CSize size = m_pGump[TRACK]->GetDimensions();
	CSize sizeMin = size, sizeMax = size;

	if (IsVertical()) {
		size.cy += 100;
		sizeMax.cy += 1000; 
	} else {
		size.cx += 100;
		sizeMax.cx += 1000;
	}
	
	SetConstraints(sizeMin, sizeMax);
	SetRect(rect.left, rect.top, rect.left + size.cx, rect.top + size.cy);
}

void CGumpSlider::SetGump(PART part, CGumpPtr pGump)
{
	m_pGump[part] = pGump;

	if (TRACK == part) RecalSizes();
}

void CGumpSlider::SetGump(CGumpPtr pTrack, CGumpPtr pThumb, bool bUpdateRect)
{
	if (!pTrack || !bUpdateRect) return;

	m_pGump[TRACK] = pTrack;
	m_pGump[THUMB] = pThumb;
	
	RecalSizes();	
}