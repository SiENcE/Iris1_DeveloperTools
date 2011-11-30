#pragma once


// CGumpRadioPropertyPage 대화 상자입니다.

#include "DiagramPropertyPage.h"
#include "numspinctrl.h"
#include "afxcmn.h"

class CGumpRadioPropertyPage : public CDiagramPropertyPage
{
	DECLARE_DYNAMIC(CGumpRadioPropertyPage)

public:
	CGumpRadioPropertyPage(CWnd* pParent=NULL);
	virtual ~CGumpRadioPropertyPage();

	CHoverButton m_btnTest;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RADIO_DIALOG };

	virtual void SetValues();
	virtual void ApplyValues();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	CNumSpinCtrl m_spinGroupID;
	int m_iGroupID;
};
