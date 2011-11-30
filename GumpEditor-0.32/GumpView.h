#pragma once


// CGumpView 뷰입니다.

class CDib;

class CGumpView : public CView
{
	DECLARE_DYNCREATE(CGumpView)
public:
	CGumpPtr m_pDib;
	CGumpPtr GetGump() { return m_pDib; }
	void SetGump(CGumpPtr pDIb);

protected:
	CGumpView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CGumpView();

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
	
};


