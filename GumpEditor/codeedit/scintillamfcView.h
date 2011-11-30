/* @doc
 * @module ScintillamfcView.h | Definition of a Scintilla syntax coloring edit view for MFC
 * See www.scintilla.org for details
 * Author: Horst Br?kner, hb@ec-logic.com
 * Environment: VisualC++ Version 6, static Build of Scintilla, SciLexer.dll as Lexer
 */
#if !defined(AFX_SCINTILLAMFCVIEW_H__8370B793_3C97_4B71_BE55_1520C8497972__INCLUDED_)
#define AFX_SCINTILLAMFCVIEW_H__8370B793_3C97_4B71_BE55_1520C8497972__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "scintillawnd.h"

class CFindDlg;
class CReplaceDlg;

////////////////////////////////////
// @class CScintillaWnd | Class of a GCL Scintilla syntax coloring edit view for MFC
// @base public | CView
//
class CScintillamfcView : public CView
{
protected: // Nur aus Serialisierung erzeugen
// @access protected constructor - destructor
// @cmember empty Constructor
	CScintillamfcView();
	DECLARE_DYNCREATE(CScintillamfcView)

public:
// @access public function member
// @cmember get document
// @cmember process notification from scintilla control
   virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );
// @cmember save document
   virtual BOOL Save(LPCSTR szPath);
// @cmember get the edit control pointer
   virtual CScintillaWnd *GetEditControl(){return &m_wndScintilla;};

   void SetFindDlg(CFindDlg* pDlg) { m_pFindDlg = pDlg; }
   void SetReplaceDlg(CReplaceDlg* pDlg) { m_pReplaceDlg = pDlg; }

   CFindDlg* GetFindDlg() { return m_pFindDlg; }
   CReplaceDlg* GetReplaceDlg() { return m_pReplaceDlg; }

protected:
// @access protected data members
// @cmember the scintilla window/view
	CScintillaWnd     m_wndScintilla;
// @cmember the find dialog
	CFindDlg*		  m_pFindDlg;
// @cmember the replace dialog
	CReplaceDlg*	  m_pReplaceDlg;

	void ShowDialogInCenter(CWnd *pDlg);

public:
// ?erladungen
	// Vom Klassenassistenten generierte ?erladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CScintillamfcView)
	public:
	virtual void OnDraw(CDC* pDC);  // ?erladen zum Zeichnen dieser Ansicht
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CScintillamfcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CScintillamfcView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnOptionsViewLinenumbers();
	afx_msg void OnUpdateOptionsViewLinenumbers(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStyle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFold(CCmdUI* pCmdUI);
	afx_msg void OnEditClear();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnOptionsSetfont();
	afx_msg void OnOptionsForeground();
	afx_msg void OnOptionsBackground();
	afx_msg void OnOptionsWordlist();
	afx_msg void OnUpdateInsert(CCmdUI* pCmdUI);
	afx_msg void OnToggleInsert();
	afx_msg void OnOptionsFoldMargin();
	afx_msg void OnOptionsSelectionMargin();
	afx_msg void OnUpdateOptionsSelectionMargin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsFoldMargin(CCmdUI* pCmdUI);
	afx_msg void OnOptionsAddmarker();
	afx_msg void OnUpdateOptionsAddmarker(CCmdUI* pCmdUI);
	afx_msg void OnOptionsDeletemarker();
	afx_msg void OnUpdateOptionsDeletemarker(CCmdUI* pCmdUI);
	afx_msg void OnOptionsFindNextmarker();
	afx_msg void OnOptionsFindPrevmarker();
	afx_msg void OnOptionsGotopos();
	afx_msg void OnOptionsGotoline();
	afx_msg void OnOptionsDlg();
	afx_msg void OnEditFind();
	afx_msg void OnEditReplace();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f?t unmittelbar vor der vorhergehenden Zeile zus?zliche Deklarationen ein.

#endif // !defined(AFX_SCINTILLAMFCVIEW_H__8370B793_3C97_4B71_BE55_1520C8497972__INCLUDED_)
