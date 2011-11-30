#include "stdafx.h"
#include "GumpEditor.h"
#include "diagram/DiagramEditor/DiagramEntity.h"
#include "gumpradio.h"
#include "StdGrfx.h"
#include "GumpEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGumpRadio

CGumpRadio::CGumpRadio(CGumpPtr pNormal, CGumpPtr pChecked, int iGroupID, bool bCheck)
: CGumpCheckbox(pNormal,pChecked,bCheck)
{
//	SetGump(pNormal, pChecked);	
	
	SetTitle("radio");
	SetType("radio");
	
	CString strName;
	strName.Format("radio_%x", pNormal ? pNormal->GetGumpID() : 0);
	SetName(strName);

	SetGroupID(iGroupID);

	AddPropertyPage( &m_page );
}

CDiagramEntity* CGumpRadio::Clone()
{
	CGumpRadio* obj = new CGumpRadio(m_pGump[NORMAL],m_pGump[CHECKED], GetGroupID(), IsChecked());
	obj->Copy( this );
	return obj;

}

void CGumpRadio::Draw( CDC* dc, CRect rect )
{
	STATE state = IsChecked() ? CHECKED : NORMAL;
	if (!m_pGump[state]) {
		CGumpEntity::DrawRadiobutton(dc,rect);
		return;
	}
	
	m_pGump[state]->DrawGump(dc, rect.TopLeft(), GetZoom());
}


CGumpEntity* CGumpRadio::CreateFromNode( XML::Node* node )
{
	CGumpRadio* obj = new CGumpRadio(NULL,NULL);
	if (!obj->FromString( node ))
	{
		delete obj;
		obj = NULL;
	}

	return obj;
}

BOOL CGumpRadio::FromString( XML::Node* node )
{
	if (!CGumpCheckbox::FromString(node)) return FALSE;
	int group=0;

	XML::Node* group_node = node->findNode("group");
	if (group_node) {
		group = group_node->asInteger();
	}

	SetGroupID(group);
	
	return TRUE;
}


CString CGumpRadio::GetString(BOOL bBegin) const
{
	CString ret, str;
	if (bBegin) {
		ret += CGumpCheckbox::GetString(TRUE) + "\n";
		str.Format(" <group>%d</group>", GetGroupID());
		ret += str;
	} else {
		ret += CGumpCheckbox::GetString(FALSE);
	}
	
	return ret;
}

