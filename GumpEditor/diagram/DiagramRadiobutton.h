#if !defined(AFX_DIAGRAMRADIOBUTTON_H__F29766BB_546B_4DB9_8228_17DDC74885BF__INCLUDED_)
#define AFX_DIAGRAMRADIOBUTTON_H__F29766BB_546B_4DB9_8228_17DDC74885BF__INCLUDED_

// DiagramRadiobutton.h : header file
//

#include "DiagramEditor/DiagramEntity.h"
#include "ButtonPropertyDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDiagramRadiobutton window

class CDiagramRadiobutton : public CDiagramEntity
{
// Construction
public:
	CDiagramRadiobutton();

	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
	virtual CString	Export( UINT format = 0 );

private:
	CButtonPropertyDlg	m_dlg;

};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_DIAGRAMRADIOBUTTON_H__F29766BB_546B_4DB9_8228_17DDC74885BF__INCLUDED_)
