/* @doc
* @module ReplaceDlg.cpp | Implementation of a modeless search/replace dialog for use with the scintilla edit control
*/
//

#include "stdafx.h"
#include "../GumpEditor.h"
#include "ReplaceDlg.h"
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
CReplaceDlg::CReplaceDlg(CWnd* pParent /*=NULL*/)
: CDialog(CReplaceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReplaceDlg)
	m_bMatchcase = FALSE;
	m_bMatchregex = FALSE;
	m_bMatchwords = FALSE;
	m_bSelection = FALSE;
	//}}AFX_DATA_INIT
	m_pEdit = NULL;
	m_bForwardSearch = TRUE;
	m_bNotForward = FALSE;
	m_pParent = dynamic_cast<CScintillamfcView*>(pParent);
}
/////////////////////////////////////
// @mfunc set/get data to/from controls in dialog
// @rvalue void | not used
//
void CReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReplaceDlg)
	DDX_Control(pDX, IDC_SEARCHTEXT, m_ctrlSearchtext);
	DDX_Control(pDX, IDC_REPLACETEXT, m_ctrlReplacetext);
	DDX_Check(pDX, IDC_MATCHCASE, m_bMatchcase);
	DDX_Check(pDX, IDC_MATCHREGEX, m_bMatchregex);
	DDX_Check(pDX, IDC_MATCHWORDS, m_bMatchwords);
	DDX_Check(pDX, IDC_MATCHSELECTION, m_bSelection);
	DDX_Radio(pDX, IDC_FORWARD, m_bNotForward);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReplaceDlg, CDialog)
	//{{AFX_MSG_MAP(CReplaceDlg)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_REPLACE, OnReplace)
	ON_BN_CLICKED(IDC_REPLACEALL, OnReplaceall)
	ON_BN_CLICKED(IDC_MATCHSELECTION, OnMatchselection)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////
// @mfunc Last mesage called - delete myself because this dialog is modeless
// @rvalue void | not used
//
void CReplaceDlg::PostNcDestroy() 
{

	CDialog::PostNcDestroy();
	// set pointer in parent = NULL
	if (m_pParent) m_pParent->SetReplaceDlg(NULL);
	// delete myself - only valid if dialog is modeless
	delete this;

}
/////////////////////////////////////
// @mfunc init dialog members
// @rvalue void | not used
//
BOOL CReplaceDlg::InitDialog() 
{
#if 1
	// get app with old searchstrings
	CGumpEditorApp *app = (CGumpEditorApp*)AfxGetApp();
	// set selected text to dropdown
	if (m_pEdit != NULL && app != NULL)
	{
		int i = 0;
		m_ctrlSearchtext.ResetContent();
		//    get old find strings and add to combobox
		for (i= app->m_strFindstrings.GetSize()-1;i>=0;i--)
		{
			m_ctrlSearchtext.AddString(app->m_strFindstrings.GetAt(i));
		}
		m_ctrlReplacetext.ResetContent();
		//    get old replace strings and add to combobox
		for (i= app->m_strReplacestrings.GetSize()-1;i>=0;i--)
		{
			m_ctrlReplacetext.AddString(app->m_strReplacestrings.GetAt(i));
		}
		//    set new searchtext and check if already present in combobox
		CString strFind = m_pEdit->GetSelectedText();
		if (strFind.FindOneOf(_T("\r\n")) < 0)
		{
			m_ctrlSearchtext.SetWindowText(strFind);
			//m_ctrlReplacetext.SetWindowText(strFind);
		}
	}
#endif
	return TRUE;
}
/////////////////////////////////////
// @mfunc store new searchstring to global array of searchstrings. Never store one twice.
// @rvalue void | not used
//
void CReplaceDlg::StoreSearchString(
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
/////////////////////////////////////
// @mfunc store new searchstring to global array of searchstrings. Never store one twice.
// @rvalue void | not used
//
void CReplaceDlg::StoreReplaceString(
									 CGumpEditorApp *app,    //@parm application handle
									 LPCSTR szReplace) //@parm text to store
{
#if 1
	if (app != NULL)
	{
		//    try to find string in old stringlist
		for (int i=0;i<app->m_strReplacestrings.GetSize();i++)
		{
			if (strcmp(app->m_strReplacestrings.GetAt(i), szReplace) == 0)
				return;
		}
		//    if we get here no match was found and we add the new string
		app->m_strReplacestrings.Add(szReplace);
		m_ctrlReplacetext.InsertString(0, szReplace);
	}
#endif
}

void CReplaceDlg::Find(int nSearchFlags, bool bNext)
{
	if (bNext)
	{
		//    if we dont operate on the selection 
		if (!m_bSelection)
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
// @mfunc goto next occurance of searchstring
// @rvalue void | not used
//
void CReplaceDlg::OnNext() 
{
	UpdateData();
	m_bForwardSearch = !m_bNotForward;
	
	// set search flags
	int nSearchFlags = 0;
	if (m_bMatchwords)
		nSearchFlags |= SCFIND_WHOLEWORD;
	if (m_bMatchcase)
		nSearchFlags |= SCFIND_MATCHCASE;
	if (m_bMatchregex)
		nSearchFlags |= SCFIND_REGEXP;
	// try to find text
	if (!m_pEdit) return;

	Find(nSearchFlags, m_bForwardSearch);

	SetFocus();
}

/////////////////////////////////////
// @mfunc replace current searchstring
// @rvalue void | not used
//
void CReplaceDlg::OnReplace() 
{
	UpdateData(TRUE);
	// set search flags
	int nSearchFlags = 0;
	if (m_bMatchwords)
		nSearchFlags |= SCFIND_WHOLEWORD;
	if (m_bMatchcase)
		nSearchFlags |= SCFIND_MATCHCASE;
	if (m_bMatchregex)
		nSearchFlags |= SCFIND_REGEXP;
	if (m_pEdit != NULL)
	{
		m_pEdit->SetSearchflags(nSearchFlags);
		long lStart = m_pEdit->GetSelectionStart();
		long lEnd = m_pEdit->GetSelectionEnd();
		if (lStart == lEnd) return;
		CString strReplace;
		m_ctrlReplacetext.GetWindowText(strReplace);
		m_pEdit->ReplaceSearchedText(strReplace);
		StoreReplaceString ((CGumpEditorApp*)AfxGetApp(), strReplace);
		if (m_bForwardSearch)
		{
			m_pEdit->GotoPosition(lEnd);
			OnNext();
		}
		else
		{
			m_pEdit->GotoPosition(lStart);
			OnNext();
		}
	}
}
/////////////////////////////////////
// @mfunc replace all occurences of the search string
// @rvalue void | not used
//
void CReplaceDlg::OnReplaceall() 
{
	UpdateData(TRUE);
	// set search flags
	int nSearchFlags = 0;
	if (m_bMatchwords)
		nSearchFlags |= SCFIND_WHOLEWORD;
	if (m_bMatchcase)
		nSearchFlags |= SCFIND_MATCHCASE;
	if (m_bMatchregex)
		nSearchFlags |= SCFIND_REGEXP;
	if (m_pEdit != NULL)
	{
		CString strFind;
		m_ctrlSearchtext.GetWindowText(strFind);
		CString strReplace;
		m_ctrlReplacetext.GetWindowText(strReplace);
		m_pEdit->SetSearchflags(nSearchFlags);
		m_pEdit->ReplaceAll(strFind, strReplace, m_bSelection);
		StoreSearchString ((CGumpEditorApp*)AfxGetApp(), strFind);
		StoreReplaceString ((CGumpEditorApp*)AfxGetApp(), strReplace);
	}

	SetFocus();
}

/////////////////////////////////////
// @mfunc Handle "selection only" state - disable/enable some controls
// @rvalue void | not used
//
void CReplaceDlg::OnMatchselection() 
{
	UpdateData();
	// if we should operate on the selection disable next/prev/replace - only replace all works now
	if (m_bSelection)
	{
		CWnd *pWnd = GetDlgItem(IDC_NEXT);
		if (pWnd != NULL)
			pWnd->EnableWindow(FALSE);
		//pWnd = GetDlgItem(IDC_PREV);
		//if (pWnd != NULL)
		//	pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_REPLACE);
		if (pWnd != NULL)
			pWnd->EnableWindow(FALSE);
	}
	else
	{
		CWnd *pWnd = GetDlgItem(IDC_NEXT);
		if (pWnd != NULL)
			pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_PREV);
		if (pWnd != NULL)
			pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_REPLACE);
		if (pWnd != NULL)
			pWnd->EnableWindow(TRUE);
	}
}


void CReplaceDlg::SetReplaceString(LPCTSTR szReplace)
{
	m_ctrlReplacetext.SetWindowText(szReplace);
}

BOOL CReplaceDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) {
		OnNext();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
