#pragma once

#include "DiagramPropertyPage.h"
#include "NumSpinCtrl.h"

class CGumpPropertyPage : public CDiagramPropertyPage
{
	DECLARE_DYNAMIC(CGumpPropertyPage)

public:
	CGumpPropertyPage();
	virtual ~CGumpPropertyPage();
	virtual void SetValues();
	virtual void ApplyValues();
	virtual void ResetValues();

	enum { IDD = IDD_GUMP_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()

	
	CNumSpinCtrl m_spinX;
	CNumSpinCtrl m_spinY;
	CNumSpinCtrl m_spinW;
	CNumSpinCtrl m_spinH;
	CNumSpinCtrl m_spinAlpha;
	int m_xPos;
	int m_yPos;
	int m_iWidth;
	int m_iHeight;
	CString m_strEvType;
	CString m_strEvHandler;
	CString m_strName;
	CString m_strType;
	int m_iAlpha;
public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedCodeButton();
	BOOL m_bVisible;
	BOOL m_bDisable;
};
