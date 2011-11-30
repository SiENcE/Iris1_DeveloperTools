#pragma once

class CGumpControlTypeDlg : public CDialog
{
	DECLARE_DYNAMIC(CGumpControlTypeDlg)

public:
	CGumpControlTypeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGumpControlTypeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CTRL_TYPE_DIALOG };
	enum { PICTURE=0, BORDER, PAPERDOLL, BUTTON, CHECKBOX, RADIO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nControlType;
	int GetControlType() { return m_nControlType; }
};
 