/* ==========================================================================
	CDiagramButton

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
#include "DiagramButton.h"
#include "StdGrfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramButton

CDiagramButton::CDiagramButton()
/* ============================================================
	Function :		CDiagramButton::CDiagramButton
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
	SetMinimumSize( CSize(1,1) );
	SetTitle( "Button" );
	SetType( "button" );
	SetName( "button" );
//	SetPropertyDialog( &m_dlg, CButtonPropertyDlg::IDD );

}

CDiagramEntity* CDiagramButton::Clone()
/* ============================================================
	Function :		CDiagramButton::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{

	CDiagramButton* obj = new CDiagramButton;
	obj->Copy( this );
	return obj;

}

void CDiagramButton::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramButton::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{
	CStdGrfx::drawframed3dBox( dc, rect );

	LOGFONT lf;
	CFont font;

	GetFont( lf );
	// MS Sans Serif will not scale below 8 pts.
	if( GetZoom() < 1 )
		lstrcpy( lf.lfFaceName, _T( "Arial" ) );

	font.CreateFontIndirect( &lf );

	dc->SelectObject( &font );

	dc->SetBkMode( TRANSPARENT );
	dc->DrawText( GetTitle(), rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

	dc->SelectStockObject( ANSI_VAR_FONT );

}

CDiagramEntity* CDiagramButton::CreateFromString( XML::Node* node )
/* ============================================================
	Function :		CDiagramButton::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramButton* obj = new CDiagramButton;
	if(!obj->FromString( node ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramButton::Export( UINT /*format*/ )
/* ============================================================
	Function :		CDiagramButton::Export
	Description :	Exports this object to str using format
					
	Return :		CString		-	The resulting string
	Parameters :	UINT format	-	The format to export to
					
	Usage :			An example Export-function. In this case, 
					we are not using the format parameter, as 
					we only have one format.

   ============================================================*/
{
	return "";


	CString input( "\t<input class='controls' style='position:absolute;left:%i;top:%i;width:%i;height:%i;' type=button onclick='JavaScript:buttonHandler(this)' name='%s' value='%s'></input>" );
	CString str;
	CRect rect = GetRect();
	CString title = GetTitle();
	title.Replace( " ", "&nbsp;" );
	str.Format( input, rect.left - 2, rect.top - 2, rect.Width(), rect.Height(), GetName(), title );

	return str;
}