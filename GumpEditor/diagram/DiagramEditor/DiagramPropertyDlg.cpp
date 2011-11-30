/* ==========================================================================
	CDiagramPropertyDlg

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-31

	Purpose :		CDiagramPropertyDlg represents a property dialog for a 
					CDiagramEntity object.	

	Description :	The class is a pure virtual class derived from CDialog.

	Usage :			Create a property dialog in the resource editor, but 
					map it to CDiagramPropertyDlg instead of CDialog. Add 
					an override of the pure virtual function SetValues. 
					This function is expected to put data from m_entity in 
					appropriate fields in the dialog. Set data in m_entity 
					as appropriate (from an Apply-button handler or 
					whatever appropriate), and call GetParent()->RedrawWindow() 
					if needed.

					In the CDiagramEntity-derived class, add a member of 
					the CDiagramPropertyDlg-derived class, and call 
					SetPropertyDialog in the constructor.

					The dialog is displayed as a modeless dialog. 
					CDiagramEditor will hide the dialog automatically when 
					another object is selected, no special Close-button is 
					necessary.

   ========================================================================*/

#include "stdafx.h"
#include "DiagramPropertyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramPropertyDlg
//

CDiagramPropertyDlg::CDiagramPropertyDlg( UINT res, CWnd* parent ) : CDialog( res, parent )
/* ============================================================
	Function :		CDiagramPropertyDlg::CDiagramPropertyDlg
	Description :	constructor
					
	Return :		void
	Parameters :	UINT res		-	Resource ID of the 
										dialog template.
					CWnd* parent	-	Parent of the dialog - 
										the CDiagramEditor.
					
	Usage :			

   ============================================================*/
{

	m_entity = NULL;
	m_redrawWnd = NULL;

}

void CDiagramPropertyDlg::SetEntity( CDiagramEntity* entity )
/* ============================================================
	Function :		CDiagramPropertyDlg::SetEntity
	Description :	
					
	Return :		void
	Parameters :	CDiagramEntity* entity	-	Set the object 
												for this dialog.
					
	Usage :			Call to set the CDiagramEntity-derive object 
					to be associated with this dialog.

   ============================================================*/
{

	m_entity = entity;

}

CDiagramEntity* CDiagramPropertyDlg::GetEntity() const
/* ============================================================
	Function :		CDiagramPropertyDlg::GetEntity
	Description :	Returns the CDiagramEntity object of this 
					dialog
					
	Return :		CDiagramEntity*	-	The object attached to 
										this dialog
	Parameters :	none

	Usage :			Call to get the CDiagramEntity-derived 
					object associated to this dialog.

   ============================================================*/
{

	return m_entity;

}

BOOL CDiagramPropertyDlg::Create( UINT nIDTemplate, CWnd* pParentWnd )
/* ============================================================
	Function :		CDiagramPropertyDlg::Create
	Description :	Creates the dialog.
					
	Return :		BOOL				-	TRUE if window was 
											created ok.
	Parameters :	UINT nIDTemplate	-	Resource id of 
											dialog template.
					CWnd* pParentWnd	-	Parent of dialog 
											(normally the 
											CDiagramEditor)
					
	Usage :			Called internally to create the property 
					dialog.

   ============================================================*/
{

	BOOL result;

	result = CDialog::Create( nIDTemplate, pParentWnd );
	SetRedrawWnd( pParentWnd );

	return result;

}
