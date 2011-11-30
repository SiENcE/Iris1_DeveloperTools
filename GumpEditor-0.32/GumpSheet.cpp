// GumpSheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include ".\gumpsheet.h"


// CGumpSheet

IMPLEMENT_DYNAMIC(CGumpSheet, CPropertySheet)
CGumpSheet::CGumpSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CGumpSheet::CGumpSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CGumpSheet::~CGumpSheet()
{
}


BEGIN_MESSAGE_MAP(CGumpSheet, CPropertySheet)
END_MESSAGE_MAP()


// CGumpSheet 메시지 처리기입니다.

BOOL CGumpSheet::OnInitDialog()
{
	m_bModeless = FALSE;   
	m_nFlags |= WF_CONTINUEMODAL;

	BOOL bResult = CPropertySheet::OnInitDialog();

	m_bModeless = TRUE;
	m_nFlags &= ~WF_CONTINUEMODAL;
	return bResult;

}

void CGumpSheet::SetValues(void)
{

}
