// WordlistDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "../GumpEditor.h"
#include "WordlistDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CWordlistDlg 


CWordlistDlg::CWordlistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWordlistDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWordlistDlg)
	m_strWordlistNumber = _T("");
	m_strKeywords = _T("and and_eq asm auto bitand bitor bool break "
		"case catch char class compl const const_cast continue default delete do double dynamic_cast "
		"else enum explicit export extern false float for friend goto if inline int long"
		"mutable namespace new not not_eq operator or or_eq "
		"private protected public register reinterpret_cast return "
		"short signed sizeof static static_cast struct switch "
		"template this throw true try typedef typeid typename "
		"union unsigned using virtual void volatile wchar_t while xor xor_eq");
	//}}AFX_DATA_INIT
}


void CWordlistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWordlistDlg)
	DDX_CBString(pDX, IDC_WORDLISTNO, m_strWordlistNumber);
	DDX_Text(pDX, IDC_KEYWORDS, m_strKeywords);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWordlistDlg, CDialog)
	//{{AFX_MSG_MAP(CWordlistDlg)
		// HINWEIS: Der Klassen-Assistent f?t hier Zuordnungsmakros f? Nachrichten ein
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen f? Nachrichten CWordlistDlg 
