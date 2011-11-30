#include "stdafx.h"
#include "GumpEditor.h"
#include ".\gumpbutton.h"
#include "StdGrfx.h"
#include "GumpEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGumpButton

LPCTSTR CGumpButton::m_szButtonType[CGumpButton::NUM_TYPE] =
{
	"button", "checkbox", "radio"
};

void CGumpButton::SetButtonTypeStr(CString strType)
{
	m_eType = (TYPE)StrToEnum(strType, m_szButtonType, NUM_TYPE, m_eType);
}

CGumpButton::CGumpButton(CGumpPtr pNormal, CGumpPtr pHover, CGumpPtr pPressed, 
						 TYPE eType, bool bChecked, bool bGrouped)
{
	//ASSERT(pNormal);
	SetButtonType(eType);
	SetChecked(bChecked);
	SetGrouped(bGrouped);

	SetGump(pNormal, pHover, pPressed);	
	
	SetTitle("button");
	SetType("button");
	
	CString strName;
	strName.Format("button_%x", pNormal ? pNormal->GetGumpID() : 0);
	SetName(strName);

	AddPropertyPage( &m_page );
}

CDiagramEntity* CGumpButton::Clone()
{
	CGumpButton* obj = new CGumpButton(m_pGump[NORMAL], m_pGump[HOVER], m_pGump[PRESSED],
		m_eType, m_bChecked, m_bGrouped);
	obj->Copy( this );
	return obj;
}


void CGumpButton::Draw( CDC* dc, CRect rect )
{
	CGumpPtr pGump = m_pGump[NORMAL];
	
	if (!pGump) {
		if (m_eType == RADIO) 
			CGumpEntity::DrawRadiobutton(dc, rect);
		else if (m_eType == CHECKBOX)
			CGumpEntity::DrawCheckbox(dc, rect);
		else
			CGumpEntity::DrawButton(dc, rect);
		return;
	}
	
	if (m_eType == BUTTON) {
		pGump->DrawGump(dc, rect.TopLeft(), GetZoom());
		return;
	}

	CSize size =  pGump->GetDimensions();

	if (rect.Size().cy > size.cy) 
		rect.top += (rect.Size().cy - size.cy) / 2;
	
	if (GetTextAlign() == ALIGN_LEFT) {
		pGump->DrawGump(dc, rect.TopLeft(), GetZoom());
		rect.left  += size.cx + TEXT_MARGIN;
	} else {
		pGump->DrawGump(dc, CPoint(rect.right - size.cx, rect.top), GetZoom());
		rect.right -= size.cx + TEXT_MARGIN;
	}
	
	
	CGumpStatic::Draw(dc, rect);

}

CGumpEntity* CGumpButton::CreateFromNode( XML::Node* node )
{
	CGumpButton* obj = new CGumpButton(NULL,NULL,NULL);
	if (!obj->FromString( node )) {
		delete obj;
		obj = NULL;
	}

	return obj;

}

BOOL CGumpButton::FromString( XML::Node* node )
{
	if (!CGumpStatic::FromString(node)) return FALSE;

	int normal=0, over=0, pressed=0;

	XML::Node* gump_node = node->findNode("gump");
	if (gump_node) { 
		gump_node->lookupAttribute("normal", normal);
		gump_node->lookupAttribute("over", over);
		gump_node->lookupAttribute("pressed", pressed);
	}

	std::string type="push";
	int checked=0, grouped=0;
	XML::Node* button_node = node->findNode("button");
	if (button_node) {
		button_node->lookupAttribute("type", type);
		button_node->lookupAttribute("checked", checked);
		button_node->lookupAttribute("grouped", grouped);
	}

	CSize size = GetRect().Size();
	//SetConstraints(size,size);
	
	CGumpEditorDoc* pDoc = GfxGetGumpDocument(); ASSERT(pDoc);
	
	m_pGump[NORMAL] = pDoc->GetGump(normal);
	m_pGump[HOVER] = pDoc->GetGump(over);
	m_pGump[PRESSED] = pDoc->GetGump(pressed);

	SetButtonTypeStr(type.c_str());
	SetChecked(checked);
	SetGrouped(grouped);

	if (GetButtonType() == BUTTON) {	
		SetConstraints(size,size);
	} else {
		size.cx += DEFAULT_SIZE;
		SetConstraints(CSize(1, 1), CSize(-1, -1));
	}
	
	return TRUE;
}


CString CGumpButton::GetString(BOOL bBegin) const
{
	CString ret, str;
	if (bBegin) {
		ret += CGumpStatic::GetString(TRUE) + "\n";
		str.Format(" <gump normal='0x%X' over='0x%X' pressed='0x%X'/>", 
			GetGumpID(NORMAL), GetGumpID(HOVER), GetGumpID(PRESSED));
		ret += str;
		str.Format(" <button type='%s' checked='%d' grouped='%d'/>",
			GetButtonTypeStr(), IsChecked(), IsGrouped());
		ret += str;
	} else {
		ret += CGumpStatic::GetString(FALSE);
	}
	
	return ret;
}

bool CGumpButton::SetGump(CGumpPtr pNormal, CGumpPtr pHover, CGumpPtr pPressed)
{
	if (!pNormal || !pHover || !pPressed) return false;

	SetGump(NORMAL, pNormal);
	SetGump(HOVER, pHover);
	SetGump(PRESSED, pPressed);

	return true;
}


void CGumpButton::GetGump(CGumpPtr& pNormal, CGumpPtr& pHover, CGumpPtr& pPressed)
{
	pNormal = m_pGump[NORMAL];
	pHover = m_pGump[HOVER];
	pPressed = m_pGump[PRESSED];
}

CGumpPtr CGumpButton::GetGump(STATE state)
{
	return m_pGump[state];
}

int CGumpButton::GetGumpID(STATE state) const
{
	return m_pGump[state] ? m_pGump[state]->GetGumpID() : -1;
}

void CGumpButton::SetGump(STATE state, CGumpPtr pGump)
{
	//ASSERT(pGump);
	m_pGump[state] = pGump;

	if (NORMAL==state && pGump) {
		CRect rect = GetRect();
		CSize size = pGump->GetDimensions();
		if (m_eType == BUTTON) {	
			SetConstraints(size,size);
		} else {
			size.cx += DEFAULT_SIZE;
			SetConstraints(CSize(1, 1), CSize(-1, -1));
		}
		SetRect(rect.left, rect.top, rect.left + size.cx, rect.top + size.cy);
	}
}

void CGumpButton::SetButtonType(TYPE eType) 
{ 
	if (m_eType == eType) return;
	bool bDontUpdateSize = m_eType != BUTTON && eType != BUTTON;
	
	m_eType = eType; 
	CGumpPtr pGump = m_pGump[NORMAL];

	if (!pGump || bDontUpdateSize) return;
	
	CSize size = pGump->GetDimensions();

	if (m_eType == BUTTON) {	
		SetConstraints(size,size);
	} else {
		size.cx += DEFAULT_SIZE;
		SetConstraints(CSize(1, 1), CSize(-1, -1));
	}
}

