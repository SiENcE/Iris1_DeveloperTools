#pragma once
#include "afxcmn.h"


// CAnimSetup dialog

class CAnimSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnimSetupDlg)

public:
	CAnimSetupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnimSetupDlg();

// Dialog Data
	enum { IDD = IDD_ANISETUP_DIALOG };

	void SetTime(int iTime);
	int  GetTime();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	int m_iTime;
public:
	CSliderCtrl m_slider;
	virtual BOOL OnInitDialog();

};
