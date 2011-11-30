#pragma once

// CDiagramPropertySheet

class CDiagramPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CDiagramPropertySheet)

public:
	CDiagramPropertySheet();
	virtual ~CDiagramPropertySheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void SetValues();
	void ApplyValues();
	void ResetValues();
	CButton m_btnApply;
	CButton m_btnReset;
	afx_msg void OnApply();
	afx_msg void OnReset();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


