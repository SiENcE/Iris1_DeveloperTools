#pragma once

#include "DiagramPropertyPage.h"
#include "numspinctrl.h"

class CGumpBorderPropertyPage : public CDiagramPropertyPage
{
	DECLARE_DYNAMIC(CGumpBorderPropertyPage)

public:
	CGumpBorderPropertyPage();
	virtual ~CGumpBorderPropertyPage();
	
	enum { IDD = IDD_BORDER_DIALOG };

	virtual void SetValues();
	virtual void ApplyValues();
	virtual void ResetValues();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strGumpID;
	virtual BOOL OnInitDialog();
	
	CNumSpinCtrl m_spinCtrl;
};
