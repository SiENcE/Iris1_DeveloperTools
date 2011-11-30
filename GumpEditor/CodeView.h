#pragma once

#include "codeedit/scintillamfcView.h"

class CCSLDlg;

class CCodeView : public CScintillamfcView
{
	DECLARE_DYNCREATE(CCodeView)

protected:
	CCodeView();          
	virtual ~CCodeView();

public:
	void SetCSLDlg(CCSLDlg *pDlg) { m_pCSLDlg = pDlg; }
	CCSLDlg *m_pCSLDlg;
public:
	virtual void OnDraw(CDC* pDC);   
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void InsertText(CString strText);

protected:
	DECLARE_MESSAGE_MAP()
public:
	void GotoEventHandler(CString strEvHandler, CString strEvHandlerParams);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	CString GetText(void);
	void SetText(LPCTSTR text);
	BOOL IsModified();
public:
	afx_msg void OnViewCslscript();
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};


