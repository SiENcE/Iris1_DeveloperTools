/* ==========================================================================
	CEditPropertyDlg

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-31

	Purpose :		CEditPropertyDlg is a CDiagramPropertyDlg derived 
					class to edit objects having a a name (surprisingly
					enough for example CEditButton...)

	Description :	Implements an update function (OnApply) and the pure 
					virtual member SetValues.

	Usage :			See CDiagramPropertyDlg.


   ========================================================================*/

#include "stdafx.h"
#include "DialogEditorDemo.h"
#include "EditPropertyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditPropertyDlg dialog


CEditPropertyDlg::CEditPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDiagramPropertyDlg(CEditPropertyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditPropertyDlg)
	//}}AFX_DATA_INIT
}


void CEditPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditPropertyDlg)
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditPropertyDlg, CDialog)
	//{{AFX_MSG_MAP(CEditPropertyDlg)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditPropertyDlg message handlers

void CEditPropertyDlg::OnButtonApply() 
{

	if( m_hWnd )
	{
		if( GetEntity() )
		{
			CString name;
			m_editName.GetWindowText( name );

			GetEntity()->SetName( name );
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// CEditPropertyDlg virtuals

void CEditPropertyDlg::SetValues()
{

	if( m_hWnd )
		if( GetEntity() )
			m_editName.SetWindowText( GetEntity()->GetName() );

}

