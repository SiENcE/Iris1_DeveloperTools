#pragma once


// CCodeDocument 문서입니다.

class CCodeDocument : public CDocument
{
	DECLARE_DYNCREATE(CCodeDocument)

public:
	CCodeDocument();
	virtual ~CCodeDocument();
	virtual void Serialize(CArchive& ar);   // 문서 입/출력을 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
