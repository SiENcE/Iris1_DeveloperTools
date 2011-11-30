#pragma once


// CSetupDlg dialog

class CSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupDlg)

public:
	CSetupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bTextChunk;
	BOOL m_bMesh;
	BOOL m_bObject;
	BOOL m_bObjPtr;
	BOOL m_bBone;
	BOOL m_bBoneAnim;
public:
	BOOL m_bBoneObj;
};
