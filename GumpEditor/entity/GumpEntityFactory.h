#pragma once

#include "xml.h"
#include "entity/gumpentity.h"

class CGumpEntityFactory
{
public:
	enum TYPE 
	{ 
		EDIT, STATIC, CUSTOM,								// gump id 가 필요하지 않은 컨트롤을 앞쪽에 배치
		PICTURE, BORDER, PAPERDOLL, 
		BUTTON, CHECKBOX, RADIO, LISTBOX,
		VSLIDER, HSLIDER, VSCROLLBAR, HSCROLLBAR, 
		NUM_TYPE 
	};
	
	static LPCTSTR GetTypeName(TYPE type);
	static TYPE GetTypeByName(CString strTypeName, TYPE eDefault=PICTURE);
	static int GetTypeCount() { return NUM_TYPE; }

	static CGumpEntity* CreateFromType(TYPE type, CGumpPtr pGump);
	static CGumpEntity* CreateFromNode(XML::Node* node);

protected:
	CGumpEntityFactory(void);
	~CGumpEntityFactory(void);

	static LPCTSTR m_szTypeNames[NUM_TYPE];
};
