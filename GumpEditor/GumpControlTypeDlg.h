#pragma once
//#include "afxwin.h"
#include "entity/gumpentityfactory.h"

class CGumpControlTypeDlg : public CDialog
{
	DECLARE_DYNAMIC(CGumpControlTypeDlg)

public:
	CGumpControlTypeDlg(CWnd* pParent = NULL); 
	virtual ~CGumpControlTypeDlg();

	enum { IDD = IDD_CTRL_TYPE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	CGumpEntityFactory::TYPE GetControlType();
	CListBox m_lbControl;
	virtual BOOL OnInitDialog();
	int m_iCtrlType;
	const int m_iStartCtrlType;
};
 