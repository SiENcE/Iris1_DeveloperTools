// LeftView.h : interface of the CGumpListView class
//

#pragma once

#include "dyndroptarget.h"

class CGumpEditorDoc;

class CGumpListView : public CListView
{
protected: // create from serialization only
	CGumpListView();
	DECLARE_DYNCREATE(CGumpListView)

// Attributes
public:

protected:
	CGumpEditorDoc* GetDocument();
	UINT m_nIDClipFormat;
	CDynDropSource dynDropSource;
	
	std::map<int, std::string> m_mapGumpDesc;
	bool LoadGumpDesc(LPCTSTR szDescFile);
	bool SaveGumpDesc(LPCTSTR szDescFile);
	LPCTSTR GetGumpDesc(int iGumpId);

// Operations
public:
	int GetSelectedItem(int nItem = -1)
	{
		return GetListCtrl().GetNextItem(nItem, LVNI_ALL | LVNI_SELECTED);
	}

// Overrides
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CGumpListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	int GetSelectedGumpID(void);
	void UpdateGump(void);
	afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void SelectGump(int iGumpID);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnPopupCopygumpid();
	afx_msg void OnPopupInsertgump();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	bool m_bInit;
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
public:
	afx_msg void OnPopupExportgumpdesc();
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CGumpEditorDoc* CGumpListView::GetDocument()
   { return reinterpret_cast<CGumpEditorDoc*>(m_pDocument); }
#endif

