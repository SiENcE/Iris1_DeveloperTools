#pragma once


// CTestPage 대화 상자입니다.

class CTestPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTestPage)

public:
	CTestPage();
	virtual ~CTestPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROPPAGE_LARGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
