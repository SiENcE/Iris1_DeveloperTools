#pragma once
#pragma warning(disable:4800)

#include "DiagramEditor/DiagramEntity.h"
#include "GumpPropertyPage.h"
#include "xml.h"

class CGumpEntity  : public CDiagramEntity
{
public:
	enum FLAG { DISALBE = 0x1, VISIBLE = 0x2 };

	CGumpEntity(void);
	~CGumpEntity(void) {}

	virtual BOOL	FromString( XML::Node* node );
	virtual CString	GetString(BOOL bBegin) const;
	static	CGumpEntity* CreateFromNode( XML::Node* node );

	// Overrides
	virtual CDiagramEntity* Clone();
	virtual void	Copy( CDiagramEntity* obj );
	virtual void	Draw( CDC* dc, CRect rect );

	enum EVENT { ONCLICK, ONCHANGE, ONDRAGDROP, ONTIMER, NUM_EVENT };

	EVENT GetEventType() const { return m_eEventType; }
	CString GetEventTypeName() const { return GetEventTypeName(m_eEventType); }
	void SetEventType(EVENT e) { m_eEventType = e; }
	void SetEventTypeByName(LPCTSTR szEventTypeName) 
		{ m_eEventType = GetEventTypeByName(szEventTypeName, m_eEventType); }
	
	static CString GetEventTypeName(EVENT e) { return m_szEventTypeNames[e]; }
	static EVENT GetEventTypeByName(CString strEventTypeName, EVENT eDefault = ONCLICK );
	
	CString GetEventHandler() const { return m_strEventHandler; }
	CString GetEventHandlerParams() const { return m_strEventHandlerParams; }
	void SetEventHandler(LPCTSTR szEventHandler) { m_strEventHandler = szEventHandler; }
	void SetEventHandlerParams(LPCTSTR szEventHandlerParams) { m_strEventHandlerParams = szEventHandlerParams; }
	void SetDefaultEventHandler(LPCTSTR szPrefix=NULL);

	int  GetAlpha() const { return m_iAlpha; }
	void SetAlpha(int iAlpha) { m_iAlpha = iAlpha; }

	int  GetFlags() const { return m_iFlags; }
	void SetFlags(int iFlags) { m_iFlags = iFlags; }
	bool IsSetFlags(int iFlags) const { return m_iFlags & iFlags; }
	bool IsFixedSize() const { return GetMaximumSize() == GetMinimumSize(); }

protected:
	CGumpPropertyPage m_page;
	
	CString m_strEventHandler;
	CString m_strEventHandlerParams;
	EVENT m_eEventType;

	int m_iAlpha;
	int m_iFlags;

	static UINT m_iEnitiyCount;
	void SetDefaultEntityName();

	void DrawEdit(CDC* pDC, CRect rect);
	void DrawCombobox(CDC* pDC, CRect rect);
	void DrawButton(CDC* pDC, CRect rect);
	void DrawCheckbox(CDC* pDC, CRect rect);
	void DrawRadiobutton(CDC* pDC, CRect rect);
	void DrawStatic(CDC* pDC, CRect rect);
	void DrawGroupbox(CDC* pDC, CRect rect);


	static LPCTSTR m_szEventTypeNames[NUM_EVENT];
	static LPCTSTR m_szEventTypeParams[NUM_EVENT];

	static int StrToEnum(CString str, LPCTSTR *szEnum, int iEnumSize, int iDefault = 0);
};


#pragma warning(default:4800)