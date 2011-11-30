
#include "stdafx.h"
#include "GumpEditor.h"
#include "CSLDlg.h"
#include ".\csldlg.h"
#include "GumpEditorDoc.h"
#include "CodeView.h"

CTagInfo CCSLDlg::m_tagInfo;

IMPLEMENT_DYNAMIC(CCSLDlg, CDialog)
CCSLDlg::CCSLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCSLDlg::IDD, pParent)
	, m_strName(_T(""))
{
	m_bLoad = false;
	m_pParent = dynamic_cast<CCodeView*>(pParent);
}

CCSLDlg::~CCSLDlg()
{
}

void CCSLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CTRL_LIST, m_lsControl);
	DDX_Text(pDX, IDC_NAME, m_strName);
}


BEGIN_MESSAGE_MAP(CCSLDlg, CDialog)
	ON_BN_CLICKED(IDC_FIND, &CCSLDlg::OnBnClickedFind)
	ON_BN_CLICKED(IDC_RELOAD_TAG, &CCSLDlg::OnBnClickedReload)
	ON_BN_CLICKED(IDC_INSERT, &CCSLDlg::OnBnClickedInsert)
END_MESSAGE_MAP()



BOOL CCSLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CSortListCtrl& ctrl = m_lsControl;

	ctrl.ModifyStyle(0, LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	ctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	ctrl.SetHeadings( _T("File,100;Line,40;Name,400") );
	ctrl.LoadColumnInfo();

	if (!m_bLoad) {
		OnBnClickedReload();
		m_bLoad = true;
	}

	return TRUE; 
}

void CCSLDlg::OnBnClickedFind()
{
	UpdateData();

	CString strName = m_strName.MakeLower(), strText;

	CSortListCtrl& ctrl = m_lsControl;
	for (int i =0; i < ctrl.GetItemCount(); i++) {
		strText = ctrl.GetItemText(i,2).MakeLower();
		if (strncmp(strText, strName, strName.GetLength())==0) {
			ctrl.SetItemState(i,LVNI_SELECTED,LVNI_SELECTED);
			ctrl.EnsureVisible(i, FALSE);
			break;
		}
		/*else 
			ctrl.SetItemState(i,0,LVNI_SELECTED);*/
	}

}

CString CCSLDlg::GetSelectedFuncName()
{
	CSortListCtrl& ctrl = m_lsControl;
	int i = ctrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	i = ctrl.GetItemData(i);
	if (i < 0 || i >= m_tagInfo.GetSize()) return "";

	return m_tagInfo[i].full_name;
}


BOOL CCSLDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) {
		OnBnClickedFind();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CCSLDlg::OnBnClickedReload()
{
	GfxResetWorkingDirectory();

	CString strTagFile = "iris.tag";

	m_tagInfo.GenerateTagInfo("script/*.csl", strTagFile);

	if (m_tagInfo.ReadTagInfoFromFile(strTagFile) < 0) {
		AfxMessageBox("Can't Open " + strTagFile);
		return;
	}

	CSortListCtrl& ctrl = m_lsControl;

	m_lsControl.DeleteAllItems();

	for (int i = 0;i < m_tagInfo.GetSize(); i++)
	{
		const sTagInfo &tag = m_tagInfo[i];
		
		int iItem = ctrl.AddItem(tag.file, tag.line, tag.full_name);
		ctrl.SetItemData(iItem, i);
	}
}



void CCSLDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::PostNcDestroy();
	// set pointer in parent = NULL
	if (m_pParent) m_pParent->SetCSLDlg(NULL);
	// delete myself - only valid if dialog is modeless
	delete this;
}

void CCSLDlg::OnBnClickedInsert()
{
	// TODO: Add your control notification handler code here
	if (!m_pParent) return;	

	CString strFunc = GetSelectedFuncName();
	if (strFunc.IsEmpty()) return;

	m_pParent->InsertText(strFunc);
}

void CCSLDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pParent) m_pParent->SetCSLDlg(NULL);
	DestroyWindow();
}
