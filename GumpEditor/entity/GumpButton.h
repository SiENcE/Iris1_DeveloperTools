#pragma once 

#include "GumpStatic.h"
#include "GumpButtonPropertyPage.h"

class CGumpButton : public CGumpStatic
{
	// Construction
public:
	enum TYPE { BUTTON, CHECKBOX, RADIO, NUM_TYPE };
	CGumpButton(CGumpPtr pNormal, CGumpPtr pHover, CGumpPtr pPressed, 
		TYPE eType = BUTTON, bool bChecked = false, bool bGrouped = false);

	virtual BOOL	FromString( XML::Node* node );
	virtual CString	GetString(BOOL bBegin) const;
	static	CGumpEntity* CreateFromNode( XML::Node* node );

	// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );

public:
	enum { TEXT_MARGIN = 10, DEFAULT_SIZE = 100 };

	enum STATE { NORMAL=0, HOVER, PRESSED, NUM_STATE };
	CGumpPtr m_pGump[NUM_STATE];

	bool SetGump(CGumpPtr  pNormal, CGumpPtr  pHover, CGumpPtr  pPressed);
	void GetGump(CGumpPtr &pNormal, CGumpPtr &pHover, CGumpPtr &pPressed);

	int GetGumpID(STATE state) const;
	CGumpPtr GetGump(STATE state);
	void SetGump(STATE state, CGumpPtr pGump);

	TYPE GetButtonType() const { return m_eType; }
	void SetButtonType(TYPE eType);

	CString GetButtonTypeStr() const { return m_szButtonType[m_eType]; }
	void SetButtonTypeStr(CString strType);
	
	bool IsChecked() const { return m_bChecked; }
	void SetChecked(bool bChecked) { m_bChecked = bChecked; }

	bool IsGrouped() const { return m_bGrouped; }
	void SetGrouped(bool bGrouped) { m_bGrouped = bGrouped; }

protected:
	TYPE m_eType;
	bool m_bChecked;
	bool m_bGrouped;
	CGumpButtonPropertyPage	m_page;

	static LPCTSTR m_szButtonType[NUM_TYPE];
};
