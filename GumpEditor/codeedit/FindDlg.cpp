/* @doc
* @module FindDlg.cpp | Implementation of a modeless find dialog for use with the scintilla edit control
*/

#include "stdafx.h"
#include "../GumpEditor.h"
#include "FindDlg.h"
#include "MainFrm.h"
#include "ScintillaWnd.h"
#include "scintillamfcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////
// @mfunc constructor
// @rvalue void | not used
//
CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
: CDialog(CFindDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindDlg)
	m_bMatchcase = FALSE;
	m_bMacthregex = FALSE;
	m_bMatchwords = FALSE;
	m_bNotForward = FALSE;
	//}}AFX_DATA_INIT
	m_pEdit = NULL;
	m_pParent = dynamic_cast<CScintillamfcView*>(pParent);
}
/////////////////////////////////////
// @mfunc set/get data to/from controls in dialog
// @rvalue void | not used
//
void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindDlg)
	DDX_Control(pDX, IDC_SEARCHTEXT, m_ctrlSearchtext);
	DDX_Check(pDX, IDC_MATCHCASE, m_bMatchcase);
	DDX_Check(pDX, IDC_MATCHREGEX, m_bMacthregex);
	DDX_Check(pDX, IDC_MATCHWORDS, m_bMatchwords);
	DDX_Radio(pDX, IDC_FORWARD, m_bNotForward);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindDlg, CDialog)
	//{{AFX_MSG_MAP(CFindDlg)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////
// @mfunc Last mesage called - delete myself because this dialog is modeless
// @rvalue void | not used
//
void CFindDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	// set pointer in parent = NULL
	if (m_pParent) m_pParent->SetFindDlg(NULL);
	// delete myself - only valid if dialog is modeless
	delete this;

}
/////////////////////////////////////
// @mfunc modeless handler for OK button - destroy myself and rely on PostNcDestroy to free memory
// @rvalue void | not used
//
void CFindDlg::OnOK() 
{
	DestroyWindow();
}
/////////////////////////////////////
// @mfunc init dialog members
// @rvalue void | not used
//
BOOL CFindDlg::InitDialog() 
{
#if 1
	// get app with old searchstrings
	CGumpEditorApp *app = (CGumpEditorApp*)AfxGetApp();
	// set selected text to dropdown
	if (m_pEdit != NULL && app != NULL)
	{
		m_ctrlSearchtext.ResetContent();
		//    get old find strings and add to combobox
		for (int i= app->m_strFindstrings.GetSize()-1;i>=0;i--)
		{
			m_ctrlSearchtext.AddString(app->m_strFindstrings.GetAt(i));
		}
		//    set new searchtext and check if already present in combobox
		CString strFind = m_pEdit->GetSelectedText();
		if (strFind.FindOneOf(_T("\r\n")) < 0)
			m_ctrlSearchtext.SetWindowText(strFind);
	}
#endif
	return TRUE;
}
/////////////////////////////////////
// @mfunc try to find string forward
// @rvalue void | not used
//
void CFindDlg::OnNext() 
{
	UpdateData();
	// set search flags
	int nSearchFlags = 0;
	if (m_bMatchwords)
		nSearchFlags |= SCFIND_WHOLEWORD;
	if (m_bMatchcase)
		nSearchFlags |= SCFIND_MATCHCASE;
	if (m_bMacthregex)
		nSearchFlags |= SCFIND_REGEXP;

	Find(nSearchFlags, !m_bNotForward);
	
	SetFocus();
}

void CFindDlg::Find(int nSearchFlags, bool bNext)
{
	// try to find text
	if (!m_pEdit) return;

	if (bNext)
	{
		CString strFind;
		m_ctrlSearchtext.GetWindowText(strFind);
		StoreSearchString ((CGumpEditorApp*)AfxGetApp(), strFind);
		m_pEdit->SetSearchflags(nSearchFlags);
		if(!m_pEdit->SearchForward(strFind.LockBuffer()))
		{
			MessageBeep(0);
		}
		strFind.UnlockBuffer();
	}
	else
	{
		CString strFind;
		m_ctrlSearchtext.GetWindowText(strFind);
		StoreSearchString ((CGumpEditorApp*)AfxGetApp(), strFind);
		m_pEdit->SetSearchflags(nSearchFlags);
		if(!m_pEdit->SearchBackward(strFind.LockBuffer()))
		{
			MessageBeep(0);
		}
		strFind.UnlockBuffer();
	}
}

/////////////////////////////////////
// @mfunc try to find string backward
// @rvalue void | not used
//

/////////////////////////////////////
// @mfunc store new searchstring to global array of searchstrings. Never store one twice.
// @rvalue void | not used
//
void CFindDlg::StoreSearchString(
								 CGumpEditorApp *app,    //@parm application handle
								 LPCSTR szSearch)  //@parm text to store
{
#if 1
	if (app != NULL)
	{
		//    try to find string in old stringlist
		for (int i=0;i<app->m_strFindstrings.GetSize();i++)
		{
			if (strcmp(app->m_strFindstrings.GetAt(i), szSearch) == 0)
				return;
		}
		//    if we get here no match was found and we add the new string
		app->m_strFindstrings.Add(szSearch);
		m_ctrlSearchtext.InsertString(0, szSearch);
	}
#endif
}


void CFindDlg::SetSearchString(LPCTSTR szSearch)
{
	m_ctrlSearchtext.SetWindowText(szSearch);
	//UpdateData(FALSE);
}

BOOL CFindDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) {
		OnNext();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
