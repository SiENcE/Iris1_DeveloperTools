#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpEditorDoc.h"
#include "GumpEdit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGumpEdit


CGumpEdit::CGumpEdit(int hueId, int fontId, TEXT_ALIGN textAlign, bool passwordMode) 
: CGumpStatic(hueId, fontId, textAlign)
, m_passwordMode(passwordMode)
{
	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle( "edit" );
	SetType( "edit" );
	//SetName( "edit" );

	SetDefaultEntityName();

	m_eEventType = ONCHANGE;

	AddPropertyPage( &m_page );
}

CGumpEntity* CGumpEdit::Clone()
{
	CGumpEdit* obj = new CGumpEdit(GetHueId(), GetFontId(), GetTextAlign(), IsPasswordMode());
	obj->Copy( this );
	return obj;
}

void CGumpEdit::Draw( CDC* dc, CRect rect )
{
	CString strText = GetTitle();
	if (m_passwordMode) SetTitle(CString('*', strText.GetLength()));
	CGumpStatic::Draw(dc,rect);
	if (m_passwordMode) SetTitle(strText);
}

CGumpEntity* CGumpEdit::CreateFromNode( XML::Node* node )
{
	CGumpEdit* obj = new CGumpEdit;
	if (!obj->FromString( node ))
	{
		delete obj;
		obj = NULL;
	}

	return obj;
}

BOOL CGumpEdit::FromString( XML::Node* node )
{
	if (!CGumpStatic::FromString(node)) return FALSE;

	int password=0;

	XML::Node* passwd_node = node->findNode("password");
	if (passwd_node) {
		password = passwd_node->asBool();
	}

	m_passwordMode = (bool)password;
	
	return TRUE;
}


CString CGumpEdit::GetString(BOOL bBegin) const
{
	CString ret, str;
	if (bBegin) {
		ret += CGumpStatic::GetString(TRUE) + "\n";
		str.Format(" <password>%d</password>", IsPasswordMode());
		ret += str;
	} else {
		ret += CGumpStatic::GetString(FALSE);
	}
	
	return ret;
}
