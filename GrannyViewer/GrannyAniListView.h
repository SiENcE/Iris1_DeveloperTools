#pragma once


// CGrannyAniListView 뷰입니다.

class CGrannyAniListView : public CGrannyListView
{
	DECLARE_DYNCREATE(CGrannyAniListView)

protected:
	CGrannyAniListView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CGrannyAniListView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


