// LogView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GrannyViewer.h"
#include "LogView.h"
#include ".\logview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLogView

IMPLEMENT_DYNCREATE(CLogView, CEditView)

CLogView::CLogView()
{
}

CLogView::~CLogView()
{
}

BEGIN_MESSAGE_MAP(CLogView, CEditView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CLogView 진단입니다.

#ifdef _DEBUG
void CLogView::AssertValid() const
{
	CEditView::AssertValid();
}

void CLogView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG


// CLogView 메시지 처리기입니다.

int CLogView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetEditCtrl().SetReadOnly();

	CWinApp *pApp = AfxGetApp();
	CString strFont = pApp->GetProfileString(_T("font"), _T("name"), _T("Courier"));
	int iSize = pApp->GetProfileInt(_T("font"), _T("size"), 10);

	SetFont(strFont, iSize, false);

	return 0;
}

void CLogView::Log(LPCTSTR lpszLog)
{
	CEdit& edit = GetEditCtrl();
	edit.SetSel((int)GetBufferLength(), -1);

	CString strLog(lpszLog);
	strLog.Replace("\n", "\r\n");
	edit.ReplaceSel(strLog);
}

void CLogView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();
}

void CLogView::Clear(void)
{
	CEdit& edit = GetEditCtrl();
	edit.SetSel(0,-1);
	edit.ReplaceSel("");
}

void CLogView::SetFont(LPCTSTR szFontName, int iFontSize, bool bSave)
{
	if (bSave) {
		CWinApp *pApp = AfxGetApp();
		pApp->WriteProfileString(_T("font"), _T("name"), szFontName);
		pApp->WriteProfileInt(_T("font"), _T("size"), iFontSize);
	}

	m_font.DeleteObject();

	m_font.CreatePointFont(iFontSize, szFontName);
	SendMessage(WM_SETFONT, (WPARAM)(HFONT)m_font);
}