#pragma once

#include "DiagramPropertyPage.h"

class CGumpCustomPropertyPage : public CDiagramPropertyPage
{
	DECLARE_DYNAMIC(CGumpCustomPropertyPage)

public:
	CGumpCustomPropertyPage();
	virtual ~CGumpCustomPropertyPage();

	enum { IDD = IDD_CUSTOM_DIALOG };

	virtual void SetValues();
	virtual void ApplyValues();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strCustom;
};


