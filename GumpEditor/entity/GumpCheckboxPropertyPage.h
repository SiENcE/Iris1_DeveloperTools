#pragma once


// CGumpCheckboxPropertyPage 대화 상자입니다.

#include "DiagramPropertyPage.h"
#include "HoverButton.h"
#include "numspinctrl.h"

class CGumpCheckboxPropertyPage : public CDiagramPropertyPage
{
	DECLARE_DYNAMIC(CGumpCheckboxPropertyPage)

public:
	CGumpCheckboxPropertyPage(CWnd* pParent=NULL);
	virtual ~CGumpCheckboxPropertyPage();

	CHoverButton m_btnTest;

	enum { IDD = IDD_CHECKBOX_DIALOG };

	virtual void SetValues();
	virtual void ApplyValues();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	CString m_strNormalID;
	CString m_strCheckedID;
	CNumSpinCtrl m_spinNormal;
	CNumSpinCtrl m_spinChecked;
	BOOL m_bChecked;
};
