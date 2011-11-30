#include "stdafx.h"
#include "GumpEditor.h"
#include "gumpcheckbox.h"
#include "StdGrfx.h"
#include "GumpEditorDoc.h"


CGumpCheckbox::CGumpCheckbox(CGumpPtr pNormal, CGumpPtr pChecked, bool bCheck)
{
	//ASSERT(pNormal);
	SetGump(pNormal, pChecked);	
	
	SetTitle("checkbox");
	SetType("checkbox");
	
	CString strName;
	strName.Format("checkbox_%x", pNormal ? pNormal->GetGumpID() : 0);
	SetName(strName);

	SetCheck(bCheck);

	AddPropertyPage( &m_page );
}

CDiagramEntity* CGumpCheckbox::Clone()
{
	CGumpCheckbox* obj = new CGumpCheckbox(m_pGump[NORMAL],m_pGump[CHECKED], IsChecked());
	obj->Copy( this );
	return obj;

}

void CGumpCheckbox::Draw( CDC* dc, CRect rect )
{
	STATE state = IsChecked() ? CHECKED : NORMAL;
	if (!m_pGump[state]) {
		CGumpStatic::DrawCheckbox(dc,rect);
		return;
	}

	CSize size =  m_pGump[state]->GetDimensions();

	if (rect.Size().cy > size.cy) 
		rect.top += (rect.Size().cy - size.cy) / 2;
	
	if (GetTextAlign() == ALIGN_LEFT) {
		m_pGump[state]->DrawGump(dc, rect.TopLeft(), GetZoom());
		rect.left  += size.cx + TEXT_MARGIN;
	} 
	else {
		m_pGump[state]->DrawGump(dc, CPoint(rect.right - size.cx, rect.top), GetZoom());
		rect.right -= size.cx + TEXT_MARGIN;
	}
	
	
	CGumpStatic::Draw(dc, rect);
}

CGumpEntity* CGumpCheckbox::CreateFromNode( XML::Node* node )
{
	CGumpCheckbox* obj = new CGumpCheckbox(NULL,NULL);
	if (!obj->FromString( node )) {
		delete obj;
		obj = NULL;
	}

	return obj;

}

BOOL CGumpCheckbox::FromString( XML::Node* node )
{
	if (!CGumpStatic::FromString(node)) return FALSE;

	int ischecked=0,normal=0,checked=0;

	XML::Node* gump_node = node->findNode("gump");
	if (gump_node) { 
		gump_node->lookupAttribute("normal", normal);
		gump_node->lookupAttribute("checked", checked);
	}
	
	XML::Node* check_node = node->findNode("checked");
	if (check_node) {
		ischecked = check_node->asBool();
	}

	SetCheck(ischecked);

	CSize size = GetRect().Size();
	SetConstraints(size,size);
	
	CGumpEditorDoc* pDoc = GfxGetGumpDocument(); ASSERT(pDoc);
	
	m_pGump[NORMAL]  = pDoc->GetGump(normal);
	m_pGump[CHECKED] = pDoc->GetGump(checked);
	
	return TRUE;
}


CString CGumpCheckbox::GetString(BOOL bBegin) const
{
	CString ret, str;
	if (bBegin) {
		ret += CGumpStatic::GetString(TRUE) + "\n";
		str.Format(" <gump normal='0x%X' checked='0x%X'/>\n", 
			GetGumpID(NORMAL), GetGumpID(CHECKED));
		ret += str;
		str.Format(" <checked>%d</checked>", IsChecked());
		ret += str;
	} else {
		ret += CGumpStatic::GetString(FALSE);
	}
	
	return ret;
}

bool CGumpCheckbox::SetGump(CGumpPtr pNormal, CGumpPtr pChecked)
{
	if (!pNormal || !pChecked) return false;

	SetGump(NORMAL, pNormal);
	SetGump(CHECKED, pChecked);

	return true;
}


void CGumpCheckbox::GetGump(CGumpPtr& pNormal, CGumpPtr& pChecked)
{
	pNormal  = m_pGump[NORMAL];
	pChecked = m_pGump[CHECKED];
}

CGumpPtr CGumpCheckbox::GetGump(STATE state)
{
	return m_pGump[state];
}

int CGumpCheckbox::GetGumpID(STATE state) const
{
	return m_pGump[state] ? m_pGump[state]->GetGumpID() : -1;
}

void CGumpCheckbox::SetGump(STATE state, CGumpPtr pGump)
{
	//ASSERT(pGump);
	m_pGump[state] = pGump;

	if (NORMAL==state && pGump) {
		CRect rect = GetRect();
		CSize size = pGump->GetDimensions();
		size.cx += DEFAULT_SIZE;
		//SetConstraints(size,size);
		
		SetRect(rect.left, rect.top, rect.left + size.cx, rect.top + size.cy);
	}
}

