#pragma once
#include "afxcmn.h"

#include "TagInfo.h"
#include "SortListCtrl.h"


class CCodeView;

class CCSLDlg : public CDialog
{
	DECLARE_DYNAMIC(CCSLDlg)

public:
	CCSLDlg(CWnd* pParent = NULL); 
	virtual ~CCSLDlg();

	enum { IDD = IDD_CSL_DIALOG };

	static CTagInfo m_tagInfo;
	CString GetSelectedFuncName();

	CSortListCtrl m_lsControl;
	CString m_strName;
	bool m_bLoad;

	CCodeView *m_pParent;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedFind();
	afx_msg void OnBnClickedReload();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnBnClickedInsert();
protected:
	virtual void OnCancel();
};
