#pragma once

#include "HueWnd.h"

class CHuePickerDlg : public CDialog
{
	DECLARE_DYNAMIC(CHuePickerDlg)

public:
	CHuePickerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHuePickerDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };
	CHueWnd m_wndHue;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
