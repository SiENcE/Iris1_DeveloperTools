/* ==========================================================================
	CDiagramRadiobutton

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
#include "DiagramRadiobutton.h"
#include "StdGrfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramRadiobutton

CDiagramRadiobutton::CDiagramRadiobutton()
/* ============================================================
	Function :		CDiagramRadiobutton::CDiagramRadiobutton
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle( "Radiobutton");
	SetType( "radiobutton");
	SetName( "radiobutton" );

//	SetPropertyDialog( &m_dlg, CButtonPropertyDlg::IDD );

}

CDiagramEntity* CDiagramRadiobutton::Clone()
/* ============================================================
	Function :		CDiagramRadiobutton::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CDiagramRadiobutton* obj = new CDiagramRadiobutton;
	obj->Copy( this );
	return obj;
}

void CDiagramRadiobutton::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramRadiobutton::Draw
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
	CFont chk;
	CFont font;

	GetFont( lf );
	// MS Sans Serif will not scale below 8 pts
	if( GetZoom() < 1 )
		lstrcpy( lf.lfFaceName, _T( "Arial" ) );
	font.CreateFontIndirect( &lf );

	// Marlett is used for the circle
	chk.CreateFont(  ( int ) ( ( double ) lf.lfHeight * 1.25 ), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Marlett" );
	dc->SetBkMode( TRANSPARENT );
	dc->SelectObject( &chk );

	dc->SetTextColor( ::GetSysColor( COLOR_WINDOW ) );
	dc->TextOut( rect.left, rect.top, "n" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DSHADOW ) );
	dc->TextOut( rect.left, rect.top, "j" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DHIGHLIGHT  ) );
	dc->TextOut( rect.left, rect.top, "k" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DDKSHADOW ) );
	dc->TextOut( rect.left, rect.top, "l" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DLIGHT ) );
	dc->TextOut( rect.left, rect.top, "m" );

	dc->SelectObject( &font );

	dc->SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );
	rect.left += ( int ) ( ( double ) abs( lf.lfHeight ) * 1.5 );
	dc->DrawText( GetTitle(), rect, DT_SINGLELINE );

	dc->SelectStockObject( DEFAULT_GUI_FONT );
	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

}

CDiagramEntity* CDiagramRadiobutton::CreateFromString( const CString& str )
/* ============================================================
	Function :		CDiagramRadiobutton::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramRadiobutton* obj = new CDiagramRadiobutton;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramRadiobutton::Export( UINT /*format*/ )
/* ============================================================
	Function :		CDiagramRadiobutton::Export
	Description :	Exports this object to str using format
					
	Return :		CString		-	The resulting string
	Parameters :	UINT format	-	The format to export to
					
	Usage :			An example Export-function. In this case, 
					we are not using the format parameter, as 
					we only have one format.

   ============================================================*/
{
	return "";

	CString input( "\t<div class='controls' style='position:absolute;left:%i;top:%i;width:%i;height:%i;'><input onfocus='blur()' onclick='JavaScript:radiobuttonHandler(this)' type=radio name='%s'>%s</input></div>" );
	CString str;
	CRect rect = GetRect();
	CString title = GetTitle();
	title.Replace( " ", "&nbsp;" );
	str.Format( input, rect.left - 2, rect.top - 2, rect.Width(), rect.Height(), GetName(), title );

	return str;
}