#include "StdAfx.h"
#include "GumpEditor.h"
#include "StdGrfx.h"
#include ".\gumppicture.h"
#include "GumpEditorDoc.h"


CGumpPicture::CGumpPicture(CGumpPtr pGump)
{
	SetGump(pGump);

	SetTitle("Picture");
	SetType("picture");
	
	CString strName;
	strName.Format("picture_%x", pGump ? pGump->GetGumpID() : 0);
	SetName(strName);

	AddPropertyPage( &m_page );
}

CGumpPicture::~CGumpPicture(void)
{
}

CDiagramEntity* CGumpPicture::Clone()
{
	CGumpPicture* obj = new CGumpPicture(m_pGump);
	obj->Copy( this );
	return obj;
}

#if 0
void CGumpPicture::Copy(CDiagramEntity* obj)
{

}
#endif

void CGumpPicture::Draw( CDC* dc, CRect rect )
{
	if (!m_pGump) {
		CGumpEntity::Draw(dc,rect);
		return;
	}
	
	m_pGump->DrawGump(dc, rect, GetZoom());
}


CDiagramEntity* CGumpPicture::CreateFromString( XML::Node* node )
{

	CGumpPicture* obj = new CGumpPicture(NULL);
	if(!obj->FromString( node ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

BOOL CGumpPicture::FromString( XML::Node* node )
{
	if (!CGumpEntity::FromString(node)) return FALSE;
	
	int gump=0;
	XML::Node* gump_node = node->findNode("gump");
	if (gump_node) 
		gump = GfxAtoX(gump_node->asString().c_str()); 
	
	ASSERT(GfxGetGumpDocument());
	SetGump(GfxGetGumpDocument()->LoadGump(gump),false);
	
	return TRUE;
}

CString	CGumpPicture::GetString(BOOL bBegin) const
{
	CString str, pic;
	if (bBegin) {
		str += CGumpEntity::GetString(TRUE) + "\n";
		pic.Format(" <gump>0x%X</gump>", GetGumpID());
		str += pic;
	} else {
		str += CGumpEntity::GetString(FALSE);
	}
	
	return str;
}



int CGumpPicture::GetGumpID(void) const
{
	return m_pGump ? m_pGump->GetGumpID() : -1;
}

void CGumpPicture::SetGump(CGumpPtr pGump, bool bUpdateRect)
{
	m_pGump = pGump;
	if (!pGump || !bUpdateRect) return;

	CRect rect = GetRect();
	CSize size = m_pGump->GetDimensions();
	SetRect(rect.left,rect.top,rect.left+size.cx,rect.top+size.cy);
}
