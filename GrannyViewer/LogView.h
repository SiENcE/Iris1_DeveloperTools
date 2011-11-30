#pragma once


// CLogView 뷰입니다.

class CLogView : public CEditView
{
	DECLARE_DYNCREATE(CLogView)

protected:
	CLogView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CLogView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void Log(LPCTSTR lpszLog);
	virtual void OnInitialUpdate();
	void Clear(void);

	void SetFont(LPCTSTR szFontName, int iFontSize, bool bSave = true);

	CFont m_font;
};


