
#include "stdafx.h"
#include "GumpEditor.h"
#include "CodeView.h"
#include ".\codeview.h"
#include "csldlg.h"

// CCodeView

IMPLEMENT_DYNCREATE(CCodeView, CScintillamfcView)

CCodeView::CCodeView()
{
	m_pCSLDlg = NULL;
}

CCodeView::~CCodeView()
{
}

BEGIN_MESSAGE_MAP(CCodeView, CScintillamfcView)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_VIEW_CSLSCRIPT, OnViewCslscript)
END_MESSAGE_MAP()


void CCodeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}


#ifdef _DEBUG
void CCodeView::AssertValid() const
{
	CScintillamfcView::AssertValid();
}

void CCodeView::Dump(CDumpContext& dc) const
{
	CScintillamfcView::Dump(dc);
}
#endif //_DEBUG


//#include "ctags/ctagsapi.h"

void CCodeView::GotoEventHandler(CString strEvHandler, CString strEvHandlerParams)
{
	UINT nLen = m_wndScintilla.SendMessage(SCI_GETLENGTH)+1;
	if (nLen > (UINT)strEvHandler.GetLength()) {
		char *szBuf = new char[nLen];
		m_wndScintilla.SendMessage(SCI_GETTEXT, (WPARAM)nLen, (LPARAM)szBuf);

		CTagInfo taginfo;
		taginfo.ReadTagInfoFromBuf(szBuf);
		int nLine = taginfo.FindTagLine(strEvHandler);

		delete [] szBuf; szBuf = NULL;
		
		if (nLine >= 0) {
			m_wndScintilla.SendMessage(SCI_GOTOLINE,nLine+1);
			return;
		}
	}

	CString strTmp = "\n";
	strTmp += strEvHandler + strEvHandlerParams + "\n{\n\n}\n";
	m_wndScintilla.SendMessage(SCI_APPENDTEXT, strTmp.GetLength(), (LPARAM)(LPCTSTR)strTmp);
	//m_wndScintilla.SendMessage(SCI_GOTOPOS,0);
	//m_wndScintilla.SendMessage(SCI_SETSEL,0,strEvHandler.GetLength());
	m_wndScintilla.SendMessage(SCI_GOTOLINE,m_wndScintilla.SendMessage(SCI_GETLINECOUNT)-3);
	
	//SetFocus();
	m_wndScintilla.SetFocus();
	m_wndScintilla.SetActiveWindow();
}

void CCodeView::OnSetFocus(CWnd* pOldWnd)
{
	CScintillamfcView::OnSetFocus(pOldWnd);

	m_wndScintilla.SendMessage(WM_SETFOCUS);
	
}

CString CCodeView::GetText(void)
{
	CString str;
	UINT iLen = m_wndScintilla.SendMessage(SCI_GETLENGTH);
	if (iLen > 0)
		m_wndScintilla.SendMessage(SCI_GETTEXT, iLen+1, (LPARAM)str.GetBufferSetLength(iLen+1));
	return str;
}

void CCodeView::SetText(LPCTSTR text)
{
	m_wndScintilla.SendMessage(SCI_SETTEXT, 0 , (LPARAM)text);
}

BOOL CCodeView::IsModified()
{
	return (m_wndScintilla.SendMessage(SCI_GETMODIFY, 0 , 0) != 0);
}

BOOL CCodeView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: Add your specialized code here and/or call the base class

	return CScintillamfcView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CCodeView::OnViewCslscript()
{
    if (!m_pCSLDlg)
    {
		m_pCSLDlg = new CCSLDlg(this);
		SetCSLDlg(m_pCSLDlg);
		m_pCSLDlg->Create(CCSLDlg::IDD);
		//m_pFindDlg->ShowWindow(SW_SHOW);
		//m_pCSLDlg->OnInitDialog();
    }
	else
		m_pCSLDlg->SetActiveWindow();

	ShowDialogInCenter(m_pCSLDlg);
}


void CCodeView::InsertText(CString strText)
{
	m_wndScintilla.SendMessage(SCI_ADDTEXT, strText.GetLength(), (LPARAM)(LPCTSTR)strText);
}