#pragma once

#include "DiagramPropertyPage.h"
#include "HoverButton.h"
#include "numspinctrl.h"
#include "afxwin.h"

class CGumpButtonPropertyPage : public CDiagramPropertyPage
{
	DECLARE_DYNAMIC(CGumpButtonPropertyPage)

public:
	CGumpButtonPropertyPage(CWnd* pParent=NULL);
	virtual ~CGumpButtonPropertyPage();

	CHoverButton m_btnTest;

	enum { IDD = IDD_BUTTON_DIALOG };

	virtual void SetValues();
	virtual void ApplyValues();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CString m_strNormalID;
	CString m_strHoverID;
	CString m_strPressedID;
	CNumSpinCtrl m_spinNormal;
	CNumSpinCtrl m_spinHover;
	CNumSpinCtrl m_spinPressed;
	CComboBox m_strType;
	BOOL m_bChecked;
	BOOL m_bGrouped;
	int m_iType;
};
