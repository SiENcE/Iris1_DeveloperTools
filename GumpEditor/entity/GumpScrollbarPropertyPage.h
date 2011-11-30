#pragma once

#include "numspinctrl.h"
#include "DiagramPropertyPage.h"

class CGumpScrollbarPropertyPage : public CDiagramPropertyPage
{
	DECLARE_DYNAMIC(CGumpScrollbarPropertyPage)

public:
	CGumpScrollbarPropertyPage();
	virtual ~CGumpScrollbarPropertyPage();

	enum { IDD = IDD_SCROLLBAR_DIALOG };

	virtual void SetValues();
	virtual void ApplyValues();
	virtual void ResetValues();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	BOOL m_bVertical;
	BOOL m_bUseArrow;
	
	int m_iMin;
	int m_iMax;
	int m_iPos;

	// ref CGumpScrollBar::PART
	enum PART { 
		TRACK=0, THUMB, 
		LTUP_NORMAL, LTUP_HOVER, LTUP_PRESSED, 
		RTDN_NORMAL, RTDN_HOVER, RTDN_PRESSED,
		NUM_PART 
	};

	CString m_strGumpID[NUM_PART];
	CNumSpinCtrl m_spinGumpID[NUM_PART];

	CNumSpinCtrl m_spinMin;
	CNumSpinCtrl m_spinMax;
	CNumSpinCtrl m_spinPos;

	virtual BOOL OnInitDialog();
};
