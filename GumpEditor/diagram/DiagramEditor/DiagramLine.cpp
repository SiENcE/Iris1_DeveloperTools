/* ==========================================================================
	CDiagramLine

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-30

	Purpose :		Encapsulates a line object. Other line objects can be 
					derived from this class.

	Description :	First of all, we do not want constraints to the line 
					size (as we can't move the end points freely if that 
					is the case), so one SetRect must be modified. Second,
					we need a non-rectangular body area for hit testing, a 
					line in this case. Third, we need only a subset of the 
					selection markers. All this is implemented in this 
					class, to serve as a model or base class for other line 
					objects.

	Usage :			Use as a model for line objects.
   ========================================================================*/

#include "stdafx.h"
#include "DiagramLine.h"
#include "Tokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

VOID CALLBACK HitTest( int X, int Y, LPARAM data )
/* ============================================================
	Function :		HitTest
	Description :	Checks if the coordinate in the hitParams 
					struct falls within +/- 1 of the x, y 
					coordinates of this point of the line.
					
	Return :		nothing
	Parameters :	int X		-	Current x coordinate
					int Y		-	Current y coordinate
					LPARAM data	-	Pointer to a hitParams
									struct containing the x,y 
									coordinates to check. The 
									hit element will be set to 
									TRUE if the condition is 
									met.
					
	Usage :			LineDDA callback function. Called for each 
					point of the line

   ============================================================*/
{

	hitParams* obj = ( hitParams* ) data;

	if( obj->x >= X - 1 && obj->x <= X + 1 && obj->y >= Y - 1 && obj->y <= Y + 1 )
		obj->hit = TRUE;

}

VOID CALLBACK HitTestRect( int X, int Y, LPARAM data )
/* ============================================================
	Function :		HitTestRect
	Description :	Checks if the x, y coordinates of the line 
					falls withing the hitParamsRect rectangle.
					
	Return :		nothing
	Parameters :	int X		-	Current x coordinate
					int Y		-	Current y coordinate
					LPARAM data	-	Pointer to a hitParamsRect
									struct containing the rect
									to check. The hit element 
									will be set to TRUE if the 
									condition is met.
					
	Usage :			LineDDA callback function. Called for each 
					point of the line.

   ============================================================*/
{

	hitParamsRect* obj = ( hitParamsRect* ) data;
	CPoint pt( X, Y );

	if( obj->rect.PtInRect( pt ) )
		obj->hit = TRUE;

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramLine
//

CDiagramLine::CDiagramLine()
/* ============================================================
	Function :		CDiagramLine::CDiagramLine
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetMinimumSize( CSize( -1, -1 ) );
	SetType( _T( "line" ) );

}

CDiagramEntity* CDiagramLine::Clone()
/* ============================================================
	Function :		CDiagramLine::Clone
	Description :	Clones this object and returns a new one.
					
	Return :		CDiagramEntity*	-	The resulting clone
	Parameters :	none

	Usage :			

   ============================================================*/
{

	CDiagramLine* obj = new CDiagramLine;
	obj->Copy( this );
	return obj;

}

void CDiagramLine::SetRect( CRect rect )
/* ============================================================
	Function :		CDiagramLine::SetRect
	Description :	Sets the rect of the object.
					
	Return :		void
	Parameters :	CRect rect	-	
					
	Usage :			Overriden to avoid normalization.

   ============================================================*/
{

	SetLeft( rect.left );
	SetTop( rect.top );
	SetRight( rect.right );
	SetBottom( rect.bottom );

}

BOOL CDiagramLine::BodyInRect( CRect rect ) const
/* ============================================================
	Function :		CDiagramLine::BodyInRect
	Description :	Checks if some part of the body of this 
					object lies inside the rectangle rect.
					
	Return :		BOOL		-	TRUE if any part of the 
									object lies inside rect.
	Parameters :	CRect rect	-	The rect to test.
					
	Usage :			Shows one way to test a non-rectangular 
					object body - in this case a line.

   ============================================================*/
{

	BOOL result = FALSE;
	hitParamsRect hit;
	hit.rect = rect;
	hit.hit = FALSE;

	LineDDA(	( int ) GetLeft(), 
				( int ) GetTop(), 
				( int ) GetRight(), 
				( int ) GetBottom(), 
				HitTestRect, 
				( LPARAM ) &hit );

	if( hit.hit )
		result = TRUE;

	return result;

}

int CDiagramLine::GetHitCode( CPoint point ) const
/* ============================================================
	Function :		CDiagramLine::GetHitCode
	Description :	Returns the hit point constant (DEHT_, 
					defined in DiagramEntity.h) for point. 
					
	Return :		int				-	The resulting hit point 
										constant, DEHT_NONE if 
										none.
	Parameters :	CPoint point	-	The point to test.
					
	Usage :			Shows one way to hit point test a non-
					rectangular area.

   ============================================================*/
{

	int result = DEHT_NONE;

	CRect rect = GetRect();

	hitParams hit;
	hit.hit = FALSE;
	hit.x = point.x;
	hit.y = point.y;

	LineDDA(	( int ) GetLeft(), 
				( int ) GetTop(), 
				( int ) GetRight(), 
				( int ) GetBottom(), 
				HitTest, 
				(LPARAM) &hit );

	if( hit.hit )
		result = DEHT_BODY;

	CRect rectTest;

	rectTest = GetSelectionMarkerRect( DEHT_TOPLEFT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_TOPLEFT;

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_BOTTOMRIGHT;

	return result;

}

void CDiagramLine::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramLine::Draw
	Description :	Draws the object.
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	True (zoomed) rectangle to 
									draw to.
					
	Usage :			

   ============================================================*/
{

	dc->SelectStockObject( BLACK_PEN );

	dc->MoveTo( rect.TopLeft() );
	dc->LineTo( rect.BottomRight() );

}

HCURSOR CDiagramLine::GetCursor( int hit ) const
/* ============================================================
	Function :		CDiagramLine::GetCursor
	Description :	Returns the cursor for a specific hit 
					point.
					
	Return :		HCURSOR	-	The cursor to display, or NULL 
								if default.
	Parameters :	int hit	-	The hit point constant (DEHT_, 
								defined in DiagramEntity.h) to 
								show a cursor for.
					
	Usage :			Shows the cursor for a subset of the hit 
					points. Will also show cursors different 
					from the standard ones.

   ============================================================*/
{

	HCURSOR cursor = NULL;
	switch( hit )
	{
		case DEHT_BODY:
		cursor = LoadCursor( NULL, IDC_SIZEALL );
		break;
		case DEHT_TOPLEFT:
		cursor = LoadCursor( NULL, IDC_SIZEALL );
		break;
		case DEHT_BOTTOMRIGHT:
		cursor = LoadCursor( NULL, IDC_SIZEALL );
		break;
	}

	return cursor;

}

void CDiagramLine::DrawSelectionMarkers( CDC* dc, CRect rect ) const
/* ============================================================
	Function :		CDiagramLine::DrawSelectionMarkers
	Description :	Draws selection markers for this object.
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to.
					CRect rect	-	True object rectangle.
					
	Usage :			Draws a subset of the standard selection 
					markers.

   ============================================================*/
{

	// Draw selection markers
	CRect rectSelect;
	dc->SelectStockObject( BLACK_BRUSH );

	rectSelect = GetSelectionMarkerRect( DEHT_TOPLEFT, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT, rect );
	dc->Rectangle( rectSelect );

}

CDiagramEntity* CDiagramLine::CreateFromString( const CString& str )
/* ============================================================
	Function :		CDiagramLine::CreateFromString
	Description :	Static factory function to create a 
					CDiagramLine object from str.
					
	Return :		CDiagramEntity*		-	Resulting object, 
											NULL if str is not 
											a representation of 
											a CDiagramLine.
	Parameters :	const CString& str	-	String representation 
											to decode.
					
	Usage :			One proposed mechanism for loading/creating 
					CDiagramEntity-derived objects from a text 
					stream.

   ============================================================*/
{

	CDiagramLine* obj = new CDiagramLine;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

