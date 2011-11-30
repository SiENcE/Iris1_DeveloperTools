/* ==========================================================================
	CDiagramEntity

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-29

	Purpose :		CDiagramEntity is the base class for all objects that can 
					be drawn and managed by CDiagramEditor.

	Description :	CDiagramEntity is derived from CObject, to allow 
					instances to be stored in CObArrays.

	Usage :			Classes should be derived from CDiagramEntity. Clone 
					must be overridden, returning a copy of the this 
					pointer.

					Normally, Draw should also be overridden. 

					The class supports basic saving to a text file. If this 
					is desired, SetType must be called from the derived 
					class ctor with a string uniquely identifying the class. 
					FromString and GetString must be overridden if other 
					properties than the default are to be saved. Loading can 
					be accomplished by creating a static "factory"-function 
					returning an instance of the class if FromString returns 
					TRUE for a given line from a data file. See CreateFromString 
					in this class for a model implementation.

					Minimum- and maximum sizes for an instance of the 
					derived object can be set in the class ctor by calling
					SetConstraints. A 0-constraint means that the object 
					can't be turned "inside out", while -1 means no 
					constraints.

					Popup menus for the derived classes can be created by 
					overriding ShowPopup. Command ids for the menu items 
					must be in the range CMD_START to CMD_END inclusively, 
					and if a menu alternative is selected, it will be 
					returned to the class instance through DoMessage - 
					which must of course also be overriddden.

					Each derived class can also have a property dialog. The 
					dialog class must be derived from CDiagramPropertyDlg. 
					The derived CDiagramEntity class must have a class 
					member instance of the desired CDiagramPropertyDlg-
					derived class, and call SetPropertyDialog in the ctor. 
					Transport of data to and from the object is made in the 
					CDiagramPropertyDlg-derived class (see 
					CDiagramPropertyDlg.cpp)

					The number, position and types of the selection rects 
					can be modified by overriding GetHitCode and 
					DrawSelectionMarkers, see CDiagramLine.cpp for an 
					example.

					BodyInRect can be overridden to allow non-rect hit 
					testing, see CDiagramLine.cpp for an example.

					GetCursor can be overridden to display other cursors 
					than the default ones.

	Changes:		10/4 2004	Changed accessors for m_type to public.
					24/4 2004	Added colon as a replace-character for 
								saving
					30/4 2004	Added redraw parent to the property dialog
   ========================================================================*/
#include "stdafx.h"
#include "DiagramEntity.h"
#include "DiagramEntityContainer.h"
#include "Tokenizer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntity

CDiagramEntity::CDiagramEntity()
/* ============================================================
	Function :		CDiagramEntity::CDiagramEntity
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
	SetParent(NULL);
	//SetPropertyDialog( NULL, 0 );
	Clear();
	SetType( _T( "basic" ) );

}

CDiagramEntity::~CDiagramEntity()
/* ============================================================
	Function :		CDiagramEntity::~CDiagramEntity
	Description :	destructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

void CDiagramEntity::Clear()
/* ============================================================
	Function :		CDiagramEntity::Clear
	Description :	Zero all properties of this object.
					
	Return :		void
	Parameters :	none

	Usage :			Call to initialize the object.

   ============================================================*/
{
	SetParent( NULL );
	SetRect( 0.0, 0.0, 0.0, 0.0 );
	SetMarkerSize( CSize( 4, 4 ) );
	SetConstraints( CSize( 1, 1 ), CSize( -1, -1 ) );
	Select( FALSE );
	SetName( _T( "" ) );
	Freeze( FALSE );
	SetVisible( TRUE );
}

CDiagramEntity* CDiagramEntity::Clone()
/* ============================================================
	Function :		CDiagramEntity::Clone
	Description :	Clone this object to a new object.
					
	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The 
					caller will have to delete the object.

   ============================================================*/
{
	CDiagramEntity* obj = new CDiagramEntity;
	obj->Copy( this );
	
	return obj;
}

void CDiagramEntity::Copy( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntity::Copy
	Description :	Copy the information in obj to this object.
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to copy 
											from.
					
	Usage :			Copies basic information. from obj to this.
					GetType can be used to check for the correct 
					object type in overridden versions.
   ============================================================*/
{

	Clear();

	SetMarkerSize( obj->GetMarkerSize() );
	SetConstraints( obj->GetMinimumSize(), obj->GetMaximumSize() );

	Select( obj->IsSelected() );
	SetParent( obj->GetParent() );
	SetType( obj->GetType() );
	SetTitle( obj->GetTitle() );
	SetName( obj->GetName() );
	Freeze( obj->IsFreezed());
	SetVisible(obj->IsVisible());

	SetRect( obj->GetLeft(), obj->GetTop(), obj->GetRight(), obj->GetBottom() );
}

BOOL CDiagramEntity::FromString( XML::Node* node )
/* ============================================================
	Function :		CDiagramEntity::FromString
	Description :	Sets the values for an object from str. 
					
	Return :		BOOL				-	TRUE if str 
											represents an 
											object of this 
											type.
	Parameters :	const CString& str	-	Possible text 
											format 
											representation.
					
	Usage :			Can be called to fill an existing object 
					with information from a string created with 
					GetString.

   ============================================================*/
{
	std::string type, name;
	int left=0,top=0,width=0,height=0,freezed=0,visible=1;

	if (!node->lookupAttribute("class", type)) return FALSE;
	if (GetType().CompareNoCase(type.c_str()) != 0) return FALSE;

	node->lookupAttribute("name", name);
	node->lookupAttribute("left", left);
	node->lookupAttribute("top", top);
	node->lookupAttribute("width", width);
	node->lookupAttribute("height", height);
	node->lookupAttribute("freezed", freezed);
	node->lookupAttribute("visible", visible);
	
	SetType(type.c_str());
	SetName(name.c_str());

	SetRect(left,top,left+width,top+height);
	Freeze(freezed);
	SetVisible(visible);
	return TRUE;
}

CDiagramEntity* CDiagramEntity::CreateFromString( XML::Node* node )
/* ============================================================
	Function :		CDiagramEntity::CreateFromString
	Description :	Static factory function that creates and 
					returns an instance of this class if str 
					is a valid representation.
					
	Return :		CDiagramEntity*		-	The object, or NULL 
											if str is not a 
											representation of 
											this type.
	Parameters :	const CString& str	-	The string to create 
											from.
					
	Usage :			Can be used as a factory for text file loads. 
					Each object type should have its own 
					version - the default one is a model 
					implementation.

   ============================================================*/
{

	CDiagramEntity* obj = new CDiagramEntity;
	if(!obj->FromString( node ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CDiagramEntity::GetString(BOOL bBegin) const
/* ============================================================
	Function :		CDiagramEntity::GetString
	Description :	Creates a string representing the object.
					
	Return :		CString	-	The resulting string
	Parameters :	none

	Usage :			Used to save this object to a text file.

   ============================================================*/
{
	CString str;

	if (bBegin) {
		CRect rect = GetRect();
		str.Format("<control class='%s' name='%s' left='%d' top='%d' width='%d' height='%d' freezed='%d' visible='%d'>", 
			GetType(), GetName(), rect.left, rect.top, rect.Width(), rect.Height(), IsFreezed(), IsVisible());

	} else {
		str = "</control>";
	}

	return str;
}

CString CDiagramEntity::GetString() const
{
	return GetString(TRUE) + "\n" + GetString(FALSE);
}

CRect CDiagramEntity::GetRect() const
/* ============================================================
	Function :		CDiagramEntity::GetRect
	Description :	Returns the object rect.
					
	Return :		CRect	-	The object rect.
	Parameters :	none

	Usage :			Call to get the object position and size. 
					Will round of fractions.

   ============================================================*/
{

	CRect rect( ( int ) GetLeft(), ( int ) GetTop(), ( int ) GetRight(), ( int ) GetBottom() );
	return rect;

}

void CDiagramEntity::SetRect( CRect rect )
/* ============================================================
	Function :		CDiagramEntity::SetRect
	Description :	Sets the object rect, normalized.
					
	Return :		void
	Parameters :	CRect rect	-	The rect to set.
					
	Usage :			Call to place the object.

   ============================================================*/
{

	rect.NormalizeRect();
	SetRect( ( double ) rect.left, ( double ) rect.top, ( double ) rect.right, ( double ) rect.bottom );

}

void CDiagramEntity::SetRect( double left, double top, double right, double bottom )
/* ============================================================
	Function :		CDiagramEntity::SetRect
	Description :	Sets the object rect.
					
	Return :		void
	Parameters :	double left		-	Left edge
					double top		-	Top edge
					double right	-	Right edge
					double bottom	-	Bottom edge
					
	Usage :			Call to place the object.

   ============================================================*/
{

	SetLeft( left );
	SetTop( top );
	SetRight( right );
	SetBottom( bottom );

	if( GetMinimumSize().cx != -1 )
		if( GetRect().Width() < GetMinimumSize().cx )
			SetRight( GetLeft() + GetMinimumSize().cx );

	if( GetMinimumSize().cy != -1 )
		if( GetRect().Height() < GetMinimumSize().cy )
			SetBottom( GetTop() + GetMinimumSize().cy );

	if( GetMaximumSize().cx != -1 )
		if( GetRect().Width() > GetMaximumSize().cx )
			SetRight( GetLeft() + GetMaximumSize().cx );

	if( GetMaximumSize().cy != -1 )
		if( GetRect().Height() > GetMaximumSize().cy )
			SetBottom( GetTop() + GetMaximumSize().cy );

  if( GetPropertySheet() )
		GetPropertySheet()->SetValues();

}

void CDiagramEntity::MoveRect( double x, double y )
/* ============================================================
	Function :		CDiagramEntity::MoveRect
	Description :	Moves the object rect.
					
	Return :		void
	Parameters :	double x	-	Move x steps horizontally.
					double y	-	Move y steps vertically.
					
	Usage :			Call to move the object on screen.

   ============================================================*/
{

	SetRect( GetLeft() + x, GetTop() + y, GetRight() + x, GetBottom() + y );

}

void CDiagramEntity::Select( BOOL selected )
/* ============================================================
	Function :		CDiagramEntity::Select
	Description :	Sets the object select state.
					
	Return :		void
	Parameters :	BOOL selected	-	TRUE to select, FALSE 
										to unselect.
					
	Usage :			Call to select/deselect the object.

   ============================================================*/
{
	if (m_parent) {
		m_parent->Select(this, selected);
	} else {
		m_selected = selected;
	}
}

BOOL CDiagramEntity::IsSelected() const
/* ============================================================
	Function :		CDiagramEntity::IsSelected
	Description :	Checks if the object is selected.
					
	Return :		BOOL	-	TRUE if the object is selected.
	Parameters :	none

	Usage :			Call to see if the object is selected.

   ============================================================*/
{
	if (!IsSelectable()) return FALSE;
	return m_selected;

}

BOOL CDiagramEntity::BodyInRect( CRect rect ) const
/* ============================================================
	Function :		CDiagramEntity::BodyInRect
	Description :	Used to see if any part of the object lies 
					in rect.
					
	Return :		BOOL		-	TRUE if any part of the 
									object lies inside rect.
	Parameters :	CRect rect	-	The rect to check.
					
	Usage :			Call to see if the object overlaps - for 
					example - a selection rubberband.

   ============================================================*/
{

	BOOL result = FALSE;
	CRect rectEntity = GetRect();
	CRect rectIntersect;

	rect.NormalizeRect();
	rectEntity.NormalizeRect();

	rectIntersect.IntersectRect( rect, rectEntity );
	if( !rectIntersect.IsRectEmpty() )
		result = TRUE;

	return result;

}

BOOL CDiagramEntity::IsSelectable() const
{
	return !IsFreezed() && IsVisible();
}


void CDiagramEntity::Freeze(BOOL freezed)
{
	m_freezed = freezed;
	if (freezed) Select(FALSE);
}

BOOL CDiagramEntity::IsFreezed() const
{
	return m_freezed;
}

void CDiagramEntity::SetVisible(BOOL visible)
{
	m_visible = visible;
	if (!visible) Select(FALSE);
}

BOOL CDiagramEntity::IsVisible() const
{
	return m_visible;
}


int CDiagramEntity::GetHitCode( CPoint point ) const
/* ============================================================
	Function :		CDiagramEntity::GetHitCode
	Description :	Returns the hit point constant for point.
					
	Return :		int				-	The hit point, 
										DEHT_NONE if none.
	Parameters :	CPoint point	-	The point to check
					
	Usage :			Call to see in what part of the object point 
					lies.

   ============================================================*/
{

	int result = DEHT_NONE;
	CRect rect = GetRect();

	if( rect.PtInRect( point ) )
		result = DEHT_BODY;

	CRect rectTest;

	rectTest = GetSelectionMarkerRect( DEHT_TOPLEFT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_TOPLEFT;

	rectTest = GetSelectionMarkerRect( DEHT_TOPMIDDLE, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_TOPMIDDLE;

	rectTest = GetSelectionMarkerRect( DEHT_TOPRIGHT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_TOPRIGHT;

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMLEFT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_BOTTOMLEFT;

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMMIDDLE, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_BOTTOMMIDDLE;

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_BOTTOMRIGHT;

	rectTest = GetSelectionMarkerRect( DEHT_LEFTMIDDLE, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_LEFTMIDDLE;

	rectTest = GetSelectionMarkerRect( DEHT_RIGHTMIDDLE, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_RIGHTMIDDLE;

	return result;

}

BOOL CDiagramEntity::DoMessage( UINT msg, CDiagramEntity* /*sender*/, CWnd* from )
/* ============================================================
	Function :		CDiagramEntity::DoMessage
	Description :	Message handler for the object.

	Return :		BOOL					-	TRUE to stop 
												further processing.
	Parameters :	UINT msg				-	The message.
					CDiagramEntity* sender	-	Original sender of 
												this message, or 
												NULL if not an object.

	Usage :			The container can send messages to all 
					objects. The messages should lie in the 
					range CMD_START to CMD_STOP inclusively - 
					a few are already predefined in 
					DiagramEntity.h. This function will be 
					called as response to those messages. This 
					mechanism is already used for sending back 
					messages from CDiagramEditor to the 
					relevant object when a object popup menu 
					alternative is selected.

   ============================================================*/
{

	BOOL stop = FALSE;
	switch( msg )
	{
		case CMD_CUT:
			if( m_parent && IsSelected() )
			{
				stop = TRUE;
				m_parent->Cut( this );
			}
		break;

		case CMD_COPY:
			if( m_parent && IsSelected() )
			{
				stop = TRUE;
				m_parent->Copy( this );
			}
		break;

		case CMD_UP:
			if( m_parent && IsSelected() )
			{
				stop = TRUE;
				m_parent->Up( this );
			}
		break;

		case CMD_DOWN:
			if( m_parent && IsSelected() )
			{
				stop = TRUE;
				m_parent->Down( this );
			}
		break;

		case CMD_FRONT:
			if( m_parent && IsSelected() )
			{
				stop = TRUE;
				m_parent->Front( this );
			}
		break;

		case CMD_BOTTOM:
			if( m_parent && IsSelected() )
			{
				stop = TRUE;
				m_parent->Bottom( this );
			}
		break;

		case CMD_DUPLICATE:
			if( m_parent && IsSelected() )
			{
				stop = TRUE;
				m_parent->Duplicate( this );
				Select( FALSE );
			}
		break;

		case CMD_PROPERTIES:
			if( IsSelected() )
			{
				ShowProperties( from ); // MOD
				stop = TRUE;
			}
		break;

		case CMD_FREEZE:
			if( IsSelected() )
			{
				stop = FALSE;
				Freeze(TRUE);
			}
		break;

		case CMD_UNFREEZE_ALL:
			if( IsFreezed() )
			{
				stop = FALSE;
				Freeze(FALSE);
			}
		break;

		case CMD_HIDE:
			if( IsSelected() )
			{
				stop = FALSE;
				SetVisible(FALSE);
			}
		break;

		case CMD_HIDE_UNSEL:
			if( !IsSelected() )
			{
				stop = FALSE;
				SetVisible(FALSE);
			}
		break;

		case CMD_UNHIDE_ALL:
			if( !IsVisible() )
			{
				stop = FALSE;
				SetVisible(TRUE);
			}
		break;
	}

	return stop;

}

void CDiagramEntity::ShowPopup( CPoint point, CWnd* parent )
/* ============================================================
	Function :		CDiagramEntity::ShowPopup
	Description :	Shows the popup menu for the object.

	Return :		void
	Parameters :	CPoint point	-	The point to track.
					CWnd* parent	-	The parent CWnd of the 
										menu (should be the 
										CDiagramEditor)

	Usage :			The function uses hardcoded strings to 
					avoid having to include resource file 
					fragments. Derived classes needing a non-
					standard or localized menu should load 
					menues from resources instead.

   ============================================================*/
{

	CMenu menu;
	if( menu.CreatePopupMenu() )
	{

		menu.AppendMenu( MF_STRING, CMD_CUT, _T( "Cu&t" ) );
		menu.AppendMenu( MF_STRING, CMD_COPY, _T( "&Copy" ) );
		menu.AppendMenu( MF_STRING, CMD_DUPLICATE, _T( "Du&plicate" ) );
		menu.AppendMenu( MF_SEPARATOR );
		menu.AppendMenu( MF_STRING, CMD_UP, _T( "&Up" ) );
		menu.AppendMenu( MF_STRING, CMD_DOWN, _T( "&Down" ) );
		menu.AppendMenu( MF_STRING, CMD_FRONT, _T( "To &front" ) );
		menu.AppendMenu( MF_STRING, CMD_BOTTOM, _T( "To &back" ) );
		menu.AppendMenu( MF_SEPARATOR );
		menu.AppendMenu( MF_STRING, CMD_FREEZE, _T( "Freeze Selection" ) );
		menu.AppendMenu( MF_STRING, CMD_UNFREEZE_ALL, _T( "Unfreeze All" ) );
		menu.AppendMenu( MF_SEPARATOR );
		menu.AppendMenu( MF_STRING, CMD_HIDE, _T( "Hide Selection" ) );	
		menu.AppendMenu( MF_STRING, CMD_UNHIDE_ALL, _T( "Unhide All" ) );
		menu.AppendMenu( MF_STRING, CMD_HIDE_UNSEL, _T( "Hide Unselected" ) );
		menu.AppendMenu( MF_SEPARATOR );
		menu.AppendMenu( MF_STRING, CMD_CONTROL_LIST, _T( "Control List " ) );
		
		menu.AppendMenu( MF_SEPARATOR );
		menu.AppendMenu( MF_STRING, CMD_PROPERTIES, _T( "P&roperties" ) );
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, parent );

	}

}

void CDiagramEntity::ShowProperties( CWnd* parent, BOOL show )
/* ============================================================
	Function :		CDiagramEntity::ShowProperties
	Description :	Shows the property dialog for the object.

	Return :		void
	Parameters :	CWnd* parent	-	Parent of the dialog
					BOOL show		-	TRUE to show, FALSE 
										to hide.

	Usage :			Call to show the property dialog for this 
					object.

   ============================================================*/
{

	if( show ) {
		if( !m_propertysheet.GetSafeHwnd()) {
			m_propertysheet.Create(parent);
			
			if (parent) {
				CRect rect;
				parent->GetWindowRect(rect);
				m_propertysheet.SetWindowPos(NULL,
					rect.left+150,rect.top+100,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW);
			}
		}

		m_propertysheet.ShowWindow( SW_SHOW );
		m_propertysheet.SetValues();
		m_propertysheet.SetFocus();
	} else if( m_propertysheet.GetSafeHwnd() ) {
		m_propertysheet.ShowWindow( SW_HIDE );
	}

}

void CDiagramEntity::DrawObject( CDC* dc, double zoom )
/* ============================================================
	Function :		CDiagramEntity::DrawObject
	Description :	Top-level drawing function for the object.
					
	Return :		void
	Parameters :	CDC* dc		-	
					double zoom	-	
					
	Usage :			Even though virtual, this function should 
					normally not be overridden (use Draw 
					instead). The function stores the zoom and 
					calculates the true drawing rectangle.

   ============================================================*/
{

	SetZoom( zoom );
	CRect rect( round( GetLeft() * zoom ), 
				round( GetTop() * zoom ), 
				round( GetRight() * zoom ), 
				round( GetBottom() * zoom ) );

	Draw( dc, rect );

	if( IsSelected() )
		DrawSelectionMarkers( dc, rect );

}

void CDiagramEntity::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramEntity::Draw
	Description :	Draws the object.

	Return :		void
	Parameters :	CDC* dc		-	The CDC to draw to. 
					CRect rect	-	The real rectangle of the 
									object.

	Usage :			The function should clean up all selected 
					objects. Note that the CDC is a memory CDC, 
					so creating a memory CDC in this function 
					will probably not speed up the function.

   ============================================================*/
{

	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

	dc->Rectangle( rect );


}

HCURSOR CDiagramEntity::GetCursor( int hit ) const
/* ============================================================
	Function :		CDiagramEntity::GetCursor
	Description :	Returns the cursor for the given hit point.
					
	Return :		HCURSOR	-	The cursor to show
	Parameters :	int hit	-	The hit point constant (DEHT_) 
								to get the cursor for.

	Usage :			Call to get the cursor for a specific hit 
					point constant.

   ============================================================*/
{
	HCURSOR cursor = NULL;
	switch( hit )
	{
		case DEHT_BODY:
			cursor = LoadCursor( NULL, IDC_SIZEALL );
		break;
		case DEHT_TOPLEFT:
			cursor = LoadCursor( NULL, IDC_SIZENWSE );
		break;
		case DEHT_TOPMIDDLE:
			cursor = LoadCursor( NULL, IDC_SIZENS );
		break;
		case DEHT_TOPRIGHT:
			cursor = LoadCursor( NULL, IDC_SIZENESW );
		break;
		case DEHT_BOTTOMLEFT:
			cursor = LoadCursor( NULL, IDC_SIZENESW );
		break;
		case DEHT_BOTTOMMIDDLE:
			cursor = LoadCursor( NULL, IDC_SIZENS );
		break;
		case DEHT_BOTTOMRIGHT:
			cursor = LoadCursor( NULL, IDC_SIZENWSE );
		break;
		case DEHT_LEFTMIDDLE:
			cursor = LoadCursor( NULL, IDC_SIZEWE );
		break;
		case DEHT_RIGHTMIDDLE:
			cursor = LoadCursor( NULL, IDC_SIZEWE );
		break;
	}
	return cursor;
}

void CDiagramEntity::DrawSelectionMarkers( CDC* dc, CRect rect ) const
/* ============================================================
	Function :		CDiagramEntity::DrawSelectionMarkers
	Description :	Draws the selection markers for the 
					object.
					
	Return :		void
	Parameters :	CDC* dc		-	The CDC to draw to
					CRect rect	-	The real object rectangle.
					
	Usage :			rect is the true rectangle (zoomed) of the 
					object.

   ============================================================*/
{

	// Draw selection markers
	CRect rectSelect;

	dc->SelectStockObject( BLACK_BRUSH );
	rectSelect = GetSelectionMarkerRect( DEHT_TOPLEFT, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_TOPMIDDLE, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_TOPRIGHT, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMLEFT, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMMIDDLE, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_RIGHTMIDDLE, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_LEFTMIDDLE, rect );
	dc->Rectangle( rectSelect );

}

CRect CDiagramEntity::GetSelectionMarkerRect( UINT marker, CRect rect ) const
/* ============================================================
	Function :		CDiagramEntity::GetSelectionMarkerRect
	Description :	Gets the selection marker rectangle for 
					marker, given the true object rectangle 
					rect.
					
	Return :		CRect		-	The marker rect
	Parameters :	UINT marker	-	The marker type (DEHT_-
									constants defined in 
									DiargramEntity.h)
					CRect rect	-	The object rect
					
	Usage :			

   ============================================================*/
{

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

void CDiagramEntity::SetParent( CDiagramEntityContainer * parent )
/* ============================================================
	Function :		CDiagramEntity::SetParent
	Description :	Set the container owning the object.
					
	Return :		void
	Parameters :	CDiagramEntityContainer * parent	-	the 
															parent.
					
	Usage :			Call to set the parent of the object. 
					Objects must know their parent, to allow 
					copying etc. 

   ============================================================*/
{

	m_parent = parent;

}

void CDiagramEntity::GetFont( LOGFONT& lf ) const
/* ============================================================
	Function :		CDiagramEntity::GetFont
	Description :	Returns the system GUI font in a LOGFONT 
					scaled to the zoom level of the object.
					
	Return :		void
	Parameters :	LOGFONT& lf	-	The LOGFONT for the system
									GUI font.
					
	Usage :			Call to get the system font. Note that MS 
					Sans Serif will not scale below 8 points.

   ============================================================*/
{

	HFONT hfont = ( HFONT ) ::GetStockObject( DEFAULT_GUI_FONT );
	CFont* font = CFont::FromHandle( hfont );
	font->GetLogFont( &lf );
	lf.lfHeight = round( ( double ) lf.lfHeight * m_zoom );

}

CString CDiagramEntity::GetType() const
/* ============================================================
	Function :		CDiagramEntity::GetType
	Description :	Returns the object type.
					
	Return :		CString	-	The type of the object.
	Parameters :	none

	Usage :			Call to get the type of the object. The type 
					is used when saving and loading objects 
					to/from a text file.

   ============================================================*/
{
	return m_type;
}

void CDiagramEntity::SetType( CString type )
/* ============================================================
	Function :		CDiagramEntity::SetType
	Description :	Set the object type.
					
	Return :		void
	Parameters :	CString type	-	The type to set
					
	Usage :			Call to set the object type - normally in 
					the ctor of this object. The type is used 
					when saving and loading objects to/from a 
					text file.

   ============================================================*/
{
	m_type = type;
}

CString CDiagramEntity::GetTitle() const
/* ============================================================
	Function :		CDiagramEntity::GetTitle
	Description :	Gets the Title property
					
	Return :		CString	-	The current title
	Parameters :	none

	Usage :			Call to get the title of the object. Title 
					is a property that the object can use in
					whatever way it wants.

   ============================================================*/
{

	return m_title;

}

void CDiagramEntity::SetTitle( CString title )
/* ============================================================
	Function :		CDiagramEntity::SetTitle
	Description :	Sets the Title property
					
	Return :		void
	Parameters :	CString title	-	The new title
					
	Usage :			Call to set the title of the object. Title 
					is a property that the object can use in
					whatever way it wants.

   ============================================================*/
{

	m_title = title;

}

CString CDiagramEntity::GetName() const
/* ============================================================
	Function :		CDiagramEntity::GetName
	Description :	Gets the Name property
					
	Return :		CString	-	The current name
	Parameters :	none

	Usage :			Call to get the name of the object. Name is 
					a property that the object can use in
					whatever way it wants.


   ============================================================*/
{

	return m_name;

}

void CDiagramEntity::SetName( CString name )
/* ============================================================
	Function :		CDiagramEntity::SetName
	Description :	Sets the Name property
					
	Return :		void
	Parameters :	CString name	-	The new name
					
	Usage :			Call to set the name of the object. Name is 
					a property that the object can use in
					whatever way it wants.

   ============================================================*/
{

	m_name = name;

}

double CDiagramEntity::GetLeft() const
/* ============================================================
	Function :		CDiagramEntity::GetLeft
	Description :	Gets the left edge of the object rectangle
					
	Return :		double	-	
	Parameters :	none

	Usage :			Call to get the left edge of the object. 
					Note that if minimum sizes are not set for 
					the object, the left edge might be bigger 
					than the right.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{

	return m_left;

}

double CDiagramEntity::GetRight() const
/* ============================================================
	Function :		CDiagramEntity::GetRight
	Description :	Gets the right edge of the object 
					rectangle
					
	Return :		double	-	
	Parameters :	none

	Usage :			Call to get the right edge of the object.
					Note that if minimum sizes are not set for 
					the object, the left edge might be bigger 
					than the right.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{

	return m_right;

}

double CDiagramEntity::GetTop() const
/* ============================================================
	Function :		CDiagramEntity::GetTop
	Description :	Gets the top edge of the object rectangle
					
	Return :		double	-	
	Parameters :	none

	Usage :			Call to get the top edge of the object.
					Note that if minimum sizes are not set for 
					the object, the top edge might be bigger 
					than the bottom.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{

	return m_top;

}

double CDiagramEntity::GetBottom() const
/* ============================================================
	Function :		CDiagramEntity::GetBottom
	Description :	Gets the bottom edge of the object 
					rectangle
					
	Return :		double	-	
	Parameters :	none

	Usage :			Call to get the bottom edge of the object.
					Note that if minimum sizes are not set for 
					the object, the top edge might be bigger 
					than the bottom.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{

	return m_bottom;

}

void CDiagramEntity::SetLeft( double left )
/* ============================================================
	Function :		CDiagramEntity::SetLeft
	Description :	Sets the left edge of the object rectangle
					
	Return :		void
	Parameters :	double left	-	
					
	Usage :			Call to set the left edge of the object.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{

	m_left = left;

}

void CDiagramEntity::SetRight( double right )
/* ============================================================
	Function :		CDiagramEntity::SetRight
	Description :	Sets the right edge of the object 
					rectangle
					
	Return :		void
	Parameters :	double right	-	
					
	Usage :			Call to set the right edge of the object.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{

	m_right = right;

}

void CDiagramEntity::SetTop( double top )
/* ============================================================
	Function :		CDiagramEntity::SetTop
	Description :	Sets the top edge of the object rectangle
					
	Return :		void
	Parameters :	double top	-	
					
	Usage :			Call to set the top edge of the object.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{

	m_top = top;

}

void CDiagramEntity::SetBottom( double bottom )
/* ============================================================
	Function :		CDiagramEntity::SetBottom
	Description :	Sets the bottom edge of the object 
					rectangle
					
	Return :		void
	Parameters :	double bottom	-	
					
	Usage :			Call to set the bottom edge of the object.
					The object coordinates are expressed as 
					double values to allow unlimited zoom.

   ============================================================*/
{

	m_bottom = bottom;

}

void CDiagramEntity::SetMarkerSize( CSize markerSize )
/* ============================================================
	Function :		CDiagramEntity::SetMarkerSize
	Description :	Gets the size of selection markers
					
	Return :		void
	Parameters :	CSize markerSize	-	The new size of a 
											selection marker
					
	Usage :			Call to set a new selection marker size for 
					the object.

   ============================================================*/
{

	m_markerSize = markerSize;

}

CSize CDiagramEntity::GetMarkerSize() const
/* ============================================================
	Function :		CDiagramEntity::GetMarkerSize
	Description :	Gets the size of selection marker
					
	Return :		CSize	-	The current size of a 
								selection marker
	Parameters :	none

	Usage :			Call to get the selection marker size for
					the object.

   ============================================================*/
{

	return m_markerSize;

}

void CDiagramEntity::SetMinimumSize( CSize minimumSize )
/* ============================================================
	Function :		CDiagramEntity::SetMinimumSize
	Description :	Sets the minimum size for instances of 
					this object.
					
	Return :		void
	Parameters :	CSize minimumSize	-	The minimum allowed 
											size
					
	Usage :			Call to set the minimum size of the object.
					It is not possible to resize an object to a 
					size smaller than the minimum allowed size.

   ============================================================*/
{

	m_minimumSize = minimumSize;

}

CSize CDiagramEntity::GetMinimumSize() const
/* ============================================================
	Function :		CDiagramEntity::GetMinimumSize
	Description :	Gets the minimum size for instances of 
					this object.
					
	Return :		CSize	-	The minimum allowed size
	Parameters :	none

	Usage :			Call to get the minimum size of the object.
					It is not possible to resize an object to a 
					size smaller than the minimum allowed size.

   ============================================================*/
{

	return m_minimumSize;

}

void CDiagramEntity::SetMaximumSize( CSize maximumSize )
/* ============================================================
	Function :		CDiagramEntity::SetMaximumSize
	Description :	Sets the maximum size for instances of 
					this object.
					
	Return :		void
	Parameters :	CSize maximumSize	-	The maximum allowed 
											size.
					
	Usage :			Call to set the maximum size of the object.
					It is not possible to resize an object to a 
					size larger than the maximum allowed size.

   ============================================================*/
{

	m_maximumSize = maximumSize;

}

CSize CDiagramEntity::GetMaximumSize() const
/* ============================================================
	Function :		CDiagramEntity::GetMaximumSize
	Description :	Returns the maximum size for instances of 
					this object.
					
	Return :		CSize	-	The maximum allowed size.
	Parameters :	none

	Usage :			Call to get the maximum size of the object.
					It is not possible to resize an object to a 
					size larger than the maximum allowed size.

   ============================================================*/
{

	return m_maximumSize;

}

void CDiagramEntity::SetConstraints( CSize min, CSize max )
/* ============================================================
	Function :		CDiagramEntity::SetConstraints
	Description :	Sets the minimum and maximum sizes for 
					instances of this object. -1 means no 
					constraints.
					
	Return :		void
	Parameters :	CSize min	-	Minimum size
					CSize max	-	Maximum size
					
	Usage :			Call to set the minimum and maximum sizes 
					of the object.
					It is not possible to resize an object to 
					smaller or bigger than the min- and max 
					size.

   ============================================================*/
{

	m_minimumSize = min;
	m_maximumSize = max;

}

CDiagramEntityContainer* CDiagramEntity::GetParent() const
/* ============================================================
	Function :		CDiagramEntity::GetParent
	Description :	Returns a pointer to the parent container.
					
	Return :		CDiagramEntityContainer*	-	Parent
													container.
	Parameters :	none

	Usage :			Call to get the parent of the object.

   ============================================================*/
{

	return m_parent;

}

void CDiagramEntity::AddPropertyPage( CDiagramPropertyPage* page)
/* ============================================================
	Function :		CDiagramEntity::AddPropertyDialog
	Description :	Add the property page pointer.
					
	Return :		void
	Parameters :	CDiagramPropertyPage* page -	a pointer 
													to a page
													instance. 
					UINT dlgresid				-	The resource 
													id of the 
													dialog template.
					
	Usage :			Call to add a property page for the object 
					(normally in the ctor). 

   ============================================================*/
{
	if (page) {
		page->SetEntity(this);
		m_propertysheet.AddPage(page);
	}
}

CDiagramPropertySheet* CDiagramEntity::GetPropertySheet() 
/* ============================================================
	Function :		CDiagramEntity::GetPropertyDialog
	Description :	Returns a pointer to the class property 
					dialog.
					
	Return :		CDiagramPropertyDlg*	-	The dialog 
												pointer. NULL 
												if none.
	Parameters :	none

	Usage :			Call to get a pointer to the object property 
					dialog.

   ============================================================*/
{

	return &m_propertysheet;

}

double CDiagramEntity::GetZoom() const
/* ============================================================
	Function :		CDiagramEntity::GetZoom
	Description :	Returns the zoom level for the object.
					
	Return :		double	-	
	Parameters :	none

	Usage :			Internal function. Can be called by derived 
					classes to get the zoom level. The zoom 
					level is set by the owning editor when 
					drawing the object, is read-only and this 
					function should only be called from Draw.

   ============================================================*/
{

	return m_zoom;

}

void CDiagramEntity::SetZoom( double zoom )
/* ============================================================
	Function :		CDiagramEntity::SetZoom
	Description :	Sets the zoom level
					
	Return :		nothing
	Parameters :	double zoom	-	The new zoom level
					
	Usage :			Internal call.

   ============================================================*/
{
	m_zoom = zoom;
}

CString CDiagramEntity::Export( UINT /*format*/ ) const
/* ============================================================
	Function :		CDiagramEntity::Export
	Description :	Exports the object to format
					
	Return :		CString		-	The object representation 
									in format.
	Parameters :	UINT format	-	The format to export to.
					
	Usage :			Virtual function to allow easy exporting of 
					the objects to different text based formats.

   ============================================================*/
{

	return _T( "" );

}