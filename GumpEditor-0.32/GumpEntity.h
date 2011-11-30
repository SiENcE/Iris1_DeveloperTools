#pragma once

#include "DiagramEditor/DiagramEntity.h"
#include "GumpPropertyPage.h"

class CGumpEntity  : public CDiagramEntity
{
public:
	CGumpEntity(void);
	~CGumpEntity(void) {}

	virtual BOOL	FromString( XML::Node* node );
	virtual CString	GetString(BOOL bBegin) const;
	static	CDiagramEntity* CreateFromString( XML::Node* node );

	// Overrides
	virtual CDiagramEntity* Clone();
	virtual void	Copy( CDiagramEntity* obj );
	virtual void	Draw( CDC* dc, CRect rect );

	enum EVENT { ONCLICK, ONCLOSE, ONMOUSEDOWN, ONMOUSEUP, ONKEYPRESSED, NUM_EVENT };
	
	CString GetEventHandler(EVENT e) const { return m_strEventHandler[e]; }
	void SetEventHandler(EVENT e, LPCTSTR szEventHandler) { m_strEventHandler[e] = szEventHandler; }
	
	void GetEventHandler(CString& strEvClick, CString& strEvClose, CString& strEvMouseUp, CString& strEvMouseDown, CString& strEvKeyPressed) const;
	void SetEventHandler(LPCTSTR szEvClick, LPCTSTR szEvClose, LPCTSTR szEvMouseUp, LPCTSTR szEvMouseDown, LPCTSTR szEvKeyPressed);

	int GetAlpha() const { return m_iAlpha; }
	void SetAlpha(int iAlpha) { m_iAlpha = iAlpha; }

	int GetFlags() const { return m_iFlags; }
	void SetFlags(int iFlags) { m_iFlags = iFlags; }
	bool IsFixedSize() const { return GetMaximumSize() == GetMinimumSize(); }
protected:
	CGumpPropertyPage m_page;
	CString m_strEventHandler[NUM_EVENT];
	int m_iAlpha;
	int m_iFlags;
};
