#pragma once 

#include "GumpEntity.h"
#include "GumpButtonPropertyPage.h"

class CGumpButton : public CGumpEntity
{
	// Construction
public:
	CGumpButton(CGumpPtr pNormal, CGumpPtr pHover, CGumpPtr pPressed);

	virtual BOOL	FromString( XML::Node* node );
	virtual CString	GetString(BOOL bBegin) const;
	static	CDiagramEntity* CreateFromString( XML::Node* node );

	// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );


public:
	enum STATE { NORMAL=0, HOVER, PRESSED, NUM_STATE };
	CGumpPtr m_pGump[NUM_STATE];

	bool SetGump(CGumpPtr pNormal, CGumpPtr pHover, CGumpPtr pPressed);
	void GetGump(CGumpPtr& pNormal, CGumpPtr& pHover, CGumpPtr& pPressed);

	int GetGumpID(STATE state) const;
	CGumpPtr GetGump(STATE state);
	void SetGump(STATE state, CGumpPtr pGump);

private:
	CGumpButtonPropertyPage	m_page;
};
