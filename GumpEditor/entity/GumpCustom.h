#pragma once

#include "GumpEntity.h"
#include "GumpCustomPropertyPage.h"

class CGumpCustom : public CGumpEntity
{
public:
	CGumpCustom(LPCTSTR szCustom=NULL);
	~CGumpCustom(void);

	virtual BOOL	FromString( XML::Node* node );
	virtual CString	GetString(BOOL bBegin) const;
	static	CGumpEntity* CreateFromNode( XML::Node* node );

	// Overrides

	virtual CDiagramEntity* Clone();
	//virtual void Copy(CDiagramEntity* obj);
	virtual void			Draw( CDC* dc, CRect rect );

	CString GetCustom() const { return m_strCustom; }
	void SetCustom(LPCTSTR szCustom) { m_strCustom = szCustom; }
protected:
	CString  m_strCustom;
	CGumpCustomPropertyPage	m_page;
	CGumpPtr m_pGump;
};
