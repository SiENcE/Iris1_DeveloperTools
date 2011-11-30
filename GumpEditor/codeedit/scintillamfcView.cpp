/* @doc
* @module scintillamfcView.cpp | Implementation of a Scintilla syntax coloring view for MFC
*/

#include "stdafx.h"
#include "../GumpEditor.h"

//#include "scintillamfcDoc.h"
#include "scintillamfcView.h"
#include "wordlistdlg.h"
#include "GotoLineDlg.h"
#include "GotoPosDlg.h"
#include "FindDlg.h"
#include "ReplaceDlg.h"

#include "scintilla.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScintillamfcView

IMPLEMENT_DYNCREATE(CScintillamfcView, CView)

BEGIN_MESSAGE_MAP(CScintillamfcView, CView)
	//{{AFX_MSG_MAP(CScintillamfcView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_OPTIONS_VIEW_LINENUMBERS, OnOptionsViewLinenumbers)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_VIEW_LINENUMBERS, OnUpdateOptionsViewLinenumbers)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_OPTIONS_SETFONT, OnOptionsSetfont)
	ON_COMMAND(ID_OPTIONS_FOREGROUND, OnOptionsForeground)
	ON_COMMAND(ID_OPTIONS_BACKGROUND, OnOptionsBackground)
	ON_COMMAND(ID_OPTIONS_WORDLIST, OnOptionsWordlist)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR, OnUpdateInsert)
	ON_COMMAND(ID_OPTIONS_FOLD_MARGIN, OnOptionsFoldMargin)
	ON_COMMAND(ID_OPTIONS_SELECTION_MARGIN, OnOptionsSelectionMargin)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SELECTION_MARGIN, OnUpdateOptionsSelectionMargin)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_FOLD_MARGIN, OnUpdateOptionsFoldMargin)
	ON_COMMAND(ID_OPTIONS_ADDMARKER, OnOptionsAddmarker)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ADDMARKER, OnUpdateOptionsAddmarker)
	ON_COMMAND(ID_OPTIONS_DELETEMARKER, OnOptionsDeletemarker)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_DELETEMARKER, OnUpdateOptionsDeletemarker)
	ON_COMMAND(ID_OPTIONS_FIND_NEXTMARKER, OnOptionsFindNextmarker)
	ON_COMMAND(ID_OPTIONS_FIND_PREVMARKER, OnOptionsFindPrevmarker)
	ON_COMMAND(ID_OPTIONS_GOTOPOS, OnOptionsGotopos)
	ON_COMMAND(ID_OPTIONS_GOTOLINE, OnOptionsGotoline)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	//}}AFX_MSG_MAP
	// Standard-Druckbefehle
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

	//ON_COMMAND(ID_OPTIONS_DLG, OnOptionsDlg)
//	ON_COMMAND(ID_TOGGLE_INSERT, OnToggleInsert)
	//ON_UPDATE_COMMAND_UI(ID_INDICATOR_LINE, OnUpdateLine) 
	//ON_UPDATE_COMMAND_UI(ID_INDICATOR_STYLE, OnUpdateStyle) 
	//ON_UPDATE_COMMAND_UI(ID_INDICATOR_FOLD, OnUpdateFold) 

/////////////////////////////////////
// @mfunc constructor
// @rvalue void | not used
//
CScintillamfcView::CScintillamfcView()
: m_pReplaceDlg(NULL), m_pFindDlg(NULL)
{
}
/////////////////////////////////////
// @mfunc destructor
// @rvalue void | not used
//
CScintillamfcView::~CScintillamfcView()
{
}
/////////////////////////////////////
// @mfunc called before window is created
// @rvalue void | not used
//
BOOL CScintillamfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}
/////////////////////////////////////
// @mfunc Called when view should be redrawn - not used - handled by scintilla window
// @rvalue void | not used
//
void CScintillamfcView::OnDraw(CDC* pDC)
{

}
/////////////////////////////////////
// @mfunc not handled
// @rvalue void | not used
//
BOOL CScintillamfcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

/////////////////////////////////////
// @mfunc not handled
// @rvalue void | not used
//
void CScintillamfcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
/////////////////////////////////////
// @mfunc not handled
// @rvalue void | not used
//
void CScintillamfcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
#ifdef _DEBUG
void CScintillamfcView::AssertValid() const
{
	CView::AssertValid();
}

void CScintillamfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

/////////////////////////////////////
// @mfunc return document handle
// @rvalue void | not used
//
#endif //_DEBUG

/////////////////////////////////////
// @mfunc Called when window is created - we create our scintilla window here
// @rvalue int | 0 on succescc else -1
//
int CScintillamfcView::OnCreate(
								LPCREATESTRUCT lpCreateStruct) //@parm pointer to propsed create structure - may be modified
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_wndScintilla.Create(_T("Title"), WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, 10000)) // hb - todo autogenerate id
	{   
		LPVOID lpMsgBuf;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf,0,NULL);
		// Write to stderr
		TRACE (_T("%s\n"), (LPCTSTR)lpMsgBuf);
		// Free the buffer.
		LocalFree( lpMsgBuf );
		return -1;
	}

	return 0;
}

/////////////////////////////////////
// @mfunc Called when view is resized - we adjust the size of the scintilla window to fit client size
// @rvalue void | not used
//
void CScintillamfcView::OnSize(
							   UINT nType, //@parm indicator
							   int cx,     //@parm new width in pixel
							   int cy)     //@parm new height in pixel
{
	CView::OnSize(nType, cx, cy);
	if (m_wndScintilla.GetSafeHwnd())
	{
		m_wndScintilla.MoveWindow(0, 0, cx, cy);
	}
}

/////////////////////////////////////
// @mfunc Called when view was created - we try to let scintilla load a file if there is one
// @rvalue void | not used
//
void CScintillamfcView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	CDocument *pDoc = GetDocument();
	CString strTitle = pDoc->GetPathName();
	m_wndScintilla.Init();
	//m_wndScintilla.LoadFile(strTitle);
	m_wndScintilla.SetLexer(SCLEX_CPP);
	m_wndScintilla.SendMessage(SCI_SETKEYWORDS, 0, (long)_T(
		"argCount and and_eq  "
		"bitand bitor break "
		"cslVersion cslCompiler cslBuilt cslFileName cslFunction "
		"case catch const continue "
		"default do "
		"else extern exists"
		"false for "
		"if "
		"mainArgVals MAXLONG "
		"not not_eq "
		"or or_eq "
		"PATHSEPARATOR "
		"resize return "
		"sizeof static switch "
		"throw true try "
		"var void "
		"while xor xor_eq"
	));
	//m_wndScintilla.SendMessage(SCI_SETCONTROLCHARSYMBOL, 0, 0);
	//m_wndScintilla.SendMessage(SCI_STYLESETVISIBLE, STYLE_CONTROLCHAR, 0);
	//m_wndScintilla.SetControlCharSymbol();
}
/////////////////////////////////////
// @mfunc menu handler - toggle display of the line number display in scintilla
// @rvalue void | not used
//
void CScintillamfcView::OnOptionsViewLinenumbers() 
{
	m_wndScintilla.SetDisplayLinenumbers(!m_wndScintilla.GetDisplayLinenumbers());
}
/////////////////////////////////////
// @mfunc menu check handler - check the menu item if line numbers are displayed
// @rvalue void | not used
//
void CScintillamfcView::OnUpdateOptionsViewLinenumbers(
	CCmdUI* pCmdUI) //@parm menu handle
{
	pCmdUI->SetCheck(m_wndScintilla.GetDisplayLinenumbers());
}
/////////////////////////////////////
// @mfunc menu handler - copy selected text to clipboard
// @rvalue void | not used
//
void CScintillamfcView::OnEditCopy() 
{
	m_wndScintilla.Copy();
}
/////////////////////////////////////
// @mfunc menu check handler - do nothing
// @rvalue void | not used
//
void CScintillamfcView::OnUpdateEditCopy(
	CCmdUI* pCmdUI) //@parm menu handle
{
}
/////////////////////////////////////
// @mfunc menu handler - cut selected text to clipboard
// @rvalue void | not used
//
void CScintillamfcView::OnEditCut() 
{
	m_wndScintilla.Cut();
}
/////////////////////////////////////
// @mfunc menu check handler - do nothing
// @rvalue void | not used
//
void CScintillamfcView::OnUpdateEditCut(
										CCmdUI* pCmdUI) //@parm menu handle
{
}
/////////////////////////////////////
// @mfunc menu handler - paste text from clipboard
// @rvalue void | not used
//
void CScintillamfcView::OnEditPaste() 
{
	m_wndScintilla.Paste();
}
/////////////////////////////////////
// @mfunc menu check handler - check if there is somethiong to paste
// @rvalue void | not used
//
void CScintillamfcView::OnUpdateEditPaste(
	CCmdUI* pCmdUI) //@parm menu handle
{
	pCmdUI->Enable(m_wndScintilla.CanPaste());
}
/////////////////////////////////////
// @mfunc menu handler - undo last action if possible
// @rvalue void | not used
//
void CScintillamfcView::OnEditUndo() 
{
	if (m_wndScintilla.CanUndo())
		m_wndScintilla.Undo();
}
/////////////////////////////////////
// @mfunc menu check handler - check if there is an action to undo
// @rvalue void | not used
//
void CScintillamfcView::OnUpdateEditUndo(
	CCmdUI* pCmdUI) //@parm menu handle
{
	pCmdUI->Enable(m_wndScintilla.CanUndo());
}
/////////////////////////////////////
// @mfunc update line number
// @rvalue void | not used
//
void CScintillamfcView::OnUpdateLine(
									 CCmdUI* pCmdUI) //@parm handle of textpane for linenumbers - see mainfrm.cpp at top
{
#if 0
	CString strLine;
	long lLine = m_wndScintilla.GetCurrentLine();
	long lColumn = m_wndScintilla.GetCurrentColumn();
	long lPos = m_wndScintilla.GetCurrentPosition();
	strLine.Format(IDS_LINE_INDICATOR, lLine, lColumn, lPos);
	pCmdUI->SetText(strLine);
#endif
} 
/////////////////////////////////////
// @mfunc update style number
// @rvalue void | not used
//
void CScintillamfcView::OnUpdateStyle(
									  CCmdUI* pCmdUI) //@parm handle of textpane for style number - see mainfrm.cpp at top
{
#if 0
	CString strLine;
	int nStyle = m_wndScintilla.GetCurrentStyle();
	strLine.Format(IDS_STYLE_INDICATOR, nStyle);
	pCmdUI->SetText(strLine);
#endif
} 
/////////////////////////////////////
// @mfunc update folding level
// @rvalue void | not used
//
void CScintillamfcView::OnUpdateFold(
									 CCmdUI* pCmdUI) //@parm handle of textpane for folding level - see mainfrm.cpp at top
{
#if 0
	CString strLine;
	int nFold = m_wndScintilla.GetCurrentFoldinglevel();
	strLine.Format(IDS_FOLD_INDICATOR, nFold);
	pCmdUI->SetText(strLine);
#endif
} 

/////////////////////////////////////
// @mfunc menu handler - delete selection
// @rvalue void | not used
//
void CScintillamfcView::OnEditClear() 
{
	m_wndScintilla.Clear();
}

/////////////////////////////////////
// @mfunc menu handler - redo last undone action
// @rvalue void | not used
//
void CScintillamfcView::OnEditRedo() 
{
	if (m_wndScintilla.CanRedo())
		m_wndScintilla.Redo();
}

/////////////////////////////////////
// @mfunc menu check handler - check if there is an action to redo
// @rvalue void | not used
//
void CScintillamfcView::OnUpdateEditRedo(
	CCmdUI* pCmdUI) //@parm menu handle
{
	pCmdUI->Enable(m_wndScintilla.CanRedo());
}

/////////////////////////////////////
// @mfunc menu handler - select complete text
// @rvalue void | not used
//
void CScintillamfcView::OnEditSelectAll() 
{
	m_wndScintilla.SelectAll();
}

/////////////////////////////////////
// @mfunc menu handler - display a window font selection dialog and set font attributes for style at caret position
// @rvalue void | not used
//
void CScintillamfcView::OnOptionsSetfont() 
{
	CFontDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_wndScintilla.SetFontname(m_wndScintilla.GetCurrentStyle(), dlg.GetFaceName());
		m_wndScintilla.SetFontheight(m_wndScintilla.GetCurrentStyle(), dlg.GetSize()/10);
		m_wndScintilla.SetBold(m_wndScintilla.GetCurrentStyle(), dlg.IsBold());
		m_wndScintilla.SetItalic(m_wndScintilla.GetCurrentStyle(), dlg.IsItalic());
		m_wndScintilla.SetUnderline(m_wndScintilla.GetCurrentStyle(), dlg.IsUnderline());
	}
}
/////////////////////////////////////
// @mfunc menu handler - display a color selection dialog and set foreground color attribute for style at caret position
// @rvalue void | not used
//
void CScintillamfcView::OnOptionsForeground() 
{
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_wndScintilla.SetForeground(m_wndScintilla.GetCurrentStyle(), dlg.GetColor());
	}
}
/////////////////////////////////////
// @mfunc menu handler - display a color selection dialog and set background color attribute for style at caret position
// @rvalue void | not used
//
void CScintillamfcView::OnOptionsBackground() 
{
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_wndScintilla.SetBackground(m_wndScintilla.GetCurrentStyle(), dlg.GetColor());
	}
}

/////////////////////////////////////
// @mfunc menu handler - display a wordlist dialog and set a wordlist with keywords for current lexer
// @rvalue void | not used
//
void CScintillamfcView::OnOptionsWordlist() 
{
	CWordlistDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		int nWordlist = ::atoi(dlg.m_strWordlistNumber);
		m_wndScintilla.SendMessage(SCI_SETKEYWORDS, nWordlist, (long)(LPCSTR)dlg.m_strKeywords);
		m_wndScintilla.Refresh();
	}

}
/////////////////////////////////////
// @mfunc menu check handler - set indicator for overstrike mode
// @rvalue void | not used
//
void CScintillamfcView::OnUpdateInsert(
									   CCmdUI* pCmdUI) //@parm menu handle
{
	CString strText;
	if (m_wndScintilla.GetOverstrike())
		strText.LoadString(ID_INDICATOR_OVR);
	pCmdUI->SetText(strText);
}
/////////////////////////////////////
// @mfunc command handler - set control to overstrike mode - see accelarator table in ressource file
// @rvalue void | not used
//
void CScintillamfcView::OnToggleInsert()
{
	m_wndScintilla.SetOverstrike(!m_wndScintilla.GetOverstrike());
}
/////////////////////////////////////
// @mfunc menu handler - toggle display of folding margin
// @rvalue void | not used
//
void CScintillamfcView::OnOptionsFoldMargin() 
{
	m_wndScintilla.SetDisplayFolding();
}
/////////////////////////////////////
// @mfunc menu handler - toggle display of selection margin
// @rvalue void | not used
//
void CScintillamfcView::OnOptionsSelectionMargin() 
{
	m_wndScintilla.SetDisplaySelection(!m_wndScintilla.GetDisplaySelection());
}

/////////////////////////////////////
// @mfunc menu check handler - check menu when selection margin is displayed
// @rvalue void | not used
//
void CScintillamfcView::OnUpdateOptionsSelectionMargin(
	CCmdUI* pCmdUI) //@parm menu handle
{
	pCmdUI->SetCheck(m_wndScintilla.GetDisplaySelection());
}
/////////////////////////////////////
// @mfunc menu check handler - check menu when folding margin is displayed
// @rvalue void | not used
//
void CScintillamfcView::OnUpdateOptionsFoldMargin(
	CCmdUI* pCmdUI) //@parm menu handle
{
	pCmdUI->SetCheck(m_wndScintilla.GetDisplayFolding());
}

/////////////////////////////////////
// @mfunc menu handler - add a bookmark at the caret line
// @rvalue void | not used
//
void CScintillamfcView::OnOptionsAddmarker() 
{
	int line = m_wndScintilla.GetCurrentLine();
	if (m_wndScintilla.HasBookmark(line))
		m_wndScintilla.DeleteBookmark(line);
	else
		m_wndScintilla.AddBookmark(line);
}
/////////////////////////////////////
// @mfunc menu check handler - disable menu item when there is already a marker at that line
// @rvalue void | not used
//
void CScintillamfcView::OnUpdateOptionsAddmarker(
	CCmdUI* pCmdUI) //@parm menu handle
{
	pCmdUI->Enable();
	//pCmdUI->Enable(!m_wndScintilla.HasBookmark(m_wndScintilla.GetCurrentLine()));
}
/////////////////////////////////////
// @mfunc menu handler - delete a bookmark at the caret line
// @rvalue void | not used
//
void CScintillamfcView::OnOptionsDeletemarker() 
{
	m_wndScintilla.DeleteBookmark(m_wndScintilla.GetCurrentLine());
}
/////////////////////////////////////
// @mfunc menu check handler - disable menu item when there is no marker at that line
// @rvalue void | not used
//
void CScintillamfcView::OnUpdateOptionsDeletemarker(
	CCmdUI* pCmdUI) //@parm menu handle
{
	pCmdUI->Enable(m_wndScintilla.HasBookmark(m_wndScintilla.GetCurrentLine()));
}
/////////////////////////////////////
// @mfunc menu handler - find next bookmark from the caret line
// @rvalue void | not used
//
void CScintillamfcView::OnOptionsFindNextmarker() 
{
	if (!m_wndScintilla.FindNextBookmark())
		m_wndScintilla.FindNextBookmark(0);
}
/////////////////////////////////////
// @mfunc menu handler - find previous bookmark from the caret line
// @rvalue void | not used
//
void CScintillamfcView::OnOptionsFindPrevmarker() 
{
	if (!m_wndScintilla.FindPreviousBookmark())
		m_wndScintilla.FindPreviousBookmark(m_wndScintilla.GetLineCount()-1);
}
/////////////////////////////////////
// @mfunc menu handler - find previous bookmark from the caret line
// @rvalue BOOL | TRUE if we handled the message - FALSE if message should be processed further
//
BOOL CScintillamfcView::OnNotify( 
								 WPARAM wParam,       //@parm not used
								 LPARAM lParam,       //@parm pointer to notification structure
								 LRESULT* pResult )   //@parm result
{
	NMHDR *phDR;
	phDR = (NMHDR*)lParam;
	// does notification come from my scintilla window?
	if (phDR != NULL && phDR->hwndFrom == m_wndScintilla.m_hWnd)
	{
		SCNotification *pMsg = (SCNotification*)lParam;
		switch (phDR->code)
		{
		case SCN_STYLENEEDED:
			break;
		case SCN_CHARADDED:
			break;
		case SCN_SAVEPOINTREACHED:
			break;
			//       called when the document is changed - mark document modified
		case SCN_SAVEPOINTLEFT:
			{
				CDocument *pDoc = GetDocument();
				pDoc->SetModifiedFlag(TRUE);
			}
			break;
		case SCN_MODIFYATTEMPTRO:
			break;
		case SCN_KEY:
			break;
		case SCN_DOUBLECLICK:
			break;
			//       called when something changes and we want to show new indicator state or brace matching
		case SCN_UPDATEUI:
			{
				m_wndScintilla.UpdateUI();
			}
			break;
		case SCN_MODIFIED:
			break;
		case SCN_MACRORECORD:
			break;
			//       user clicked margin - try folding action
		case SCN_MARGINCLICK:
			{
				m_wndScintilla.DoDefaultFolding(pMsg->margin, pMsg->position);
			}
			break;
		case SCN_NEEDSHOWN:
			break;
		case SCN_PAINTED:
			break;
		case SCN_USERLISTSELECTION:
			break;
		case SCN_URIDROPPED:
			break;
		case SCN_DWELLSTART:
			break;
		case SCN_DWELLEND:
			break;
		}
		return TRUE; // we processed the message
	}
	return CWnd::OnNotify(wParam, lParam, pResult);
}
/////////////////////////////////////
// @mfunc menu handler - save current document - let the control do the work
// @rvalue void | not used
//
BOOL CScintillamfcView::Save(
							 LPCSTR szPath) //@parm path of document to save to
{
	return m_wndScintilla.SaveFile(szPath);
}

/////////////////////////////////////
// @mfunc menu handler - goto to a character position in the control
// @rvalue void | not used
//
void CScintillamfcView::OnOptionsGotopos() 
{
	CGotoPosDlg dlg;
	dlg.m_nPos = m_wndScintilla.GetCurrentPosition();
	if (dlg.DoModal() == IDOK)
	{
		m_wndScintilla.GotoPosition(dlg.m_nPos);
		m_wndScintilla.SetFocus();
	}
}
/////////////////////////////////////
// @mfunc menu handler - goto to a character position in the control
// @rvalue void | not used
//
void CScintillamfcView::OnOptionsGotoline() 
{
	CGotoLineDlg dlg;
	dlg.m_nLine = m_wndScintilla.GetCurrentLine();
	if (dlg.DoModal() == IDOK)
	{
		m_wndScintilla.GotoLine(dlg.m_nLine);
		m_wndScintilla.SetFocus();
	}
}

/////////////////////////////////////
// @mfunc menu handler - 
// @rvalue void | not used
//
void CScintillamfcView::OnOptionsDlg() 
{
	//CScintillaDlg dlg;
	//dlg.DoModal();

}

void CScintillamfcView::ShowDialogInCenter(CWnd *pDlg)
{
	if (!pDlg) return;
	CRect r1,r2;
	pDlg->GetClientRect(r1);
	GetWindowRect(r2);
	CPoint pt = r2.TopLeft();
	pt.x += (r2.Width()-r1.Width())/2, pt.y += (r2.Height()-r1.Height())/2;
	pDlg->SetWindowPos(&wndTop, pt.x, pt.y, 0,0, SWP_NOSIZE | SWP_SHOWWINDOW);
}


/////////////////////////////////////
// @mfunc Display the Search dialog for some views which use the Scintilla edit control
// @rvalue void | not used
//
void CScintillamfcView::OnEditFind() 
{
    m_pFindDlg = new CFindDlg(this);
    if (m_pFindDlg != NULL)
    {
		m_pFindDlg->Create(CFindDlg::IDD);
		m_pFindDlg->m_pEdit = GetEditControl();
		//m_pFindDlg->ShowWindow(SW_SHOW);
		m_pFindDlg->InitDialog();
    }

	m_pFindDlg->SetSearchString(m_wndScintilla.GetSelectedText());
	ShowDialogInCenter(m_pFindDlg);
}

/////////////////////////////////////
// @mfunc Display the Search/Replace dialog for some views which use the Scintilla edit control
// @rvalue void | not used
//
void CScintillamfcView::OnEditReplace() 
{
    m_pReplaceDlg = new CReplaceDlg(this);
    if (m_pReplaceDlg != NULL)
    {
		m_pReplaceDlg->Create(CReplaceDlg::IDD);
		m_pReplaceDlg->m_pEdit = GetEditControl();
		m_pReplaceDlg->ShowWindow(SW_SHOW);
		m_pReplaceDlg->InitDialog();
    }

	m_pReplaceDlg->SetReplaceString(m_wndScintilla.GetSelectedText());
	ShowDialogInCenter(m_pReplaceDlg);
}
