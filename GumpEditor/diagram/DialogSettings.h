#pragma once

#include "../resource.h"
#include "../NumSpinCtrl.h"
#include "afxcmn.h"

class CDialogSettings : public CDialog
{
public:
	enum { IDD = IDD_DIALOG_SETTINGS };
	int	m_height;
	int	m_width;
	int	m_gridHeight;
	int	m_gridWidth;
	int	m_marginLeft;
	int	m_marginTop;
	int	m_marginBottom;
	int	m_marginRight;
	int	m_iAlpha;
	int	m_iFlags;
	int m_iFadeAlpha;
	int m_iFadeTime;

	CString m_strName;
	CString m_strEvClick;
	CString m_strEvClose;
	CString m_strEvMouseUp;
	CString m_strEvMouseDown;
	CString m_strEvKeyPressed;
	
	CNumSpinCtrl m_spinAlpha;
	CNumSpinCtrl m_spinFormWidth;
	CNumSpinCtrl m_spinFormHeight;
	CNumSpinCtrl m_spinGridWidth;
	CNumSpinCtrl m_spinGridHeight;
	CNumSpinCtrl m_spinFadeAlpha;
	CNumSpinCtrl m_spinFadeTime;
	CNumSpinCtrl m_spinLeft;
	CNumSpinCtrl m_spinTop;
	CNumSpinCtrl m_spinRight;
	CNumSpinCtrl m_spinBottom;

	CDialogSettings(CWnd* pParent =NULL);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
		
	CString m_strShapeName;
};

