// GumpEditorView.h : interface of the CGumpEditorView class
//

#pragma once

#include "diagram/DialogEditorDemoView.h"
#include "dyndroptarget.h"

class CGumpEditorView : public CDialogEditorDemoView
{
protected: // create from serialization only
	CGumpEditorView();
	DECLARE_DYNCREATE(CGumpEditorView)
public:
	CDynDropTarget m_DynDropTarget;

// Attributes
public:
	CGumpEditorDoc* GetDocument() const;

// Operations
public:
	CDEditor& GetDEditor() { return m_editor; }
	
// Overrides
	public:
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CGumpEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	bool m_bInit;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg long AddControl(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAddStatic();
	afx_msg void OnAddEdit();
	afx_msg void OnAddCustom();
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewControllist();
	afx_msg void OnUpdateAddCustom(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in GumpEditorView.cpp
inline CGumpEditorDoc* CGumpEditorView::GetDocument() const
   { return reinterpret_cast<CGumpEditorDoc*>(m_pDocument); }
#endif

