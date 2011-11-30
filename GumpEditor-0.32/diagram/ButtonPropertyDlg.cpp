/* ==========================================================================
	CButtonPropertyDlg

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-31

	Purpose :		CButtonPropertyDlg is a CDiagramPropertyDlg derived 
					class to edit objects having a title and a name 
					(surprisingly enough for example CDiagramButton...)

	Description :	Implements an update function (OnApply) and the pure 
					virtual member SetValues.

	Usage :			See CDiagramPropertyDlg.

   ========================================================================*/

#include "stdafx.h"
#include "ButtonPropertyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonPropertyDlg dialog


CButtonPropertyDlg::CButtonPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDiagramPropertyDlg(CButtonPropertyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CButtonPropertyDlg)
	//}}AFX_DATA_INIT
}


void CButtonPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CButtonPropertyDlg)
	DDX_Control(pDX, IDC_EDIT_TITLE, m_editTitle);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CButtonPropertyDlg, CDialog)
	//{{AFX_MSG_MAP(CButtonPropertyDlg)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonPropertyDlg message handlers

void CButtonPropertyDlg::OnButtonApply() 
{
	if( m_hWnd )
	{
		if( GetEntity() )
		{
			CString title;
			CString name;
			m_editTitle.GetWindowText( title );
			m_editName.GetWindowText( name );

			if( GetEntity()->GetTitle() != title || GetEntity()->GetName() != name )
			{
				GetEntity()->SetTitle( title );
				GetEntity()->SetName( name );
				GetParent()->RedrawWindow();
			}
		}
	}	
}

/////////////////////////////////////////////////////////////////////////////
// CButtonPropertyDlg virtuals

void CButtonPropertyDlg::SetValues()
{
	if( m_hWnd )
	{
		if( GetEntity() )
		{
			m_editTitle.SetWindowText( GetEntity()->GetTitle() );
			m_editName.SetWindowText( GetEntity()->GetName() );
		}

	}
}

