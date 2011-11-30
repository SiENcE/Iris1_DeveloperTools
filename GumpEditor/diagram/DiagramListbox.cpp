/* ==========================================================================
	CDiagramListbox

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-31

	Purpose :		Encapsulates a "command button" as a CDiagramEntity 
					object.

	Description :	An example of a CDiagramEntity-derived class. Includes 
					an example of the Export function.

					The class uses a static class with GDI-helpers, 
					CStdGrfx, for drawing.

	Usage :			

   ========================================================================*/

#include "stdafx.h"
#include "DiagramListbox.h"
#include "StdGrfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramListbox

CDiagramListbox::CDiagramListbox()
/* ============================================================
	Function :		CDiagramListbox::CDiagramListbox
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetMinimumSize( CSize( 20, 20 ) );
	SetTitle( "Listbox" );
	SetType( "listbox" );
	SetName( "listbox" );

//	SetPropertyDialog( &m_dlg, CEditPropertyDlg::IDD );

}

CDiagramEntity* CDiagramListbox::Clone()
/* ============================================================
	Function :		CDiagramListbox::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CDiagramListbox* obj = new CDiagramListbox;
	obj->Copy( this );
	return obj;
}

void CDiagramListbox::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramListbox::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{
	CRect clientRect( rect );

	CStdGrfx::drawsunkenframed3dWindow( dc, rect );

	LOGFONT lf;
	GetFont( lf );
	dc->SetBkMode( TRANSPARENT );
	dc->SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );

	// Sidebar
	dc->SelectObject( CStdGrfx::scrollBrush() );
	dc->SelectObject( CStdGrfx::scrollPen() );
	int side = ( int ) ( ( double ) GetMinimumSize().cx * GetZoom() );

	rect.left = rect.right - side;
	rect.InflateRect( -2, -2 );
	dc->Rectangle( rect );

	CFont chk;
	chk.CreateFont( lf.lfHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Marlett" );
	dc->SelectObject( &chk );

	CRect rectTop( rect );
	CRect rectBottom( rect );
	rectTop.bottom = rectTop.top + rect.Width();
	rectBottom.top = rectBottom.bottom - rect.Width();

	int clientHeight = clientRect.Height() - 4;
	if( clientHeight < rectTop.Height() + rectBottom.Height() )
	{
		rectTop.bottom = rectTop.top + clientHeight / 2;
		rectBottom.top = rectTop.bottom;
	}


	CStdGrfx::drawframed3dBox( dc, rectTop );
	CStdGrfx::drawframed3dBox( dc, rectBottom );

	dc->DrawText( "u", rectTop, DT_VCENTER | DT_SINGLELINE | DT_CENTER );
	dc->DrawText( "u", rectBottom, DT_VCENTER | DT_SINGLELINE | DT_CENTER );

	dc->SelectStockObject( DEFAULT_GUI_FONT );
	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

}

CDiagramEntity* CDiagramListbox::CreateFromString( const CString& str )
/* ============================================================
	Function :		CDiagramListbox::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramListbox* obj = new CDiagramListbox;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramListbox::Export( UINT /*format*/ )
/* ============================================================
	Function :		CDiagramListbox::Export
	Description :	Exports this object to str using format
					
	Return :		CString		-	The resulting string
	Parameters :	UINT format	-	The format to export to
					
	Usage :			An example Export-function. In this case, 
					we are not using the format parameter, as 
					we only have one format.

   ============================================================*/
{
	return "";

	CString input( "\t<select size=2 class='controls' style='position:absolute;left:%i;top:%i;width:%i;height:%i;' onchange='JavaScript:listboxHandler(this)' name='%s'></select>" );
	CString str;
	CRect rect = GetRect();
	str.Format( input, rect.left - 2, rect.top - 2, rect.Width(), rect.Height(), GetName() );

	return str;
}