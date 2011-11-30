#include "stdafx.h"
#include "GumpEditor.h"
#include ".\gumpedit.h"
#include "GumpEditorDoc.h"
#include "StdGrfx.h"

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
	SetName( "edit" );

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
#if 0
	CStdGrfx::drawsunkenframed3dWindow( dc, rect );

	dc->SelectObject( CStdGrfx::dialogBrush() );
	dc->SelectObject( CStdGrfx::dialogPen() );

	CGumpEditorDoc* pDoc = GfxGetGumpDocument();
	ASSERT(pDoc);

	//dc->Rectangle( rect );

	//LOGFONT lf;
	//CFont font;
	//GetFont( lf );
	//// MS Sans Serif will not scale below 8 pts
	//if( GetZoom() < 1 )
	//	lstrcpy( lf.lfFaceName, _T( "Arial" ) );

	//
	//font.CreateFontIndirect( &lf );

	rect.DeflateRect(2,2,2,2);

	CFont* pFont = pDoc->GetFont(GetFontId());
	CFont* pOld = dc->SelectObject(pFont);

	UINT align[] = { TA_LEFT, DT_CENTER, TA_RIGHT };
	UINT format =  DT_VCENTER | DT_SINGLELINE | align[GetTextAlign()];
	int oldMode = dc->SetBkMode( TRANSPARENT );
	
	COLORREF color = pDoc->GetHueColor(GetHueId(), GetFontId());
	COLORREF oldColor = dc->SetTextColor(color);
	
	CString strText = GetTitle();
	if (m_passwordMode) strText = CString('*', strText.GetLength());

	dc->DrawText( strText, rect, format);
	
	dc->SetBkMode(oldMode);
	dc->SetTextColor(oldColor);

	dc->SelectStockObject( ANSI_VAR_FONT );
	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );
#endif

}

CGumpEntity* CGumpEdit::CreateFromString( XML::Node* node )
{
	CGumpEdit* obj = new CGumpEdit;
	if(!obj->FromString( node ) )
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
