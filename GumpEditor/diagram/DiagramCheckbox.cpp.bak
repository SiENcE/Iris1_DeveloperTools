/* ==========================================================================
	CDiagramCheckbox

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
#include "DiagramEditor\DiagramEntity.h"
#include "DiagramCheckbox.h"
#include "StdGrfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramCheckbox

CDiagramCheckbox::CDiagramCheckbox()
/* ============================================================
	Function :		CDiagramCheckbox::CDiagramCheckbox
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle( "Checkbox" );
	SetType( "checkbox" );
	SetName( "checkbox" );

//	SetPropertyDialog( &m_dlg, CButtonPropertyDlg::IDD );

}

CDiagramEntity* CDiagramCheckbox::Clone()
/* ============================================================
	Function :		CDiagramCheckbox::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CDiagramCheckbox* obj = new CDiagramCheckbox;
	obj->Copy( this );
	return obj;
}

void CDiagramCheckbox::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramCheckbox::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{

	dc->SelectObject( CStdGrfx::dialogBrush() );
	dc->SelectObject( CStdGrfx::dialogPen() );

	LOGFONT lf;
	CFont chk;
	CFont font;
	GetFont( lf );
	// MS Sans Serif will not scale below 8 pts
	if( GetZoom() < 1 )
		lstrcpy( lf.lfFaceName, _T( "Arial" ) );
	font.CreateFontIndirect( &lf );

	// Marlett is used for the checkbox
	chk.CreateFont( ( int ) ( ( double ) lf.lfHeight * 1.25 ), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Marlett" );
	dc->SelectObject( &chk );

	dc->Rectangle( rect );

	dc->SetBkMode( TRANSPARENT );
	dc->SetTextColor( ::GetSysColor( COLOR_WINDOW ) );
	dc->TextOut( rect.left, rect.top, "g" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DSHADOW ) );
	dc->TextOut( rect.left, rect.top, "d" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DHIGHLIGHT ) );
	dc->TextOut( rect.left, rect.top, "d" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DDKSHADOW ) );
	dc->TextOut( rect.left, rect.top, "e" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DLIGHT ) );
	dc->TextOut( rect.left, rect.top, "f" );

	dc->SelectObject( &font );
	dc->SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );
	rect.left += ( int ) ( ( double ) abs( lf.lfHeight ) * 1.5 );
	dc->DrawText( GetTitle(), rect, DT_SINGLELINE );

	dc->SelectStockObject( DEFAULT_GUI_FONT );
	dc->SelectStockObject( NULL_PEN );
	dc->SelectStockObject( LTGRAY_BRUSH );

}

CDiagramEntity* CDiagramCheckbox::CreateFromString( XML::Node* node )
/* ============================================================
	Function :		CDiagramCheckbox::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramCheckbox* obj = new CDiagramCheckbox;
	if(!obj->FromString( node ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramCheckbox::Export( UINT /*format*/ )
/* ============================================================
	Function :		CDiagramCheckbox::Export
	Description :	Exports this object to str using format
					
	Return :		CString		-	The resulting string
	Parameters :	UINT format	-	The format to export to
					
	Usage :			An example Export-function. In this case, 
					we are not using the format parameter, as 
					we only have one format.

   ============================================================*/
{
	return "";


	CString input( "\t<div class='controls' style='position:absolute;left:%i;top:%i;width:%i;height:%i;'><input type=checkbox onfocus='blur()' onclick='JavaScript:checkboxHandler(this)' name='%s' >%s</input></div>" );
	CString str;
	CRect rect = GetRect();
	CString title = GetTitle();
	title.Replace( " ", "&nbsp;" );
	str.Format( input, rect.left - 2, rect.top - 2, rect.Width(), rect.Height(), GetName(), title );
	return str;
}