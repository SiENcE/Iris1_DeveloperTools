#pragma once

#include "DiagramEditor/DiagramEntity.h"
#include "EditPropertyDlg.h"


class CDiagramCombobox : public CDiagramEntity
{
// Construction
public:
	CDiagramCombobox();

	static	CDiagramEntity* CreateFromString( XML::Node* node );

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
	virtual CString	Export( UINT format = 0 );

private:
	CEditPropertyDlg	m_dlg;


};

