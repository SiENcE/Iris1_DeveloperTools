#pragma once
#include "DiagramPropertyPage.h"
#include "numspinctrl.h"


class CGumpSliderPropertyPage :public CDiagramPropertyPage
{
	DECLARE_DYNAMIC(CGumpSliderPropertyPage)

public:
	CGumpSliderPropertyPage();
	virtual ~CGumpSliderPropertyPage();

	enum { IDD = IDD_SLIDER_DIALOG };

	virtual void SetValues();
	virtual void ApplyValues();
	virtual void ResetValues();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bVertical;
	int m_iMin;
	int m_iMax;
	int m_iPos;
	CString m_strTrackID;
	CString m_strThumbID;
	CNumSpinCtrl m_spinMin;
	CNumSpinCtrl m_spinMax;
	CNumSpinCtrl m_spinPos;
	CNumSpinCtrl m_spinTrack;
	CNumSpinCtrl m_spinThumb;
	virtual BOOL OnInitDialog();
};
