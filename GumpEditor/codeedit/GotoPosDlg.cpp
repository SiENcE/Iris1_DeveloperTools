// GotoPosDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "../GumpEditor.h"
#include "GotoPosDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CGotoPosDlg 


CGotoPosDlg::CGotoPosDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGotoPosDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGotoPosDlg)
	m_nPos = 0;
	//}}AFX_DATA_INIT
}


void CGotoPosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGotoPosDlg)
	DDX_Text(pDX, IDC_POSITION, m_nPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGotoPosDlg, CDialog)
	//{{AFX_MSG_MAP(CGotoPosDlg)
		// HINWEIS: Der Klassen-Assistent f?t hier Zuordnungsmakros f? Nachrichten ein
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen f? Nachrichten CGotoPosDlg 
