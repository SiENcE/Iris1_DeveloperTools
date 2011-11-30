// DialogEditorDemoView.cpp : implementation of the CDialogEditorDemoView class
//

#include "stdafx.h"
#include "DialogEditorDemo.h"

#include "DialogEditorDemoDoc.h"
#include "DialogEditorDemoView.h"

// --- DiagramEditor ---
#include "StdGrfx.h"
#include "DialogSettings.h"

#include "DiagramButton.h"
#include "DiagramEdit.h"
#include "DiagramStatic.h"
#include "DiagramCheckbox.h"
#include "DiagramRadiobutton.h"
#include "DiagramCombobox.h"
#include "DiagramListbox.h"
#include "DiagramGroupbox.h"
#include "../GumpEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorDemoView

IMPLEMENT_DYNCREATE(CDialogEditorDemoView, CView)

BEGIN_MESSAGE_MAP(CDialogEditorDemoView, CView)
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	//ON_COMMAND(IDC_SETTINGS, OnSettings)
	ON_COMMAND(IDC_PROPERTIES, OnProperties)
	ON_COMMAND(IDC_100PERC, On100perc)
	ON_COMMAND(IDC_110PERC, On110perc)
	ON_COMMAND(IDC_120PERC, On120perc)
	ON_COMMAND(IDC_130PERC, On130perc)
	ON_COMMAND(IDC_140PERC, On140perc)
	ON_COMMAND(IDC_150PERC, On150perc)
	ON_COMMAND(IDC_200PERC, On200perc)
	ON_COMMAND(IDC_25PERC, On25perc)
	ON_COMMAND(IDC_50PERC, On50perc)
	ON_COMMAND(IDC_60PERC, On60perc)
	ON_COMMAND(IDC_70PERC, On70perc)
	ON_COMMAND(IDC_80PERC, On80perc)
	ON_COMMAND(IDC_90PERC, On90perc)
	ON_UPDATE_COMMAND_UI(IDC_PROPERTIES, OnUpdateProperties)
	ON_COMMAND(IDC_SELECT_ALL, OnSelectAll)
	ON_COMMAND(ID_ADD_BUTTON, OnAddButton)
	ON_COMMAND(ID_ADD_EDIT, OnAddEdit)
	ON_COMMAND(ID_ADD_STATIC, OnAddStatic)
	ON_COMMAND(ID_ADD_CHECKBOX, OnAddCheckbox)
	ON_COMMAND(ID_ADD_COMBOBOX, OnAddCombobox)
	ON_COMMAND(ID_ADD_GROUPBOX, OnAddGroupbox)
	ON_COMMAND(ID_ADD_LISTBOX, OnAddListbox)
	ON_COMMAND(ID_ADD_RADIOBUTTON, OnAddRadiobutton)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_SHOW_GRID, OnShowGrid)
	ON_UPDATE_COMMAND_UI(ID_SHOW_GRID, OnUpdateShowGrid)
	ON_COMMAND(ID_SNAP_TO_GRID, OnSnapToGrid)
	ON_UPDATE_COMMAND_UI(ID_SNAP_TO_GRID, OnUpdateSnapToGrid)
	ON_COMMAND(ID_MARGINS, OnMargins)
	ON_UPDATE_COMMAND_UI(ID_MARGINS, OnUpdateMargins)
	ON_COMMAND(ID_RESTRAIN, OnRestraints)
	ON_UPDATE_COMMAND_UI(ID_RESTRAIN, OnUpdateRestraints)
	ON_UPDATE_COMMAND_UI(IDC_SELECT_ALL, OnUpdateSelectAll)
	ON_COMMAND(ID_BOTTOM, OnBottom)
	ON_UPDATE_COMMAND_UI(ID_BOTTOM, OnUpdateBottom)
	ON_COMMAND(ID_BOTTOM_ALIGN, OnBottomAlign)
	ON_UPDATE_COMMAND_UI(ID_BOTTOM_ALIGN, OnUpdateBottomAlign)
	ON_COMMAND(ID_DOWN, OnDown)
	ON_UPDATE_COMMAND_UI(ID_DOWN, OnUpdateDown)
	ON_COMMAND(ID_FRONT, OnFront)
	ON_UPDATE_COMMAND_UI(ID_FRONT, OnUpdateFront)
	ON_COMMAND(ID_LEFT_ALIGN, OnLeftAlign)
	ON_UPDATE_COMMAND_UI(ID_LEFT_ALIGN, OnUpdateLeftAlign)
	ON_COMMAND(ID_MAKE_SAME_SIZE, OnMakeSameSize)
	ON_UPDATE_COMMAND_UI(ID_MAKE_SAME_SIZE, OnUpdateMakeSameSize)
	ON_COMMAND(ID_RIGHT_ALIGN, OnRightAlign)
	ON_UPDATE_COMMAND_UI(ID_RIGHT_ALIGN, OnUpdateRightAlign)
	ON_COMMAND(ID_TOP_ALIGN, OnTopAlign)
	ON_UPDATE_COMMAND_UI(ID_TOP_ALIGN, OnUpdateTopAlign)
	ON_COMMAND(ID_UP, OnUp)
	ON_UPDATE_COMMAND_UI(ID_UP, OnUpdateUp)
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_UPDATE_COMMAND_UI(ID_EXPORT, OnUpdateExport)
	ON_COMMAND(ID_ADD_NONE, OnAddNone)
	ON_UPDATE_COMMAND_UI(ID_ADD_NONE, OnUpdateAddNone)
	ON_UPDATE_COMMAND_UI(ID_ADD_BUTTON, OnUpdateAddButton)
	ON_UPDATE_COMMAND_UI(ID_ADD_CHECKBOX, OnUpdateAddCheckbox)
	ON_UPDATE_COMMAND_UI(ID_ADD_COMBOBOX, OnUpdateAddCombobox)
	ON_UPDATE_COMMAND_UI(ID_ADD_EDIT, OnUpdateAddEdit)
	ON_UPDATE_COMMAND_UI(ID_ADD_GROUPBOX, OnUpdateAddGroupbox)
	ON_UPDATE_COMMAND_UI(ID_ADD_LISTBOX, OnUpdateAddListbox)
	ON_UPDATE_COMMAND_UI(ID_ADD_RADIOBUTTON, OnUpdateAddRadiobutton)
	ON_UPDATE_COMMAND_UI(ID_ADD_STATIC, OnUpdateAddStatic)
	ON_COMMAND(ID_BUTTON_MULTIDRAW, OnButtonMultidraw)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MULTIDRAW, OnUpdateButtonMultidraw)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorDemoView construction/destruction

CDialogEditorDemoView::CDialogEditorDemoView()
{

	// --- DiagramEditor ---
	m_screenResolutionX = 0;
	m_drawObject = DRAW_OBJECT_NONE;

	//m_bitmap = ::LoadBitmap( AfxGetResourceHandle(), MAKEINTRESOURCE( IDB_BITMAP1 ) );
}

CDialogEditorDemoView::~CDialogEditorDemoView()
{
	//::DeleteObject( m_bitmap );
}

BOOL CDialogEditorDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorDemoView drawing

void CDialogEditorDemoView::OnDraw(CDC* pDC)
{
	CDialogEditorDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// --- DiagramEditor ---
	// An example of how to handle print 
	// and print preview
	if( pDC->IsPrinting() )
	{
		// Print zoom is the difference between screen- 
		// and printer resolution.
		double zoom = pDC->GetDeviceCaps( LOGPIXELSX ) / m_screenResolutionX;

		CRect rect( 0,0, 
			( int )( ( double ) m_editor.GetVirtualSize().cx * zoom ), 
			( int )( ( double ) m_editor.GetVirtualSize().cy * zoom ) );

		m_editor.Print( pDC, rect, zoom );

	}

	// If we are not printing, we don't have do do 
	// anything special, the editor is a child window 
	// of the view and will be redrawn automatically.

}

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorDemoView printing

BOOL CDialogEditorDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CDialogEditorDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CDialogEditorDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorDemoView diagnostics

#ifdef _DEBUG
void CDialogEditorDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CDialogEditorDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDialogEditorDemoDoc* CDialogEditorDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDialogEditorDemoDoc)));
	return (CDialogEditorDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorDemoView message handlers

void CDialogEditorDemoView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// --- DiagramEditor ---
	if( !m_editor.m_hWnd )
	{

		// Creating the editor window
		CDialogEditorDemoDoc* pDoc = GetDocument();

		CRect rect;
		GetClientRect( rect );
		m_editor.Create( WS_CHILD | WS_VISIBLE, rect, this, pDoc->GetData() );
		
		m_editor.ShowGrid( FALSE );
		//m_editor.SetSnapToGrid( TRUE );
		//m_editor.ShowMargin( TRUE );
		//m_editor.SetRestraints( RESTRAINT_MARGIN );

		m_editor.SetGridColor( ::GetSysColor( COLOR_3DDKSHADOW ) );

		m_editor.SetVirtualSize( CSize( 640, 480) );
		m_editor.SetMargins(0,0,0,0);
		m_editor.SetResize( TRUE );
		m_editor.SetModified( FALSE );

		// We get the screen resolution, which we will use 
		// for scaling to printer. See also OnDraw.
		CClientDC dc( this );
		m_screenResolutionX = dc.GetDeviceCaps( LOGPIXELSX );
	}
	else
		m_editor.Clear();
	
}


void CDialogEditorDemoView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	// --- DiagramEditor ---
	// We resize the editor as well
	if( m_editor.m_hWnd )
		m_editor.MoveWindow(0,0,cx,cy);
	
}

BOOL CDialogEditorDemoView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	// --- DiagramEditor ---
	// To avoid flicker. The editor
	// covers all of the client area
	return TRUE;
}

///////////////////////////////////////////////////////////
// Copy/paste/undo commands and update handlers
//

void CDialogEditorDemoView::OnEditCopy() 
{
	m_editor.Copy();
}

void CDialogEditorDemoView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	m_editor.UpdateCopy( pCmdUI );	
}

void CDialogEditorDemoView::OnEditCut() 
{
	m_editor.Cut();
}

void CDialogEditorDemoView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	m_editor.UpdateCut( pCmdUI );	
}

void CDialogEditorDemoView::OnEditPaste() 
{
	m_editor.Paste();
}

void CDialogEditorDemoView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	m_editor.UpdatePaste( pCmdUI );
}

void CDialogEditorDemoView::OnEditUndo() 
{
	m_editor.Undo();
	
}

void CDialogEditorDemoView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	m_editor.UpdateUndo( pCmdUI );
}

void CDialogEditorDemoView::OnSelectAll() 
{
	m_editor.SelectAll();	
}

void CDialogEditorDemoView::OnUpdateSelectAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetObjectCount() );
}

///////////////////////////////////////////////////////////
// Misc dialogs
//

void CDialogEditorDemoView::OnProperties() 
{
	// Showing the property for the 
	// selected object, if any.
	m_editor.ShowProperties();
	
}

void CDialogEditorDemoView::OnUpdateProperties(CCmdUI* pCmdUI) 
{
	// We allowing showing of the property dialog
	// only if one and only one object is selected.
	BOOL res = FALSE;
	if( m_editor.GetSelectCount() == 1 )
		res = TRUE;
	pCmdUI->Enable( res );
}

void CDialogEditorDemoView::OnSettings() 
{
#if 0
	CGumpEditorDoc* pDoc = GfxGetGumpDocument(); ASSERT(pDoc);

	CDialogSettings	dlg;

	dlg.m_width = m_editor.GetVirtualSize().cx;
	dlg.m_height = m_editor.GetVirtualSize().cy;
	dlg.m_gridWidth = m_editor.GetGridSize().cx;
	dlg.m_gridHeight = m_editor.GetGridSize().cy;
	m_editor.GetMargins( dlg.m_marginLeft, dlg.m_marginTop, dlg.m_marginRight, dlg.m_marginBottom );
	
	dlg.m_strName = pDoc->GetName();
	dlg.m_iAlpha = pDoc->GetAlpha();
	dlg.m_iFlags = pDoc->GetFlags();
	pDoc->GetEventHandler(dlg.m_strEvClick, dlg.m_strEvClose, dlg.m_strEvMouseUp, dlg.m_strEvMouseDown, dlg.m_strEvKeyPressed);

	if( dlg.DoModal() == IDOK )
	{
		m_editor.SetGridSize( CSize( dlg.m_gridWidth, dlg.m_gridHeight ) );
		m_editor.SetVirtualSize( CSize( dlg.m_width, dlg.m_height ) );
		m_editor.SetMargins( dlg.m_marginLeft, dlg.m_marginTop, dlg.m_marginRight, dlg.m_marginBottom );
		
		pDoc->SetName(dlg.m_strName);
		pDoc->SetAlpha(dlg.m_iAlpha);
		pDoc->SetFlags(dlg.m_iFlags);
		pDoc->SetEventHandler(dlg.m_strEvClick, dlg.m_strEvClose, dlg.m_strEvMouseUp, dlg.m_strEvMouseDown, dlg.m_strEvKeyPressed);
		
		m_editor.RedrawWindow();
	}
#endif
}

///////////////////////////////////////////////////////////
// Adding controls. The functions creates an object of the 
// correct type and call StartDrawingObject on the editor.
//

void CDialogEditorDemoView::OnAddButton() 
{
	CDiagramButton* obj = new CDiagramButton;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_BUTTON;
}

void CDialogEditorDemoView::OnAddEdit() 
{
	CDiagramEdit* obj = new CDiagramEdit;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_EDIT;
}

void CDialogEditorDemoView::OnAddStatic() 
{
	CDiagramStatic* obj = new CDiagramStatic;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_STATIC;
}

void CDialogEditorDemoView::OnAddCheckbox() 
{
	CDiagramCheckbox* obj = new CDiagramCheckbox;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_CHECKBOX;
}

void CDialogEditorDemoView::OnAddCombobox() 
{
	CDiagramCombobox* obj = new CDiagramCombobox;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_COMBOBOX;
}

void CDialogEditorDemoView::OnAddGroupbox() 
{
	CDiagramGroupbox* obj = new CDiagramGroupbox;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_GROUPBOX;
}

void CDialogEditorDemoView::OnAddListbox() 
{
	CDiagramListbox* obj = new CDiagramListbox;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_LISTBOX;
}

void CDialogEditorDemoView::OnAddRadiobutton() 
{
	CDiagramRadiobutton* obj = new CDiagramRadiobutton;
	m_editor.StartDrawingObject( obj );
	m_drawObject = DRAW_OBJECT_RADIOBUTTON;
}

void CDialogEditorDemoView::OnAddNone() 
{
	m_editor.StartDrawingObject( NULL );
	m_drawObject = DRAW_OBJECT_NONE;
}

void CDialogEditorDemoView::OnUpdateAddButton(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_BUTTON );
}

void CDialogEditorDemoView::OnUpdateAddCheckbox(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_CHECKBOX );
}

void CDialogEditorDemoView::OnUpdateAddCombobox(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_COMBOBOX );
}

void CDialogEditorDemoView::OnUpdateAddEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_EDIT );
}

void CDialogEditorDemoView::OnUpdateAddGroupbox(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_GROUPBOX );
}

void CDialogEditorDemoView::OnUpdateAddListbox(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_LISTBOX );
}

void CDialogEditorDemoView::OnUpdateAddRadiobutton(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_RADIOBUTTON );
}

void CDialogEditorDemoView::OnUpdateAddStatic(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_STATIC );
}

void CDialogEditorDemoView::OnUpdateAddNone(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( !m_editor.IsDrawing() );
}

///////////////////////////////////////////////////////////
// Zoom handling
//

void CDialogEditorDemoView::OnZoomIn() 
{
	m_editor.SetZoom( m_editor.GetZoom() + m_editor.GetZoomFactor() * 10 );
}

void CDialogEditorDemoView::OnZoomOut() 
{
	m_editor.SetZoom( m_editor.GetZoom() - m_editor.GetZoomFactor() * 10 );
}
void CDialogEditorDemoView::On100perc() 
{
	m_editor.SetZoom( 1.0 );
}

void CDialogEditorDemoView::On110perc() 
{
	m_editor.SetZoom( 1.1 );
}

void CDialogEditorDemoView::On120perc() 
{
	m_editor.SetZoom( 1.2 );
}

void CDialogEditorDemoView::On130perc() 
{
	m_editor.SetZoom( 1.3 );
}

void CDialogEditorDemoView::On140perc() 
{
	m_editor.SetZoom( 1.4 );
}

void CDialogEditorDemoView::On150perc() 
{
	m_editor.SetZoom( 1.5 );
}

void CDialogEditorDemoView::On200perc() 
{
	m_editor.SetZoom( 2.0 );
}

void CDialogEditorDemoView::On25perc() 
{
	m_editor.SetZoom( .25 );
}

void CDialogEditorDemoView::On50perc() 
{
	m_editor.SetZoom( .5 );
}

void CDialogEditorDemoView::On60perc() 
{
	m_editor.SetZoom( .6 );
}

void CDialogEditorDemoView::On70perc() 
{
	m_editor.SetZoom( .7 );
}

void CDialogEditorDemoView::On80perc() 
{
	m_editor.SetZoom( .8 );
}

void CDialogEditorDemoView::On90perc() 
{
	m_editor.SetZoom( .9 );
}

///////////////////////////////////////////////////////////
// Grid, margin, multidraw and restraints handling
//

void CDialogEditorDemoView::OnShowGrid() 
{
	m_editor.ShowGrid( !m_editor.IsGridVisible() );
}

void CDialogEditorDemoView::OnUpdateShowGrid(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsGridVisible() );	
}

void CDialogEditorDemoView::OnSnapToGrid() 
{
	m_editor.SetSnapToGrid( !m_editor.GetSnapToGrid() );
	
}

void CDialogEditorDemoView::OnUpdateSnapToGrid(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.GetSnapToGrid() );	
}

void CDialogEditorDemoView::OnMargins() 
{
	m_editor.ShowMargin( !m_editor.IsMarginVisible() );
}

void CDialogEditorDemoView::OnUpdateMargins(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.IsMarginVisible() );
}

void CDialogEditorDemoView::OnRestraints() 
{
	if( m_editor.GetRestraints() == RESTRAINT_MARGIN )
		m_editor.SetRestraints( RESTRAINT_NONE );
	else
		m_editor.SetRestraints( RESTRAINT_MARGIN );
}

void CDialogEditorDemoView::OnUpdateRestraints(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.GetRestraints() == RESTRAINT_MARGIN );
	
}

void CDialogEditorDemoView::OnButtonMultidraw() 
{
	m_editor.SetMultidraw( !m_editor.GetMultidraw() );
	m_drawObject = DRAW_OBJECT_NONE;
}

void CDialogEditorDemoView::OnUpdateButtonMultidraw(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_editor.GetMultidraw() );
}

///////////////////////////////////////////////////////////
// Align menu
//

void CDialogEditorDemoView::OnLeftAlign() 
{
	m_editor.LeftAlignSelected();
}
void CDialogEditorDemoView::OnTopAlign() 
{
	m_editor.TopAlignSelected();
}
void CDialogEditorDemoView::OnRightAlign() 
{
	m_editor.RightAlignSelected();
}
void CDialogEditorDemoView::OnBottomAlign() 
{
	m_editor.BottomAlignSelected();
}

void CDialogEditorDemoView::OnUpdateLeftAlign(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() > 1 );
}
void CDialogEditorDemoView::OnUpdateTopAlign(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() > 1 );
}
void CDialogEditorDemoView::OnUpdateRightAlign(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() > 1 );
}
void CDialogEditorDemoView::OnUpdateBottomAlign(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() > 1 );
}

void CDialogEditorDemoView::OnMakeSameSize() 
{
	m_editor.MakeSameSizeSelected();
}
void CDialogEditorDemoView::OnUpdateMakeSameSize(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() > 1 );
}

void CDialogEditorDemoView::OnUp() 
{
	m_editor.Up();
}
void CDialogEditorDemoView::OnDown() 
{
	m_editor.Down();
}
void CDialogEditorDemoView::OnFront() 
{
	m_editor.Front();
}
void CDialogEditorDemoView::OnBottom() 
{
	m_editor.Bottom();
}

void CDialogEditorDemoView::OnUpdateUp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() == 1 );
}
void CDialogEditorDemoView::OnUpdateDown(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() == 1 );
}
void CDialogEditorDemoView::OnUpdateFront(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() == 1 );
}
void CDialogEditorDemoView::OnUpdateBottom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetSelectCount() == 1 );
}

///////////////////////////////////////////////////////////
// Exporting
//

void CDialogEditorDemoView::OnExport() 
{
	CDialogEditorDemoDoc* pDoc = GetDocument();
	pDoc->Export();	
}

void CDialogEditorDemoView::OnUpdateExport(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_editor.GetObjectCount() );
}

