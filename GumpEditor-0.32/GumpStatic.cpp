#include "stdafx.h"
#include "GumpEditor.h"
#include ".\gumpstatic.h"
#include "GumpEditorDoc.h"
#include "StdGrfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGumpStatic


CGumpStatic::CGumpStatic(int hueId, int fontId, TEXT_ALIGN textAlign) 
: m_textAlign(textAlign), m_hueId(hueId), m_fontId(fontId)
{
	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle("label");
	SetType("label");
	SetName("label");

	AddPropertyPage( &m_page );
}

CGumpEntity* CGumpStatic::Clone()
{
	CGumpStatic* obj = new CGumpStatic(GetHueId(), GetFontId(), GetTextAlign());
	obj->Copy( this );
	return obj;
}

void CGumpStatic::Draw( CDC* dc, CRect rect )
{
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

	CFont* pFont = pDoc->GetFont(m_fontId);
	CFont* pOld = dc->SelectObject(pFont);

	UINT align[] = { TA_LEFT, DT_CENTER, TA_RIGHT };
	UINT format =  DT_TOP | DT_SINGLELINE | align[m_textAlign];
	int oldMode = dc->SetBkMode( TRANSPARENT );
	
	
	COLORREF color = pDoc->GetHueColor(m_hueId, m_fontId);
	COLORREF oldColor = dc->SetTextColor(color);
	
	dc->DrawText( GetTitle(), rect, format);
	
	dc->SetBkMode(oldMode);
	dc->SetTextColor(oldColor);

	dc->SelectStockObject( ANSI_VAR_FONT );
	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );
}


CGumpEntity* CGumpStatic::CreateFromString( XML::Node* node )
{
	CGumpStatic* obj = new CGumpStatic;
	if(!obj->FromString( node ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;
}

BOOL CGumpStatic::FromString( XML::Node* node )
{
	if (!CGumpEntity::FromString(node)) return FALSE;

	std::string title, align;
	int font=0,hue=0,password=0;

	XML::Node* text_node = node->findNode("text");
	if (text_node) {
		title = text_node->asString();
		text_node->lookupAttribute("font", font);
		text_node->lookupAttribute("hue", hue);
		text_node->lookupAttribute("align", align);
	}
	
	SetTitle(title.c_str());

	m_fontId = font;
	m_hueId = hue;
	m_textAlign = GfxTexttoAlign(align.c_str());
	
	return TRUE;
}

CString	CGumpStatic::GetString(BOOL bBegin) const
{
	CString ret, str;
	if (bBegin) {
		ret += CGumpEntity::GetString(TRUE) + "\n";
		str.Format(" <text font='%d' hue='0x%X' align='%s'>%s</text>", 
			GetFontId(), GetHueId(), GfxAligntoText(m_textAlign), GetTitle());
		ret += str;
	} else {
		ret += CGumpEntity::GetString(FALSE);
	}
	
	return ret;
}

