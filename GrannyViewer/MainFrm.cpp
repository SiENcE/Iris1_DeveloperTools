// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "GrannyViewer.h"

#include "MainFrm.h"
#include "ST_SplitterWnd.h"
#include "GrannyListView.h"
#include ".\mainfrm.h"
#include "LogView.h"
#include "GrannyViewerView.h"
#include "GrannyViewerDoc.h"
#include "ModelListView.h"
#include "GrannyAniListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	for (int i = 0; i < 4; i++)
		m_pSplitterWnd[i] = NULL;	
}

CMainFrame::~CMainFrame()
{
	for (int i = 0; i < 4; i++)
		delete m_pSplitterWnd[i];	
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}


// CMainFrame 진단

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


// CMainFrame 메시지 처리기


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// create splitter window
	m_pSplitterWnd[0] = new ST_SplitterWnd();

	//m_pSplitterWnd->Create(this,RUNTIME_CLASS(CGrannyListView),pContext->m_pNewViewClass,pContext,true);

	m_pSplitterWnd[0]->Create(this,NULL,NULL,pContext,true);

	m_pSplitterWnd[1] = m_pSplitterWnd[0]->AddSubDivision(RIGHT_SIDE,
		RUNTIME_CLASS(CGrannyViewerView), RUNTIME_CLASS(CLogView),pContext,false);   

	m_pSplitterWnd[2] = m_pSplitterWnd[0]->AddSubDivision(LEFT_SIDE,
		RUNTIME_CLASS(CGrannyListView), NULL, pContext,false);

	m_pSplitterWnd[3] = m_pSplitterWnd[2]->AddSubDivision(BOTTOM_SIDE,
		RUNTIME_CLASS(CGrannyAniListView), RUNTIME_CLASS(CModelListView),pContext,false);


	m_pSplitterWnd[0]->SetInitialStatus();

	return TRUE;
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	CGrannyViewerDoc* pDoc = GetDocument();
	if (pDoc) {
		CView* pView = pDoc->FindView(RUNTIME_CLASS(CGrannyViewerView));
		if (pView && pView->OnCmdMsg(nID,nCode,pExtra,pHandlerInfo)) return TRUE;
	}

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

CGrannyViewerDoc * CMainFrame::GetDocument(void)
{
	CDocument* pDoc = GetActiveDocument();
	if (pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CGrannyViewerDoc))) 
		return dynamic_cast<CGrannyViewerDoc*>(pDoc);
	return NULL;
}
