#pragma once

#include "GumpEntity.h"
#include "GumpPicturePropertyPage.h"

class CGumpPicture : public CGumpEntity
{
public:
	enum TYPE { GUMP, FRAME, RECTANGLE, NUM_TYPE };
	CGumpPicture(CGumpPtr pGump, TYPE eType = GUMP, DWORD hueId = 1);
	~CGumpPicture(void);

	virtual BOOL	FromString( XML::Node* node );
	virtual CString	GetString(BOOL bBegin) const;
	static	CGumpEntity* CreateFromNode( XML::Node* node );

	// Overrides

	virtual CDiagramEntity* Clone();
	//virtual void Copy(CDiagramEntity* obj);
	virtual void			Draw( CDC* dc, CRect rect );

	virtual int GetGumpID(void) const;
	virtual CGumpPtr GetGump() { return m_pGump; }
	virtual void SetGump(CGumpPtr pDib, bool bUpdateRect=true);

	DWORD GetHueId() const { return m_hueId; }
	void  SetHueId(DWORD hueId) { m_hueId = hueId; }

	TYPE GetPictureType() const { return m_eType; }
	void SetPictureType(TYPE eType) { m_eType = eType; }

	CString GetPictureTypeStr() const { return m_szPictureType[m_eType]; }
	void SetPictureTypeStr(CString strType);
protected:
	DWORD m_hueId;
	TYPE m_eType;
	CGumpPtr m_pGump;
	CGumpPicturePropertyPage	m_page;

	static LPCTSTR m_szPictureType[NUM_TYPE];
};
