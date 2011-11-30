// GumpControlListDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpControlListDlg.h"
#include ".\gumpcontrollistdlg.h"
#include "GumpEditorDoc.h"
#include "GumpEditorView.h"


// CGumpControlListDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGumpControlListDlg, CDialog)
CGumpControlListDlg::CGumpControlListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGumpControlListDlg::IDD, pParent), m_pView(NULL)
	, m_strName(_T(""))
{
}

CGumpControlListDlg::~CGumpControlListDlg()
{
}

void CGumpControlListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CTRL_LIST, m_lsControl);
	DDX_Text(pDX, IDC_NAME, m_strName);
}


BEGIN_MESSAGE_MAP(CGumpControlListDlg, CDialog)
	ON_BN_CLICKED(IDC_SELECT, OnBnClickedSelect)
	ON_BN_CLICKED(IDC_UNSELECT, OnBnClickedUnselect)
	ON_BN_CLICKED(IDC_UNFREEZE, OnBnClickedUnfreeze)
	ON_BN_CLICKED(IDC_HIDE, OnBnClickedHide)
	ON_BN_CLICKED(IDC_UNHIDE, OnBnClickedUnhide)
	ON_BN_CLICKED(IDC_ALL, OnBnClickedAll)
	ON_BN_CLICKED(IDC_NONE, OnBnClickedNone)
	ON_BN_CLICKED(IDC_INVERT, OnBnClickedInvert)
	ON_BN_CLICKED(IDC_FREEZE, OnBnClickedFreeze)
	ON_BN_CLICKED(IDC_FIND, OnBnClickedFind)
END_MESSAGE_MAP()


// CGumpControlListDlg 메시지 처리기입니다.

BOOL CGumpControlListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT(GfxGetGumpDocument());
	m_pView = (CGumpEditorView*)GfxGetGumpDocument()->FindView(RUNTIME_CLASS(CGumpEditorView)); 
	if (!m_pView) return FALSE;
	
	CDEditor& editor = m_pView->GetDEditor();
	CSortListCtrl& ctrl = m_lsControl;

	ctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	ctrl.SetHeadings( _T("Name,80;Type,60;Selected,60;Visible,60;Freezed,60") );
	ctrl.LoadColumnInfo();

	int iItem = 0;
	CDiagramEntity* e=NULL;
	
	for (int i = 0; i < editor.GetObjectCount(); i++) {
		e = editor.GetObject(i); ASSERT(e);
		iItem = ctrl.AddItem(e->GetName(), e->GetType(), 
			e->IsSelected() ? "T" : "F",
			e->IsVisible()  ? "T" : "F",
			e->IsFreezed()  ? "T" : "F");
		ctrl.SetItemData(iItem, i);
	}

	return TRUE; 
}

void CGumpControlListDlg::UpdateEntity(UPDATE_TYPE updateType, BOOL bValue)
{
	CDEditor& editor = m_pView->GetDEditor();
	CSortListCtrl& ctrl = m_lsControl;
	
	editor.Snapshot();

	for (int i =0; i < ctrl.GetItemCount(); i++) {
		if (ctrl.GetItemState(i,LVNI_SELECTED) & LVNI_SELECTED) {
			CDiagramEntity* e = editor.GetObject(ctrl.GetItemData(i));
			if (!e) continue;
			if (updateType==SELECT) {
				e->Select(bValue);
			} else if (updateType==VISIBLE) {
				e->SetVisible(bValue);
			} else if (updateType==FREEZE) {
				e->Freeze(bValue);
			}
		}
	}

	editor.SetModified(TRUE);
	editor.RedrawWindow();

	OnOK();
}

void CGumpControlListDlg::OnBnClickedSelect()
{
	UpdateEntity(SELECT, TRUE);
}

void CGumpControlListDlg::OnBnClickedUnselect()
{
	UpdateEntity(SELECT, FALSE);
}

void CGumpControlListDlg::OnBnClickedFreeze()
{
	UpdateEntity(FREEZE, TRUE);
}


void CGumpControlListDlg::OnBnClickedUnfreeze()
{
	UpdateEntity(FREEZE, FALSE);
}

void CGumpControlListDlg::OnBnClickedHide()
{
	UpdateEntity(VISIBLE, FALSE);
}

void CGumpControlListDlg::OnBnClickedUnhide()
{
	UpdateEntity(VISIBLE, TRUE);
}

void CGumpControlListDlg::OnBnClickedAll()
{
	CSortListCtrl& ctrl = m_lsControl;

	for (int i =0; i < ctrl.GetItemCount(); i++)
		ctrl.SetItemState(i,LVNI_SELECTED,LVNI_SELECTED);
}

void CGumpControlListDlg::OnBnClickedNone()
{
	CSortListCtrl& ctrl = m_lsControl;

	for (int i =0; i < ctrl.GetItemCount(); i++)
		ctrl.SetItemState(i,0,LVNI_SELECTED);
}

void CGumpControlListDlg::OnBnClickedInvert()
{
	CSortListCtrl& ctrl = m_lsControl;

	for (int i =0; i < ctrl.GetItemCount(); i++) {
		UINT iState = ctrl.GetItemState(i,LVNI_SELECTED) & LVNI_SELECTED ? 0 : LVNI_SELECTED;
		ctrl.SetItemState(i,iState,LVNI_SELECTED);
	}
}


void CGumpControlListDlg::OnBnClickedFind()
{
	UpdateData();

	CSortListCtrl& ctrl = m_lsControl;
	for (int i =0; i < ctrl.GetItemCount(); i++) {
		if (ctrl.GetItemText(i,0).CompareNoCase(m_strName)==0)
			ctrl.SetItemState(i,LVNI_SELECTED,LVNI_SELECTED);
		else 
			ctrl.SetItemState(i,0,LVNI_SELECTED);
	}
}

BOOL CGumpControlListDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) {
		OnBnClickedFind();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
