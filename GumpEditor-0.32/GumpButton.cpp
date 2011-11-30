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

CGumpButton::CGumpButton(CGumpPtr pNormal, CGumpPtr pHover, CGumpPtr pPressed)
{
	//ASSERT(pNormal);
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
	CGumpButton* obj = new CGumpButton(m_pGump[NORMAL],m_pGump[HOVER],m_pGump[PRESSED]);
	obj->Copy( this );
	return obj;
}


void CGumpButton::Draw( CDC* dc, CRect rect )
{
	if (!m_pGump[NORMAL]) {
		CGumpEntity::Draw(dc,rect);
		return;
	}
	
	//m_pGump[NORMAL]->DrawTransparent(dc, rect.TopLeft(), RGB(255,255,255));
	m_pGump[NORMAL]->DrawGump(dc, rect.TopLeft(), GetZoom());
}

CDiagramEntity* CGumpButton::CreateFromString( XML::Node* node )
{
	CGumpButton* obj = new CGumpButton(NULL,NULL,NULL);
	if(!obj->FromString( node ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

BOOL CGumpButton::FromString( XML::Node* node )
{
	if (!CGumpEntity::FromString(node)) return FALSE;

	int normal, over, pressed;;

	XML::Node* gump_node = node->findNode("gump");
	if (gump_node) { 
		gump_node->lookupAttribute("normal", normal);
		gump_node->lookupAttribute("over", over);
		gump_node->lookupAttribute("pressed", pressed);
	}

	CSize size = GetRect().Size();
	SetConstraints(size,size);
	
	CGumpEditorDoc* pDoc = GfxGetGumpDocument(); ASSERT(pDoc);
	
	m_pGump[NORMAL] = pDoc->LoadGump(normal);
	m_pGump[HOVER] = pDoc->LoadGump(over);
	m_pGump[PRESSED] = pDoc->LoadGump(pressed);
	
	return TRUE;
}


CString CGumpButton::GetString(BOOL bBegin) const
{
	CString ret, str;
	if (bBegin) {
		ret += CGumpEntity::GetString(TRUE) + "\n";
		str.Format(" <gump normal='0x%X' over='0x%X' pressed='0x%X'/>", 
			GetGumpID(NORMAL), GetGumpID(HOVER), GetGumpID(PRESSED));
		ret += str;
	} else {
		ret += CGumpEntity::GetString(FALSE);
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
		SetConstraints(size,size);
		SetRect(rect.left,rect.top,rect.left+size.cx,rect.top+size.cy);
	}
}
