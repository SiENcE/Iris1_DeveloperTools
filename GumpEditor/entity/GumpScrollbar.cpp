#include "StdAfx.h"
#include "GumpEditor.h"
#include "StdGrfx.h"
#include "GumpScrollbar.h"
#include "GumpEditorDoc.h"

CGumpScrollbar::CGumpScrollbar(CGumpPtr pTrack, bool bVertical, bool bUseArrowButton, int iMin, int iMax, int iPos)
: m_bVertical(bVertical), m_bUseArrowButton(bUseArrowButton), m_iMin(iMin), m_iMax(iMax), m_iPos(iPos)
{
	for (int i = 0; i < NUM_PART; i++)
		//m_pGump[i] = pTrack;
		SetGump(PART(i), pTrack, false);

	SetTitle("scrollbar");
	SetType("scrollbar");
	
	CString strName;
	strName.Format("scrollbar_%x", pTrack? pTrack->GetGumpID() : 0);
	SetName(strName);

	SetEventType(ONCHANGE);
	SetEventHandlerParams("(var sender, var pos)");

	AddPropertyPage( &m_page );
}


CGumpScrollbar::CGumpScrollbar(CGumpScrollbar* pOther)
: m_bVertical(false), m_bUseArrowButton(false), m_iMin(0), m_iMax(100), m_iPos(0)
{
	if (pOther) {
		SetGump(pOther->m_pGump, false);
		m_bVertical = pOther->m_bVertical;
		m_bUseArrowButton = pOther->m_bUseArrowButton;
		m_iMin = pOther->m_iMin;
		m_iMax = pOther->m_iMax;
		m_iPos = pOther->m_iPos;
	}

	SetTitle("scrollbar");
	SetType("scrollbar");
	
	CString strName;
	strName.Format("scrollbar_%x", m_pGump[TRACK] ? m_pGump[TRACK]->GetGumpID() : 0);
	SetName(strName);

	SetEventType(ONCHANGE);
	SetEventHandlerParams("(var sender, var pos)");

	AddPropertyPage( &m_page );
}


CGumpScrollbar::~CGumpScrollbar(void)
{
}

CDiagramEntity* CGumpScrollbar::Clone()
{
	CGumpScrollbar* obj = new CGumpScrollbar(this);
	obj->Copy( this );
	return obj;

}

void CGumpScrollbar::Draw( CDC* dc, CRect rect )
{
	if (!m_pGump[TRACK]) {
		CGumpEntity::Draw(dc,rect);
		return;
	}

	double zoom = GetZoom();
	int x = 0, y = 0;
	CGumpPtr pGump;

	if (m_bUseArrowButton) {
		pGump = m_pGump[LTUP_NORMAL];
		if (pGump) {
			pGump->DrawGump(dc, rect.TopLeft(), zoom);
			if (m_bVertical)
				rect.top  += pGump->GetDimensions().cy;
			else
				rect.left += pGump->GetDimensions().cx;
		}

		pGump = m_pGump[RTDN_NORMAL];
		if (pGump) {
			if (m_bVertical) {
				rect.bottom -= pGump->GetDimensions().cy;
				pGump->DrawGump(dc, CPoint(rect.left,  rect.bottom), zoom);
			} else {
				rect.right  -= pGump->GetDimensions().cx;
				pGump->DrawGump(dc, CPoint(rect.right, rect.top), zoom);
			}
		}
	}

	pGump = m_pGump[TRACK];
	if (pGump)
		pGump->DrawGump(dc, rect, zoom);

	pGump = m_pGump[THUMB];
	if (pGump)
		pGump->DrawGump(dc, rect.TopLeft(), zoom);
}

CGumpEntity* CGumpScrollbar::CreateFromNode( XML::Node* node )
{
	CGumpScrollbar* obj = new CGumpScrollbar(NULL);
	if (!obj->FromString( node ))
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

/*
 * <gump>
 *	<ltup normal=[gump_id] over=[gump_id] pressed=[gump_id]/>
 *  <rtdn normal=[gump_id] over=[gump_id] pressed=[gump_id]/>
 *  <body track=[gump_id] thumb=[gump_id]/>
 * </gump>
 * <range min=[number] max=[number] pos=[min-max]/>
 * <option arrow=[0|1] vert=[0|1]/>
 */
BOOL CGumpScrollbar::FromString( XML::Node* node )
{
	if (!CGumpEntity::FromString(node)) return FALSE;

	int gump_id[NUM_PART];;
	int min = 0, max = 100, pos = 0;
	int arrow = 0, vert = 0; 

	for (int i = 0; i < NUM_PART; i++)
		gump_id[i] = -1;

	XML::Node* gump_node = node->findNode("gump");
	if (gump_node) { 
		XML::Node* body_node = gump_node->findNode("body");
		XML::Node* ltup_node = gump_node->findNode("ltup");
		XML::Node* rtdn_node = gump_node->findNode("rtdn");
		if (body_node) {
			body_node->lookupAttribute("track", gump_id[TRACK]);
			body_node->lookupAttribute("thumb", gump_id[THUMB]);
		}
		if (ltup_node) {
			ltup_node->lookupAttribute("normal", gump_id[LTUP_NORMAL]);
			ltup_node->lookupAttribute("over", gump_id[LTUP_HOVER]);
			ltup_node->lookupAttribute("pressed", gump_id[LTUP_PRESSED]);
		}
		if (rtdn_node) {
			rtdn_node->lookupAttribute("normal", gump_id[RTDN_NORMAL]);
			rtdn_node->lookupAttribute("over", gump_id[RTDN_HOVER]);
			rtdn_node->lookupAttribute("pressed", gump_id[RTDN_PRESSED]);
		}
	}

	XML::Node* range_node = node->findNode("<range>");
	if (range_node) {
		range_node->lookupAttribute("min", min);
		range_node->lookupAttribute("max", max);
		range_node->lookupAttribute("pos", pos);
	}

	XML::Node* option_node = node->findNode("option");
	if (option_node) {
		option_node->lookupAttribute("arrow", arrow);
		option_node->lookupAttribute("vert", vert);
	}
	
	CGumpEditorDoc* pDoc = GfxGetGumpDocument(); ASSERT(pDoc);
	
	SetVertical(vert);
	SetUseArrowButton(arrow);

	SetRange(min, max);
	SetPos(pos);

	for (int i = 0; i < NUM_PART; i++)
		SetGump(PART(i), pDoc->GetGump(gump_id[i]));
	
	return TRUE;
}

CString CGumpScrollbar::GetString(BOOL bBegin) const
{
	CString ret, str;
	if (bBegin) {
		ret += CGumpEntity::GetString(TRUE) + "\n";
		ret += " <gump>\n";
		str.Format("  <body track='0x%X' thumb='0x%X'/>",
			GetGumpID(TRACK), GetGumpID(THUMB));
		ret += str;
		str.Format("  <ltup normal='0x%X' over='0x%X' pressed='0x%X'/>",
			GetGumpID(LTUP_NORMAL), GetGumpID(LTUP_HOVER), GetGumpID(LTUP_PRESSED));
		ret += str;
		str.Format("  <rtdn normal='0x%X' over='0x%X' pressed='0x%X'/>",
			GetGumpID(RTDN_NORMAL), GetGumpID(RTDN_HOVER), GetGumpID(RTDN_PRESSED));
		ret += str;
		ret += " </gump>\n";
		str.Format(" <range min='%d' max='%d' pos='%d'/>", 
			m_iMin, m_iMax, m_iPos);
		ret += str;
		str.Format(" <option arrow='%d' vert='%d'/>", 
			m_bUseArrowButton, m_bVertical);
		ret += str;
	} else {
		ret += CGumpEntity::GetString(FALSE);
	}
	
	return ret;
}

void CGumpScrollbar::SetVertical(bool bVertical)
{
	m_bVertical = bVertical;
}

void CGumpScrollbar::RecalSizes()
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

void CGumpScrollbar::SetGump(PART part, CGumpPtr pGump, bool bUpdateRect)
{
	m_pGump[part] = pGump;

	if (TRACK == part) RecalSizes();
}

void CGumpScrollbar::SetGump(CGumpPtr pGump[], bool bUpdateRect)
{
	//if (!pTrack || !bUpdateRect) return;
	
	for (int i = 0; i < NUM_PART; i++)
		m_pGump[i] = pGump[i];

	RecalSizes();	
}