#if !defined(AFX_EDITPROPERTYDLG_H__0B4288EF_2332_43C6_84A6_D90C0981FE21__INCLUDED_)
#define AFX_EDITPROPERTYDLG_H__0B4288EF_2332_43C6_84A6_D90C0981FE21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditPropertyDlg.h : header file
//

#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CEditPropertyDlg dialog

class CEditPropertyDlg : public CDiagramPropertyDlg
{
// Construction
public:
	CEditPropertyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditPropertyDlg)
	enum { IDD = IDD_DIALOG_EDIT_PROPERTIES };
	CEdit	m_editName;
	//}}AFX_DATA

	virtual void SetValues();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditPropertyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditPropertyDlg)
	afx_msg void OnButtonApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITPROPERTYDLG_H__0B4288EF_2332_43C6_84A6_D90C0981FE21__INCLUDED_)
