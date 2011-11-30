#ifndef _DIAGRAMENTITY_H_
#define _DIAGRAMENTITY_H_

#include "../DiagramPropertyPage.h"
#include "../DiagramPropertySheet.h"

#define CMD_START			100
#define CMD_CUT				100
#define CMD_COPY			101
#define CMD_DUPLICATE		102
#define CMD_PROPERTIES		103
#define CMD_UP				104
#define CMD_DOWN			105
#define CMD_FRONT			106
#define CMD_BOTTOM			107
#define CMD_FREEZE			108
#define CMD_UNFREEZE_ALL	109
#define CMD_HIDE			110
#define CMD_HIDE_UNSEL		111
#define CMD_UNHIDE_ALL		112
#define CMD_CONTROL_LIST	113
#define CMD_SHOW_CODE		114
#define CMD_END				200

#define DEHT_NONE				0
#define DEHT_BODY				1
#define DEHT_TOPLEFT			2
#define DEHT_TOPMIDDLE			3
#define DEHT_TOPRIGHT			4
#define DEHT_BOTTOMLEFT			5
#define DEHT_BOTTOMMIDDLE		6
#define DEHT_BOTTOMRIGHT		7
#define DEHT_LEFTMIDDLE			8
#define DEHT_RIGHTMIDDLE		9

#define round(a) ( int ) ( a + .5 )

class CDiagramEntityContainer;
class CDiagramPropertyPage;


class CDiagramEntity : public CObject
{

friend class CDiagramEntityContainer;

public:

	// Creation/initialization
	CDiagramEntity();
	virtual ~CDiagramEntity();

protected:

	virtual void	Clear();

public:

	virtual	CDiagramEntity* Clone();
	virtual void	Copy( CDiagramEntity* obj );

	virtual BOOL	FromString( const CString& str );
	virtual CString	Export( UINT format = 0 ) const;
	virtual CString	GetString( BOOL bBegin ) const;
	virtual CString GetString() const;
	virtual CString GetEventHandler() const;
	static	CDiagramEntity* CreateFromString( const CString& str );

	// Object rectangle handling
	virtual CRect	GetRect() const;
	virtual void	SetRect( CRect rect );
	virtual void	SetRect( double left, double top, double right, double bottom );
	virtual void	MoveRect( double x, double y );

	double			GetLeft() const;
	double			GetRight() const;
	double			GetTop() const;
	double			GetBottom() const;

	virtual void	SetLeft( double left );
	virtual void	SetRight( double right );
	virtual void	SetTop( double top );
	virtual void	SetBottom( double bottom );

	virtual void	SetMinimumSize( CSize minimumSize );
	virtual CSize	GetMinimumSize() const;
	virtual void	SetMaximumSize( CSize minimumSize );
	virtual CSize	GetMaximumSize() const;
	virtual void	SetConstraints( CSize min, CSize max );

	double			GetZoom() const;

	// Selection handling
	virtual void	Select( BOOL selected );
	virtual BOOL	IsSelected() const;
	virtual BOOL	BodyInRect( CRect rect ) const;
	virtual BOOL	IsSelectable() const;

	virtual void	Freeze(BOOL freezed);
	virtual BOOL	IsFreezed() const;

	virtual void	SetVisible(BOOL visible);
	virtual BOOL	IsVisible() const; 
 
	// Interaction
	virtual int		GetHitCode( CPoint point ) const;
	virtual BOOL	DoMessage( UINT msg, CDiagramEntity* sender, CWnd* from = NULL );

	// Auxilliary
	virtual void	ShowProperties( CWnd* parent, BOOL show = TRUE );
	virtual void	ShowPopup( CPoint point, CWnd* parent );

	// Visuals
	virtual void	Draw( CDC* dc, CRect rect );
	virtual HCURSOR GetCursor( int hit ) const;
	virtual void	DrawObject( CDC* dc, double zoom );

	// Properties
	virtual CString	GetTitle() const;
	virtual void	SetTitle( CString title );

	virtual CString	GetName() const;
	virtual void	SetName( CString name );

	CString			GetType() const;
	void			SetType( CString type );

protected:

	// Selection
	virtual void	DrawSelectionMarkers( CDC* dc, CRect rect ) const;
	virtual CRect	GetSelectionMarkerRect( UINT marker, CRect rect ) const;

	// Visuals
	void			GetFont( LOGFONT& lf ) const;

	// Properties
	void			SetMarkerSize( CSize markerSize );
	CSize			GetMarkerSize() const;

	void			SetZoom( double zoom );

	void						SetParent( CDiagramEntityContainer* parent );
	CDiagramEntityContainer*	GetParent() const;

	void						AddPropertyPage( CDiagramPropertyPage* page);
	CDiagramPropertySheet*		GetPropertySheet() ;

private:

	// Position
	double	m_left;
	double	m_right;
	double	m_top;
	double	m_bottom;

	// Sizes
	CSize	m_markerSize;
	CSize	m_minimumSize;
	CSize	m_maximumSize;

	// States
	double	m_zoom;
	BOOL	m_selected;
	BOOL	m_freezed;
	BOOL	m_visible;

	// Data
	CString m_type;
	CString m_title;
	CString m_name;

	CDiagramPropertySheet		m_propertysheet;

	CDiagramEntityContainer*	m_parent;

};

#endif // _DIAGRAMENTITY_H_
