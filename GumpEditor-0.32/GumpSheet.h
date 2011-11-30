#pragma once



// CGumpSheet

class CGumpSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CGumpSheet)

public:
	CGumpSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CGumpSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CGumpSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void SetValues(void);
};


