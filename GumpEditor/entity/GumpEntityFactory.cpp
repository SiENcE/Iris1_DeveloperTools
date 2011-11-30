#include "StdAfx.h"
#include "GumpEditor.h"
#include "GumpEditorDoc.h"

#include "GumpEntityFactory.h"
#include "GumpStatic.h"
#include "GumpPicture.h"
#include "GumpButton.h"
#include "GumpEdit.h"
#include "GumpBorder.h"
#include "GumpPaperdoll.h"
#include "GumpScrollbar.h"
#include "GumpListbox.h"
#include "GumpCustom.h"


CGumpEntityFactory::CGumpEntityFactory(void)
{
}

CGumpEntityFactory::~CGumpEntityFactory(void)
{
}


LPCTSTR CGumpEntityFactory::m_szTypeNames[CGumpEntityFactory::NUM_TYPE] =
{
	"EDIT", "STATIC", "CUSTOM",
	"PICTURE", "BORDER", "PAPERDOLL", 
	"BUTTON", "CHECKBOX", "RADIO", "LISTBOX",
	"VSLIDER", "HSLIDER", "VSCROLLBAR", "HSCROLLBAR"
};

LPCTSTR CGumpEntityFactory::GetTypeName(TYPE type)
{
	return m_szTypeNames[type];
}

CGumpEntityFactory::TYPE CGumpEntityFactory::GetTypeByName(CString strTypeName, TYPE eDefault)
{
	for (int i = 0; i < NUM_TYPE; i++)
		if (strTypeName.CompareNoCase(m_szTypeNames[i]) == 0) return static_cast<TYPE>(i);

	return eDefault;
}
 
CGumpEntity* CGumpEntityFactory::CreateFromType(TYPE type, CGumpPtr pGump)
{
	CGumpEditorDoc* pDoc = GfxGetGumpDocument(); ASSERT(pDoc);
	CGumpEntity* obj=NULL;
	CGumpPtr pGump2;
	
	switch (type) 
	{
	case PICTURE :
		obj = new CGumpPicture(pGump);
		break;
	case BORDER :
		obj = new CGumpBorder(pGump);
		break;
	case PAPERDOLL :
		obj = new CGumpPaperdoll(pGump);
		break;
	case BUTTON :
		if (pGump) pGump2 = pDoc->FindGump(pGump->GetGumpID()+1,pGump->GetWidth(),pGump->GetHeight());
		obj = new CGumpButton(pGump, pGump, pGump2 ? pGump2 : pGump, CGumpButton::BUTTON);
		break;
	case CHECKBOX :
		if (pGump) pGump2 = pDoc->FindGump(pGump->GetGumpID()+1,pGump->GetWidth(),pGump->GetHeight());
		obj = new CGumpButton(pGump, pGump, pGump2 ? pGump2 : pGump, CGumpButton::CHECKBOX);
		break;
	case RADIO :
		if (pGump) pGump2 = pDoc->FindGump(pGump->GetGumpID()+1,pGump->GetWidth(),pGump->GetHeight());
		obj = new CGumpButton(pGump, pGump, pGump2 ? pGump2 : pGump, CGumpButton::RADIO);
		break;
	case EDIT :
		obj = new CGumpEdit();
		break;
	case STATIC :
		obj = new CGumpStatic();
		break;
	case CUSTOM :
		obj = new CGumpCustom();
		break;
	case VSLIDER :
		obj = new CGumpScrollbar(pGump, true, false);
		break;
	case HSLIDER :
		obj = new CGumpScrollbar(pGump, false, false);
		break;
	case VSCROLLBAR :
		obj = new CGumpScrollbar(pGump, true, true);
		break;
	case HSCROLLBAR :
		obj = new CGumpScrollbar(pGump, false, true);
		break;
	case LISTBOX :
	default:
		break;
	}

	return obj;
}

CGumpEntity* CGumpEntityFactory::CreateFromNode(XML::Node* node)
{
#if 0
	std::string type;
	if (!node->lookupAttribute("class", type)) return FALSE;

	TYPE eType = GetTypeByName(type.c_str());
	CGumpEntity *obj = CreateFromType(eType, NULL);
	if (!obj->FromString(node))
	{
		delete obj;
		obj = NULL;
	}
#endif

	CGumpEntity *obj = NULL;

	obj = CGumpPicture::CreateFromNode(node);
	if (!obj)
		obj = CGumpButton::CreateFromNode(node);
	if (!obj)
		obj = CGumpStatic::CreateFromNode(node);	
	if (!obj)
		obj = CGumpEdit::CreateFromNode(node);
	if (!obj)
		obj = CGumpBorder::CreateFromNode(node);
	if (!obj)
		obj = CGumpPaperdoll::CreateFromNode(node);
	if (!obj)
		obj = CGumpScrollbar::CreateFromNode(node);
	if (!obj)
		obj = CGumpCustom::CreateFromNode(node);


	return obj;
}
