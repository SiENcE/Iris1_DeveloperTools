/* @doc
* @module ReplaceDlg.h | definition of a modeless search/replace dialog for use with the scintilla edit control
*/
#ifndef __REPLACEDLG_H__
#define __REPLACEDLG_H__

class CScintillaWnd;
class CScintillamfcView;

////////////////////////////////////
// @class CReplaceDlg | Class of a modeless search/replace dialog for use with the scintilla edit control
// @base public | CDialog
//
class CReplaceDlg : public CDialog
{
public:
	// @access public constructor
	// @cmember empty Constructor
	CReplaceDlg(CWnd* pParent = NULL);

public:
	// @access public data members
	// @cmember init dialog
	virtual BOOL InitDialog();
	// @cmember store new search string to global strings
	void StoreSearchString(CGumpEditorApp *app, LPCSTR szSearch);
	// @cmember store new search string to global strings
	void StoreReplaceString(CGumpEditorApp *app, LPCSTR szReplace);

	void SetReplaceString(LPCTSTR szReplace);
public:
	// @access public data members
	// @cmember a pointer to the scintilla edit control
	CScintillaWnd *m_pEdit;   // editcontrol to use
	// @cmember a flag for the default serach direction
	BOOL          m_bForwardSearch; 
	CScintillamfcView* m_pParent;

protected:
	void Find(int nSearchFlag, bool bNext);

public:
	// Dialogfelddaten
	//{{AFX_DATA(CReplaceDlg)
	enum { IDD = IDD_REPLACE };
	CComboBox	m_ctrlSearchtext;
	CComboBox	m_ctrlReplacetext;
	BOOL	m_bMatchcase;
	BOOL	m_bMatchregex;
	BOOL	m_bMatchwords;
	BOOL	m_bSelection;
	BOOL	m_bNotForward;
	//}}AFX_DATA


	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// ?erschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions?erschreibungen
	//{{AFX_VIRTUAL(CReplaceDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst?zung
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CReplaceDlg)
	afx_msg void OnNext();
	afx_msg void OnReplace();
	afx_msg void OnReplaceall();
	afx_msg void OnMatchselection();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f?t unmittelbar vor der vorhergehenden Zeile zus?zliche Deklarationen ein.

#endif // AFX_REPLACEDLG_H__31BB518C_B58B_498E_A338_FC992EB24B75__INCLUDED_
