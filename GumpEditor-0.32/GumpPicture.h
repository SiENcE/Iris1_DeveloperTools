#pragma once

#include "GumpEntity.h"
#include "GumpPicturePropertyPage.h"

class CGumpPicture : public CGumpEntity
{
public:
	CGumpPicture(CGumpPtr pGump);
	~CGumpPicture(void);

	virtual BOOL	FromString( XML::Node* node );
	virtual CString	GetString(BOOL bBegin) const;
	static	CDiagramEntity* CreateFromString( XML::Node* node );

	// Overrides

	virtual CDiagramEntity* Clone();
	//virtual void Copy(CDiagramEntity* obj);
	virtual void			Draw( CDC* dc, CRect rect );

	virtual int GetGumpID(void) const;
	virtual CGumpPtr GetGump() { return m_pGump; }
	virtual void SetGump(CGumpPtr pDib, bool bUpdateRect=true);
protected:
	CGumpPtr m_pGump;
	CGumpPicturePropertyPage	m_page;
};
