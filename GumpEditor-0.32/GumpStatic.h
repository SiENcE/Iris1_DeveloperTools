#pragma once

#include "GumpEntity.h"
#include "GumpStaticPropertyPage.h"

class CGumpStatic : public CGumpEntity
{
public:
	CGumpStatic(int hueId=1, int fontId=3, TEXT_ALIGN textAlign=ALIGN_LEFT);

	virtual BOOL	FromString( XML::Node* node );
	virtual CString	GetString(BOOL bBegin) const;
	static	CGumpEntity* CreateFromString( XML::Node* node );

	// Overrides
	virtual CGumpEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );

	DWORD GetHueId() const { return m_hueId; }
	void SetHueId(DWORD hueId) { m_hueId = hueId; }
	
	int GetFontId() const { return m_fontId; }
	void SetFontId(int fontId) { m_fontId = fontId; }
	
	TEXT_ALIGN GetTextAlign() const { return m_textAlign; }
	void  SetTextAlign(TEXT_ALIGN align) { m_textAlign = align; }
protected:
	DWORD m_hueId;
	int m_fontId;
	TEXT_ALIGN m_textAlign;
	CGumpStaticPropertyPage	m_page;
};
