#if !defined(AFX_DIAGRAMGROUPBOX_H__F29766BB_546B_4DB9_8228_17DDC74885BF__INCLUDED_)
#define AFX_DIAGRAMGROUPBOX_H__F29766BB_546B_4DB9_8228_17DDC74885BF__INCLUDED_

// DiagramGroupbox.h : header file
//

#include "DiagramEditor/DiagramEntity.h"
#include "ButtonPropertyDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDiagramGroupbox window

class CDiagramGroupbox : public CDiagramEntity
{
// Construction
public:
	CDiagramGroupbox();

	static	CDiagramEntity* CreateFromString( XML::Node* node );

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
	virtual CString	Export( UINT format = 0 );

private:
	CButtonPropertyDlg	m_dlg;

};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_DIAGRAMGROUPBOX_H__F29766BB_546B_4DB9_8228_17DDC74885BF__INCLUDED_)
