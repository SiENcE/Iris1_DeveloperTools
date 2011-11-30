#include "StdAfx.h"

#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpEditorDoc.h"
#include "StdGrfx.h"
#include ".\gumpentity.h"

/////////////////////////////////////////////////////////////////////////////
// CGumpEntity

void CGumpEntity::GetEventHandler(CString& strEvClick, CString& strEvClose, 
								  CString& strEvMouseUp, CString& strEvMouseDown, CString& strEvKeyPressed) const
{
	strEvClick = GetEventHandler(ONCLICK);
	strEvClose = GetEventHandler(ONCLOSE);
	strEvMouseUp = GetEventHandler(ONMOUSEUP);
	strEvMouseDown = GetEventHandler(ONMOUSEDOWN);
	strEvKeyPressed = GetEventHandler(ONKEYPRESSED);
}

void CGumpEntity::SetEventHandler(LPCTSTR szEvClick, LPCTSTR szEvClose, 
								  LPCTSTR szEvMouseUp, LPCTSTR szEvMouseDown, LPCTSTR szEvKeyPressed)
{
	SetEventHandler(ONCLICK, szEvClick);
	SetEventHandler(ONCLOSE, szEvClose);
	SetEventHandler(ONMOUSEUP, szEvMouseUp);
	SetEventHandler(ONMOUSEDOWN, szEvMouseDown);
	SetEventHandler(ONKEYPRESSED, szEvKeyPressed);
}


CGumpEntity::CGumpEntity()
{
	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle( "control" );
	SetType( "control" );
	SetName( "control" );
	SetAlpha(0);
	SetFlags(0);

	AddPropertyPage( &m_page );
}

CDiagramEntity* CGumpEntity::Clone()
{
	CGumpEntity* obj = new CGumpEntity;
	obj->Copy( this );
	return obj;
}

void CGumpEntity::Copy(CDiagramEntity* obj)
{
	CDiagramEntity::Copy(obj);
	CGumpEntity* e = reinterpret_cast<CGumpEntity*>(obj);
	if (!e) return;
	m_iAlpha = e->m_iAlpha;
	m_iFlags = e->m_iFlags;
	for (int i = 0; i < NUM_EVENT; i++)
		m_strEventHandler[i] = e->m_strEventHandler[i];
}

void CGumpEntity::Draw( CDC* dc, CRect rect )
{
	CStdGrfx::drawframed3dBox( dc, rect );
	dc->SetBkMode( TRANSPARENT );
	dc->DrawText( GetTitle(), rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	return;
}

CDiagramEntity* CGumpEntity::CreateFromString( XML::Node* node )
{
	CGumpEntity* obj = new CGumpEntity;
	if(!obj->FromString( node ) ) {
		delete obj;
		obj = NULL;
	}

	return obj;

}

BOOL CGumpEntity::FromString( XML::Node* node )
{
	if (!CDiagramEntity::FromString(node)) return FALSE;

	std::string onclick, onclose, onmouseup, onmousedown, onkeypressed;
	int alpha=0,flags=0;

	node->lookupAttribute("alpha", alpha);
	node->lookupAttribute("flags", flags);
	
	XML::Node* event_node = node->findNode("event");
	if (event_node) {
		event_node->lookupAttribute("onclick", onclick);
		event_node->lookupAttribute("onclose", onclose);
		event_node->lookupAttribute("onmouseup", onmouseup);
		event_node->lookupAttribute("onmousedown", onmousedown);
		event_node->lookupAttribute("onkeypressed", onkeypressed);
	}
	
	SetAlpha(alpha);
	SetFlags(flags);

	SetEventHandler(onclick.c_str(), onclose.c_str(), onmouseup.c_str(), onmousedown.c_str(), onkeypressed.c_str());

	return TRUE;
}


CString CGumpEntity::GetString(BOOL bBegin) const
{
	CString str, ret;

	if (bBegin) {
		CRect rect = GetRect();
		str.Format("<control class='%s' name='%s' left='%d' top='%d' width='%d' height='%d' freezed='%d' visible='%d' alpha='%d' flags='%d'>\n", 
			GetType(), GetName(), rect.left, rect.top, rect.Width(), rect.Height(), IsFreezed(), IsVisible(), GetAlpha(), GetFlags());
		ret += str;
		str.Format(" <event onclick='%s' onclose='%s' onmouseup='%s' onmousedown='%s' onkeypressed='%s'/>",
			GetEventHandler(ONCLICK), GetEventHandler(ONCLOSE), GetEventHandler(ONMOUSEUP), GetEventHandler(ONMOUSEDOWN), GetEventHandler(ONKEYPRESSED));
		ret += str;
	} else {
		ret = "</control>";
	}

	return ret;
}




