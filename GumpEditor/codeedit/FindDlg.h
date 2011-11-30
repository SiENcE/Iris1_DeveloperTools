/* @doc
* @module FindDlg.h | definition of a modeless find dialog for use with the scintilla edit control
*/
#ifndef __FINDDLG_H__
#define __FINDDLG_H__


class CScintillaWnd;
class CScintillamfcView;

////////////////////////////////////
// @class CFindDlg | Class of a modeless find dialog for use with the scintilla edit control
// @base public | CDialog
//

class CFindDlg : public CDialog
{
public:
	// @access public constructor
	// @cmember empty Constructor
	CFindDlg(CWnd* pParent = NULL);   // Standardkonstruktor

public:
	// @access public data members
	// @cmember init dialog
	virtual BOOL InitDialog();
	// @cmember store new search string to global strings
	void StoreSearchString(CGumpEditorApp *app, LPCSTR szSearch);

	void SetSearchString(LPCTSTR szSearch);
protected:
	void Find(int nSearchFlag, bool bNext);
public:
	// @access public data members
	// @cmember a pointer to the scintilla edit control
	CScintillaWnd* m_pEdit;   // editcontrol to use
	CScintillamfcView* m_pParent;

public:
	// Dialogfelddaten
	//{{AFX_DATA(CFindDlg)
	enum { IDD = IDD_FIND };
	CComboBox	m_ctrlSearchtext;
	BOOL	m_bMatchcase;
	BOOL	m_bMacthregex;
	BOOL	m_bMatchwords;
	BOOL	m_bNotForward;
	//}}AFX_DATA

	virtual BOOL PreTranslateMessage(MSG* pMsg);


	// Vom Klassen-Assistenten generierte virtuelle Funktions?erschreibungen
	//{{AFX_VIRTUAL(CFindDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst?zung
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CFindDlg)
	virtual void OnOK();
	afx_msg void OnNext();
	afx_msg void OnPrev();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f?t unmittelbar vor der vorhergehenden Zeile zus?zliche Deklarationen ein.

#endif // AFX_FINDDLG_H__F7A11A65_5BD1_467D_AE06_896882CBBE2A__INCLUDED_
