// GumpEditorView.cpp : implementation of the CGumpEditorView class
//

#include "stdafx.h"
#include "GumpEditor.h"

#include "GumpEditorDoc.h"
#include "GumpEditorView.h"
#include "dyndroptarget.h"


#include "entity/GumpEntityFactory.h"
//#include "entity/GumpEdit.h"
//#include "entity/GumpStatic.h"

#include "GumpControlTypeDlg.h"
#include "GumpControlListDlg.h"
#include ".\gumpeditorview.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGumpEditorView

IMPLEMENT_DYNCREATE(CGumpEditorView, CDialogEditorDemoView)

BEGIN_MESSAGE_MAP(CGumpEditorView, CDialogEditorDemoView)
	ON_MESSAGE(WM_ADDCONTROL,AddControl)
	ON_COMMAND(ID_ADD_STATIC, OnAddStatic)
	ON_COMMAND(ID_ADD_EDIT, OnAddEdit)
	ON_COMMAND(ID_ADD_CUSTOM, OnAddCustom)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CONTROLLIST, OnViewControllist)
	ON_UPDATE_COMMAND_UI(ID_ADD_CUSTOM, OnUpdateAddCustom)
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

	CGumpEntityFactory::TYPE type = dlg.GetControlType();
	CGumpEntity* obj = CGumpEntityFactory::CreateFromType(type, pGump);

	if (!obj) return 0;

	if (type == CGumpEntityFactory::STATIC) {
		m_editor.StartDrawingObject( obj );
		m_drawObject = DRAW_OBJECT_STATIC;
	}
	else
	if (type == CGumpEntityFactory::EDIT) {
		m_editor.StartDrawingObject( obj );
		m_drawObject = DRAW_OBJECT_EDIT;
	}
	else {
		CSize size = obj->GetRect().Size();
		CRect rect(point,point+size);
		m_editor.AdjustRect(rect);
		obj->SetRect(rect);
		m_editor.UnselectAll();
		m_editor.SelectObject( obj, TRUE);
		m_editor.AddObject( obj );
	}
	
	return 0;
}

void CGumpEditorView::OnAddStatic()
{
	CGumpEntity* obj = CGumpEntityFactory::CreateFromType(CGumpEntityFactory::STATIC, NULL);
	m_editor.StartDrawingObject(obj);
	m_drawObject = DRAW_OBJECT_STATIC;
}

void CGumpEditorView::OnAddEdit()
{
	CGumpEntity* obj = CGumpEntityFactory::CreateFromType(CGumpEntityFactory::EDIT, NULL);
	m_editor.StartDrawingObject(obj);
	m_drawObject = DRAW_OBJECT_EDIT;
}

void CGumpEditorView::OnAddCustom()
{
	CGumpEntity* obj = CGumpEntityFactory::CreateFromType(CGumpEntityFactory::CUSTOM, NULL);
	m_editor.StartDrawingObject(obj);
	m_drawObject = DRAW_OBJECT_CUSTOM;
}



void CGumpEditorView::OnUpdateAddCustom(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_CUSTOM );
}
