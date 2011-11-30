#pragma once

#include "GumpPicture.h"
//#include "GumpBorderPropertyPage.h"

class CGumpBorder : public CGumpPicture
{
public:
	CGumpBorder(CGumpPtr pGump);
	~CGumpBorder(void);

	//virtual BOOL	FromString( XML::Node* node );
	//virtual CString	GetString(BOOL bBegin) const;
	static	CDiagramEntity* CreateFromString( XML::Node* node );

	// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );

	virtual int GetGumpID(void) const;
	virtual CGumpPtr GetGump() { return m_pGumpB[LT]; }
	virtual void SetGump(CGumpPtr pDib, bool bUpdateRect=true);

protected:
	enum PART { LT = 0, TOP, RT, LC, CENTER, RC, LB, BOTTOM, RB, NUM_PART };
	CGumpPtr m_pGumpB[NUM_PART];
	CSize m_sizeGumpB[NUM_PART];
	//CGumpBorderPropertyPage	m_page;
};
