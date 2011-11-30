#pragma once


// CPropView 뷰입니다.

#include "proptree/PropTree.h"

class CGumpEditorDoc;
class CDynControl;

class CPropView : public CView
{
	DECLARE_DYNCREATE(CPropView)
public:
	CPropTree m_Tree;
	CDynControl* m_pControl;

	CGumpEditorDoc* GetDocument() { return (CGumpEditorDoc*)m_pDocument; }

protected:
	CPropView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CPropView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif



protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult);
	void UpdatePropView(void);
};


