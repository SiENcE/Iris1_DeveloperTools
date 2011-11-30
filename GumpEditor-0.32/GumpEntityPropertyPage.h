#pragma once


// CGumpEntityPropertyDlg 대화 상자입니다.

class CGumpEntityPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CGumpEntityPropertyDlg)

public:
	CGumpEntityPropertyDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGumpEntityPropertyDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GUMP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
