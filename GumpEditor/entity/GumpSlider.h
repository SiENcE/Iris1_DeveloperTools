#pragma once

#include "GumpEntity.h"
#include "GumpSliderPropertyPage.h"

class CGumpSlider : public CGumpEntity
{
public:
	CGumpSlider(CGumpPtr pTrack, CGumpPtr pThumb, bool bVertical = false, int iMin=0, int iMax=100, int iPos=0);
	~CGumpSlider(void);

	virtual BOOL	FromString( XML::Node* node );
	virtual CString	GetString(BOOL bBegin) const;
	static	CGumpEntity* CreateFromNode( XML::Node* node );

	// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );

public:
	enum PART { TRACK=0, THUMB, NUM_PART };

	void GetGump(CGumpPtr& pTrack, CGumpPtr& pThumb) const
		{ pTrack = GetGump(TRACK); pThumb = GetGump(THUMB); }
	void SetGump(CGumpPtr pTrack, CGumpPtr pThumb, bool bUpdateRect=true);

	int GetGumpID(PART part) const { return m_pGump[part] ? m_pGump[part]->GetGumpID() : -1; }
	CGumpPtr GetGump(PART part) const { return m_pGump[part]; }
	void SetGump(PART part, CGumpPtr pGump);

	void GetRange(int &iMin, int &iMax) const { iMin=m_iMin; iMax=m_iMax; }
	void SetRange(int iMin, int iMax) { m_iMin=iMin; m_iMax=iMax; }
	
	int  GetPos() const { return m_iPos; }
	void SetPos(int iPos) { m_iPos; }

	bool IsVertical() const { return m_bVertical; }
	void SetVertical(bool bVertical);

protected:
	CGumpPtr m_pGump[NUM_PART];
	int m_iMin, m_iMax, m_iPos;
	bool m_bVertical;

	void RecalSizes();
private:
	CGumpSliderPropertyPage	m_page;
};
