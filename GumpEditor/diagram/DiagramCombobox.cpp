/* ==========================================================================
	CDiagramCombobox

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-31

	Purpose :		Encapsulates a "combobox" as a CDiagramEntity 
					object.

	Description :	An example of a CDiagramEntity-derived class. Includes 
					an example of the Export function.

					The class uses a static class with GDI-helpers, 
					CStdGrfx, for drawing.

					The also class demonstrates custom size constraints.

	Usage :			

   ========================================================================*/

#include "stdafx.h"
#include "DiagramCombobox.h"
#include "StdGrfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramCombobox

CDiagramCombobox::CDiagramCombobox()
/* ============================================================
	Function :		CDiagramCombobox::CDiagramCombobox
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	// Note the constraints
	SetConstraints( CSize( 20, 20 ), CSize( -1, 20 ) );

	SetTitle( "Combobox" );
	SetType( "combobox" );
	SetName( "combobox" );

//	SetPropertyDialog( &m_dlg, CEditPropertyDlg::IDD );

}

CDiagramEntity* CDiagramCombobox::Clone()
/* ============================================================
	Function :		CDiagramCombobox::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CDiagramCombobox* obj = new CDiagramCombobox;
	obj->Copy( this );
	return obj;
}

void CDiagramCombobox::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramCombobox::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{

	CStdGrfx::drawsunkenframed3dWindow( dc, rect );

	LOGFONT lf;
	GetFont( lf );
	dc->SetBkMode( TRANSPARENT );
	dc->SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );

	CRect rectArrow( rect );
	rectArrow.left = rectArrow.right - ( int ) ( ( double ) GetMinimumSize().cy * GetZoom() );
	rectArrow.InflateRect( -2, -2 );

	CStdGrfx::drawframed3dBox( dc, rectArrow );

	CFont chk;
	chk.CreateFont( lf.lfHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Marlett" );
	dc->SelectObject( &chk );
	dc->DrawText( "u", rectArrow, DT_VCENTER | DT_SINGLELINE | DT_CENTER );

	dc->SelectStockObject( DEFAULT_GUI_FONT );

}

CDiagramEntity* CDiagramCombobox::CreateFromString( const CString& str )
/* ============================================================
	Function :		CDiagramCombobox::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramCombobox* obj = new CDiagramCombobox;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramCombobox::Export( UINT /*format*/ )
/* ============================================================
	Function :		CDiagramCombobox::Export
	Description :	Exports this object to str using format
					
	Return :		CString		-	The resulting string
	Parameters :	UINT format	-	The format to export to
					
	Usage :			An example Export-function. In this case, 
					we are not using the format parameter, as 
					we only have one format.

   ============================================================*/
{
	return "";


	CString input( "\t<select class='controls' style='position:absolute;left:%i;top:%i;width:%i;height:%i;' onchange='JavaScript:comboboxHandler(this)' name='%s'></select>" );
	CString str;
	CRect rect = GetRect();
	str.Format( input, rect.left - 2, rect.top - 2, rect.Width(), rect.Height(), GetName() );

	return str;
}
