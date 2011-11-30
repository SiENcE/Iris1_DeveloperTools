/* ==========================================================================
	CDiagramGroupbox

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
#include "DiagramGroupbox.h"
#include "StdGrfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramGroupbox

CDiagramGroupbox::CDiagramGroupbox()
/* ============================================================
	Function :		CDiagramGroupbox::CDiagramGroupbox
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle( "Groupbox" );
	SetType( "groupbox" );
	SetName( "groupbox" );

//	SetPropertyDialog( &m_dlg, CButtonPropertyDlg::IDD );

}

CDiagramEntity* CDiagramGroupbox::Clone()
/* ============================================================
	Function :		CDiagramGroupbox::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CDiagramGroupbox* obj = new CDiagramGroupbox;
	obj->Copy( this );
	return obj;
}

void CDiagramGroupbox::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramGroupbox::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{

	CRect rect3d( rect );
	dc->SelectObject( CStdGrfx::dialogBrush() );
	dc->SelectObject( CStdGrfx::dialogPen() );

	rect.InflateRect( -2, 0 );

	LOGFONT lf;
	CFont font;
	GetFont( lf );
	// MS Sans Serif will not scale below 8 pts
	if( GetZoom() < 1 )
		lstrcpy( lf.lfFaceName, _T( "Arial" ) );

	rect3d.top += abs( lf.lfHeight ) / 2;

	CStdGrfx::drawsunken3dFrame( dc, rect3d );

	rect3d.InflateRect( -1, -1 );
	CStdGrfx::draw3dFrame( dc, rect3d );

	font.CreateFontIndirect( &lf );

	dc->SelectObject( &font );

	dc->SetBkMode( OPAQUE );
	dc->SetBkColor( RGB( 192, 192, 192 ) );
	rect.left += abs( lf.lfHeight ) / 2;
	dc->DrawText( " " + GetTitle() + " ", rect, DT_SINGLELINE );

	dc->SelectStockObject( ANSI_VAR_FONT );
	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

}

CDiagramEntity* CDiagramGroupbox::CreateFromString( XML::Node* node )
/* ============================================================
	Function :		CDiagramGroupbox::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramGroupbox* obj = new CDiagramGroupbox;
	if(!obj->FromString( node ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramGroupbox::Export( UINT /*format*/ )
/* ============================================================
	Function :		CDiagramGroupbox::Export
	Description :	Exports this object to str using format
					
	Return :		CString		-	The resulting string
	Parameters :	UINT format	-	The format to export to
					
	Usage :			An example Export-function. In this case, 
					we are not using the format parameter, as 
					we only have one format.

   ============================================================*/
{
	return "";

	CString input( "\t<div class='controls' style='border-width:2;border-style:groove;position:absolute;left:%i;top:%i;width:%i;height:%i;'><div class='controls' style='position:absolute;left:6;top:-6;background-color:#c0c0c0;'>%s</div></div>" );
	CString str;
	CRect rect = GetRect();
	CString title = GetTitle();
	title.Replace( " ", "&nbsp;" );
	str.Format( input, rect.left - 2, rect.top - 2, rect.Width(), rect.Height(), title );
	return str;
}