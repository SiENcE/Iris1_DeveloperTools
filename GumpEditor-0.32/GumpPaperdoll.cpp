#include "StdAfx.h"
#include "GumpEditor.h"
#include ".\gumppaperdoll.h"


CGumpPaperdoll::CGumpPaperdoll(CGumpPtr pGump) : CGumpPicture(NULL)
{
	SetGump(pGump);

	SetTitle("paperdoll");
	SetType("paperdoll");
	
	CString strName;
	strName.Format("paperdoll_%x", pGump ? pGump->GetGumpID() : 0);
	SetName(strName);

	//AddPropertyPage( &m_page );
}

CGumpPaperdoll::~CGumpPaperdoll(void)
{
}


CDiagramEntity* CGumpPaperdoll::Clone()
{
	CGumpPaperdoll* obj = new CGumpPaperdoll(m_pGump);
	obj->Copy( this );
	return obj;
}

CDiagramEntity* CGumpPaperdoll::CreateFromString( XML::Node* node )
{
	CGumpPaperdoll* obj = new CGumpPaperdoll(NULL);
	if(!obj->FromString( node ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;
}
