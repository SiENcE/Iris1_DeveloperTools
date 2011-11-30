#pragma once

#include "GumpStatic.h"
#include "GumpEditPropertyPage.h"

class CGumpEdit : public CGumpStatic
{
public:
	CGumpEdit(int hueId=1, int fontId=3, TEXT_ALIGN textAlign=ALIGN_LEFT, bool passwordMode=false);

	virtual BOOL	FromString( XML::Node* node );
	virtual CString	GetString(BOOL bBegin) const;
	static	CGumpEntity* CreateFromNode( XML::Node* node );

	// Overrides
	virtual CGumpEntity* Clone();
	virtual void	Draw( CDC* dc, CRect rect );
	
	bool IsPasswordMode() const { return m_passwordMode; }
	void SetPasswordMode(bool passMode) { m_passwordMode = passMode; }
protected:
	bool m_passwordMode;

	CGumpEditPropertyPage m_page;
};
