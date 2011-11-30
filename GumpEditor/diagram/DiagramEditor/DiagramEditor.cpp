/* ==========================================================================
	CDiagramEditor

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-29

	Purpose :		CDiagramEditor encapsulates a basic vector editor geared 
					towards diagrams rather than graphics. It supports 
					virtual screens both smaller and larger than the window, 
					snap to grid, copy/paste, unlimited undo, zoom etc. It 
					can be added to both dialogbox- and doc/view apps due to 
					the use of a separate container for data.

					Normally, this class need only to be inherited from when 
					the drawing functionality is insufficient - many of the 
					settings can be modified through member accessors.

	Description :	The class derives from CWnd, and handles Windows messages 
					relevant to the operations. 

	Usage :			

	Changes :		8/4  2004	SendMessageToObjects sets TRUE flag
								sending message only to selected objects.
					9/4  2004	Made some message mapping functions
								virtual to allow enhancements in 
								derived classes.
					9/4  2004	Added accessors for states to allow
								access for derived classes
					28/4 2004	Set interact mode to MODE_NONE after
								ctrl+clicking item (to not move lines)
					28/4 2004	Setting scrollbar positions to zero
								in Clear
					39/4 2004	Sending this window as a parent to the 
								property dialog. This is to be able to 
								redraw the editor properly after changes 
								to the dialog.
					30/4 2004	Changed c-style casts to static_cast
					30/4 2004	Remove ShowProperties-const-ness.
   ========================================================================*/

#include "stdafx.h"
#include "DiagramEditor.h"
#include "Tokenizer.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning( disable : 4706 )

/////////////////////////////////////////////////////////////////////////////
// CDiagramEditor construction/destruction/initialization

CDiagramEditor::CDiagramEditor()
/* ============================================================
	Function :		CDiagramEditor::CDiagramEditor
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	m_bkgndCol = ::GetSysColor( COLOR_WINDOW );
	m_nonClientBkgndCol = ::GetSysColor( COLOR_3DSHADOW );

	m_gridCol = RGB( 192, 192, 192 );
	m_grid = TRUE;
	m_gridStyle = PS_SOLID;
	m_gridSize = CSize( 8, 8 );

	m_snap = FALSE;
	m_bgResize = FALSE;
	m_bgResizeZone = 10;
	m_bgResizeSelected = FALSE;
	m_markerSize = CSize( 6, 6 );

	m_drawObj = NULL;
	m_objs = NULL;
	m_multiSelObj = NULL;
	m_internalData = NULL;

	m_restraint = RESTRAINT_NONE;

	m_leftMargin = 8;
	m_topMargin = 8;
	m_rightMargin = 8;
	m_bottomMargin = 16;
	m_margin = FALSE;
	m_marginColor = RGB( 128, 128, 255 );

	m_popupMenu = NULL;
	m_multiDraw = FALSE;

	m_zoomFactor = 0.01;
	m_zoomMax = 10.0;
	m_zoomMin = 0.0;

	m_keyInterface = KEY_ALL;

	Clear();

}

CDiagramEditor::~CDiagramEditor()
/* ============================================================
	Function :		CDiagramEditor::~CDiagramEditor
	Description :	destructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	delete m_drawObj;
	delete m_internalData;
	delete m_popupMenu;

}

void CDiagramEditor::Clear()
/* ============================================================
	Function :		CDiagramEditor::Clear
	Description :	Clears internal run-time variables.
					
	Return :		void
	Parameters :	none

	Usage :			Call to reset internal states.

   ============================================================*/
{

	// Clearing internal states and vars
	m_selectionRect.SetRectEmpty();
	m_interactMode = MODE_NONE;
	m_zoom = 1.0;
	m_bgResizeSelected = FALSE;
	m_deltaPoint = CSize( 0, 0 );
	m_multiSelObj = NULL;
	m_drawing = FALSE;

	delete m_drawObj;
	m_drawObj = NULL;

	if( m_hWnd )
	{

		SetupScrollbars();

		SCROLLINFO si;
		si.cbSize = sizeof( SCROLLINFO );
		si.fMask = SIF_POS;
		si.nPos = 0;

		SetScrollInfo( SB_HORZ, &si );
		SetScrollInfo( SB_VERT, &si );

		RedrawWindow();

	}

}

BOOL CDiagramEditor::Create( DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, CDiagramEntityContainer* data )
/* ============================================================
	Function :		CDiagramEditor::Create
	Description :	Creates a CDiagramEditor window.
					
	Return :		BOOL							-	TRUE if success
	Parameters :	DWORD dwStyle					-	Window styles for 
														the editor
					const RECT &rect				-	Window rectangle
					CWnd *pParentWnd				-	Parent of the 
														editor
					CDiagramEntityContainer* data	-	Pointer to data. 
														Might be NULL.
					
	Usage :			If data is NULL, a CDiagramEntityContainer 
					will be created internally, and the instance 
					of the editor will be responsible for the 
					container deletion.

   ============================================================*/
{
	if( data == NULL )
	{
		m_internalData = new CDiagramEntityContainer;
		SetInternalDiagramEntityContainer( m_internalData );
	}
	else
		SetDiagramEntityContainer( data );

	BOOL res = CWnd::Create( NULL, NULL, dwStyle, rect, pParentWnd, NULL );
	CSize virtualSize;
	if( GetVirtualSize().cx == 0 &&  GetVirtualSize().cy == 0 )
		virtualSize = CSize( rect.right - rect.left, rect.bottom - rect.top );
	else
		virtualSize = GetVirtualSize();

	SetInternalVirtualSize( virtualSize );

	return res;

}

void CDiagramEditor::New()
/* ============================================================
	Function :		CDiagramEditor::New
	Description :	Clears the current "page" and creates a 
					new one.
					
	Return :		void
	Parameters :	none

	Usage :			Call to start a new drawing.

   ============================================================*/
{

	SetRedraw( FALSE );
	Clear();
	m_objs->Clear();
	SetRedraw( TRUE );
	RedrawWindow();

}

void CDiagramEditor::SetDiagramEntityContainer( CDiagramEntityContainer* objs )
/* ============================================================
	Function :		CDiagramEditor::SetDiagramEntityContainer
	Description :	Sets the data container for the editor.
					
	Return :		void
	Parameters :	CDiagramEntityContainer* objs	-	the data 
														container
					
	Usage :			If this function is used, the caller is 
					responsible for deleting the container. Can 
					be called before or after Create.

   ============================================================*/
{

	if( m_internalData )
		delete m_internalData;

	SetInternalDiagramEntityContainer ( objs );

}

void CDiagramEditor::SetInternalDiagramEntityContainer( CDiagramEntityContainer* objs )
/* ============================================================
	Function :		CDiagramEditor::SetInternalDiagramEntityContainer
	Description :	Sets the internal data container pointer.
					
	Return :		void
	Parameters :	CDiagramEntityContainer* objs	-	A pointer to the 
														container to set.
					
	Usage :			Internal function.

   ============================================================*/
{

	m_objs = objs;

}

CDiagramEntityContainer* CDiagramEditor::GetDiagramEntityContainer() const
/* ============================================================
	Function :		CDiagramEditor::GetDiagramEntityContainer
	Description :	Returns a pointer to the data container.
					
	Return :		CDiagramEntityContainer*	-	The current data 
													container (might 
													be NULL).
	Parameters :	none

	Usage :			If modifications are made to the contents of 
					the container, SetModified must be called as 
					appropriate. If visual changes are expected, 
					RedrawWindow must be called for the editor.

   ============================================================*/
{

	return m_objs;

}

BEGIN_MESSAGE_MAP( CDiagramEditor, CWnd )
	//{{AFX_MSG_MAP( CDiagramEditor )
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP

	ON_COMMAND_RANGE( CMD_START, CMD_END, OnObjectCommand )

	ON_COMMAND( ID_EDIT_CUT, OnEditCut )
	ON_COMMAND( ID_EDIT_COPY, OnEditCopy )
	ON_COMMAND( ID_EDIT_PASTE, OnEditPaste )

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiagramEditor painting

void CDiagramEditor::OnPaint() 
/* ============================================================
	Function :		CDiagramEditor::OnPaint
	Description :	Draws the screen. A memory CDC is created 
					and drawn to.
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Do not call from code.

   ============================================================*/
{

	CPaintDC outputdc( this );

	// Getting coordinate data
	CRect rect;
	GetClientRect( &rect );

	SCROLLINFO sih;
	sih.cbSize = sizeof( SCROLLINFO );
	sih.fMask = SIF_POS;
	SCROLLINFO siv;
	siv.cbSize = sizeof( SCROLLINFO );
	siv.fMask = SIF_POS;
	if( !GetScrollInfo( SB_HORZ, &sih ) )
		sih.nPos = 0;
	if( !GetScrollInfo( SB_VERT, &siv ) )
		siv.nPos = 0;

	CRect totalRect;
	int virtwidth = round( ( double ) GetVirtualSize().cx * GetZoom() ) + 1;
	int virtheight = round( ( double ) GetVirtualSize().cy * GetZoom() ) + 1;
	totalRect.SetRect( 0, 0, virtwidth, virtheight );

	// Creating memory CDC
	CDC dc;
	dc.CreateCompatibleDC( &outputdc );
	CBitmap bmp;
	bmp.CreateCompatibleBitmap( &outputdc, rect.right, rect.bottom );
	CBitmap* oldbmp = dc.SelectObject( &bmp );

	// Painting
	EraseBackground( &dc, rect );

	dc.SetWindowOrg( sih.nPos, siv.nPos );

	Draw( &dc, totalRect );

	// Blit the memory CDC to screen
	outputdc.BitBlt( 0, 0, rect.right, rect.bottom, &dc, sih.nPos, siv.nPos, SRCCOPY );
	dc.SelectObject( oldbmp );

}

void CDiagramEditor::Draw( CDC* dc, CRect rect ) const
/* ============================================================
	Function :		CDiagramEditor::Draw
	Description :	Calls a series of (virtual) functions to 
					draw to dc. rect is the total rect to draw 
					to.
					
	Return :		void
	Parameters :	CDC* dc		-	The CDC to draw to.
					CRect rect	-	The complete rect (including 
									non-visible areas)
					
	Usage :			Should not normally be called from user code. 
					Can be called to draw the complete window. 
					Can be overriden to change drawing order.

   ============================================================*/
{

	double zoom = GetZoom();

	DrawBackground( dc, rect, zoom );

	if( m_grid )
		DrawGrid( dc, rect, zoom );

	if( m_margin )
		DrawMargins( dc, rect, zoom );

	DrawObjects( dc, zoom );

	if( m_bgResize && m_bgResizeSelected )
		DrawSelectionMarkers( dc );

}

void CDiagramEditor::Print( CDC* dc, CRect rect, double zoom ) 
/* ============================================================
	Function :		CDiagramEditor::Print
	Description :	Prints the editor to printer or print a 
					preview.
					
	Return :		void
	Parameters :	CDC* dc		-	Printer- or preview CDC to 
									draw to.
					CRect rect	-	Total rect of editor.
					double zoom	-	Desired zoom value.
					
	Usage :			The function should be called to print the 
					editor to a printer or print preview. This 
					is because the zoom will most likely be set 
					to something different from the editor itself. 
					A good zoom factor can be calculated from 
					the difference between the screen- and 
					printer resolution:

					printerDC.GetDeviceCaps( LOGPIXELSX ) / screenDC.GetDeviceCaps( LOGPIXELSX )

					The grid, margin and selection markers are 
					not printed.
					Can be overriden to change drawing order, or 
					add drawing of the grid etc.
   ============================================================*/
{

	UnselectAll();
	DrawBackground( dc, rect, zoom );
	DrawObjects( dc, zoom );

}

BOOL CDiagramEditor::OnEraseBkgnd( CDC* ) 
/* ============================================================
	Function :		CDiagramEditor::OnEraseBkgnd
	Description :	Handles the WM_ERASEBKGND message. Handled 
					to avoid flicker - the editor is completely 
					redrawn in OnPaint.
					
	Return :		BOOL	-	
	Parameters :	CDC*	-	
					
	Usage :			Called from MFC. Do not call from code.

   ============================================================*/
{

	return TRUE;

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEditor painting virtuals

void CDiagramEditor::EraseBackground( CDC* dc, CRect rect ) const
/* ============================================================
	Function :		CDiagramEditor::EraseBackground
	Description :	Draws the non-client background
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to.
					CRect rect	-	Total rect to draw to.
					
	Usage :			Virtual. Can be overridden in a derived class 
					to erase the non-client area.

   ============================================================*/
{

	dc->FillSolidRect( rect, m_nonClientBkgndCol );

}

void CDiagramEditor::DrawBackground( CDC* dc, CRect rect, double /*zoom*/) const
/* ============================================================
	Function :		CDiagramEditor::DrawBackground
	Description :	Draws the virtual background
					
	Return :		void
	Parameters :	CDC* dc			-	CDC to draw to.
					CRect rect		-	Total rect to draw to.
					double (zoom)	-	Current zoom level. 
										Coordinates can be 
										multiplied with this 
										value to get scaled.
					
	Usage :			Virtual. Can be overridden in a derived 
					class to draw the virtual paper area.

   ============================================================*/
{

	dc->FillSolidRect( &rect, m_bkgndCol );

}

void CDiagramEditor::DrawGrid( CDC* dc, CRect rect, double zoom ) const
/* ============================================================
	Function :		CDiagramEditor::DrawGrid
	Description :	Draws the grid
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to.
					CRect rect	-	Total rect to draw to.
					double zoom	-	Current zoom level. 
									Coordinates can be 
									multiplied with this value 
									to get scaled.
					
	Usage :			Virtual. Can be overridden in a derived 
					class to draw the grid. Will not be called 
					if the grid is not visible.

   ============================================================*/
{

	COLORREF gridcol = GetGridColor();
	int gridstyle = GetGridPenStyle();

	CPen pen;
	pen.CreatePen( gridstyle, 1, gridcol );

	dc->SelectObject( &pen );

	// To avoid accumulating rounding errors, we don't
	// precalculate the grid size for the given zoom...

	int width = rect.Width();
	int height = rect.Height();

	int stepx = GetVirtualSize().cx / GetGridSize().cx;
	int stepy = GetVirtualSize().cy / GetGridSize().cy;

	// ...instead we calculate the position of each line.
	for( int x = 0 ; x <= stepx ; x++ )
	{
		dc->MoveTo( round( ( double ) ( GetGridSize().cx * x ) * zoom ), 0 );
		dc->LineTo( round( ( double ) ( GetGridSize().cx * x ) * zoom ), height );
	}

	for( int y = 0; y <= stepy ; y++ )
	{
		dc->MoveTo( 0, round( ( double ) ( GetGridSize().cy * y ) * zoom ) );
		dc->LineTo( width, round( ( double ) ( GetGridSize().cy * y ) * zoom ) );
	}

	dc->SelectStockObject( BLACK_PEN );

}

void CDiagramEditor::DrawMargins( CDC* dc, CRect rect, double zoom ) const
/* ============================================================
	Function :		CDiagramEditor::DrawMargins
	Description :	Draws the margins.
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to.
					CRect rect	-	Total rect to draw to.
					double zoom	-	Current zoom level. 
									Coordinates can be 
									multiplied with this value 
									to get scaled.
					
	Usage :			Virtual. Can be overridden in a derived 
					class to draw the margins. Will not be 
					called if margins are not visible.

   ============================================================*/
{

	CPen pen;
	pen.CreatePen( PS_SOLID, 0, m_marginColor );
	dc->SelectObject( &pen );

	CPoint leftTop( rect.left + round( ( double ) m_leftMargin * zoom ), rect.top + round( ( double ) m_topMargin * zoom ) );
	CPoint leftBottom( rect.left + round( ( double ) m_leftMargin * zoom ), rect.bottom - round( ( double ) m_bottomMargin * zoom ) - 1 );
	CPoint rightTop( rect.right - round( ( double ) m_rightMargin * zoom ) - 1, rect.top + round( ( double ) m_topMargin * zoom ) );
	CPoint rightBottom( rect.right - round( ( double ) m_rightMargin * zoom ) - 1, rect.bottom - round( ( double ) m_bottomMargin * zoom ) - 1 );

	if( m_leftMargin )
	{
		dc->MoveTo( leftTop );
		dc->LineTo( leftBottom );
	}

	if( m_rightMargin )
	{
		dc->MoveTo( rightTop );
		dc->LineTo( rightBottom );
	}
	if( m_topMargin )
	{
		dc->MoveTo( leftTop );
		dc->LineTo( rightTop );
	}
	if( m_bottomMargin )
	{
		dc->MoveTo( leftBottom );
		dc->LineTo( rightBottom );
	}

	dc->SelectStockObject( BLACK_PEN );


}

void CDiagramEditor::DrawObjects( CDC* dc, double zoom ) const
/* ============================================================
	Function :		CDiagramEditor::DrawObjects
	Description :	Draws the object.
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to.
					double zoom	-	Current zoom level. 
									Coordinates can be 
									multiplied with this value 
									to get scaled.
					
	Usage :			Virtual. Can be overridden in a derived 
					class to draw the data objects.

   ============================================================*/
{

	if( m_objs )
	{
		int count = 0;
		CDiagramEntity* obj;
		while( ( obj = m_objs->GetAt( count++ ) ))
			if (obj->IsVisible()) obj->DrawObject( dc, zoom );
	}

}

void CDiagramEditor::DrawSelectionMarkers( CDC* dc ) const
/* ============================================================
	Function :		CDiagramEditor::DrawSelectionMarkers
	Description :	Draws the selection markers.
					
	Return :		void
	Parameters :	CDC* dc	-	The CDC to draw to.
					
	Usage :			Virtual. Can be overridden in a derived 
					class to draw the selection markers in 
					another way than the default black 
					rectangles. The selection rects are 
					displayed if the editor has background 
					resizing enabled and the user clicks in 
					the resize area around the virtual page 
					border. Selection markers are displayed 
					to allow resizing of the virtual page 
					with the mouse. 

   ============================================================*/
{

	// Draw selection markers
	CRect rectSelect;

	dc->SelectStockObject( BLACK_BRUSH );
	rectSelect = GetSelectionMarkerRect( DEHT_TOPLEFT );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_TOPMIDDLE );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_TOPRIGHT );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMLEFT );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMMIDDLE );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_RIGHTMIDDLE );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_LEFTMIDDLE );
	dc->Rectangle( rectSelect );

}

CRect CDiagramEditor::GetSelectionMarkerRect( UINT marker ) const
/* ============================================================
	Function :		CDiagramEditor::GetSelectionMarkerRect
	Description :	Returns the selection rect for marker
					
	Return :		CRect		-	The rect of the selection 
									marker.
	Parameters :	UINT marker	-	The marker to get the rect 
									for (DEHT_-constants 
									defined in DiagramEntity.h)
					
	Usage :			Virtual. Can be overridden in a derived 
					class to change the selection rects for 
					the virtual page. The selection rects are 
					displayed if the editor has background 
					resizing enabled and the user clicks in 
					the resize area around the virtual page 
					border. Selection markers are displayed 
					to allow resizing of the virtual page 
					with the mouse.

   ============================================================*/
{

	CRect rect( 0, 0, round( ( double ) GetVirtualSize().cx * GetZoom() ), round( ( double ) GetVirtualSize().cy * GetZoom() ) );
	CRect rectMarker;
	int horz = m_markerSize.cx / 2;
	int vert = m_markerSize.cy / 2;

	switch( marker )
	{
		case DEHT_TOPLEFT:
			rectMarker.SetRect( rect.left - horz, 
								rect.top - vert, 
								rect.left + horz, 
								rect.top + vert );
		break;

		case DEHT_TOPMIDDLE:
			rectMarker.SetRect( rect.left + ( rect.Width() / 2 ) - horz, 
								rect.top - vert, 
								rect.left + ( rect.Width() / 2 ) + horz, 
								rect.top + vert );
		break;

		case DEHT_TOPRIGHT:
			rectMarker.SetRect( rect.right - horz, 
								rect.top - vert, 
								rect.right + horz, 
								rect.top + vert );
		break;

		case DEHT_BOTTOMLEFT:
			rectMarker.SetRect( rect.left - horz, 
								rect.bottom - vert, 
								rect.left + horz, 
								rect.bottom + vert );
		break;

		case DEHT_BOTTOMMIDDLE:
			rectMarker.SetRect( rect.left + ( rect.Width() / 2 ) - horz, 
								rect.bottom - vert, 
								rect.left + ( rect.Width() / 2 ) + horz, 
								rect.bottom + vert );
		break;

		case DEHT_BOTTOMRIGHT:
			rectMarker.SetRect( rect.right - horz, 
								rect.bottom - vert, 
								rect.right + horz, 
								rect.bottom + vert );
		break;

		case DEHT_LEFTMIDDLE:
			rectMarker.SetRect( rect.left - horz, 
								rect.top + ( rect.Height() / 2 ) - vert, 
								rect.left + horz, 
								rect.top + ( rect.Height() / 2 ) + vert );
		break;

		case DEHT_RIGHTMIDDLE:
			rectMarker.SetRect( rect.right - horz, 
								rect.top + ( rect.Height() / 2 ) - vert, 
								rect.right + horz, 
								rect.top + ( rect.Height() / 2 ) + vert );
		break;
	}

	return rectMarker;

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEditor property accessors

void CDiagramEditor::SetVirtualSize( const CSize& size )
/* ============================================================
	Function :		CDiagramEditor::SetVirtualSize
	Description :	Sets the size of the virtual paper.
					
	Return :		void
	Parameters :	const CSize& size	-	New virtual size.
					
	Usage :			The virtual page of the editor is the 
					"paper" where the user can draw. This 
					function marks the data as changed.

   ============================================================*/
{

	ASSERT( m_objs );
	SetInternalVirtualSize( size );
	m_objs->SetModified( TRUE );

}

void CDiagramEditor::SetInternalVirtualSize( const CSize& size )
/* ============================================================
	Function :		CDiagramEditor::SetInternalVirtualSize
	Description :	Changes the virtual page size without 
					setting the data as modified.
					
	Return :		void
	Parameters :	const CSize& size	-	New virtual size.
					
	Usage :			Internal function. 

   ============================================================*/
{
	if( m_objs && size != GetVirtualSize() )
	{

		m_objs->SetVirtualSize( size );

		SetupScrollbars();
		if( m_hWnd )
			RedrawWindow();

	}

}

CSize CDiagramEditor::GetVirtualSize() const
/* ============================================================
	Function :		CDiagramEditor::GetVirtualSize
	Description :	Returns the virtual size of the editor.
					
	Return :		CSize	-	The current virtual size.
	Parameters :	none

	Usage :			The virtual page of the editor is the 
					"paper" where the user can draw. 

   ============================================================*/
{

	CRect rect( 0, 0, 0, 0 );
	if( m_hWnd )
		GetClientRect( &rect );
	CSize size( CSize( 0, 0 ) );

	if( m_objs )
		size = m_objs->GetVirtualSize();

	if( size.cx == 0 )
		size.cx = rect.right;
	if( size.cy == 0 )
		size.cy = rect.bottom;

	return size;

}

void CDiagramEditor::SetBackgroundColor( COLORREF col )
/* ============================================================
	Function :		CDiagramEditor::SetBackgroundColor
	Description :	Sets the background color.
					
	Return :		void
	Parameters :	COLORREF col	-	New background color 
										to set.
					
	Usage :			The background is the virtual area of the 
					editor (might both be smaller or bigger 
					than the client rect).

   ============================================================*/
{

	m_bkgndCol = col;
	if( m_hWnd )
		RedrawWindow();

}

void CDiagramEditor::SetNonClientColor( COLORREF col )
/* ============================================================
	Function :		CDiagramEditor::SetNonClientColor
	Description :	Sets the non-client area color.
					
	Return :		void
	Parameters :	COLORREF col	-	New non-client area 
										color.
					
	Usage :			The non-client color is the color of the 
					area outside the virtual page.

   ============================================================*/
{

	m_nonClientBkgndCol = col;
	if( m_hWnd )
		RedrawWindow();

}

void CDiagramEditor::ShowGrid( BOOL grid )
/* ============================================================
	Function :		CDiagramEditor::ShowGrid
	Description :	Sets grid visibility.
					
	Return :		void
	Parameters :	BOOL grid	-	TRUE to show the grid, FALSE 
									to hide.
					
	Usage :			If the grid is visible, it will be drawn 
					using the grid pen style and color. The 
					grid lines will not be scaled with the 
					zoom (the space between them will of 
					course be, however)

   ============================================================*/
{

	m_grid = grid;
	if( m_hWnd )
		RedrawWindow();

}

BOOL CDiagramEditor::IsGridVisible() const
/* ============================================================
	Function :		CDiagramEditor::IsGridVisible
	Description :	Returns the visibility state of the grid.
					
	Return :		BOOL	-	TRUE if grid is visible.
	Parameters :	none

	Usage :			If the grid is visible, it will be drawn 
					using the grid pen style and color. The 
					grid lines will not be scaled with the 
					zoom (the space between them will of 
					course be, however)

   ============================================================*/
{

	return m_grid;

}

void CDiagramEditor::SetGridColor( COLORREF col )
/* ============================================================
	Function :		CDiagramEditor::SetGridColor
	Description :	Sets a new grid pen color.
					
	Return :		void
	Parameters :	COLORREF col	-	New grip pen color.
					
	Usage :			If the grid is visible, it will be drawn 
					using the grid pen style and color. The 
					grid lines will not be scaled with the 
					zoom (the space between them will of 
					course be, however)

   ============================================================*/
{

	m_gridCol = col;
	if( m_hWnd )
		RedrawWindow();

}

COLORREF CDiagramEditor::GetGridColor() const
/* ============================================================
	Function :		CDiagramEditor::GetGridColor
	Description :	Returns the current grid pen color.
					
	Return :		COLORREF	-	The current grid color.
	Parameters :	none

	Usage :			If the grid is visible, it will be drawn 
					using the grid pen style and color. The 
					grid lines will not be scaled with the 
					zoom (the space between them will of 
					course be, however)

   ============================================================*/
{

	return m_gridCol;

}

void CDiagramEditor::SetGridSize( CSize size )
/* ============================================================
	Function :		CDiagramEditor::SetGridSize
	Description :	Sets a new grid size.
					
	Return :		void
	Parameters :	CSize size	-	The new grid size.
					
	Usage :			If snap to grid is on, added, moved and 
					resized objects snap to the closest grid 
					position. If the background is resized, it 
					will also snap to the grid.

   ============================================================*/
{

	m_gridSize = size;
	if( m_hWnd )
		RedrawWindow();

}

CSize CDiagramEditor::GetGridSize() const
/* ============================================================
	Function :		CDiagramEditor::GetGridSize
	Description :	Gets the current grid size.
					
	Return :		CSize	-	The current grid size.
	Parameters :	none

	Usage :			If snap to grid is on, added, moved and 
					resized objects snap to the closest grid 
					position. If the background is resized, it 
					will also snap to the grid.

   ============================================================*/
{

	return m_gridSize;

}

void CDiagramEditor::SetGridPenStyle( int style )
/* ============================================================
	Function :		CDiagramEditor::SetGridPenStyle
	Description :	Sets the new grid pen style.
					
	Return :		void
	Parameters :	int style	-	The new pen style, one of 
									the style constants for 
									CreatePen.
					
	Usage :			The grid (if visible) is drawn with a pen 
					created with the grid pen style. The grid 
					lines will not be scaled with the zoom 
					(the space between them will of course be, 
					however)

   ============================================================*/
{

	m_gridStyle = style;
	if( m_hWnd )
		RedrawWindow();

}

int CDiagramEditor::GetGridPenStyle() const
/* ============================================================
	Function :		CDiagramEditor::GetGridPenStyle
	Description :	Returns the pen style for the grid.
					
	Return :		int		-	The pen style, one of the style 
								constants for CreatePen.
	Parameters :	none

	Usage :			The grid (if visible) is drawn with a pen 
					created with the grid pen style. The grid 
					lines will not be scaled with the zoom 
					(the space between them will of course be, 
					however)

   ============================================================*/
{

	return m_gridStyle;

}

void CDiagramEditor::SetSnapToGrid( BOOL snap )
/* ============================================================
	Function :		CDiagramEditor::SetSnapToGrid
	Description :	Enable/disable snap to grid.
					
	Return :		void
	Parameters :	BOOL snap	-	TRUE if objects should 
									snap to grid.
					
	Usage :			If snap to grid is on, added, moved and 
					resized objects snap to the closest grid 
					position. If the background is resized, it 
					will also snap to the grid.

   ============================================================*/
{

	m_snap = snap;

}

BOOL CDiagramEditor::GetSnapToGrid() const
/* ============================================================
	Function :		CDiagramEditor::GetSnapToGrid
	Description :	Gets the state of the snap-to-grid state.
					
	Return :		BOOL	-	TRUE if snap is on.
	Parameters :	none

	Usage :			If snap to grid is on, added, moved and 
					resized objects snap to the closest grid 
					position. If the background is resized, it 
					will also snap to the grid.

   ============================================================*/
{

	return m_snap;

}

void CDiagramEditor::SetResize( BOOL bgresize )
/* ============================================================
	Function :		CDiagramEditor::SetResize
	Description :	Enables/disables background resizing.
					
	Return :		void
	Parameters :	BOOL bgresize	-	TRUE if background 
										resizing is enabled.
					
	Usage :			If background resizing is enabled and the 
					user clicks between the resize zone value 
					and the virtual edge, the background is 
					displayed as selected.

   ============================================================*/
{

	m_bgResize = bgresize;

}

BOOL CDiagramEditor::GetResize() const
/* ============================================================
	Function :		CDiagramEditor::GetResize
	Description :	Returns the state of the background resize
					flag.
					
	Return :		BOOL	-	TRUE if background resizing is 
								enabled.
	Parameters :	none

	Usage :			If background resizing is enabled and the 
					user clicks between the resize zone value 
					and the virtual edge, the background is 
					displayed as selected.

   ============================================================*/
{

	return m_bgResize;

}

void CDiagramEditor::SetResizeZone( int bgresizezone )
/* ============================================================
	Function :		CDiagramEditor::SetResizeZone
	Description :	Sets a new resize zone for the editor.
					
	Return :		void
	Parameters :	int bgresizezone	-	New resize width.
					
	Usage :			If background resizing is enabled and the 
					user clicks between the resize zone value 
					and the virtual edge, the background is 
					displayed as selected.

   ============================================================*/
{

	m_bgResizeZone = bgresizezone;

}

int CDiagramEditor::GetResizeZone() const
/* ============================================================
	Function :		CDiagramEditor::GetResizeZone
	Description :	Returns the current resize zone.
					
	Return :		int		-	The resize zone in pixels.
	Parameters :	none

	Usage :			If background resizing is enabled and the 
					user clicks between the resize zone value 
					and the virtual edge, the background is 
					displayed as selected.

   ============================================================*/
{

	return m_bgResizeZone;

}

void CDiagramEditor::SetMargins( int left, int top, int right, int bottom )
/* ============================================================
	Function :		CDiagramEditor::SetMargins
	Description :	Sets margins for the virtual screen.
					
	Return :		void
	Parameters :	int left	-	New left margin.
					int top		-	New top margin.
					int right	-	New right margin.
					int bottom	-	New bottom margin.
					
	Usage :			Call to set new margins for the editor.

   ============================================================*/
{

	m_leftMargin = left;
	m_topMargin = top;
	m_rightMargin = right;
	m_bottomMargin = bottom;

}

void CDiagramEditor::GetMargins( int& left, int& top, int& right, int& bottom ) const
/* ============================================================
	Function :		CDiagramEditor::GetMargins
	Description :	Return the current margin.
					
	Return :		void
	Parameters :	int& left	-	Current left margin.
					int& top	-	Current top margin.
					int& right	-	Current right margin.
					int& bottom	-	Current bottom margin.
					
	Usage :			Call to get the margins of the editor.

   ============================================================*/
{

	left = m_leftMargin;
	top = m_topMargin;
	right = m_rightMargin;
	bottom = m_bottomMargin;

}

void CDiagramEditor::SetMarginColor( COLORREF marginColor )
/* ============================================================
	Function :		CDiagramEditor::SetMarginColor
	Description :	Set current margin colors.
					
	Return :		void
	Parameters :	COLORREF marginColor	-	The new color.
					
	Usage :			Call to set the margin color.

   ============================================================*/
{

	m_marginColor = marginColor;

	if( m_hWnd )
		RedrawWindow();

}

COLORREF CDiagramEditor::GetMarginColor() const
/* ============================================================
	Function :		CDiagramEditor::GetMarginColor
	Description :	Returns the current margin colors.
					
	Return :		COLORREF	-	Margin colors.
	Parameters :	none

	Usage :			Call to get the margin color.

   ============================================================*/
{

	return m_marginColor;

}

void CDiagramEditor::ShowMargin( BOOL show )
/* ============================================================
	Function :		CDiagramEditor::ShowMargin
	Description :	Show/hide margins.
					
	Return :		void
	Parameters :	BOOL show	-	TRUE to show margins, FALSE 
									to hide.
					
	Usage :			Call to show/hide the margins.

   ============================================================*/
{

	m_margin = show;

	if( m_hWnd )
		RedrawWindow();

}

BOOL CDiagramEditor::IsMarginVisible() const
/* ============================================================
	Function :		CDiagramEditor::IsMarginVisible
	Description :	Returns the visibility state of the 
					margins.
					
	Return :		BOOL	-	TRUE if the margins are 
								visible.
	Parameters :	none

	Usage :			Call to se if the margin visibility flag is 
					on.

   ============================================================*/
{

	return m_margin;

}

void CDiagramEditor::SetRestraints( int restraint )
/* ============================================================
	Function :		CDiagramEditor::SetRestraints
	Description :	Sets current restraint mode
					
	Return :		void
	Parameters :	BOOL restraint	-	restraint mode, see Usage 
										below
					
	Usage :			The restraint mode can be one of the 
					following ( defined in DialogEditor.h):

					RESTRAINT_NONE		No restraints.
					RESTRAINT_VIRTUAL	No objects can be moved 
										outside the virtual 
										rectangle.
					RESTRAINT_MARGIN	No objects can be moved 
										outside the background 
										margins.

   ============================================================*/
{

	m_restraint = restraint;

}

int CDiagramEditor::GetRestraints() const
/* ============================================================
	Function :		CDiagramEditor::GetRestraints
	Description :	Returns the current restraint mode.
					
	Return :		int	-	The current restraint mode. See 
							Usage below

	Parameters :	none

	Usage :			The restraint mode can be one of the 
					following ( defined in DialogEditor.h):

					RESTRAINT_NONE		No restraints.
					RESTRAINT_VIRTUAL	No objects can be moved 
										outside the virtual 
										rectangle.
					RESTRAINT_MARGIN	No objects can be moved 
										outside the background 
										margins.

   ============================================================*/
{

	return m_restraint;

}

BOOL CDiagramEditor::GetMultidraw() const
/* ============================================================
	Function :		CDiagramEditor::GetMultidraw
	Description :	Returns the multi draw state
					
	Return :		BOOL	-	TRUE if multi draw is set
	Parameters :	none

	Usage :			Multi draw means that the user can continue 
					to add controls after one is placed.

   ============================================================*/
{
	return m_multiDraw;
}

void CDiagramEditor::SetMultidraw( BOOL multidraw )
/* ============================================================
	Function :		CDiagramEditor::SetMultidraw
	Description :	Sets the multi draw state.
					
	Return :		void
	Parameters :	BOOL multidraw	-	New multi draw state
					
	Usage :			Multi draw means that the user can continue 
					to add controls after one is placed.

   ============================================================*/
{
	m_multiDraw = multidraw;
	m_drawing = FALSE;
	m_interactMode = MODE_NONE;
	delete m_drawObj;
	m_drawObj = NULL;
}

void CDiagramEditor::SetZoom( double zoom )
/* ============================================================
	Function :		CDiagramEditor::SetZoom
	Description :	Set the current zoom level.
					
	Return :		void
	Parameters :	double zoom	-	New zoom level. 1.0 means 
					no zoom.
					
	Usage :			If the virtual screen should be zoomed in 
					to double size, the zoom value should be 
					2.0, for example.

   ============================================================*/
{

	m_zoom = max( m_zoomMin, zoom );
	m_zoom = min( m_zoomMax, zoom );
	SetupScrollbars();
	if( m_hWnd )
		RedrawWindow();

}

double CDiagramEditor::GetZoom() const
/* ============================================================
	Function :		CDiagramEditor::GetZoom
	Description :	Returns the current zoom level of the editor. 
					
	Return :		double	-	The current zoom level. 1.0 is 
								no zoom.
	Parameters :	none

	Usage :			The zoom level is expressed as a double, 
					.25 means that all zoomed coordinates 
					should be multiplied with .25, for example.

   ============================================================*/
{

	return m_zoom;

}

void CDiagramEditor::SetZoomFactor( double zoomFactor )
/* ============================================================
	Function :		CDiagramEditor::SetZoomFactor
	Description :	Sets a new zoom factor
					
	Return :		void
	Parameters :	double zoomFactor	-	The new zoom factor
					
	Usage :			The zoom factor is used when the + and - 
					keys are enabled to zoom. The default value 
					is 0.01.

   ============================================================*/
{
	m_zoomFactor = zoomFactor;
}

double CDiagramEditor::GetZoomFactor() const
/* ============================================================
	Function :		CDiagramEditor::GetZoomFactor
	Description :	Returns the current zoom factor
					
	Return :		double	-	The zoom factor
	Parameters :	none

	Usage :			The zoom factor is used when the + and - 
					keys are enabled to zoom. The default value 
					is 0.01.

   ============================================================*/
{
	return m_zoomFactor;
}

double CDiagramEditor::GetZoomMax() const
/* ============================================================
	Function :		CDiagramEditor::GetZoomMax
	Description :	Returns the maximum allowed zoom level
					
	Return :		double	-	The maximum zoom level
	Parameters :	none

	Usage :			The zoom level will never go below or above 
					the min and max zoom levels

   ============================================================*/
{

	return m_zoomMax;

}

double CDiagramEditor::GetZoomMin() const
/* ============================================================
	Function :		CDiagramEditor::GetZoomMin
	Description :	Returns the minimum allowed zoom level
					
	Return :		double	-	The minimum zoom level
	Parameters :	none

	Usage :			The zoom level will never go below or above 
					the min and max zoom levels

   ============================================================*/
{

	return m_zoomMin;

}

void CDiagramEditor::SetZoomMax( double zoommax )
/* ============================================================
	Function :		CDiagramEditor::SetZoomMax
	Description :	Sets the maximum zoom level
					
	Return :		void
	Parameters :	double zoommax	-	The new max zoom
					
	Usage :			The zoom level will never go below or above 
					the min and max zoom levels

   ============================================================*/
{

	m_zoomMax = max( 0, zoommax );

}

void CDiagramEditor::SetZoomMin( double zoommin )
/* ============================================================
	Function :		CDiagramEditor::SetZoomMin
	Description :	Sets the mim zoom level
					
	Return :		void
	Parameters :	double zoommin	-	New minimum zoom level
					
	Usage :			The zoom level will never go below or above 
					the min and max zoom levels

   ============================================================*/
{

	m_zoomMin = max( 0, zoommin );

}

void CDiagramEditor::SetModified( BOOL dirty )
/* ============================================================
	Function :		CDiagramEditor::SetModified
	Description :	Set the modified-state of the data for the 
					document.
					
	Return :		void
	Parameters :	BOOL dirty	-	TRUE if the data should be 
									set as modified.
					
	Usage :			Call to mark the data as modified/saved.

   ============================================================*/
{

	if( m_objs )
		m_objs->SetModified( dirty );

}

BOOL CDiagramEditor::IsModified() const
/* ============================================================
	Function :		CDiagramEditor::IsModified
	Description :	Returns TRUE if the data in the editor is 
					modified.
					
	Return :		BOOL	-	TRUE if modified.
	Parameters :	none

	Usage :			Call to check if the data is modified.

   ============================================================*/
{

	BOOL res = FALSE;
	if( m_objs )
		res = m_objs->IsModified();

	return res;

}

CSize CDiagramEditor::GetMarkerSize() const
/* ============================================================
	Function :		CDiagramEditor::GetMarkerSize
	Description :	Gets the current selection marker size for 
					the editor background.
					
	Return :		CSize	-	The current size.
	Parameters :	none

	Usage :			Call to get the selection marker size.

   ============================================================*/
{

	return m_markerSize;

}

void CDiagramEditor::SetMarkerSize( CSize markerSize )
/* ============================================================
	Function :		CDiagramEditor::SetMarkerSize
	Description :	Sets the size of the selection markers for 
					the editor background.
					
	Return :		void
	Parameters :	CSize markerSize	-	New size of 
											markers.
					
	Usage :			Call to set the selection marker size.
					Will not set the selection marker size for 
					objects.

   ============================================================*/
{

	m_markerSize = markerSize;

}

UINT CDiagramEditor::GetKeyboardInterface() const
/* ============================================================
	Function :		CDiagramEditor::GetKeyboardInterface
	Description :	Returns the flags for the keyboard 
					interface
					
	Return :		UINT	-	The current flags
	Parameters :	none

	Usage :			The keyboard interface decides what keys 
					should be reacted on. The following flags 
					can be used:

						KEY_ARROW		Will handle arrow keys. 
										If shift is pressed, the 
										selected objects will be 
										resized, moved otherwise.
						KEY_PGUPDOWN	Will handle Pg up & pg 
										down. If Ctrl is pressed, 
										the selected object will 
										move to the top or the 
										bottom of the z-order, 
										one step up or down 
										otherwise.
						KEY_DELETE		Will handle delete key. 
										The selected object(s) 
										will be deleted, put into 
										the container clipboard 
										if Ctrl is pressed.
						KEY_ESCAPE		Will handle escape key. 
										If multi-draw mode, no 
										object type is selected 
										for drawing.
						KEY_INSERT		Will handle insert key. 
										The selected object will 
										be copied if Ctrl is 
										pressed, duplicated 
										otherwise.
						KEY_PLUSMINUS	Will handle the plus- 
										and minus key. 
										Will zoom in or out.
						KEY_CTRL		Will handle Ctrl+A,Z,X,
										C,V and Enter keys. 
										A = Select all
										Z = Undo
										X = Cut
										C = Copy
										V = Paste
										Enter = Show property 
										dialog for the selected 
										object.
					KEY_ALL sets all flags. KEY_NONE no flags.

   ============================================================*/
{

	return m_keyInterface;

}

void CDiagramEditor::SetKeyboardInterface( int keyInterface )
/* ============================================================
	Function :		CDiagramEditor::SetKeyboardInterface
	Description :	Sets the keyboard interface flag.
					
	Return :		void
	Parameters :	int keyInterface	-	The new flags
					
	Usage :			Call to set the keys the editor will handle.
					See GetKeyboardInterface.

   ============================================================*/
{

	m_keyInterface = keyInterface;

}

void CDiagramEditor::SetPopupMenu( CDiagramMenu* popupmenu )
/* ============================================================
	Function :		CDiagramEditor::SetPopupMenu
	Description :	Sets the editor popup menu to popupmenu.
					
	Return :		void
	Parameters :	CDiagramMenu* popupmenu	-	The popup menu 
												to use.
					
	Usage :			Call to set the popup menu for the editor.
					Note that the editor owns the popup menu.

   ============================================================*/
{
	if( m_popupMenu )
		delete m_popupMenu;

	m_popupMenu = popupmenu;

}

CDiagramMenu* CDiagramEditor::GetPopupMenu() const
/* ============================================================
	Function :		CDiagramEditor::GetPopupMenu
	Description :	Returns a pointer to the editor popup menu.
					
	Return :		CDiagramMenu*	-	The popup menu. 
										Might be NULL.
	Parameters :	none

	Usage :			Call to get a pointer to the editor popup 
					menu. Note that the editor owns the menu.

   ============================================================*/
{

	return m_popupMenu;

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEditor mouse handling

void CDiagramEditor::OnLButtonDown( UINT nFlags, CPoint point ) 
/* ============================================================
	Function :		CDiagramEditor::OnLButtonDown
	Description :	Handles the WM_LBUTTONDOWN message. We 
					select/deselecte objects and set internal 
					modes as appropriate.
					
	Return :		void
	Parameters :	UINT nFlags		-	Key-down flag
					CPoint point	-	Mouse position
					
	Usage :			Called from MFC. Do not call from code.

   ============================================================*/
{

	// Declarations
	int count = 0;
	CDiagramEntity* obj;
	CPoint virtpoint( point );
	ScreenToVirtual( virtpoint );

	// Setting up
	SetFocus();
	SetCapture();

	// Resetting modes
	if( m_interactMode != MODE_DRAWING )
	{
		m_interactMode = MODE_NONE;
		m_subMode = DEHT_NONE;
	}

	m_bgResizeSelected = FALSE;
	m_multiSelObj = NULL;

	// If we click on an already selected object, 
	// and have more than one object selected,
	// we want to move all selected objects
	if( m_objs && GetSelectCount() > 1 && !( nFlags & MK_CONTROL ) )
	{
		while( ( obj = m_objs->GetAt( count++ ) ) )
		{
			if( obj->GetHitCode( virtpoint ) == DEHT_BODY && obj->IsSelected() && !obj->IsFreezed())
			{
				m_interactMode = MODE_MOVING;
				CRect rect = obj->GetRect();

				// We might need to use a denormalized
				// rect, as lines will not be normalized.
				BOOL useright = FALSE;
				BOOL usebottom = FALSE;
				if( rect.left > rect.right )
					useright = TRUE;
				if( rect.top > rect.bottom )
					usebottom = TRUE;

				VirtualToScreen( rect );

				int startx = rect.left;
				if( useright )
					startx = rect.right;
				int starty = rect.top;
				if( usebottom )
					starty = rect.bottom;

				// Anchor object
				m_multiSelObj = obj;

				// Offset from top-left corner
				// in the anchor object.
				m_deltaPoint = point - CPoint( startx, starty );

			}
		}

		if( m_interactMode == MODE_MOVING )
		{
			// We have clicked in one of many selected objects.
			// Set cursor and snapshot for Undo.
			::SetCursor( m_multiSelObj->GetCursor( DEHT_BODY ) );
			m_objs->Snapshot();
		}

	}

	if( m_objs && m_interactMode == MODE_DRAWING )
	{
		// The user wants to draw an object.
		// We add it to the data container and sets the appropriate mode

		m_objs->Snapshot();

		UnselectAll();

		CDiagramEntity* newobj = m_drawObj->Clone();

		// If snap-to-grid is on, we must
		// update the desired position
		if( m_snap )
		{

			virtpoint.x = SnapX( virtpoint.x );
			virtpoint.y = SnapY( virtpoint.y );

		}

		newobj->SetRect( CRect( virtpoint.x, virtpoint.y, virtpoint.x , virtpoint.y ) );
		//newobj->Select( TRUE );
		SelectObject(newobj,TRUE);
		

		// Add the object to the container
		m_objs->Add( newobj );
		

		// Set modes
		m_interactMode = MODE_RESIZING;
		m_subMode = DEHT_BOTTOMRIGHT;

	}

	if( m_objs && m_interactMode == MODE_NONE )
	{

		// Clearing states
		// If Ctrl is not held down, we
		// clear all selections
		if( !( nFlags & MK_CONTROL ) )
			UnselectAll();

		count = GetObjectCount();
		BOOL goon = TRUE;

		// We check if we click on any object. If that is 
		// the case, we return on what part of the object 
		// we clicked.
		while( goon && ( obj = m_objs->GetAt( --count ) ))
		{
			if (!obj->IsSelectable()) continue;
			int hitCode = obj->GetHitCode( virtpoint );
			if( hitCode != DEHT_NONE )
			{
				goon = FALSE;
				if( !( nFlags & MK_CONTROL ) )
				{
					// We unselect all again, as we might
					// have overlapping objects
					UnselectAll();
					//obj->Select( TRUE );
					SelectObject(obj,TRUE);
				}
				else
				{
					// We toggle the selection of the 
					// object if Ctrl is pressed
					SelectObject(obj, !obj->IsSelected());
				}

				// We set the appropriate mode, either
				// moving or resizing depending on where
				// the click hit.
				if( hitCode == DEHT_BODY && !( nFlags & MK_CONTROL ) )
				{
					::SetCursor( obj->GetCursor( DEHT_BODY ) );
					m_interactMode = MODE_MOVING;

					CRect rect = obj->GetRect();

					// We might need to use a denormalized
					// rect, as lines will not be normalized.
					BOOL useright = FALSE;
					BOOL usebottom = FALSE;
					if( rect.left > rect.right )
						useright = TRUE;
					if( rect.top > rect.bottom )
						usebottom = TRUE;

					VirtualToScreen( rect );

					int startx = rect.left;
					if( useright )
						startx = rect.right;
					int starty = rect.top;
					if( usebottom )
						starty = rect.bottom;

					m_deltaPoint = point - CPoint( startx, starty );

				}
				else if( !( nFlags & MK_CONTROL ) )
				{
					m_interactMode = MODE_RESIZING;
					m_subMode = hitCode;
				}
				else
					m_interactMode = MODE_NONE;
			}
		}

		// Save to undo-stack...
		if( m_interactMode != MODE_NONE )
			m_objs->Snapshot();

		// If no objects were selected, we assume either 
		// rubberbanding or background resize
		if( !IsAnyObjectSelected() )
		{
			if( m_bgResize )
			{
				// If we allow background resizing, we test if
				// the click was inside the resize zone.
				if( ( virtpoint.x >= GetVirtualSize().cx - m_bgResizeZone &&
					virtpoint.x <= GetVirtualSize().cx )
					||
					( virtpoint.y >= GetVirtualSize().cy - m_bgResizeZone && 
					virtpoint.y <= GetVirtualSize().cy )
					 )
				{
					m_bgResizeSelected = TRUE;
					int hitCode = GetHitCode( virtpoint );
					if( hitCode != DEHT_NONE && hitCode != DEHT_BODY )
					{
						// It was - set the appropriate mode
						m_interactMode = MODE_BGRESIZING;
						m_subMode = hitCode;
						m_objs->Snapshot();
					}

					RedrawWindow();
				}
			}

			if( m_interactMode == MODE_NONE )
			{
				// If nothing else is set, we assume
				// rubberbanding.
				m_selectionRect.left = m_selectionRect.right = point.x;
				m_selectionRect.top = m_selectionRect.bottom = point.y;
				m_interactMode = MODE_RUBBERBANDING;
			}
		}
	}

	// Cleaning up and redrawing as necessary.
	RemoveUnselectedPropertyDialogs();
	if( m_interactMode != MODE_NONE )
		RedrawWindow();

	CWnd::OnLButtonDown( nFlags, point );

}

void CDiagramEditor::OnMouseMove( UINT nFlags, CPoint point )
/* ============================================================
	Function :		CDiagramEditor::OnMouseMove
	Description :	Handles the WM_MOUSEMOVE message. We handle 
					moving or resizing of objects, or resizing 
					of the virtual screen.
					
	Return :		void
	Parameters :	UINT nFlags		-	Key-down flags
					CPoint point	-	Mouse position
					
	Usage :			Called from MFC. Do not call from code.

   ============================================================*/
{

	if( m_interactMode != MODE_NONE )
	{

		CClientDC	dc( this );
		CRect clientRect;
		GetClientRect( &clientRect );
		CPoint virtpoint( point );
		ScreenToVirtual( virtpoint );

		if( m_interactMode == MODE_BGRESIZING )
		{
			if( m_snap )
			{
				virtpoint.x = SnapX( virtpoint.x );
				virtpoint.y = SnapY( virtpoint.y );
			}

			virtpoint.x = max( virtpoint.x, 1 );
			virtpoint.y = max( virtpoint.y, 1 );
			int xsize = GetVirtualSize().cx;
			int ysize = GetVirtualSize().cy;

			if( m_subMode == DEHT_TOPRIGHT )
			{
				xsize = virtpoint.x;
			}
			else if( m_subMode == DEHT_RIGHTMIDDLE )
			{
				xsize = virtpoint.x;
			}
			else if( m_subMode == DEHT_BOTTOMRIGHT )
			{
				xsize = virtpoint.x;
				ysize = virtpoint.y;
			}
			else if( m_subMode == DEHT_BOTTOMMIDDLE )
			{
				ysize = virtpoint.y;
			}
			else if( m_subMode == DEHT_BOTTOMLEFT )
			{
				ysize = virtpoint.y;
			}

			if( m_restraint == RESTRAINT_VIRTUAL )
			{
				CSize size = GetContainingSize();
				xsize = max( size.cx, xsize );
				ysize = max( size.cy, ysize );
			}
			else if( m_restraint == RESTRAINT_MARGIN )
			{
				CSize size = GetContainingSize();
				xsize = max( size.cx + m_rightMargin, xsize );
				ysize = max( size.cy + m_bottomMargin, ysize );
				xsize = max( m_leftMargin + m_rightMargin, xsize );
				ysize = max( m_topMargin + m_bottomMargin, ysize );
			}

			SetVirtualSize( CSize( xsize, ysize ) );
			ScrollPoint( point );

		}
		if( m_interactMode == MODE_RESIZING )
		{

			if( m_snap )
			{

				virtpoint.x = SnapX( virtpoint.x );
				virtpoint.y = SnapY( virtpoint.y );

			}

			// If we are moving, we will update one or 
			// two sides of the object.
			double ypos = ( double ) virtpoint.y;
			double xpos = ( double ) virtpoint.x;

			CDiagramEntity* obj = GetSelectedObject();
			if( obj && obj->IsSelectable())
			{
				CSize minimum = obj->GetMinimumSize();
				CSize maximum = obj->GetMaximumSize();
				if( m_subMode == DEHT_BOTTOMMIDDLE )
				{

					// Constraints
					if( minimum.cy != -1 && ypos - obj->GetTop() < minimum.cy )
						ypos = obj->GetTop() + minimum.cy;
					if( maximum.cy != -1 && ypos - obj->GetTop() > maximum.cy )
						ypos = obj->GetTop() + maximum.cy;

					AdjustForRestraints( xpos, ypos );
					obj->SetRect( obj->GetLeft(), obj->GetTop(), obj->GetRight(), ypos );

				}
				else if( m_subMode == DEHT_TOPMIDDLE )
				{

					// Constraints
					if( minimum.cy != -1 && obj->GetBottom() - ypos < minimum.cy )
						ypos = obj->GetBottom() - minimum.cy;
					if( maximum.cy != -1 && obj->GetBottom() - ypos > maximum.cy )
						ypos = obj->GetBottom() - maximum.cy;

					AdjustForRestraints( xpos, ypos );
					obj->SetRect( obj->GetLeft(), ypos, obj->GetRight(), obj->GetBottom() );

				}
				else if( m_subMode == DEHT_LEFTMIDDLE )
				{

					// Constraints
					if( minimum.cx != -1 && obj->GetRight() - xpos < minimum.cx )
						xpos = obj->GetRight() - minimum.cx;
					if( maximum.cx != -1 && obj->GetRight() - xpos > maximum.cx )
						xpos = obj->GetRight() - maximum.cx;

					AdjustForRestraints( xpos, ypos );
					obj->SetRect( xpos, obj->GetTop(), obj->GetRight(), obj->GetBottom() );

				}
				else if( m_subMode == DEHT_RIGHTMIDDLE )
				{

					// Constraints
					if( minimum.cx != -1 && xpos - obj->GetLeft() < minimum.cx )
						xpos = obj->GetLeft() + minimum.cx;
					if( maximum.cx != -1 && xpos - obj->GetLeft() > maximum.cx )
						xpos = obj->GetLeft() + maximum.cx;

					AdjustForRestraints( xpos, ypos );
					obj->SetRect( obj->GetLeft(), obj->GetTop(), xpos, obj->GetBottom() );

				}
				else if( m_subMode == DEHT_TOPLEFT )
				{

					// Constraints
					if( minimum.cy != -1 && obj->GetBottom() - ypos < minimum.cy )
						ypos = obj->GetBottom() - minimum.cy;
					if( minimum.cx != -1 && obj->GetRight() - xpos < minimum.cx )
						xpos = obj->GetRight() - minimum.cx;
					if( maximum.cy != -1 && obj->GetBottom() - ypos > maximum.cy )
						ypos = obj->GetBottom() - maximum.cy;
					if( maximum.cx != -1 && obj->GetRight() - xpos > maximum.cx )
						xpos = obj->GetRight() - maximum.cx;

					AdjustForRestraints( xpos, ypos );
					obj->SetRect( xpos, ypos, obj->GetRight(), obj->GetBottom() );

				}
				else if( m_subMode == DEHT_TOPRIGHT )
				{

					// Constraints
					if( minimum.cy != -1 && obj->GetBottom() - ypos < minimum.cy )
						ypos = obj->GetBottom() - minimum.cy;
					if( minimum.cx != -1 && xpos - obj->GetLeft() < minimum.cx )
						xpos = obj->GetLeft() + minimum.cx;
					if( maximum.cy != -1 && obj->GetBottom() - ypos > maximum.cy )
						ypos = obj->GetBottom() - maximum.cy;
					if( maximum.cx != -1 && xpos - obj->GetLeft() > maximum.cx )
						xpos = obj->GetLeft() + maximum.cx;

					AdjustForRestraints( xpos, ypos );
					obj->SetRect( obj->GetLeft(), ypos, xpos, obj->GetBottom() );

				}
				else if( m_subMode == DEHT_BOTTOMLEFT )
				{

					// Constraints
					if( minimum.cy != -1 && ypos - obj->GetTop() < minimum.cy )
						ypos = obj->GetTop() + minimum.cy;
					if( minimum.cx != -1 && obj->GetRight() - xpos < minimum.cx )
						xpos = obj->GetRight() - minimum.cx;
					if( maximum.cy != -1 && ypos - obj->GetTop() > maximum.cy )
						ypos = obj->GetTop() + maximum.cy;
					if( maximum.cx != -1 && obj->GetRight() - xpos > maximum.cx )
						xpos = obj->GetRight() - maximum.cx;

					AdjustForRestraints( xpos, ypos );
					obj->SetRect( xpos, obj->GetTop(), obj->GetRight(), ypos );

				}
				else if( m_subMode == DEHT_BOTTOMRIGHT )
				{

					// Constraints
					if( minimum.cy != -1 && ypos - obj->GetTop() < minimum.cy )
						ypos = obj->GetTop() + minimum.cy;
					if( minimum.cx != -1 && xpos - obj->GetLeft() < minimum.cx )
						xpos = obj->GetLeft() + minimum.cx;
					if( maximum.cy != -1 && ypos - obj->GetTop() > maximum.cy )
						ypos = obj->GetTop() + maximum.cy;
					if( maximum.cx != -1 && xpos - obj->GetLeft() > maximum.cx )
						xpos = obj->GetLeft() + maximum.cx;

					AdjustForRestraints( xpos, ypos );
					obj->SetRect( obj->GetLeft(), obj->GetTop(), xpos, ypos );

				}

			}

			// Scroll if we are outside any edge
			ScrollPoint( point );
			RedrawWindow();
		}
		else if( m_objs && m_interactMode == MODE_MOVING )
		{

			// If we move, we just update the positions
			// of all the objects.
			double offsetx = round( ( double ) m_deltaPoint.x / GetZoom() );
			double offsety = round( ( double ) m_deltaPoint.y / GetZoom() );
			int count = 0;
			CDiagramEntity* obj;
			double width;
			double height;

			double left;
			double top;

			if( m_multiSelObj)
			{
				left = virtpoint.x - offsetx;
				top = virtpoint.y - offsety;
				if( m_snap )
				{
					left = SnapX( ( int ) left );
					top = SnapY( ( int ) top );
				}

				offsetx = left - m_multiSelObj->GetLeft();
				offsety = top - m_multiSelObj->GetTop();

				InsideRestraints( offsetx, offsety );

				m_multiSelObj->SetRect( m_multiSelObj->GetLeft() + offsetx, m_multiSelObj->GetTop() + offsety, 
					m_multiSelObj->GetRight() + offsetx, m_multiSelObj->GetBottom() + offsety );

				while( ( obj = m_objs->GetAt( count++ ) ) )
					if( obj->IsSelected() && obj != m_multiSelObj && obj->IsSelectable())
						obj->MoveRect( offsetx, offsety );
			}
			else
			{
				obj = GetSelectedObject();
				if( obj && obj->IsSelectable())
				{

					width = obj->GetRight() - obj->GetLeft();
					height = obj->GetBottom() - obj->GetTop();

					left = virtpoint.x - offsetx;
					top = virtpoint.y - offsety;

					if( m_snap )
					{
						left = SnapX( ( int ) left );
						top = SnapY( ( int ) top );
					}

					double right = left + width;
					double bottom = top + height;

					AdjustForRestraints( left, top, right, bottom );
					obj->SetRect( left, top, right , bottom );

				}
			}

			// Scroll if we are outside any edge
			CPoint outside = ScrollPoint( point );
			RedrawWindow();

		}
		else if( m_interactMode == MODE_RUBBERBANDING )
		{

			// We are selecting objects
			CRect rect( m_selectionRect );

			// Erase old selection rect
			if( m_selectionRect.left != m_selectionRect.right || m_selectionRect.top != m_selectionRect.bottom )
			{
				rect.NormalizeRect();
				dc.DrawFocusRect( rect );
			}

			// Scroll if we are outside any edge
			CPoint outside = ScrollPoint( point );
			m_selectionRect.OffsetRect( -outside.x, -outside.y );

			// Update and draw the selection rect
			m_selectionRect.right = point.x;
			m_selectionRect.bottom = point.y;
			rect = m_selectionRect;
			rect.NormalizeRect();

			dc.DrawFocusRect( rect );

		}

	}

	CWnd::OnMouseMove( nFlags, point );

}

void CDiagramEditor::OnLButtonUp( UINT nFlags, CPoint point )
/* ============================================================
	Function :		CDiagramEditor::OnLButtonUp
	Description :	Handles the WM_LBUTTONUP message. Mainly, 
					we snap to grid if appropriate.
					
	Return :		void
	Parameters :	UINT nFlags		-	not interested.
					CPoint point	-	The position of the mouse.
					
	Usage :			Called from MFC. Do not call from code.

   ============================================================*/
{

	// Reset modes
	ReleaseCapture();

	int count = 0;
	CDiagramEntity* obj;

	if( m_objs && m_interactMode == MODE_MOVING )
	{

		// If we move objects, and snap to grid is on
		// we snap here
		if( m_snap )
		{
			while( ( obj = m_objs->GetAt( count++ ) ) )
			{
				if( obj->IsSelected() )
				{
					CRect rect = obj->GetRect();
					int height = rect.Height();
					int width = rect.Width();

					// Find closest snap-points
					rect.left = SnapX( rect.left );
					rect.top = SnapY( rect.top );

					rect.right = rect.left + width;
					rect.bottom = rect.top + height;

					obj->SetRect( rect );
				}
			}
		}

		m_objs->SetModified( TRUE );

	}
	else if( m_objs && m_interactMode == MODE_RESIZING )
	{
		// If we resize objects, and snap to grid is on
		// we snap here

		if( m_snap )
		{
			while( ( obj = m_objs->GetAt( count++ ) ) )
			{
				if( obj->IsSelected() )
				{
					CRect rect = obj->GetRect();
					CRect newrect = rect;

					// Find closest snap-points
					rect.left = SnapX( rect.left );
					rect.top = SnapY( rect.top );
					rect.right = SnapX( rect.right );
					rect.bottom = SnapY ( rect.bottom );

					// Updating rect depending on resize-type.
					switch( m_subMode )
					{
						case DEHT_BOTTOMMIDDLE:
							newrect.bottom = rect.bottom;
						break;
						case DEHT_TOPMIDDLE:
							newrect.top = rect.top;
						break;
						case DEHT_LEFTMIDDLE:
							newrect.left = rect.left;
						break;
						case DEHT_RIGHTMIDDLE:
							newrect.right = rect.right;
						break;
						case DEHT_TOPLEFT:
							newrect.top = rect.top;
							newrect.left = rect.left;
						break;
						case DEHT_TOPRIGHT:
							newrect.top = rect.top;
							newrect.right = rect.right;
						break;
						case DEHT_BOTTOMLEFT:
							newrect.bottom = rect.bottom;
							newrect.left = rect.left;
						break;
						case DEHT_BOTTOMRIGHT:
							newrect.bottom = rect.bottom;
							newrect.right = rect.right;
						break;
					}

					obj->SetRect( newrect );
				}
			}
		}
		m_objs->SetModified( TRUE );

	}
	else if( m_objs && m_interactMode == MODE_RUBBERBANDING )
	{

		// Remove all former selections
		UnselectAll();

		// We convert the selection rect to virtual coordinates, 
		// and make sure that the rect contains at least some 
		// width and height ( IntersectRect will not work otherwise )
		CRect rect( m_selectionRect );
		ScreenToVirtual( rect );
		if( rect.TopLeft() == rect.BottomRight() )
			rect.InflateRect( 1, 1 );

		// We loop all objects, checking if we got any overlap.
		count = 0;
		while( ( obj = m_objs->GetAt( count++ ) ) )
		{
			if( obj->BodyInRect( rect ) )
				//obj->Select( TRUE );
				SelectObject(obj,TRUE);
		}

	}

	// Redraw and reset modes
	RedrawWindow();
	if( m_multiDraw && m_drawing && m_drawObj )
		m_interactMode = MODE_DRAWING;
	else
	{
		m_drawing = FALSE;
		m_interactMode = MODE_NONE;
	}
	m_subMode = DEHT_NONE;
	CWnd::OnLButtonUp( nFlags, point );

}

void CDiagramEditor::OnLButtonDblClk( UINT nFlags, CPoint point ) 
/* ============================================================
	Function :		CDiagramEditor::OnLButtonDblClk
	Description :	Handles the WM_LBUTTONDBLCLK message. We 
					show a property dialog if appropriate.
					
	Return :		void
	Parameters :	UINT nFlags		-	not interested
					CPoint point	-	not interested
					
	Usage :			Called from MFC. Do not call from code.

   ============================================================*/
{

	ShowProperties();
	CWnd::OnLButtonDblClk( nFlags, point );

}

void CDiagramEditor::OnRButtonDown( UINT nFlags, CPoint point ) 
/* ============================================================
	Function :		CDiagramEditor::OnRButtonDown
	Description :	Handles the WM_RBUTTONDOWN message. 
					
	Return :		void
	Parameters :	UINT nFlags		-	not interested
					CPoint point	-	not interested
					
	Usage :			Called from MFC. Do not call from code.

   ============================================================*/
{

	if( m_objs )
	{
		int count = 0;
		CDiagramEntity* obj;
		CPoint virtpoint( point );
		ScreenToVirtual( virtpoint );

		// We check if we click on any object body. If that is 
		// the case, we select the object
		while( ( obj = m_objs->GetAt( count++ ) ) )
		{
			if( obj->GetHitCode( virtpoint ) == DEHT_BODY )
			{
				if( !obj->IsSelected() )
				{
					UnselectAll();
					//obj->Select( TRUE );
					SelectObject(obj, TRUE);
				}
			}
		}

		RedrawWindow();
	}

	m_interactMode = MODE_NONE;
	m_drawing = FALSE;

	CWnd::OnRButtonDown( nFlags, point );

}

void CDiagramEditor::OnRButtonUp( UINT nFlags, CPoint point ) 
/* ============================================================
	Function :		CDiagramEditor::OnRButtonUp
	Description :	Handles the WM_RBUTTONUP message. We show 
					popup menues as appropriate.
					
	Return :		void
	Parameters :	UINT nFlags		-	not interested
					CPoint point	-	not interested
					
	Usage :			Called from MFC. Do not call from code.

   ============================================================*/
{

	CPoint screen( point );
	CPoint virtpoint( point );

	ClientToScreen( &screen );
	ScreenToVirtual( virtpoint );

	if( GetSelectCount() == 1 )
	{
		CDiagramEntity* obj = GetSelectedObject();
		if( obj->GetHitCode( virtpoint ) == DEHT_BODY )
			obj->ShowPopup( screen, this );
		else
			ShowPopup( screen );
	}
	else
		ShowPopup( screen );

	CWnd::OnRButtonUp( nFlags, point );

}

UINT CDiagramEditor::OnGetDlgCode() 
/* ============================================================
	Function :		CDiagramEditor::OnGetDlgCode
	Description :	Handles the WM_GETDLGCODE message. We 
					return the keys we want.
					
	Return :		UINT	-	DLGC_WANTALLKEYS, we want all 
								keys.
	Parameters :	none

	Usage :			Called from MFC. Do not call from code.

   ============================================================*/
{

	return DLGC_WANTALLKEYS;

}

void CDiagramEditor::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags ) 
/* ============================================================
	Function :		CDiagramEditor::OnKeyDown
	Description :	Handles the WM_KEYDOWN message. We provide 
					a simple keyboard interface.
					
	Return :		void
	Parameters :	UINT nChar		-	Character pressed
					UINT nRepCnt	-	not interested
					UINT nFlags		-	not interested
					
	Usage :			Called from MFC. Do not call from code.

   ============================================================*/
{
	if( m_keyInterface != KEY_NONE )
	{
		// Clearing the flags
		GetAsyncKeyState( VK_SHIFT );
		GetAsyncKeyState( VK_CONTROL );

		if( m_objs )
		{

			int count = 0;
			CDiagramEntity* obj;

			double offsetstepx = 1.0 / GetZoom();
			double offsetstepy = 1.0 / GetZoom();
			if( m_snap )
			{
				offsetstepx = ( double ) m_gridSize.cx / GetZoom();
				offsetstepy = ( double ) m_gridSize.cy / GetZoom();
			}

			double offsetx = 0.0;
			double offsety = 0.0;

			BOOL move = FALSE;
			BOOL resize = FALSE;
			BOOL redraw = TRUE;
			
			if( ( m_keyInterface & KEY_ARROW ) && ( nChar == VK_DOWN ) )
			{
				offsety=offsetstepy;
				if( GetAsyncKeyState( VK_SHIFT ) )
					resize = TRUE;
				else
					move = TRUE;
			}
			else if( ( m_keyInterface & KEY_ARROW ) && ( nChar == VK_UP ) )
			{
				offsety=-offsetstepy;
				if( GetAsyncKeyState( VK_SHIFT ) )
					resize = TRUE;
				else
					move = TRUE;
			}
			else if( ( m_keyInterface & KEY_ARROW ) && ( nChar == VK_LEFT ) )
			{
				offsetx=-offsetstepx;
				if( GetAsyncKeyState( VK_SHIFT ) )
					resize = TRUE;
				else
					move = TRUE;
			}
			else if( ( m_keyInterface & KEY_ARROW ) && ( nChar == VK_RIGHT ) )
			{
				offsetx=offsetstepx;
				if( GetAsyncKeyState( VK_SHIFT ) )
					resize = TRUE;
				else
					move = TRUE;
			}
			else if( ( m_keyInterface & KEY_DELETE ) && ( nChar == VK_DELETE ) )
			{
				if( GetAsyncKeyState( VK_SHIFT ) )
					Cut();
				else
					DeleteAllSelected();
			}
			else if( ( m_keyInterface & KEY_INSERT ) && ( nChar == VK_INSERT ) )
			{
				if( GetAsyncKeyState( VK_SHIFT ) )
					Paste();
				else
					Duplicate();
			}
			else if( ( m_keyInterface & KEY_CTRL ) && nChar == VK_RETURN && GetAsyncKeyState( VK_CONTROL ) )
				ShowProperties();
			else if( ( m_keyInterface & KEY_CTRL ) && nChar == _TCHAR( 'A' ) && GetAsyncKeyState( VK_CONTROL ) )
				SelectAll();
			else if( ( m_keyInterface & KEY_CTRL ) && nChar == _TCHAR( 'X' ) && GetAsyncKeyState( VK_CONTROL ) )
				Cut();
			else if( ( m_keyInterface & KEY_CTRL ) && nChar == _TCHAR( 'V' ) && GetAsyncKeyState( VK_CONTROL ) )
				Paste();
			else if( ( m_keyInterface & KEY_CTRL ) && nChar == _TCHAR( 'C' ) && GetAsyncKeyState( VK_CONTROL ) )
				Copy();
			else if( ( m_keyInterface & KEY_CTRL ) && nChar == _TCHAR( 'Z' ) && GetAsyncKeyState( VK_CONTROL ) )
				Undo();
			else if( ( m_keyInterface & KEY_PGUPDOWN ) && nChar == VK_NEXT && GetAsyncKeyState( VK_CONTROL ) )
				Bottom();
			else if( ( m_keyInterface & KEY_PGUPDOWN ) && nChar == VK_PRIOR && GetAsyncKeyState( VK_CONTROL ) )
				Front();
			else if( ( m_keyInterface & KEY_PGUPDOWN ) && nChar == VK_NEXT)
				Down();
			else if( ( m_keyInterface & KEY_PGUPDOWN ) && nChar == VK_PRIOR)
				Up();

			if( move )
			{
				InsideRestraints( offsetx, offsety );
				m_objs->Snapshot();
				while( ( obj = m_objs->GetAt( count++ ) ) )
				{
					if( obj->IsSelected() )
					{
						double width = obj->GetRight() - obj->GetLeft();
						double height = obj->GetBottom() - obj->GetTop();

						double left = obj->GetLeft() + offsetx;
						double top = obj->GetTop() + offsety;

						if( m_snap )
						{
							left = SnapX( ( int ) left );
							top = SnapY( ( int ) top );
						}

						double right = left + width;
						double bottom = top + height;

						AdjustForRestraints( left, top, right, bottom );
						obj->SetRect( left, top, right, bottom );
						redraw = TRUE;
						m_objs->SetModified( TRUE );
					}
				}
			}

			if( resize )
			{
				m_objs->Snapshot();
				InsideRestraints( offsetx, offsety );
				while( ( obj = m_objs->GetAt( count++ ) ) )
				{
					if( obj->IsSelected() )
					{

						double right = obj->GetRight() + offsetx;
						double bottom = obj->GetBottom() + offsety;
						if( m_snap )
						{
							right = SnapX( ( int ) right );
							bottom = SnapY( ( int ) bottom );
						}

						obj->SetRect( obj->GetLeft(), obj->GetTop(), right, bottom );

						m_objs->SetModified( TRUE );
						redraw = TRUE;
					}
				}
			}

			if( redraw )
				RedrawWindow();
		}

		if( ( m_keyInterface & KEY_ESCAPE ) && ( nChar == VK_ESCAPE ) )
		{
			m_interactMode = MODE_NONE;
			m_drawing = FALSE;
		}

		// Keys independent of any data
		if( ( m_keyInterface & KEY_PLUSMINUS ) && ( nChar == VK_ADD ) )
			SetZoom( GetZoom() + GetZoomFactor() );

		if( ( m_keyInterface & KEY_PLUSMINUS ) && ( nChar == VK_SUBTRACT ) )
			SetZoom( GetZoom() - GetZoomFactor() );
	}

	CWnd::OnKeyDown( nChar, nRepCnt, nFlags );

}

BOOL CDiagramEditor::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message ) 
/* ============================================================
	Function :		CDiagramEditor::OnSetCursor
	Description :	Handles the WM_SETCURSOR message. We set 
					cursors as appropriate.
					
	Return :		BOOL			-	not interested
	Parameters :	CWnd* pWnd		-	not interested
					UINT nHitTest	-	not interested
					UINT message	-	not interested
					
	Usage :			Called from MFC. Do not call from code.

   ============================================================*/
{

	BOOL res = FALSE;
	if( m_interactMode == MODE_DRAWING )
	{
		::SetCursor( LoadCursor( NULL, IDC_CROSS ) );
		res = TRUE;
	}
	else if( m_objs )
	{
		int count = 0;
		CDiagramEntity* obj;
		const MSG* msg = GetCurrentMessage();
		CPoint point( msg->pt );
		ScreenToClient( &point );
		ScreenToVirtual( point );

		while( ( obj = m_objs->GetAt( count++ ) ) )
		{
			if( obj->IsSelected() )
			{
				res = TRUE;
				int hitCode = obj->GetHitCode( point );
				if( hitCode != DEHT_NONE && hitCode != DEHT_BODY )
					::SetCursor( obj->GetCursor( hitCode ) );
				else
					res = FALSE;

			}
		}

		if( res == FALSE )
		{
			int hitCode = GetHitCode( point );
			if( hitCode != DEHT_NONE && hitCode != DEHT_BODY && m_bgResizeSelected )
			{
				::SetCursor( GetCursor( hitCode ) );
				res = TRUE;
			}
			else
				res = CWnd::OnSetCursor( pWnd, nHitTest, message );
		}
	}
	else
		res = CWnd::OnSetCursor( pWnd, nHitTest, message );

	return res;

}

HCURSOR CDiagramEditor::GetCursor( int hit ) const
/* ============================================================
	Function :		CDiagramEditor::GetCursor
	Description :	Returns a HCURSOR for the hit-point value 
					(defined in DiagramEntity.h) 
					
	Return :		HCURSOR	-	The cursor to show, NULL if 
								default.
	Parameters :	int hit	-	The hit-point value
					
	Usage :			Virtual. Can be overridden in a derived
					class to display other resize cursors.

   ============================================================*/
{

	// We only show cursors for a subset of the hit-point codes.
	HCURSOR cursor = LoadCursor( NULL, IDC_ARROW );
	switch( hit )
	{
		case DEHT_TOPRIGHT:
			cursor = LoadCursor( NULL, IDC_SIZENESW );
		break;
		case DEHT_BOTTOMMIDDLE:
			cursor = LoadCursor( NULL, IDC_SIZENS );
		break;
		case DEHT_BOTTOMRIGHT:
			cursor = LoadCursor( NULL, IDC_SIZENWSE );
		break;
		case DEHT_BOTTOMLEFT:
			cursor = LoadCursor( NULL, IDC_SIZENESW );
		break;
		case DEHT_RIGHTMIDDLE:
			cursor = LoadCursor( NULL, IDC_SIZEWE );
		break;
	}

	return cursor;

}

int CDiagramEditor::GetHitCode( CPoint point )
/* ============================================================
	Function :		CDiagramEditor::GetHitCode
	Description :	Will return the hit-point value (defined in 
					DiagramEntity.h) of point.

	Return :		int				-	The hit-point define of 
										point. DEHT_NONE if none.
	Parameters :	CPoint point	-	The point to hit-test.
					
	Usage :			Virtual. Can be overridden in a derived 
					class to return other hit-point values. 
					Should normally not be called from code.

   ============================================================*/
{

	CRect rect( 0, 0, GetVirtualSize().cx, GetVirtualSize().cy );

	int result = DEHT_NONE;

	if( rect.PtInRect( point ) )
		result = DEHT_BODY;

	CRect rectTest;

	// We return all selection marker points for aestethical 
	// reasons, even if we can't resize the background to 
	// the top and left.
	VirtualToScreen( point );
	rectTest = GetSelectionMarkerRect( DEHT_TOPLEFT );
	if( rectTest.PtInRect( point ) )
		result = DEHT_TOPLEFT;

	rectTest = GetSelectionMarkerRect( DEHT_TOPMIDDLE );
	if( rectTest.PtInRect( point ) )
		result = DEHT_TOPMIDDLE;

	rectTest = GetSelectionMarkerRect( DEHT_TOPRIGHT );
	if( rectTest.PtInRect( point ) )
		result = DEHT_TOPRIGHT;

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMLEFT );
	if( rectTest.PtInRect( point ) )
		result = DEHT_BOTTOMLEFT;

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMMIDDLE );
	if( rectTest.PtInRect( point ) )
		result = DEHT_BOTTOMMIDDLE;

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT );
	if( rectTest.PtInRect( point ) )
		result = DEHT_BOTTOMRIGHT;

	rectTest = GetSelectionMarkerRect( DEHT_LEFTMIDDLE );
	if( rectTest.PtInRect( point ) )
		result = DEHT_LEFTMIDDLE;

	rectTest = GetSelectionMarkerRect( DEHT_RIGHTMIDDLE );
	if( rectTest.PtInRect( point ) )
		result = DEHT_RIGHTMIDDLE;

	return result;

}

void CDiagramEditor::InsideRestraints( double& x, double& y )
/* ============================================================
	Function :		CDiagramEditor::InsideRestraints
	Description :	Returns the max x and y that the selected 
					objects can move, with the desired x and y 
					as in-parameters.
					
	Return :		void
	Parameters :	double& x	-	Desired x-movement
					double& y	-	Desired y-movement.
					
	Usage :			Internal function. Will not actually move 
					the objects, only calculate the maximum 
					possible movement.

   ============================================================*/
{
	if( m_restraint != RESTRAINT_NONE )
	{

		int count = 0;
		CDiagramEntity* obj;
		double minleft = 0xffffffff;
		double mintop = 0xffffffff;
		double maxright = 0;
		double maxbottom = 0;

		while( ( obj = m_objs->GetAt( count++ ) ) )
		{
			if( obj->IsSelected() )
			{

				// Correcting, depending on restraint mode.
				// Note that checks will have to be made for all 
				// coordinates against all sides, as the coordinates 
				// might not be normalized (as for a line, for example).

				double left = obj->GetLeft() + x;
				double top = obj->GetTop() + y;
				double right = obj->GetRight() + x;
				double bottom = obj->GetBottom() + y;

				minleft = min( minleft, left );
				minleft = min( minleft, right );
				mintop = min( mintop, top );
				mintop = min( mintop, bottom );

				maxright = max( maxright, left );
				maxright = max( maxright, right );
				maxbottom = max( maxbottom, top );
				maxbottom = max( maxbottom, bottom );

			}
		}

		double leftedge = 0;
		double rightedge = 0;
		double topedge = 0;
		double bottomedge = 0;

		if( m_restraint == RESTRAINT_VIRTUAL )
		{
			leftedge = 0;
			topedge = 0;
			rightedge = GetVirtualSize().cx;
			bottomedge = GetVirtualSize().cy;
		}
		else if( m_restraint == RESTRAINT_MARGIN )
		{
			leftedge = m_leftMargin;
			topedge = m_topMargin;
			rightedge = GetVirtualSize().cx - m_rightMargin;
			bottomedge = GetVirtualSize().cy - m_bottomMargin;
		}

		if( minleft < leftedge )
			x = x - ( minleft - leftedge );
		if( mintop < topedge )
			y = y - ( mintop - topedge );
		if( maxright > rightedge )
			x = rightedge - ( maxright - x );
		if( maxbottom > bottomedge )
			y = bottomedge - ( maxbottom - y );

	}
}

void CDiagramEditor::AdjustForRestraints( double& left, double& top, double& right, double& bottom )
/* ============================================================
	Function :		CDiagramEditor::AdjustForRestraints
	Description :	Adjust the rect represented by the in-
					params to the current constraints.
					
	Return :		void
	Parameters :	double& left	-	Desired left position. 
										Contains the corrected 
										left position on return.
					double& top		-	Desired top position.
										Contains the corrected 
										top position on return.
					double& right	-	Desired right position.
										Contains the corrected 
										right position on return.
					double& bottom	-	Desired bottom position.
										Contains the corrected 
										bottom position on return.
					
	Usage :			Internal function. Will not resize the rect. 
					The coordinates need not be normalized.

   ============================================================*/
{

	// Saving the size
	double width = fabs( right - left );
	double height = fabs( bottom - top );

	// Correcting, depending on restraint mode.
	// Note that checks will have to be made for all 
	// coordinates against all sides, as the coordinates 
	// might not be normalized (as for a line, for example).
	if( m_restraint == RESTRAINT_VIRTUAL )
	{

		if( left < 0 )
		{
			left = 0;
			right = left + width;
		}
		if( top < 0 )
		{
			top = 0;
			bottom = top + height;
		}
		if( right < 0 )
		{
			right = 0;
			left = right + width;
		}
		if( bottom < 0 )
		{
			bottom = 0;
			top = bottom + height;
		}

		if( right > GetVirtualSize().cx )
		{
			right = GetVirtualSize().cx;
			left = right - width;
		}
		if( bottom > GetVirtualSize().cy )
		{
			bottom = GetVirtualSize().cy;
			top = bottom - height;
		}

		if( left > GetVirtualSize().cx )
		{
			left = GetVirtualSize().cx;
			right = left - width;
		}
		if( top > GetVirtualSize().cy )
		{
			top = GetVirtualSize().cy;
			bottom = top - height;
		}
	}
	else if( m_restraint == RESTRAINT_MARGIN )
	{
		if( left < m_leftMargin )
		{
			left = m_leftMargin;
			right = left + width;
		}
		if( top < m_topMargin )
		{
			top = m_topMargin;
			bottom = top + height;
		}
		if( right < m_leftMargin )
		{
			right = m_leftMargin;
			left = right + width;
		}
		if( bottom < m_topMargin )
		{
			bottom = m_topMargin;
			top = bottom + height;
		}

		if( right > GetVirtualSize().cx - m_rightMargin )
		{
			right = ( GetVirtualSize().cx - m_rightMargin );
			left = right - width;
		}
		if( bottom > GetVirtualSize().cy - m_bottomMargin )
		{
			bottom = ( GetVirtualSize().cy - m_bottomMargin );
			top = bottom - height;
		}
		if( left > GetVirtualSize().cx - m_rightMargin )
		{
			left = ( GetVirtualSize().cx - m_rightMargin );
			right = left - width;
		}
		if( top > GetVirtualSize().cy - m_bottomMargin )
		{
			top = ( GetVirtualSize().cy - m_bottomMargin );
			bottom = top - height;
		}
	}

}

void CDiagramEditor::AdjustForRestraints( double& xpos, double& ypos )
/* ============================================================
	Function :		CDiagramEditor::AdjustForRestraints
	Description :	Adjust the rect represented by the in-
					params to the current constraints.
					
	Return :		void
	Parameters :	double& xpos	-	Desired x position. 
										Contains the corrected 
										x position on return.
					double& ypos	-	Desired y position. 
										Contains the corrected 
										y position on return.
					
	Usage :			Internal function. Will only correct 
					position parameters, not change any object.

   ============================================================*/
{

	if( m_restraint == RESTRAINT_VIRTUAL )
	{
		xpos = max( xpos, 0 );
		xpos = min( xpos, GetVirtualSize().cx );
		ypos = max( ypos, 0 );
		ypos = min( ypos, GetVirtualSize().cy );
	}
	else if( m_restraint == RESTRAINT_MARGIN )
	{
		xpos = max( xpos, m_leftMargin );
		xpos = min( xpos, GetVirtualSize().cx - m_rightMargin );
		ypos = max( ypos, m_topMargin );
		ypos = min( ypos, GetVirtualSize().cy - m_bottomMargin );
	}

}

CSize CDiagramEditor::GetContainingSize() const
/* ============================================================
	Function :		CDiagramEditor::GetContainingSize
	Description :	Return the minimum size enveloping all 
					objects.
					
	Return :		CSize	-	Minimum size necessary to 
								contain all objects.
	Parameters :	none

	Usage :			Internal function. Calculates the minimum
					size necessary for all objects.

   ============================================================*/
{

	int count = 0;
	CDiagramEntity* obj;
	int maxx = 0;
	int maxy = 0;

	if( m_objs )
	{
		while( ( obj = m_objs->GetAt( count++ ) ) )
		{
			maxx = max( round( obj->GetRight() ), maxx );
			maxy = max( round( obj->GetBottom() ), maxy );
		}
	}

	return CSize( maxx, maxy );

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEditor scrolling

void CDiagramEditor::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar ) 
/* ============================================================
	Function :		CDiagramEditor::OnHScroll
	Description :	Handles the WM_VSCROLL message. Updates the 
					screen.
					
	Return :		void
	Parameters :	UINT nSBCode			-	Type of scroll operation
					UINT nPos				-	New scroll position
					CScrollBar* pScrollBar	-	
					
	Usage :			Called from MFC. Do not call from code.

   ============================================================*/
{

	SCROLLINFO si;
	si.cbSize = sizeof( SCROLLINFO );
	si.fMask = SIF_POS | SIF_RANGE;
	GetScrollInfo( SB_HORZ, &si );
	switch( nSBCode )
	{
		case SB_LEFT:			// Scroll to far left.
			si.nPos = si.nMin;
			break;
		case SB_LINELEFT:		// Scroll left.
			si.nPos = max( si.nPos - 1, si.nMin );
			break;
		case SB_LINERIGHT:		// Scroll right.
			si.nPos = min( si.nPos + 1, si.nMax );
			break;
		case SB_PAGELEFT:		// Scroll one page left.
			si.nPos = max( si.nPos - ( int ) si.nPage, si.nMin );
			break;
		case SB_PAGERIGHT:		// Scroll one page right.
			si.nPos = min( si.nPos + ( int ) si.nPage, si.nMax );
			break;
		case SB_RIGHT:			// Scroll to far right.
			si.nPos = si.nMax;
			break;
		case SB_THUMBPOSITION:	// Scroll to absolute position. The current position is specified by the nPos parameter.
			si.nPos = nPos;
			break;
		case SB_THUMBTRACK:		//
			si.nPos = nPos;
			break;
	}

	SetScrollInfo( SB_HORZ, &si );
	RedrawWindow();
	CWnd::OnHScroll( nSBCode, nPos, pScrollBar );

}

void CDiagramEditor::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar ) 
/* ============================================================
	Function :		CDiagramEditor::OnVScroll
	Description :	Handles the WM_VSCROLL message. Updates the 
					screen.
					
	Return :		void
	Parameters :	UINT nSBCode			-	Type of scroll 
												operation.
					UINT nPos				-	New scroll position.
					CScrollBar* pScrollBar	-	
					
	Usage :			Called from MFC. Do not call from code.

   ============================================================*/
{

	SCROLLINFO si;
	si.cbSize = sizeof( SCROLLINFO );
	si.fMask = SIF_POS | SIF_RANGE;
	GetScrollInfo( SB_VERT, &si );
	switch( nSBCode )
	{
		case SB_TOP:			// Scroll to far left.
			si.nPos = si.nMin;
			break;
		case SB_LINEUP:			// Scroll left.
			si.nPos = max( si.nPos - 1, si.nMin );
			break;
		case SB_LINEDOWN:		// Scroll right.
			si.nPos = min( si.nPos + 1, si.nMax );
			break;
		case SB_PAGEUP:			// Scroll one page left.
			si.nPos = max( si.nPos - ( int ) si.nPage, si.nMin );
			break;
		case SB_PAGEDOWN:		// Scroll one page right.
			si.nPos = min( si.nPos + ( int ) si.nPage, si.nMax );
			break;
		case SB_BOTTOM:			// Scroll to far right.
			si.nPos = si.nMax;
			break;
		case SB_THUMBPOSITION:	// Scroll to absolute position. The current position is specified by the nPos parameter.
			si.nPos = nPos;
			break;
		case SB_THUMBTRACK:		//
			si.nPos = nPos;
			break;
	}

	SetScrollInfo( SB_VERT, &si );
	RedrawWindow();

	CWnd::OnVScroll( nSBCode, nPos, pScrollBar );

}

int CDiagramEditor::HScroll( int scroll )
/* ============================================================
	Function :		CDiagramEditor::HScroll
	Description :	Scrolls the editor window scroll steps
					horisontally.
					
	Return :		int			-	The number of steps actually 
									scrolled.
	Parameters :	int scroll	-	The number of steps to scroll.
					
	Usage :			Internal function

   ============================================================*/
{

	int retval = 0;

	if( m_hWnd )
	{
		int desiredpos = 0;

		SCROLLINFO si;
		si.cbSize = sizeof( SCROLLINFO );
		if( GetScrollInfo( SB_HORZ, &si ) )
		{
			desiredpos = si.nPos + scroll;
			si.nPos = desiredpos;

			SetScrollInfo( SB_HORZ, &si );
			GetScrollInfo( SB_HORZ, &si );
			retval = scroll - ( desiredpos - si.nPos );
			if( retval )
				RedrawWindow();
		}
	}

	return retval;

}

int CDiagramEditor::VScroll( int scroll )
/* ============================================================
	Function :		CDiagramEditor::VScroll
	Description :	Scrolls the editor window scroll steps
					vertically.
					
	Return :		int			-	The number of steps actually 
									scrolled.
	Parameters :	int scroll	-	The number of steps to scroll.
					
	Usage :			Internal function.

   ============================================================*/
{

	int retval = 0;

	if( m_hWnd )
	{
		int desiredpos = 0;

		SCROLLINFO si;
		si.cbSize = sizeof( SCROLLINFO );
		if( GetScrollInfo( SB_VERT, &si ) )
		{
			desiredpos = si.nPos + scroll;
			si.nPos = desiredpos;

			SetScrollInfo( SB_VERT, &si );
			GetScrollInfo( SB_VERT, &si );
			retval = scroll - ( desiredpos - si.nPos );
			if( retval )
				RedrawWindow();
		}
	}

	return retval;

}

CPoint CDiagramEditor::ScrollPoint( CPoint point )
/* ============================================================
	Function :		CDiagramEditor::ScrollPoint
	Description :	Scrolls the editor if point is outside the 
					window.
					
	Return :		CPoint			-	The pixels scrolled 
										horisontally and 
										vertically.
	Parameters :	CPoint point	-	The position of the mouse 
										pointer.
					
	Usage :			Internal function.

   ============================================================*/
{

	CPoint out( 0, 0 );
	CRect clientRect;
	GetClientRect( &clientRect );

	if( point.x > clientRect.right )
		out.x = HScroll( 10 );

	if( point.x < 0 )
		out.x = HScroll( -10 );

	if( point.y > clientRect.bottom )
		out.y = VScroll( 10 );

	if( point.y < 0 )
		out.y = VScroll( -10 );

	return out;

}

void CDiagramEditor::SetupScrollbars()
/* ============================================================
	Function :		CDiagramEditor::SetupScrollbars
	Description :	Sets up/removes scrollbars depending on the 
					relation between the client- and virtual 
					rect.
					
	Return :		void
	Parameters :	none

	Usage :			Internal function.

   ============================================================*/
{

	if( m_hWnd )
	{
		CRect rect;
		GetClientRect( rect );

		DWORD add = 0;
		DWORD remove = 0;

		if( round( ( double ) GetVirtualSize().cx * GetZoom() ) <= rect.right )
		{
			if( GetStyle() & WS_HSCROLL )
				remove |= WS_HSCROLL;
		}
		else
		{
			if( !( GetStyle() & WS_HSCROLL ) )
				add |= WS_HSCROLL;
		}
		if( round( ( double ) GetVirtualSize().cy * GetZoom() ) <= rect.bottom )
		{
			if( GetStyle() & WS_VSCROLL )
				remove |= WS_VSCROLL;
		}
		else
		{
			if( !( GetStyle() & WS_VSCROLL ) )
				add |= WS_VSCROLL;
		}

		SCROLLINFO si;
		si.cbSize = sizeof( SCROLLINFO );
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		int width = round( ( double ) GetVirtualSize().cx * GetZoom() );
		if( width > rect.right )
		{
			si.nMax = width;
			si.nPage = rect.right;
		}
		else
		{
			si.nMax = 0;
			si.nPage = 0;
		}
		SetScrollInfo( SB_HORZ, &si );

		int height = round( ( double ) GetVirtualSize().cy * GetZoom() );
		if( height > rect.bottom )
		{
			si.nMax = height;
			si.nPage = rect.bottom;
		}
		else
		{
			si.nMax = 0;
			si.nPage = 0;
		}
		SetScrollInfo( SB_VERT, &si );

		if( add || remove )
			ModifyStyle( remove, add, SWP_FRAMECHANGED );

	}

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEditor sizing

void CDiagramEditor::OnSize( UINT nType, int cx, int cy ) 
/* ============================================================
	Function :		CDiagramEditor::OnSize
	Description :	Handles the WM_SIZE message. Sets up/modifies 
					scrollbars as necessary.
					
	Return :		void
	Parameters :	UINT nType	-	
					int cx		-	The new x size
					int cy		-	The new y size
					
	Usage :			Called from MFC. Do not call from code.

   ============================================================*/
{

	CWnd::OnSize( nType, cx, cy );
	SetupScrollbars();

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEditor coordinate handling

void CDiagramEditor::ScreenToVirtual( CRect& rect ) const
/* ============================================================
	Function :		CDiagramEditor::ScreenToVirtual
	Description :	Converts rect from screen- to virtual 
					coordinates.
					
					
	Return :		void
	Parameters :	CRect& rect	-	The rect to convert
					
	Usage :			Internal function.

   ============================================================*/
{

	// We have to normalize, add scroll bar positions and 
	// apply current zoom.

	rect.NormalizeRect();

	SCROLLINFO sih;
	SCROLLINFO siv;

	sih.cbSize = sizeof( SCROLLINFO );
	sih.fMask = SIF_POS;
	siv.cbSize = sizeof( SCROLLINFO );
	siv.fMask = SIF_POS;

	if( !::GetScrollInfo( m_hWnd, SB_HORZ, &sih ) )
		sih.nPos = 0;
	if( !::GetScrollInfo( m_hWnd, SB_VERT, &siv ) )
		siv.nPos = 0;

	rect.SetRect( round( ( double ) ( rect.left		+ sih.nPos ) / GetZoom() ), 
				  round( ( double ) ( rect.top		+ siv.nPos ) / GetZoom() ), 
				  round( ( double ) ( rect.right	+ sih.nPos ) / GetZoom() ), 
				  round( ( double ) ( rect.bottom	+ siv.nPos ) / GetZoom() ) );

}

void CDiagramEditor::ScreenToVirtual( CPoint& point ) const
/* ============================================================
	Function :		CDiagramEditor::ScreenToVirtual
	Description :	Converts point from screen- to virtual 
					coordinates.
					
	Return :		void
	Parameters :	CPoint& point	-	The point to convert.	
					
	Usage :			Internal function.

   ============================================================*/
{

	// Convert point to the virtual
	// coordinate system. We have to normalize, 
	// add scroll bar positions and apply current 
	// scale.

	SCROLLINFO sih;
	SCROLLINFO siv;

	sih.cbSize = sizeof( SCROLLINFO );
	sih.fMask = SIF_POS;
	siv.cbSize = sizeof( SCROLLINFO );
	siv.fMask = SIF_POS;

	if( !::GetScrollInfo( m_hWnd, SB_HORZ, &sih ) )
		sih.nPos = 0;
	if( !::GetScrollInfo( m_hWnd, SB_VERT, &siv ) )
		siv.nPos = 0;

	point.x = round( ( double ) ( point.x + sih.nPos ) / GetZoom() );
	point.y = round( ( double ) ( point.y + siv.nPos ) / GetZoom() );

}

void CDiagramEditor::ScreenToVirtual( CSize& size ) const
/* ============================================================
	Function :		CDiagramEditor::ScreenToVirtual
	Description :	Converts size from screen- to virtual 
					coordinates.
					
	Return :		void
	Parameters :	CSize& size	-	The size to convert.
					
	Usage :			Internal function.

   ============================================================*/
{

	// We have to deduct scroll bar positions 
	// and apply current zoom.


	SCROLLINFO sih;
	SCROLLINFO siv;

	sih.cbSize = sizeof( SCROLLINFO );
	sih.fMask = SIF_POS;
	siv.cbSize = sizeof( SCROLLINFO );
	siv.fMask = SIF_POS;

	if( !::GetScrollInfo( m_hWnd, SB_HORZ, &sih ) )
		sih.nPos = 0;
	if( !::GetScrollInfo( m_hWnd, SB_VERT, &siv ) )
		siv.nPos = 0;

	size.cx = round( ( double ) ( size.cx + sih.nPos ) / GetZoom() );
	size.cy = round( ( double ) ( size.cy + siv.nPos ) / GetZoom() );

}

void CDiagramEditor::VirtualToScreen( CRect& rect ) const
/* ============================================================
	Function :		CDiagramEditor::VirtualToScreen
	Description :	Converts rect from the virtual to the 
					screen coordinate system. 
					
	Return :		void
	Parameters :	CRect& rect	-	The rect to convert. Will be 
									normalized on return.
					
	Usage :			Internal function.

   ============================================================*/
{

	// We have to normalize, deduct scroll bar positions 
	// and apply current zoom.

	rect.NormalizeRect();

	SCROLLINFO sih;
	SCROLLINFO siv;

	sih.cbSize = sizeof( SCROLLINFO );
	sih.fMask = SIF_POS;
	siv.cbSize = sizeof( SCROLLINFO );
	siv.fMask = SIF_POS;

	if( !::GetScrollInfo( m_hWnd, SB_HORZ, &sih ) )
		sih.nPos = 0;
	if( !::GetScrollInfo( m_hWnd, SB_VERT, &siv ) )
		siv.nPos = 0;

	rect.SetRect( round( ( double ) ( rect.left		 * GetZoom() ) - sih.nPos ), 
				  round( ( double ) ( rect.top		 * GetZoom() ) - siv.nPos ), 
				  round( ( double ) ( rect.right	 * GetZoom() ) - sih.nPos ), 
				  round( ( double ) ( rect.bottom	 * GetZoom() ) - siv.nPos ) );

}

void CDiagramEditor::VirtualToScreen( CPoint& point ) const
/* ============================================================
	Function :		CDiagramEditor::VirtualToScreen
	Description :	Converts point from the virtual to the 
					screen coordinate system. 
					
	Return :		void
	Parameters :	CPoint& point	-	The point to convert. 
					
	Usage :			Internal function.

   ============================================================*/
{

	// We have to deduct scroll bar positions 
	// and apply current zoom.

	SCROLLINFO sih;
	SCROLLINFO siv;

	sih.cbSize = sizeof( SCROLLINFO );
	sih.fMask = SIF_POS;
	siv.cbSize = sizeof( SCROLLINFO );
	siv.fMask = SIF_POS;

	if( !::GetScrollInfo( m_hWnd, SB_HORZ, &sih ) )
		sih.nPos = 0;
	if( !::GetScrollInfo( m_hWnd, SB_VERT, &siv ) )
		siv.nPos = 0;

	point.x = round( ( double ) ( point.x * GetZoom() ) - sih.nPos );
	point.y = round( ( double ) ( point.y * GetZoom() ) - sih.nPos );


}

int CDiagramEditor::SnapX( int coord ) const
/* ============================================================
	Function :		CDiagramEditor::SnapX
	Description :	Snaps coord vertically to the closest 
					grid point.
					
	Return :		int			-	The resulting x-coordinate.
	Parameters :	int coord	-	The coordinate to snap
					
	Usage :			Internal function. Will snap even if snap 
					to grid is off.

   ============================================================*/
{

	double x = ( double ) coord;
	double gridx = ( double ) m_gridSize.cx;
	return round( x / gridx ) * m_gridSize.cx;

}

int CDiagramEditor::SnapY( int coord ) const
/* ============================================================
	Function :		CDiagramEditor::SnapY
	Description :	Snaps coord horisontally to the closest 
					grid point.
					
	Return :		int			-	The resulting y-coordinate.
	Parameters :	int coord	-	The coordinate to snap
					
	Usage :			Internal function. Will snap even if snap 
					to grid is off.

   ============================================================*/
{

	double y = ( double ) coord;
	double gridy = ( double ) m_gridSize.cy;
	return round( y / gridy ) * m_gridSize.cy;

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEditor object manipulations

BOOL CDiagramEditor::IsAnyObjectSelected() const
/* ============================================================
	Function :		CDiagramEditor::IsAnyObjectSelected
	Description :	Returns TRUE if any object is selected.
					
	Return :		BOOL	-	TRUE if any object is selected.
	Parameters :	none

	Usage :			Call to check if any objects are selected.

   ============================================================*/
{
	//return m_selObjs.GetSize() > 0;

	BOOL res = FALSE;
	if( m_objs )
	{
		int count = 0;
		CDiagramEntity* obj;
		while( ( obj = m_objs->GetAt( count++ ) ) )
			if( obj->IsSelected() )
				return TRUE;
	}

	return res;

}

int CDiagramEditor::GetSelectCount() const
/* ============================================================
	Function :		CDiagramEditor::GetSelectCount
	Description :	Returns the number of selected objects in 
					the data container.
					
	Return :		int	-	The number of selected objects.
	Parameters :	none

	Usage :			Call to get the number of selected objects.

   ============================================================*/
{
	return m_objs ? m_objs->GetSelectCount() : 0;
}

CDiagramEntity* CDiagramEditor::GetSelectedObject() const
/* ============================================================
	Function :		CDiagramEditor::GetSelectedObject
	Description :	Returns the top selected object in the 
					container.
					
	Return :		CDiagramEntity*	-	The top selected object, 
										or NULL if none.
	Parameters :	none

	Usage :			Call to get the currently selected object. 
					Note that this function will return a single 
					object (top in the z-order) even if several 
					are selected.

   ============================================================*/
{
	return m_objs ? m_objs->GetSelectedObject() : NULL;
}

int CDiagramEditor::GetObjectCount() const
/* ============================================================
	Function :		CDiagramEditor::GetObjectCount
	Description :	Returns the number of objects in the container.
					
	Return :		int	-	The number of objects.
	Parameters :	none

	Usage :			Call to get the number of objects in the 
					data container.

   ============================================================*/
{

	int size = 0;
	if( m_objs )
		size = m_objs->GetSize();

	return size;

}

void CDiagramEditor::AddObject( CDiagramEntity* obj ) 
/* ============================================================
	Function :		CDiagramEditor::AddObject
	Description :	Adds obj to the data container.
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to add.
					
	Usage :			Called to add objects to the container after 
					- for example - a load operation. Note that 
					the modified flag must be set manually if 
					this is desired (if implementing an Append 
					command, for example).

   ============================================================*/
{

	if( m_objs )
	{
		m_objs->Add( obj );
		RedrawWindow();
	}

}

CDiagramEntity* CDiagramEditor::GetObject( int index )  const
/* ============================================================
	Function :		CDiagramEditor::GetObject
	Description :	Gets the object at index index.
					
	Return :		CDiagramEntity*	-	The object, NULL if not 
										found.
	Parameters :	int index		-	The index to get the 
										object from
					
	Usage :			Call to get a specific object from the 
					container. 
					The objects are the responsibility of the 
					container and should not be deleted.

   ============================================================*/
{

	CDiagramEntity* obj = NULL;
	if( m_objs )
		obj = m_objs->GetAt( index );

	return obj;

}



void CDiagramEditor::StartDrawingObject( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEditor::StartDrawingObject
	Description :	Starts drawing an object of type obj.
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	Pointer to an object 
											of the desired type.
					
	Usage :			Should be called with a pointer to the 
					appropriate object type when the user 
					selects to draw an object. obj becomes 
					the property of the editor and should not 
					be deleted.

   ============================================================*/
{

	delete m_drawObj;
	m_drawObj = obj;

	if( obj )
	{
		m_drawing = TRUE;
		m_interactMode = MODE_DRAWING;
		::SetCursor( LoadCursor( NULL, IDC_CROSS ) );
	}
	else
	{
		m_drawing = FALSE;
		m_interactMode = MODE_NONE;
		::SetCursor( LoadCursor( NULL, IDC_ARROW ) );
	}

}

BOOL CDiagramEditor::IsDrawing() const
/* ============================================================
	Function :		CDiagramEditor::IsDrawing
	Description :	Returns if the user is drawing or not
					
	Return :		BOOL	-	TRUE if the user is currently 
								drawing
	Parameters :	none

	Usage :			Called for command enabling if multi-draw
					mode is enabled.

   ============================================================*/
{
	return m_drawObj && m_drawing;
}

void CDiagramEditor::UnselectAll()
/* ============================================================
	Function :		CDiagramEditor::UnselectAll
	Description :	Unselects all objects in the container.
					
	Return :		void
	Parameters :	none

	Usage :			Call to unselect all objects. 
					Should not be callable if GetObjectCount() 
					== 0 (there are no objects in the container).

   ============================================================*/
{
	if( m_objs )
	{
		int count = 0;
		CDiagramEntity* obj;
		while( ( obj = m_objs->GetAt( count++ ) ) )
			//obj->Select( FALSE );
			SelectObject(obj, FALSE);
	}

}

void CDiagramEditor::SelectObject(CDiagramEntity* obj, BOOL selected)
{
	if (m_objs)
		m_objs->Select(obj,selected);
}

void CDiagramEditor::SelectAll()
/* ============================================================
	Function :		CDiagramEditor::SelectAll
	Description :	Selects all objects.
					
	Return :		void
	Parameters :	none

	Usage :			Call to select all objects. 
					Should not be callable if GetObjectCount() 
					== 0 (there are no objects in the container).

   ============================================================*/
{

	if( m_objs )
	{
		int count = 0;
		CDiagramEntity* obj;
		while( ( obj = m_objs->GetAt( count++ ) ) )
			//obj->Select( TRUE );
			SelectObject(obj, TRUE);
		RedrawWindow();
	}

}

void CDiagramEditor::DeleteAllSelected()
/* ============================================================
	Function :		CDiagramEditor::DeleteAllSelected
	Description :	Deletes all selected objects from the 
					container.
					
	Return :		void
	Parameters :	none

	Usage :			Call to delete all selected objects from the 
					editor. Should not be callable if 
					GetSelectCount() == 0 ( or IsAnyObjectSelected() 
					== FALSE )

   ============================================================*/
{
	if( m_objs )
	{
		m_objs->Snapshot();
		m_objs->RemoveAllSelected();
		RedrawWindow();
	}

}

void CDiagramEditor::DeleteAll()
/* ============================================================
	Function :		CDiagramEditor::DeleteAll
	Description :	Deletes all objects from the container.
					
	Return :		void
	Parameters :	none

	Usage :			Call to delete all objects from the editor.

   ============================================================*/
{
	if( m_objs )
	{
		m_objs->Snapshot();
		m_objs->RemoveAll();
		RedrawWindow();
	}

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEditor layout

void CDiagramEditor::LeftAlignSelected()
/* ============================================================
	Function :		CDiagramEditor::LeftAlignSelected
	Description :	Aligns all selected objects to the left of
					the top selected object in the data container. 
					
	Return :		void
	Parameters :	none

	Usage :			Call to align the left edge of all selected 
					objects.
					Should only be callable if GetSelectCount() > 
					1, that is, more than one object is selected.

   ============================================================*/
{
	if( GetSelectCount() > 1 )
	{
		m_objs->Snapshot();
		m_objs->LeftAlignSelected();
		SetModified( TRUE );
		RedrawWindow();
	}
}

void CDiagramEditor::RightAlignSelected()
/* ============================================================
	Function :		CDiagramEditor::RightAlignSelected
	Description :	Aligns all selected objects to the right of
					the top selected object in the data container. 
					
	Return :		void
	Parameters :	none

	Usage :			Call to align the right edge of all selected 
					objects.
					Should only be callable if GetSelectCount() > 
					1, that is, more than one object is selected.

   ============================================================*/
{
	if( GetSelectCount() > 1 )
	{
		m_objs->Snapshot();
		m_objs->RightAlignSelected();
		SetModified( TRUE );
		RedrawWindow();
	}

}

void CDiagramEditor::TopAlignSelected()
/* ============================================================
	Function :		CDiagramEditor::TopAlignSelected
	Description :	Aligns all selected objects to the top of
					the top selected object in the data container. 
					
	Return :		void
	Parameters :	none

	Usage :			Call to align the top of all selected 
					objects.
					Should only be callable if GetSelectCount() > 
					1, that is, more than one object is selected.

   ============================================================*/
{
	if( GetSelectCount() > 1 )
	{
		m_objs->Snapshot();
		m_objs->TopAlignSelected();
		SetModified( TRUE );
		RedrawWindow();
	}
}

void CDiagramEditor::BottomAlignSelected()
/* ============================================================
	Function :		CDiagramEditor::BottomAlignSelected
	Description :	Aligns all selected objects to the bottom of
					the top selected object in the data container. 
					
	Return :		void
	Parameters :	none

	Usage :			Call to align the bottom of all selected 
					objects.
					Should only be callable if GetSelectCount() > 
					1, that is, more than one object is selected.

   ============================================================*/
{

	if( GetSelectCount() > 1 )
	{
		m_objs->Snapshot();
		m_objs->BottomAlignSelected();
		SetModified( TRUE );
		RedrawWindow();
	}

}

void CDiagramEditor::MakeSameSizeSelected()
/* ============================================================
	Function :		CDiagramEditor::MakeSameSizeSelected
	Description :	Makes all selected objects the same size as 
					the top selected object in the data container.
					
	Return :		void
	Parameters :	none

	Usage :			Call to make all selected objects the same 
					size.
					Should only be called if GetSelectCount() > 
					1, that is, more than one object is selected.

   ============================================================*/
{
	if( GetSelectCount() > 1 )
	{
		m_objs->Snapshot();
		m_objs->MakeSameSizeSelected();
		SetModified( TRUE );
		RedrawWindow();
	}
}

void CDiagramEditor::Duplicate()
/* ============================================================
	Function :		CDiagramEditor::Duplicate
	Description :	Duplicates the currently selected object.
					
	Return :		void
	Parameters :	none

	Usage :			Call to put a copy of the selected object on 
					the screen.

   ============================================================*/
{
	if( GetSelectCount() == 1 )
	{
		CDiagramEntity* obj = GetSelectedObject();
		if( obj )
		{
			m_objs->Snapshot();
			m_objs->Duplicate( obj );
			//obj->Select( FALSE );
			SelectObject(obj, FALSE);
			RedrawWindow();
		}
	}
}

void CDiagramEditor::Up()
/* ============================================================
	Function :		CDiagramEditor::Up
	Description :	Moves the currently selected object one 
					step up in the object z-order.
					
	Return :		void
	Parameters :	none

	Usage :			Call to move the selected object one step 
					up in the z-order.
					This command should only be callable if
					GetSelectCount() return 1, that is, if one 
					and only one object is selected.

   ============================================================*/
{

	if( GetSelectCount() == 1 )
	{
		CDiagramEntity* obj = GetSelectedObject();
		if( obj )
		{
			m_objs->Up( obj );
			RedrawWindow();
		}
	}
}

void CDiagramEditor::Down()
/* ============================================================
	Function :		CDiagramEditor::Down
	Description :	Moves the currently selected object one 
					step down in the object z-order.
					
	Return :		void
	Parameters :	none

	Usage :			Call to move the selected object one step 
					down in the z-order.
					This command should only be callable if
					GetSelectCount() return 1, that is, if one 
					and only one object is selected.

   ============================================================*/
{

	if( GetSelectCount() == 1 )
	{
		CDiagramEntity* obj = GetSelectedObject();
		if( obj )
		{
			m_objs->Down( obj );
			RedrawWindow();
		}
	}
}

void CDiagramEditor::Front()
/* ============================================================
	Function :		CDiagramEditor::Front
	Description :	Moves the selected object to the front of 
					all other objects.
					
	Return :		void
	Parameters :	none

	Usage :			Call to move the selected object to the 
					top of the z-order. 
					This command should only be callable if
					GetSelectCount() return 1, that is, if one 
					and only one object is selected.

   ============================================================*/
{

	if( GetSelectCount() == 1 )
	{
		CDiagramEntity* obj = GetSelectedObject();
		if( obj )
		{
			m_objs->Front( obj );
			RedrawWindow();
		}
	}
}

void CDiagramEditor::Bottom()
/* ============================================================
	Function :		CDiagramEditor::Bottom
	Description :	Moves the selected object to the bottom of 
					all objects.
					
	Return :		void
	Parameters :	none

	Usage :			Call to move the selected object to the 
					bottom of the z-order. 
					This command should only be callable if
					GetSelectCount() return 1, that is, if one 
					and only one object is selected.

   ============================================================*/
{

	if( GetSelectCount() == 1 )
	{
		CDiagramEntity* obj = GetSelectedObject();
		if( obj )
		{
			m_objs->Bottom( obj );
			RedrawWindow();
		}
	}
}


void CDiagramEditor::Reorder( const CUIntArray &order )
{
	if( m_objs )
	{
		m_objs->Reorder( order);
		RedrawWindow();
	}
}

void CDiagramEditor::ShowProperties()
/* ============================================================
	Function :		CDiagramEditor::ShowProperties
	Description :	Shows the property dialog for the one and 
					only selected object in the data container.
					
	Return :		void
	Parameters :	none

	Usage :			Call to display a property dialog for the 
					selected object. This command should 
					normally only be callable if GetSelectCount() 
					return 1, that is, if one and only one object 
					is selected.

   ============================================================*/
{

	if( GetSelectCount() == 1 )
	{
		CDiagramEntity* obj = GetSelectedObject();
		if( obj )
			obj->ShowProperties( this );
	}

}


void CDiagramEditor::RemoveUnselectedPropertyDialogs(BOOL bAll)
/* ============================================================
	Function :		CDiagramEditor::RemoveUnselectedPropertyDialogs
	Description :	Removes all property dialogs for unselected 
					objects.
					
	Return :		void
	Parameters :	BOOL bAll	-	TRUE   don't care, objects is selected or unselected
									FALSE  only selected objects

	Usage :			Internal function. Call to remove all 
					property dialogs for non-selected objects 
					from screen.

   ============================================================*/
{

	if( m_objs )
	{
		int count = 0;
		CDiagramEntity* obj;
		while( ( obj = m_objs->GetAt( count++ ) ) )
			if( bAll || !obj->IsSelected() )
				obj->ShowProperties( this, FALSE );
	}

}

void CDiagramEditor::ShowPopup( CPoint point ) 
/* ============================================================
	Function :		CDiagramEditor::ShowPopup
	Description :	Shows a popup menu in response to a right-
					click if no object is selected.
					
	Return :		void
	Parameters :	CPoint point	-	The point to track the 
										menu from.
					
	Usage :			Internal function. Call to show the editor 
					popup menu.

   ============================================================*/
{

	if( !m_popupMenu )
		m_popupMenu = new CDiagramMenu;

	if( m_popupMenu )
	{
		CMenu* menu = m_popupMenu->GetPopupMenu( this );
		if( menu )
			menu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
	}

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEditor copy/paste/undo

void CDiagramEditor::OnEditCut()
/* ============================================================
	Function :		CDiagramEditor::OnEditCut
	Description :	Command handler for the MFC standard 
					ID_EDIT_CUT command.
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Call Cut from code instead.

   ============================================================*/
{

	Cut();

}

void CDiagramEditor::OnEditCopy()
/* ============================================================
	Function :		CDiagramEditor::OnEditCopy
	Description :	Command handler for the MFC standard 
					ID_EDIT_COPY command.
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Call Copy from code instead.

   ============================================================*/
{

	Copy();

}

void CDiagramEditor::OnEditPaste()
/* ============================================================
	Function :		CDiagramEditor::OnEditPaste
	Description :	Command handler for the MFC standard 
					ID_EDIT_PASTE command.
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Call Paste from code instead.

   ============================================================*/
{

	Paste();

}

void CDiagramEditor::Cut()
/* ============================================================
	Function :		CDiagramEditor::Cut
	Description :	Cut the currently selected items to the 
					internal CDiagramEntityContainer clipboard.
					
	Return :		void
	Parameters :	none

	Usage :			Call to deleta and place the currently 
					selected objects to the "clipboard".

   ============================================================*/
{

	if( m_objs )
	{
		m_objs->Snapshot();
		m_objs->CopyAllSelected();
		DeleteAllSelected();
	}

}

void CDiagramEditor::Copy()
/* ============================================================
	Function :		CDiagramEditor::Copy
	Description :	Copy the currently selected items to the 
					internal CDiagramEntityContainer clipboard.
					
	Return :		void
	Parameters :	none

	Usage :			Call to copy the selected objects to the 
					"clipboard".

   ============================================================*/
{

	if( m_objs )
	{
		m_objs->Snapshot();
		m_objs->CopyAllSelected();
	}

}

void CDiagramEditor::Paste()
/* ============================================================
	Function :		CDiagramEditor::Paste
	Description :	Paste copied or cut objects to the screen.
					
	Return :		void
	Parameters :	none

	Usage :			Call to paste the contents of the clipboard 
					to the screen.

   ============================================================*/
{

	// Removes the current selection and pastes the contents
	// of the data paste-array.
	if( m_objs )
	{
		SetRedraw( FALSE );
		m_objs->Snapshot();
		UnselectAll();
		m_objs->Paste();
		SetRedraw( TRUE );
		RedrawWindow();
	}

}

void CDiagramEditor::Undo()
/* ============================================================
	Function :		CDiagramEditor::Undo
	Description :	Undo the last operation.
					
	Return :		void
	Parameters :	none

	Usage :			Call to restore the objects to the last 
					snapshot.

   ============================================================*/
{

	if( m_objs )
	{
		m_objs->Undo();
		RedrawWindow();
	}

}

void CDiagramEditor::UpdateCut( CCmdUI* pCmdUI ) const
/* ============================================================
	Function :		CDiagramEditor::UpdateCut
	Description :	Command enabling for a Cut command UI-
					element.
					
	Return :		void
	Parameters :	CCmdUI* pCmdUI	-	Command element to 
										update
					
	Usage :			Can be called from a doc/view command update 
					function

   ============================================================*/
{

	// We can only cut if there is something selected
	pCmdUI->Enable( IsAnyObjectSelected() );

}

void CDiagramEditor::UpdateCopy( CCmdUI* pCmdUI ) const
/* ============================================================
	Function :		CDiagramEditor::UpdateCopy
	Description :	Command enabling for a Copy command UI-
					element.
					
	Return :		void
	Parameters :	CCmdUI* pCmdUI	-	Command element to 
										update
					
	Usage :			Can be called from a doc/view command update 
					function

   ============================================================*/
{

	pCmdUI->Enable( IsAnyObjectSelected() );

}

void CDiagramEditor::UpdatePaste( CCmdUI* pCmdUI ) const
/* ============================================================
	Function :		CDiagramEditor::UpdatePaste
	Description :	Command enabling for a Paste command UI-
					element.
					
	Return :		void
	Parameters :	CCmdUI* pCmdUI	-	Command element to 
										update
					
	Usage :			Can be called from a doc/view command update 
					function

   ============================================================*/
{

	if( m_objs )
		pCmdUI->Enable( m_objs->ObjectsInPaste() );

}

void CDiagramEditor::UpdateUndo( CCmdUI* pCmdUI ) const
/* ============================================================
	Function :		CDiagramEditor::UpdateUndo
	Description :	Command enabling for an Undo command UI-
					element.
					
	Return :		void
	Parameters :	CCmdUI* pCmdUI	-	Command element to 
										update
					
	Usage :			Can be called from a doc/view command update 
					function

   ============================================================*/
{

	if( m_objs )
		pCmdUI->Enable( m_objs->IsUndoPossible() );

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEditor saving

void CDiagramEditor::Save( CStringArray& stra )
/* ============================================================
	Function :		CDiagramEditor::Save
	Description :	Saves a string representation of the 
					container (normally the virtual size) and 
					all objects to stra.
					
	Return :		void
	Parameters :	CStringArray& stra	-	The array to fill
					
	Usage :			Call to save the data of the editor to a 
					CStringArray. Virtual. Can be overridden in 
					a derived class to add non-container data 
					before and after the objects

   ============================================================*/
{

	if( m_objs )
	{
		stra.Add( m_objs->GetString() );
		SaveObjects( stra );
		SetModified( FALSE );
	}

}

BOOL CDiagramEditor::FromString( const CString& str )
/* ============================================================
	Function :		CDiagramEditor::FromString
	Description :	Sets the virtual size from a string 
					representation of the paper.
					
	Return :		BOOL				-	TRUE if str is a 
											representation of a 
											"paper".
	Parameters :	const CString& str	-	String representation.
					
	Usage :			Call to set the size of the virtual paper 
					from a string.

   ============================================================*/
{
	BOOL result = FALSE;
	if( m_objs )
		result = m_objs->FromString( str );

	return result;

}


void CDiagramEditor::SaveObjects( CStringArray& stra )
/* ============================================================
	Function :		CDiagramEditor::SaveObjects
	Description :	Saves a string representation of the 
					current objects to stra, one object in 
					each string.
					
	Return :		void
	Parameters :	CStringArray& stra	-	The array to fill
					
	Usage :			Virtual. Can be overridden in a derived 
					class to add more per-object information to 
					the save array.

   ============================================================*/
{

	if( m_objs )
	{
		int count = 0;
		CDiagramEntity* obj;
		while( ( obj = m_objs->GetAt( count++ ) ) ) {
			stra.Add( obj->GetString() );
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEditor command handling



void CDiagramEditor::OnObjectCommand( UINT nID )
/* ============================================================
	Function :		CDiagramEditor::OnObjectCommand
	Description :	Called from WM_COMMAND messages in the 
					range CMD_START to CMD_END inclusive 
					(defined in DiagramEntity.h). Those are 
					messages that will be passed on to all 
					selected objects.
					
	Return :		void
	Parameters :	UINT nID	-	The command to send.
					
	Usage :			Called from MFC. Do not call from code.

   ============================================================*/
{
	if (OnPreObjectCommand(nID)) return;

	if( m_objs )
	{
		m_objs->Snapshot();
		m_objs->SendMessageToObjects( nID, TRUE, NULL, static_cast< CWnd* >( this ) );
		RedrawWindow();
	}

}

int CDiagramEditor::GetInteractMode() const
{

	return m_interactMode;

}

void CDiagramEditor::SetInteractMode( int interactMode )
{

	m_interactMode = interactMode;
	m_subMode = 0;

}

CDiagramEntity* CDiagramEditor::GetDrawingObject()
{
	return m_drawObj;
}

COLORREF CDiagramEditor::GetBackgroundColor()
{
	return m_bkgndCol;
}

#pragma warning( default : 4706 )
