#ifndef _DIAGRAMLINE_H_
#define _DIAGRAMLINE_H_

#include "DiagramEntity.h"

typedef struct {

	int x;
	int y;
	BOOL hit;

} hitParams;

typedef struct {

	CRect rect;
	BOOL hit;

} hitParamsRect;

class CDiagramLine : public CDiagramEntity
{
public:
	CDiagramLine();
	virtual ~CDiagramLine() {};

	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString( const CString& str);
	virtual void			Draw( CDC* dc, CRect rect );
	virtual int				GetHitCode( CPoint point ) const;
	virtual HCURSOR			GetCursor( int hit ) const;
	virtual void			SetRect( CRect rect );
	virtual BOOL			BodyInRect( CRect rect ) const;

protected:

	virtual void	DrawSelectionMarkers( CDC* dc, CRect rect ) const;
	
};

#endif // _DIAGRAMLINE_H_
