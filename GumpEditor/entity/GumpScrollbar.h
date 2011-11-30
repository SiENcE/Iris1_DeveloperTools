#pragma once

#include "GumpEntity.h"
#include "GumpScrollbarPropertyPage.h"

class CGumpScrollbar : public CGumpEntity
{
public:
	CGumpScrollbar(CGumpPtr pTrack, bool bVertical, 
		bool bUseArrowButton = true, int iMin=0, int iMax=100, int iPos=0);
	CGumpScrollbar(CGumpScrollbar* pOther);
	~CGumpScrollbar(void);

	virtual BOOL	FromString( XML::Node* node );
	virtual CString	GetString(BOOL bBegin) const;
	static	CGumpEntity* CreateFromNode( XML::Node* node );

	// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );

public:
	enum PART { 
		TRACK=0, THUMB, 
		LTUP_NORMAL, LTUP_HOVER, LTUP_PRESSED, 
		RTDN_NORMAL, RTDN_HOVER, RTDN_PRESSED,
		NUM_PART 
	};

	void SetGump(CGumpPtr pGump[], bool bUpdateRect=true);

	int GetGumpID(PART part) const { return m_pGump[part] ? m_pGump[part]->GetGumpID() : -1; }
	CGumpPtr GetGump(PART part) const { return m_pGump[part]; }
	void SetGump(PART part, CGumpPtr pGump, bool bUpdateRect=true);

	void GetRange(int &iMin, int &iMax) const { iMin=m_iMin; iMax=m_iMax; }
	void SetRange(int iMin, int iMax) { m_iMin=iMin; m_iMax=iMax; }
	
	int  GetPos() const { return m_iPos; }
	void SetPos(int iPos) { m_iPos; }

	bool IsVertical() const { return m_bVertical; }
	void SetVertical(bool bVertical);

	bool IsUseArrowButton() const { return m_bUseArrowButton; }
	void SetUseArrowButton(bool bUseArrowButton) { m_bUseArrowButton = bUseArrowButton; }
protected:
	CGumpPtr m_pGump[NUM_PART];
	int m_iMin, m_iMax, m_iPos;
	bool m_bVertical;
	bool m_bUseArrowButton;  

	void RecalSizes();
private:
	CGumpScrollbarPropertyPage	m_page;
};
