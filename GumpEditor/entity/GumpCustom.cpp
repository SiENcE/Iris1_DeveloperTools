#include "StdAfx.h"
#include "GumpEditor.h"
#include "StdGrfx.h"
#include "gumpcustom.h"
//#include "GumpEditorDoc.h"


CGumpCustom::CGumpCustom(LPCTSTR szCustom)
{
	SetTitle(_T("Custom"));
	SetType(_T("custom"));
	
	CString strName;
	strName.Format(_T("custom_%x"), 0);
	SetName(strName);

	SetCustom(szCustom);

	AddPropertyPage( &m_page );
}

CGumpCustom::~CGumpCustom(void)
{
}

CDiagramEntity* CGumpCustom::Clone()
{
	CGumpCustom* obj = new CGumpCustom(GetCustom());
	obj->Copy( this );
	return obj;
}

#if 0
void CGumpCustom::Copy(CDiagramEntity* obj)
{

}
#endif

void CGumpCustom::Draw( CDC* dc, CRect rect )
{
	if (!m_pGump) {
		CGumpEntity::Draw(dc,rect);
		return;
	}
	
	m_pGump->DrawGump(dc, rect, GetZoom());
}


CGumpEntity* CGumpCustom::CreateFromNode( XML::Node* node )
{

	CGumpCustom* obj = new CGumpCustom(NULL);
	if (!obj->FromString( node ))
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

BOOL CGumpCustom::FromString( XML::Node* node )
{
	if (!CGumpEntity::FromString(node)) return FALSE;
	
	XML::Node* custom_node = node->findNode("custom");
	if (custom_node) {
		SetCustom(custom_node->getRawData(false).c_str());
	}
	
	return TRUE;
}

CString	CGumpCustom::GetString(BOOL bBegin) const
{
	CString str, pic;
	if (bBegin) {
		str += CGumpEntity::GetString(TRUE) + "\n";
		str += _T(" <custom>\n");
		str += GetCustom();
	} else {
		str += _T(" </custom>\n");
		str += CGumpEntity::GetString(FALSE);
	}
	
	return str;
}
