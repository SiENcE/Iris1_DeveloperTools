// CodeDocument.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "CodeDocument.h"


// CCodeDocument

IMPLEMENT_DYNCREATE(CCodeDocument, CDocument)

CCodeDocument::CCodeDocument()
{
}

BOOL CCodeDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CCodeDocument::~CCodeDocument()
{
}


BEGIN_MESSAGE_MAP(CCodeDocument, CDocument)
END_MESSAGE_MAP()


// CCodeDocument 진단입니다.

#ifdef _DEBUG
void CCodeDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CCodeDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCodeDocument serialization입니다.

void CCodeDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CCodeDocument 명령입니다.
