#pragma once 

#include "DiagramEditor/DiagramEntity.h"
#include "ButtonPropertyDlg.h"

// CDiagramButton window

class CDiagramButton : public CDiagramEntity
{
// Construction
public:
	CDiagramButton();

	static	CDiagramEntity* CreateFromString( XML::Node* node );

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
	virtual CString	Export( UINT format = 0 );
	
private:
	CButtonPropertyDlg	m_dlg;
};




