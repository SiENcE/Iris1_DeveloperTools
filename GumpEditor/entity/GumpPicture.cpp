#include "StdAfx.h"
#include "GumpEditor.h"
#include "StdGrfx.h"
#include ".\gumppicture.h"
#include "GumpEditorDoc.h"

LPCTSTR CGumpPicture::m_szPictureType[CGumpPicture::NUM_TYPE] =
{
	"gump", "frame", "rect"
};

void CGumpPicture::SetPictureTypeStr(CString strType)
{
	m_eType = (TYPE)StrToEnum(strType, m_szPictureType,  NUM_TYPE, m_eType);
}

CGumpPicture::CGumpPicture(CGumpPtr pGump, TYPE eType, DWORD hueId)
{
	SetGump(pGump);

	SetTitle("Picture");
	SetType("picture");
	
	CString strName;
	strName.Format("picture_%x", pGump ? pGump->GetGumpID() : 0);
	SetName(strName);

	SetPictureType(eType);
	SetHueId(hueId);

	AddPropertyPage( &m_page );
}

CGumpPicture::~CGumpPicture(void)
{
}

CDiagramEntity* CGumpPicture::Clone()
{
	CGumpPicture* obj = new CGumpPicture(m_pGump, m_eType, m_hueId);
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
	CGumpEditorDoc* pDoc = GfxGetGumpDocument();
	ASSERT(pDoc);

	if (m_eType == RECTANGLE)
	{
		CBrush brush(pDoc->GetHueColor(GetHueId()));
		dc->FillRect(rect, &brush);
	}	
	else
	if (m_eType == FRAME)
	{
		CBrush brush(pDoc->GetHueColor(GetHueId()));
		dc->FrameRect(rect, &brush);
	}
	else 
	if (m_eType == GUMP && m_pGump) 
	{
		m_pGump->DrawGump(dc, rect, GetZoom());
	}
	else 
	{
		CGumpEntity::Draw(dc,rect);
	}

	//dc->SelectStockObject( ANSI_VAR_FONT );
	
	
}


CGumpEntity* CGumpPicture::CreateFromNode( XML::Node* node )
{

	CGumpPicture* obj = new CGumpPicture(NULL);
	if (!obj->FromString( node ))
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

BOOL CGumpPicture::FromString( XML::Node* node )
{
	if (!CGumpEntity::FromString(node)) return FALSE;
	
	std::string type=m_szPictureType[FRAME];
	int gump=0,hueId=1;
	XML::Node* pic_node = node->findNode("picture");
	if (pic_node) {
		pic_node->lookupAttribute("type", type);
		pic_node->lookupAttribute("hue", hueId);
		pic_node->lookupAttribute("gump", gump); 	
	}
	
	ASSERT(GfxGetGumpDocument());
	SetPictureTypeStr(type.c_str());
	SetHueId(hueId);
	SetGump(GfxGetGumpDocument()->GetGump(gump),false);
	
	return TRUE;
}

CString	CGumpPicture::GetString(BOOL bBegin) const
{
	CString str, pic;
	if (bBegin) {
		str += CGumpEntity::GetString(TRUE) + "\n";
		pic.Format(" <picture type='%s' hue='0x%X' gump='0x%X'/>", GetPictureTypeStr(), GetHueId(), GetGumpID());
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
	SetRect(rect.left, rect.top, rect.left + size.cx, rect.top + size.cy);
}
