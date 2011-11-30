#pragma once
#include "afxcmn.h"

#include "SortListCtrl.h"

class CGumpEditorView;

class CGumpControlListDlg : public CDialog
{
	DECLARE_DYNAMIC(CGumpControlListDlg)

public:
	CGumpControlListDlg(CWnd* pParent = NULL); 
	virtual ~CGumpControlListDlg();

	enum { IDD = IDD_CTRL_LIST_DIALOG };

	CGumpEditorView* m_pView;
	CString m_strName;
	CSortListCtrl m_lsControl;

	enum UPDATE_TYPE { SELECT, VISIBLE, FREEZE };
	enum COLUMN { ORDER_COLUMN = 0, NAME_COLUMN };
	void UpdateEntity(UPDATE_TYPE updateType, BOOL bValue);

	int GetSelectedItem(int nItem = -1)
	{
		return m_lsControl.GetNextItem(nItem, LVNI_ALL | LVNI_SELECTED);
	}

	int FindOrderedItem(int iOrder);
	int GetSelectedItemOrder();
	bool ChangeItemOrder(int iOrder, int iNewOrder);
	void SetOnlySelectedItem(int iItem);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
public:
	

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedSelect();
	afx_msg void OnBnClickedUnselect();
	afx_msg void OnBnClickedUnfreeze();
	afx_msg void OnBnClickedHide();
	afx_msg void OnBnClickedUnhide();
	afx_msg void OnBnClickedAll();
	afx_msg void OnBnClickedNone();
	afx_msg void OnBnClickedInvert();
	afx_msg void OnBnClickedFreeze();
	afx_msg void OnBnClickedFind();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedTop();
	afx_msg void OnBnClickedBottom();
	afx_msg void OnBnClickedOrderChange();
};
