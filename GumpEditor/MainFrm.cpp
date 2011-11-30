// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "GumpEditor.h"

#include "MainFrm.h"
#include "GumpListView.h"
#include "GumpEditorView.h"
#include ".\mainfrm.h"
#include "GumpView.h"
#include "ST_SplitterWnd.h"
#include "GumpEditorDoc.h"
#include "colorpicker/HuePickerDlg.h"

#include "CodeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void GfxUpdateStatusBar(int iGumpID, CPoint point)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	pFrame->UpdateStatusBar(iGumpID, point);
}

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	//ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	//ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
	ON_WM_DESTROY()
//	ON_COMMAND_RANGE(30000, 41000, OnDoNothing)
	//ON_UPDATE_COMMAND_UI(ID_INDICATOR_POS, OnDoNothing2)
	ON_COMMAND(ID_VIEW_HUEPICKER, OnViewHuepicker)
	ON_COMMAND(ID_VIEW_VIEWGUMPDESC, &CMainFrame::OnViewViewgumpdesc)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_GUMPID,
	ID_INDICATOR_POS
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_pSplitterWnd  = NULL;
	m_pSplitterWnd2 = NULL;
}

CMainFrame::~CMainFrame()
{
	delete m_pSplitterWnd;
	delete m_pSplitterWnd2;
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_SIZE_FIXED) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBar.CreateComboBox(m_wndToolBar.m_comboBox1, 3, IDC_COMBO1, 100, 100))
	{
		TRACE0("Failed to create toolbar's combo box\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_GUMPID, SBPS_NORMAL,40);
	m_wndStatusBar.SetPaneInfo(2,ID_INDICATOR_POS, SBPS_NORMAL,80);

	return 0;
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	m_pSplitterWnd = new ST_SplitterWnd();

	m_pSplitterWnd->Create(this,NULL,NULL,pContext,true);
	m_pSplitterWnd2 = m_pSplitterWnd->AddSubDivision(LEFT_SIDE,RUNTIME_CLASS(CGumpListView),RUNTIME_CLASS(CGumpView),pContext,false);   
	m_nViewNo[0] = m_pSplitterWnd->AddView(RIGHT_SIDE,RUNTIME_CLASS(CCodeView),pContext);
	m_nViewNo[1] = m_pSplitterWnd->AddView(RIGHT_SIDE,RUNTIME_CLASS(CGumpEditorView),pContext);

	m_pSplitterWnd->SetInitialStatus();


	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

CView* CMainFrame::GetRightPane()
{
	/*
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CGumpEditorView* pView = DYNAMIC_DOWNCAST(CGumpEditorView, pWnd);
	return pView;
	*/
	return NULL;
}

void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	if (!pCmdUI)
		return;
}


void CMainFrame::OnViewStyle(UINT nCommandID)
{
	// TODO: customize or extend this code to handle choices on the View menu
	CView* pView = GetRightPane();

	// if the right-hand pane has been created and is a CGumpEditorView,
	// process the menu commands...
	
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();


}

CGumpEditorDoc* CMainFrame::GetGumpDocument()
{
	CDocument* pDoc = GetActiveDocument();
	if (pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CGumpEditorDoc))) return (CGumpEditorDoc*)pDoc;
	return NULL;
}

void CMainFrame::UpdateStatusBar(int iGumpID, CPoint point)
{
	CString strText;
	strText.Format("%dx%d", point.x,point.y);
	m_wndStatusBar.SetPaneText(1,GfxXtoA(iGumpID));
	m_wndStatusBar.SetPaneText(2,strText);
}



void CMainFrame::OnViewHuepicker()
{
	CHuePickerDlg dlg;
	dlg.DoModal();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
#if 0
	if (ID_EDIT_COPY != nID) {
		CView* pView = NULL;
		CGumpEditorDoc* pDoc = GetGumpDocument();
		if (pDoc) {
			pView = pDoc->FindView(RUNTIME_CLASS(CCodeView));
			if (pView && pView->OnCmdMsg(nID,nCode,pExtra,pHandlerInfo)) return TRUE;
		}
	}
#endif

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::SwitchView()
{
	m_pSplitterWnd->SwitchToView(RIGHT_SIDE);
}

void CMainFrame::OnViewViewgumpdesc()
{
	// TODO: Add your command handler code here
	GfxResetWorkingDirectory();
	ShellExecute(NULL, "open", "gumpid.txt",NULL,NULL,SW_SHOW);
}
