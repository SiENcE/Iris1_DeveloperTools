#pragma once

#include "GumpEntity.h"
#include "GumpStaticPropertyPage.h"

class CGumpStatic : public CGumpEntity
{
public:
	CGumpStatic(CGumpStatic* pOther=NULL);
	CGumpStatic(int hueId, int fontId=3, TEXT_ALIGN textAlign=ALIGN_LEFT, bool multiLine=false);

	virtual BOOL	FromString( XML::Node* node );
	virtual CString	GetString(BOOL bBegin) const;
	static	CGumpEntity* CreateFromNode( XML::Node* node );

	// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );

	DWORD GetHueId() const { return m_hueId; }
	void SetHueId(DWORD hueId) { m_hueId = hueId; }
	
	int GetFontId() const { return m_fontId; }
	void SetFontId(int fontId) { m_fontId = fontId; }
	
	TEXT_ALIGN GetTextAlign() const { return m_textAlign; }
	void  SetTextAlign(TEXT_ALIGN align) { m_textAlign = align; }
	
	bool IsMultiLine() const { return m_multiLine; }
	void SetMultiLine(bool multiLine) { m_multiLine = multiLine; }
protected:
	DWORD m_hueId;
	int m_fontId;
	TEXT_ALIGN m_textAlign;
	bool m_multiLine;

	CSize GetTextExtend() const;

private:
	CGumpStaticPropertyPage	m_page;
};
