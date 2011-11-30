/* ==========================================================================
	CDiagramEdit

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
#include "DiagramEdit.h"
#include "StdGrfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramEdit

CDiagramEdit::CDiagramEdit()
/* ============================================================
	Function :		CDiagramEdit::CDiagramEdit
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle("Edit");
	SetType("editbox");
	SetName( "editbox" );

//	SetPropertyDialog( &m_dlg, CEditPropertyDlg::IDD );

}

CDiagramEntity* CDiagramEdit::Clone()
/* ============================================================
	Function :		CDiagramEdit::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CDiagramEdit* obj = new CDiagramEdit;
	obj->Copy( this );
	return obj;
}

void CDiagramEdit::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramEdit::Draw
	Description :	
					
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{

	CStdGrfx::drawsunkenframed3dWindow( dc, rect );

}

CDiagramEntity* CDiagramEdit::CreateFromString( XML::Node* node )
/* ============================================================
	Function :		CDiagramEdit::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CDiagramEdit* obj = new CDiagramEdit;
	if(!obj->FromString( node ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramEdit::Export( UINT /*format*/ )
/* ============================================================
	Function :		CDiagramEdit::Export
	Description :	Exports this object to str using format
					
	Return :		CString		-	The resulting string
	Parameters :	UINT format	-	The format to export to
					
	Usage :			An example Export-function. In this case, 
					we are not using the format parameter, as 
					we only have one format.

   ============================================================*/
{
	return "";

	CString input( "\t<input class='controls' style='position:absolute;left:%i;top:%i;width:%i;height:%i;' type=text name='%s'></input>" );
	CString str;
	CRect rect = GetRect();
	str.Format( input, rect.left - 2, rect.top - 2, rect.Width(), rect.Height(), GetName() );
	return str;
}