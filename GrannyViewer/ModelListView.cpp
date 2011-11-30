// ModelListView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GrannyViewer.h"
#include "GrannyViewerDoc.h"
#include "ModelListView.h"
#include "granny/GrannyLoader.h"
#include ".\modellistview.h"
#include "granny/GrannyModelTD.h"

// CModelListView

IMPLEMENT_DYNCREATE(CModelListView, CCtrlView)

CModelListView::CModelListView() 
: CCtrlView(WC_LISTBOX, AFX_WS_DEFAULT_VIEW | WS_VSCROLL )
{
}

CModelListView::~CModelListView()
{
}

BEGIN_MESSAGE_MAP(CModelListView, CCtrlView)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_LIST_HIDEMODEL, OnListHidemodel)
	ON_COMMAND(ID_LIST_RESETCOLOR, OnListResetcolor)
	ON_COMMAND(ID_LIST_SETCOLOR, OnListSetcolor)
	ON_COMMAND(ID_LIST_UNHIDEALLMODEL, OnListUnhideallmodel)
	ON_COMMAND(ID_LIST_UNHIDEMODEL, OnListUnhidemodel)
	ON_COMMAND(ID_LIST_UNREGISTERMODEL, OnListUnregistermodel)
	ON_WM_SIZE()
	ON_COMMAND(ID_LIST_HIDEALLMODELS, OnListHideallmodels)
END_MESSAGE_MAP()


// CModelListView 진단입니다.

#ifdef _DEBUG
void CModelListView::AssertValid() const
{
	CCtrlView::AssertValid();
}

void CModelListView::Dump(CDumpContext& dc) const
{
	CCtrlView::Dump(dc);
}

CGrannyViewerDoc* CModelListView::GetDocument() const 
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGrannyViewerDoc)));
	return (CGrannyViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CModelListView 메시지 처리기입니다.

void CModelListView::OnInitialUpdate()
{
	CCtrlView::OnInitialUpdate();

}

int CModelListView::OnCreate(LPCREATESTRUCT cs)
{
	if (CCtrlView::OnCreate(cs) == -1)
		return -1;

	CListBox &ctrl = GetListBox();
	ctrl.SendMessage(WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT));

	return 0;
}


void CModelListView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	if (!menu.LoadMenu(IDM_POPUP_MENU)) return;
	menu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}


int CModelListView::GetSelectedItem(UINT *pData)
{
	CListBox &ctrl = GetListBox();

	int index = ctrl.GetCurSel();
	if (index != LB_ERR && pData) 
		*pData = ctrl.GetItemData(index);

	return index;
}


void CModelListView::UnregisterModel(void)
{
	CGrannyViewerDoc *pDoc = GetDocument(); ASSERT(pDoc);
	if (!pDoc) return;

	UINT id = 0;

	int index = GetSelectedItem(&id);
	if (index == LB_ERR) return;

	pDoc->UnregisterModel(id);

	GetListBox().SetCurSel(index);
}


void CModelListView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar != VK_DELETE) return;

	UnregisterModel();

	CCtrlView::OnKeyDown(nChar, nRepCnt, nFlags);
}

CString  CModelListView::GetModelString(UINT id, cGrannyModel *pModel)
{
	if (!pModel) return "";

	CString str;
	COLORREF c = pModel->GetColor();

	if (pModel->desc.empty())
	{
		cGrannyModelTD *td = dynamic_cast<cGrannyModelTD*>(pModel);
		if (td) td->desc = getFileName(td->m_filename);
	}
	
	str.Format("%s%04d %s (%d,%d,%d)", pModel->show ? "" : "-",
		GETID(id), pModel->desc.c_str(), GetRValue(c), GetGValue(c), GetBValue(c));

	return str;
}

int CModelListView::InsertModelString(CListBox &ctrl, UINT id, cGrannyModel *pModel)
{
	if (!pModel) return LB_ERR;
	
	int index = ctrl.AddString(GetModelString(id, pModel));
	if (index != LB_ERR)
		ctrl.SetItemData(index, id);

	return index;
}

void CModelListView::UpdateModel(void)
{
	CGrannyViewerDoc *pDoc = GetDocument(); ASSERT(pDoc);
	if (!pDoc) return;

	CListBox &ctrl = GetListBox();
	ctrl.ResetContent();

	for (UINT i = 0; i < pDoc->GetReigsterModelCount(); i++)
	{
		UINT id = pDoc->GetReigsterModelId(i);
		cGrannyModel *pModel = pDoc->GetModel(id);
		InsertModelString(ctrl, id, pModel);
	}
}


void CModelListView::SetModelColor(COLORREF *c)
{
	CGrannyViewerDoc *pDoc = GetDocument(); ASSERT(pDoc);
	if (!pDoc) return;

	UINT id = 0;
	int index = GetSelectedItem(&id);
	if (index == LB_ERR) return;

	COLORREF color;

	if (c)
		color = *c;
	else
	{
		CColorDialog dlg(pDoc->GetModelColor(id));
		if (dlg.DoModal() != IDOK) return;
		color = dlg.GetColor();
	}

	pDoc->SetModelColor(id, color);

	GetListBox().SetCurSel(index);
}



void CModelListView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	ShowModel(-1); // toggle
}

void CModelListView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	UpdateModel();
}

void CModelListView::OnListUnregistermodel()
{
	UnregisterModel();
}

void CModelListView::OnListHidemodel()
{
	ShowModel(false);
}

void CModelListView::OnListSetcolor()
{
	SetModelColor();
}


void CModelListView::OnListResetcolor()
{
	COLORREF color = RGB(255,255,255);
	SetModelColor(&color);
}



void CModelListView::ShowAllModels(int show)
{
	CGrannyViewerDoc *pDoc = GetDocument(); ASSERT(pDoc);
	if (!pDoc) return;

	int index = GetSelectedItem();
	
	pDoc->ShowAllModels(show);

	GetListBox().SetCurSel(index);
}

void CModelListView::ShowModel(int show)
{
	CGrannyViewerDoc *pDoc = GetDocument(); ASSERT(pDoc);
	if (!pDoc) return;

	UINT id = 0;
	int index = GetSelectedItem(&id);
	if (index == LB_ERR) return;
	
	pDoc->ShowModel(id, show);
	
	GetListBox().SetCurSel(index);
}

void CModelListView::OnListUnhideallmodel()
{
	ShowAllModels(true);
}

void CModelListView::OnListUnhidemodel()
{
	ShowModel(true);
}

void CModelListView::OnSize(UINT nType, int cx, int cy)
{
	//don't call CCtrlView::OnSize()
	//CCtrlView::OnSize(nType, cx, cy);
}


void CModelListView::OnListHideallmodels()
{
	ShowAllModels(false);
}
