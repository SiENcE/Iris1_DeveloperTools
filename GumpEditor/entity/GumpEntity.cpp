#include "StdAfx.h"

#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpEditorDoc.h"
#include "StdGrfx.h"
#include ".\gumpentity.h"

/////////////////////////////////////////////////////////////////////////////
// CGumpEntity

int CGumpEntity::StrToEnum(CString str, LPCTSTR *szEnum, int iEnumSize, int iDefault)
{
	for (int i = 0; i < iEnumSize; i++)
		if (str.CompareNoCase(szEnum[i])==0) return i;

	return iDefault;
}

UINT CGumpEntity::m_iEnitiyCount = 0;
LPCTSTR CGumpEntity::m_szEventTypeNames[CGumpEntity::NUM_EVENT] = 
{ "onclick", "onchange", "ondragdrop", "ontimer" };

LPCTSTR CGumpEntity::m_szEventTypeParams[CGumpEntity::NUM_EVENT] = 
{ "(var sender)", "(var sender, var pos)", "(var sender)", "(var sender)" };

void CGumpEntity::SetDefaultEntityName()
{
	CString strName;
	strName.Format("%s_%x", GetType(), ++m_iEnitiyCount);
	SetName(strName);
}

CGumpEntity::EVENT CGumpEntity::GetEventTypeByName(CString strEventTypeName, EVENT eDefault)
{
	return (EVENT)StrToEnum(strEventTypeName, m_szEventTypeNames, NUM_EVENT, eDefault);
}


void CGumpEntity::SetDefaultEventHandler(LPCTSTR szPrefix)
{
	CString strEvHandler;
	if (szPrefix)
		strEvHandler.Format("%s_%s_%s", szPrefix, (LPCTSTR)GetName(), (LPCTSTR)GetEventTypeName());
	else
		strEvHandler.Format("%s_%s", (LPCTSTR)GetName(), (LPCTSTR)GetEventTypeName());
	SetEventHandler(strEvHandler);
	//SetEventHandlerParams(m_szEventTypeParams[m_eEventType]);
}


CGumpEntity::CGumpEntity()
{
	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle( "control" );
	SetType( "control" );
	SetName( "control" );

	SetAlpha(0);
	SetFlags(0);

	SetEventType(ONCLICK);
	SetEventHandlerParams("(var sender)");

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

	m_eEventType = e->m_eEventType;
	m_strEventHandler = e->m_strEventHandler;
	//for (int i = 0; i < NUM_EVENT; i++)
	//	m_strEventHandler[i] = e->m_strEventHandler[i];
}

void CGumpEntity::Draw( CDC* dc, CRect rect )
{
	CStdGrfx::drawframed3dBox( dc, rect );
	dc->SetBkMode( TRANSPARENT );
	dc->DrawText( GetTitle(), rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	return;
}

CGumpEntity* CGumpEntity::CreateFromNode( XML::Node* node )
{
	CGumpEntity* obj = new CGumpEntity;
	if (!obj->FromString( node )) {
		delete obj;
		obj = NULL;
	}

	return obj;

}

BOOL CGumpEntity::FromString( XML::Node* node )
{
	//if (!CDiagramEntity::FromString(node)) return FALSE;

	{
		std::string type, name;
		int left=0, top=0, width=0, height=0, selected=0, freezed=0, visible=1;

		if (!node->lookupAttribute("class", type)) return FALSE;
		if (GetType().CompareNoCase(type.c_str()) != 0) return FALSE;

		node->lookupAttribute("name", name);
		node->lookupAttribute("left", left);
		node->lookupAttribute("top", top);
		node->lookupAttribute("width", width);
		node->lookupAttribute("height", height);

		XML::Node* meta_node = node->findNode("meta");
		if (meta_node) {
			meta_node->lookupAttribute("selected", selected);
			meta_node->lookupAttribute("freezed", freezed);
			meta_node->lookupAttribute("visible", visible);
		}

		SetType(type.c_str());
		SetName(name.c_str());

		SetRect(left,top,left+width,top+height);
		Freeze(freezed);
		SetVisible(visible);
		//return TRUE;
	}

	std::string ev_type, ev_handler, flags;
	int alpha=0;

	node->lookupAttribute("alpha", alpha);
	node->lookupAttribute("flags", flags);

	XML::Node* event_node = node->findNode("event");
	if (event_node) {
		event_node->lookupAttribute("type", ev_type);
		ev_handler = event_node->asString();
	}

	SetAlpha(alpha);
	SetFlags(GfxAtoX(flags.c_str()));

	SetEventTypeByName(ev_type.c_str());
	SetEventHandler(ev_handler.c_str());

	return TRUE;
}


CString CGumpEntity::GetString(BOOL bBegin) const
{
	CString str, ret;

	if (bBegin) {
		CRect rect = GetRect();
		str.Format("<control class='%s' name='%s' left='%d' top='%d' width='%d' height='%d' alpha='%d' flags='0x%X'>\n", 
			GetType(), GetName(), rect.left, rect.top, rect.Width(), rect.Height(), GetAlpha(), GetFlags());
		ret += str;
		str.Format(" <meta selected='%d' freezed='%d' visible='%d'/>", IsSelected(), IsFreezed(), IsVisible());
		ret += str;
		if (GetEventHandler().GetLength() > 0) {
			str.Format(" <event type='%s'>%s</event>", 
				(LPCTSTR)GetEventTypeName().MakeLower(), (LPCTSTR)GetEventHandler());
			ret += str;
		}
	} else {
		ret = "</control>";
	}

	return ret;
}


void CGumpEntity::DrawEdit(CDC* pDC, CRect rect)
{
	CStdGrfx::drawsunkenframed3dWindow( pDC, rect );
}

void CGumpEntity::DrawCombobox(CDC* pDC, CRect rect)
{
	CStdGrfx::drawsunkenframed3dWindow( pDC, rect );

	LOGFONT lf;
	GetFont( lf );
	pDC->SetBkMode( TRANSPARENT );
	pDC->SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );

	CRect rectArrow( rect );
	rectArrow.left = rectArrow.right - ( int ) ( ( double ) GetMinimumSize().cy * GetZoom() );
	rectArrow.InflateRect( -2, -2 );

	CStdGrfx::drawframed3dBox( pDC, rectArrow );

	CFont chk;
	chk.CreateFont( lf.lfHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Marlett" );
	pDC->SelectObject( &chk );
	pDC->DrawText( "u", rectArrow, DT_VCENTER | DT_SINGLELINE | DT_CENTER );

	pDC->SelectStockObject( DEFAULT_GUI_FONT );
}

void CGumpEntity::DrawButton(CDC* pDC, CRect rect)
{
	CStdGrfx::drawframed3dBox( pDC, rect );

	LOGFONT lf;
	CFont font;

	GetFont( lf );
	// MS Sans Serif will not scale below 8 pts.
	if( GetZoom() < 1 )
		lstrcpy( lf.lfFaceName, _T( "Arial" ) );

	font.CreateFontIndirect( &lf );

	pDC->SelectObject( &font );

	pDC->SetBkMode( TRANSPARENT );
	pDC->DrawText( GetTitle(), rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	pDC->SelectStockObject( ANSI_VAR_FONT );
}

void CGumpEntity::DrawCheckbox(CDC* pDC, CRect rect)
{
	CStdGrfx::drawsunkenframed3dWindow( pDC, rect );

	LOGFONT lf;
	GetFont( lf );
	pDC->SetBkMode( TRANSPARENT );
	pDC->SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );

	CRect rectArrow( rect );
	rectArrow.left = rectArrow.right - ( int ) ( ( double ) GetMinimumSize().cy * GetZoom() );
	rectArrow.InflateRect( -2, -2 );

	CStdGrfx::drawframed3dBox( pDC, rectArrow );

	CFont chk;
	chk.CreateFont( lf.lfHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Marlett" );
	pDC->SelectObject( &chk );
	pDC->DrawText( "u", rectArrow, DT_VCENTER | DT_SINGLELINE | DT_CENTER );

	pDC->SelectStockObject( DEFAULT_GUI_FONT );
}

void CGumpEntity::DrawRadiobutton(CDC* pDC, CRect rect)
{
	pDC->SelectObject( CStdGrfx::dialogBrush() );
	pDC->SelectObject( CStdGrfx::dialogPen() );

	pDC->Rectangle( rect );

	LOGFONT lf;
	CFont chk;
	CFont font;

	GetFont( lf );
	// MS Sans Serif will not scale below 8 pts
	if( GetZoom() < 1 )
		lstrcpy( lf.lfFaceName, _T( "Arial" ) );
	font.CreateFontIndirect( &lf );

	// Marlett is used for the circle
	chk.CreateFont(  ( int ) ( ( double ) lf.lfHeight * 1.25 ), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Marlett" );
	pDC->SetBkMode( TRANSPARENT );
	pDC->SelectObject( &chk );

	pDC->SetTextColor( ::GetSysColor( COLOR_WINDOW ) );
	pDC->TextOut( rect.left, rect.top, "n" );

	pDC->SetTextColor( ::GetSysColor( COLOR_3DSHADOW ) );
	pDC->TextOut( rect.left, rect.top, "j" );

	pDC->SetTextColor( ::GetSysColor( COLOR_3DHIGHLIGHT  ) );
	pDC->TextOut( rect.left, rect.top, "k" );

	pDC->SetTextColor( ::GetSysColor( COLOR_3DDKSHADOW ) );
	pDC->TextOut( rect.left, rect.top, "l" );

	pDC->SetTextColor( ::GetSysColor( COLOR_3DLIGHT ) );
	pDC->TextOut( rect.left, rect.top, "m" );

	pDC->SelectObject( &font );

	pDC->SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );
	rect.left += ( int ) ( ( double ) abs( lf.lfHeight ) * 1.5 );
	pDC->DrawText( GetTitle(), rect, DT_SINGLELINE );

	pDC->SelectStockObject( DEFAULT_GUI_FONT );
	pDC->SelectStockObject( BLACK_PEN );
	pDC->SelectStockObject( WHITE_BRUSH );
}

void CGumpEntity::DrawStatic(CDC* pDC, CRect rect)
{
	pDC->SelectObject( CStdGrfx::dialogBrush() );
	pDC->SelectObject( CStdGrfx::dialogPen() );

	pDC->Rectangle( rect );

	LOGFONT lf;
	CFont font;
	GetFont( lf );
	// MS Sans Serif will not scale below 8 pts
	if( GetZoom() < 1 )
		lstrcpy( lf.lfFaceName, _T( "Arial" ) );
	font.CreateFontIndirect( &lf );

	pDC->SelectObject( &font );

	pDC->SetBkMode( TRANSPARENT );
	pDC->DrawText( GetTitle(), rect, DT_SINGLELINE );

	pDC->SelectStockObject( ANSI_VAR_FONT );
	pDC->SelectStockObject( BLACK_PEN );
	pDC->SelectStockObject( WHITE_BRUSH );
}

void CGumpEntity::DrawGroupbox(CDC* pDC, CRect rect)
{
	CRect rect3d( rect );
	pDC->SelectObject( CStdGrfx::dialogBrush() );
	pDC->SelectObject( CStdGrfx::dialogPen() );

	rect.InflateRect( -2, 0 );

	LOGFONT lf;
	CFont font;
	GetFont( lf );
	// MS Sans Serif will not scale below 8 pts
	if( GetZoom() < 1 )
		lstrcpy( lf.lfFaceName, _T( "Arial" ) );

	rect3d.top += abs( lf.lfHeight ) / 2;

	CStdGrfx::drawsunken3dFrame( pDC, rect3d );

	rect3d.InflateRect( -1, -1 );
	CStdGrfx::draw3dFrame( pDC, rect3d );

	font.CreateFontIndirect( &lf );

	pDC->SelectObject( &font );

	pDC->SetBkMode( OPAQUE );
	pDC->SetBkColor( RGB( 192, 192, 192 ) );
	rect.left += abs( lf.lfHeight ) / 2;
	pDC->DrawText( " " + GetTitle() + " ", rect, DT_SINGLELINE );

	pDC->SelectStockObject( ANSI_VAR_FONT );
	pDC->SelectStockObject( BLACK_PEN );
	pDC->SelectStockObject( WHITE_BRUSH );
}

