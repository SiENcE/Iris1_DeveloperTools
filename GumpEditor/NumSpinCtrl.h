#pragma once


// CNumSpinCtrl

class CNumSpinCtrl : public CSpinButtonCtrl
{
	DECLARE_DYNAMIC(CNumSpinCtrl)

public:
	CNumSpinCtrl();
	virtual ~CNumSpinCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult);
	void SetValueForBuddy(int iNum);
	int GetPos(void);
};


