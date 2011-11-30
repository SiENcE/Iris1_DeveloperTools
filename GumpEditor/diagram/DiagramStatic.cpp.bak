/* ==========================================================================
	CDiagramStatic

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-31

	Purpose :		Encapsulates a "static control" as a CDiagramEntity 
					object.

	Description :	An example of a CDiagramEntity-derived class. Includes 
					an example of the Export function.

					The class uses a static class with GDI-helpers, 
					CStdGrfx, for drawing.

	Usage :			

   ========================================================================*/

#include "stdafx.h"
#include "DiagramEditor\DiagramEntity.h"
#include "DiagramStatic.h"
#include "StdGrfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramStatic

CDiagramStatic::CDiagramStatic()
/* ============================================================
	Function :		CDiagramStatic::CDiagramStatic
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle( "Static" );
	SetType( "static" );
	SetName( "static" );

//	SetPropertyDialog( &m_dlg, CButtonPropertyDlg::IDD );
}

CDiagramEntity* CDiagramStatic::Clone()
/* ============================================================
	Function :		CDiagramStatic::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CDiagramStatic* obj = new CDiagramStatic;
	obj->Copy( this );
	return obj;
}

void CDiagramStatic::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramStatic::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{

	dc->SelectObject( CStdGrfx::dialogBrush() );
	dc->SelectObject( CStdGrfx::dialogPen() );

	dc->Rectangle( rect );

	LOGFONT lf;
	CFont font;
	GetFont( lf );
	// MS Sans Serif will not scale below 8 pts
	if( GetZoom() < 1 )
		lstrcpy( lf.lfFaceName, _T( "Arial" ) );
	font.CreateFontIndirect( &lf );

	dc->SelectObject( &font );

	dc->SetBkMode( TRANSPARENT );
	dc->DrawText( GetTitle(), rect, DT_SINGLELINE );

	dc->SelectStockObject( ANSI_VAR_FONT );
	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

}

CDiagramEntity* CDiagramStatic::CreateFromString( XML::Node* node )
/* ============================================================
	Function :		CDiagramStatic::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramStatic* obj = new CDiagramStatic;
	if(!obj->FromString( node ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramStatic::Export( UINT /*format*/ )
/* ============================================================
	Function :		CDiagramStatic::Export
	Description :	Exports this object to str using format
					
	Return :		CString		-	The resulting string
	Parameters :	UINT format	-	The format to export to
					
	Usage :			An example Export-function. In this case, 
					we are not using the format parameter, as 
					we only have one format.

   ============================================================*/
{
	return "";

	CString input( "\t<div class='controls' style='position:absolute;left:%i;top:%i;width:%i;height:%i;'>%s</div>" );
	CString str;
	CRect rect = GetRect();
	CString title = GetTitle();
	title.Replace( " ", "&nbsp;" );
	str.Format( input, rect.left - 2, rect.top - 2, rect.Width(), rect.Height(), title );
	return str;
}