#pragma once

#include "GumpPicture.h"

class CGumpPaperdoll : public CGumpPicture
{
public:
	CGumpPaperdoll(CGumpPtr pGump);
	~CGumpPaperdoll(void);

	static	CGumpEntity* CreateFromNode( XML::Node* node );

	// Overrides
	virtual CDiagramEntity* Clone();
	//virtual void			Draw( CDC* dc, CRect rect );
};
