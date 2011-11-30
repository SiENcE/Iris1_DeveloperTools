#pragma once

class CModelListView : public CCtrlView
{
	DECLARE_DYNCREATE(CModelListView)

protected:
	CModelListView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CModelListView();

	CString GetModelString(UINT id, cGrannyModel *pModel);
	int InsertModelString(CListBox &ctrl, UINT id, cGrannyModel *pModel);
	void ShowModel(int show); // 0: hide, 1: show, -1: toggle
	void ShowAllModels(int show);
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void OnDraw(CDC *pDC) {}
	CGrannyViewerDoc *GetDocument() const; 
	void UpdateModel(void);
	CListBox &GetListBox() { return *(CListBox*)this; }
	int GetSelectedItem(UINT *pData = NULL);
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg void OnListHidemodel();
	afx_msg void OnListResetcolor();
	afx_msg void OnListSetcolor();
	afx_msg void OnListUnhideallmodel();
	afx_msg void OnListUnhidemodel();
	afx_msg void OnListUnregistermodel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void UnregisterModel(void);
	void SetModelColor(COLORREF *color = NULL);
	afx_msg void OnListHideallmodels();
};

#ifndef _DEBUG  
inline CGrannyViewerDoc* CModelListView::GetDocument() const
	{ return reinterpret_cast<CGrannyViewerDoc*>(m_pDocument); }
#endif
