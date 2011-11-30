#if !defined(AFX_DIAGRAMCHECKBOX_H__F29766BB_546B_4DB9_8228_17DDC74885BF__INCLUDED_)
#define AFX_DIAGRAMCHECKBOX_H__F29766BB_546B_4DB9_8228_17DDC74885BF__INCLUDED_

// DiagramCheckbox.h : header file
//

#include "DiagramEditor/DiagramEntity.h"
#include "ButtonPropertyDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDiagramCheckbox window

class CDiagramCheckbox : public CDiagramEntity
{
// Construction
public:
	CDiagramCheckbox();

	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
	virtual CString	Export( UINT format = 0 );


private:
	CButtonPropertyDlg	m_dlg;

};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_DIAGRAMCHECKBOX_H__F29766BB_546B_4DB9_8228_17DDC74885BF__INCLUDED_)
