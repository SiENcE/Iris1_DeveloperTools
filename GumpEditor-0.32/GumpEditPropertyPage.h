#pragma once

#include "DiagramPropertyPage.h"

class CGumpEditPropertyPage :public CDiagramPropertyPage
{
	DECLARE_DYNAMIC(CGumpEditPropertyPage)

public:
	CGumpEditPropertyPage(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGumpEditPropertyPage();

	enum { IDD = IDD_EDIT_DIALOG };
	
	virtual void SetValues();
	virtual void ApplyValues();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	
public:
	virtual BOOL OnInitDialog();
	BOOL m_bPassword;
};
