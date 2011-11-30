#if !defined(AFX_DIAGRAMEDIT_H__A34BFDFD_1CBE_463B_94EF_907D9123D614__INCLUDED_)
#define AFX_DIAGRAMEDIT_H__A34BFDFD_1CBE_463B_94EF_907D9123D614__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagramEdit.h : header file
//

#include "DiagramEditor/DiagramEntity.h"
#include "EditPropertyDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDiagramEdit window

class CDiagramEdit : public CDiagramEntity
{
// Construction
public:
	CDiagramEdit();

	static	CDiagramEntity* CreateFromString( XML::Node* node);

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
	virtual CString	Export( UINT format = 0 );

private:
	CEditPropertyDlg	m_dlg;

};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_DIAGRAMEDIT_H__A34BFDFD_1CBE_463B_94EF_907D9123D614__INCLUDED_)
