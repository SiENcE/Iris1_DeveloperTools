// GumpEditorView.cpp : implementation of the CGumpEditorView class
//

#include "stdafx.h"
#include "GumpEditor.h"

#include "GumpEditorDoc.h"
#include ".\gumpeditorview.h"
#include "dyndroptarget.h"
#include "GumpControlTypeDlg.h"
#include "GumpButton.h"
#include "GumpPicture.h"
#include "GumpStatic.h"
#include "GumpEdit.h"
#include "GumpBorder.h"
#include "GumpCheckbox.h"
#include "GumpRadio.h"
#include "GumpPaperdoll.h"
#include "GumpControlListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGumpEditorView

IMPLEMENT_DYNCREATE(CGumpEditorView, CDialogEditorDemoView)

BEGIN_MESSAGE_MAP(CGumpEditorView, CDialogEditorDemoView)
	ON_MESSAGE(WM_ADDCONTROL,AddControl)
	ON_COMMAND(ID_ADD_STATIC, OnAddStatic)
	ON_COMMAND(ID_ADD_EDIT, OnAddEdit)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CONTROLLIST, OnViewControllist)
END_MESSAGE_MAP()

// CGumpEditorView construction/destruction


CGumpEditorView::CGumpEditorView()
: m_bInit(false)
{
	// TODO: add construction code here
}

CGumpEditorView::~CGumpEditorView()
{
}


void CGumpEditorView::OnInitialUpdate()
{

	CDialogEditorDemoView::OnInitialUpdate();
}


// CGumpEditorView diagnostics

#ifdef _DEBUG
void CGumpEditorView::AssertValid() const
{
	CDialogEditorDemoView::AssertValid();
}

void CGumpEditorView::Dump(CDumpContext& dc) const
{
	CDialogEditorDemoView::Dump(dc);
}

CGumpEditorDoc* CGumpEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGumpEditorDoc)));
	return (CGumpEditorDoc*)m_pDocument;
}
#endif //_DEBUG


long CGumpEditorView::AddControl(WPARAM wParam, LPARAM lParam)
{
	// Creates a new control on the screen, caught from the
	// drop event from the toolbar control

	UINT nType = wParam;
	CPoint point(lParam>>16,lParam&0xffff);
	CGumpPtr pGump = GetDocument()->GetSelectedGump();
	if (!pGump) return 0;

	CGumpControlTypeDlg dlg;
	if (IDOK != dlg.DoModal()) return 0;

	int controlType = dlg.GetControlType();

	CDiagramEntity* obj=NULL;

	if (controlType==dlg.PICTURE) {
		obj = new CGumpPicture(pGump);
	} else if (controlType==dlg.BORDER) {
		obj = new CGumpBorder(pGump);
	} else if (controlType==dlg.PAPERDOLL) {
		obj = new CGumpPaperdoll(pGump);
	} else if (controlType==dlg.BUTTON) {
		CGumpPtr pGump2 = GetDocument()->FindGump(pGump->GetGumpID()+1,pGump->GetWidth(),pGump->GetHeight());
		obj = new CGumpButton(pGump,pGump,pGump2 ? pGump2 : pGump);
	} else if (controlType==dlg.CHECKBOX) {
		CGumpPtr pGump2 = GetDocument()->FindGump(pGump->GetGumpID()+1,pGump->GetWidth(),pGump->GetHeight());
		obj = new CGumpCheckbox(pGump, pGump2);
	} else if (controlType==dlg.RADIO) {
		CGumpPtr pGump2 = GetDocument()->FindGump(pGump->GetGumpID()+1,pGump->GetWidth(),pGump->GetHeight());
		obj = new CGumpRadio(pGump, pGump2);
	} 

	if (!obj) return 0;

	CSize size = obj->GetRect().Size();
	CRect rect(point,point+size);
	m_editor.AdjustRect(rect);
	obj->SetRect(rect);
	m_editor.UnselectAll();
	m_editor.SelectObject( obj, TRUE);
	m_editor.AddObject( obj );
	
	return 0;
}

void CGumpEditorView::OnAddStatic()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGumpStatic* obj = new CGumpStatic;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_STATIC;
}

void CGumpEditorView::OnAddEdit()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGumpEdit* obj = new CGumpEdit;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_EDIT;
}

int CGumpEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEditorDemoView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_DynDropTarget.Register(this);
	m_DynDropTarget.m_nIDClipFormat = RegisterClipboardFormat("GumpEditor");

	return 0;
}

void CGumpEditorView::OnViewControllist()
{
	CGumpControlListDlg dlg(&GetDEditor());
	dlg.DoModal();
}
