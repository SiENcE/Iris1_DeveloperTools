// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "gumpeditordoc.h"
#include "ToolBarEx.h"

class CGumpEditorView;
class ST_SplitterWnd;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	ST_SplitterWnd* m_pSplitterWnd;
	ST_SplitterWnd* m_pSplitterWnd2;


public:
//	CDynControlBar	m_wndDynControlBar;
	CGumpEditorDoc* GetGumpDocument();

// Operations
public:

// Overrides
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
	CGumpEditorView* GetRightPane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBarWithCombo    m_wndToolBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();

	afx_msg void OnDoNothing(UINT) {}
	afx_msg void OnDoNothing2(CCmdUI* pUI) {pUI->Enable();}
	afx_msg void OnDoNothing3() {}
	void UpdateStatusBar(int iGumpID, CPoint point);
	afx_msg void OnViewHuepicker();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnViewViewxml();
};




