// GumpControlListDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "GumpControlListDlg.h"
#include ".\gumpcontrollistdlg.h"
#include "GumpEditorDoc.h"
#include "GumpEditorView.h"


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
	ON_BN_CLICKED(IDC_UP, OnBnClickedUp)
	ON_BN_CLICKED(IDC_DOWN, OnBnClickedDown)
	ON_BN_CLICKED(IDC_TOP, OnBnClickedTop)
	ON_BN_CLICKED(IDC_BOTTOM, OnBnClickedBottom)
	ON_BN_CLICKED(IDC_ORDER_CHANGE, OnBnClickedOrderChange)
END_MESSAGE_MAP()



BOOL CGumpControlListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT(GfxGetGumpDocument());
	m_pView = dynamic_cast<CGumpEditorView*>(GfxGetGumpDocument()->FindView(RUNTIME_CLASS(CGumpEditorView))); 
	if (!m_pView) return FALSE;
	
	CDEditor& editor = m_pView->GetDEditor();
	CSortListCtrl& ctrl = m_lsControl;

	ctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	ctrl.SetHeadings( _T("Order,40;Name,80;Type,60;Selected,60;Visible,60;Freezed,60") );
	ctrl.LoadColumnInfo();

	int iItem = 0;
	CDiagramEntity* e=NULL;
	
	for (int i = 0; i < editor.GetObjectCount(); i++) {
		e = editor.GetObject(i); ASSERT(e);
		iItem = ctrl.AddItem(
			GfxSprintf("%d",i),
			e->GetName(), e->GetType(), 
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

void CGumpControlListDlg::SetOnlySelectedItem(int iItem)
{
	CSortListCtrl& ctrl = m_lsControl;

	for (int i =0; i < ctrl.GetItemCount(); i++) 
		if (i != iItem) ctrl.SetItemState(i,0,LVNI_SELECTED);
}


int CGumpControlListDlg::FindOrderedItem(int iOrder)
{
	CSortListCtrl& ctrl = m_lsControl;
	if (iOrder < 0 || iOrder >= ctrl.GetItemCount()) return -1;

	for (int i = 0; i < ctrl.GetItemCount(); i++)
		if (atoi(ctrl.GetItemText(i,ORDER_COLUMN)) == iOrder) return i;

	return -1;
}

int CGumpControlListDlg::GetSelectedItemOrder()
{
	CSortListCtrl& ctrl = m_lsControl;
	int iItem = GetSelectedItem();
	if (iItem < 0) return -1;

	return atoi(ctrl.GetItemText(iItem, ORDER_COLUMN));
}

bool CGumpControlListDlg::ChangeItemOrder(int iOrder,int iNewOrder)
{
	CSortListCtrl& ctrl = m_lsControl;
	int iItemCount = ctrl.GetItemCount();
	
	if (iOrder < 0 || iOrder >= iItemCount) return false;
	if (iNewOrder < 0 || iNewOrder >= iItemCount) return false;
	if (iOrder == iNewOrder) return true;

	int iDelta = iOrder > iNewOrder ? 1 : -1;

	int iStart = iNewOrder, iEnd = iOrder;

	if (iStart > iEnd) iStart = iOrder, iEnd = iNewOrder;

	for (int i = 0; i < iItemCount; i++) {
		int iCurOrder = atoi(ctrl.GetItemText(i, ORDER_COLUMN));

		if (iCurOrder == iOrder)
			ctrl.SetItemText(i, ORDER_COLUMN, GfxSprintf("%d", iNewOrder));
		else // except iCurOrder == iOrder
		if (iCurOrder >= iStart && iCurOrder <= iEnd) 
			ctrl.SetItemText(i, ORDER_COLUMN, GfxSprintf("%d", iCurOrder+iDelta));
	}

	return true;
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

	CString strName = m_strName.MakeLower(), strText;

	CSortListCtrl& ctrl = m_lsControl;
	for (int i =0; i < ctrl.GetItemCount(); i++) {
		strText = ctrl.GetItemText(i,NAME_COLUMN).MakeLower();
		if (strncmp(strText, strName, strName.GetLength())==0)
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


void CGumpControlListDlg::OnBnClickedUp()
{
	int iOrder = GetSelectedItemOrder();
	if (iOrder < 0) return;
	SetOnlySelectedItem(GetSelectedItem());
	ChangeItemOrder(iOrder, iOrder-1);
}

void CGumpControlListDlg::OnBnClickedDown()
{
	int iOrder = GetSelectedItemOrder();
	if (iOrder < 0) return;
	SetOnlySelectedItem(GetSelectedItem());
	ChangeItemOrder(iOrder, iOrder+1);
}

void CGumpControlListDlg::OnBnClickedTop()
{
	int iOrder = GetSelectedItemOrder();
	if (iOrder < 0) return;
	SetOnlySelectedItem(GetSelectedItem());
	ChangeItemOrder(iOrder, 0);
}

void CGumpControlListDlg::OnBnClickedBottom()
{
	int iOrder = GetSelectedItemOrder();
	if (iOrder < 0) return;
	SetOnlySelectedItem(GetSelectedItem());
	ChangeItemOrder(iOrder, m_lsControl.GetItemCount()-1);
}

void CGumpControlListDlg::OnBnClickedOrderChange()
{
	CDEditor& editor = m_pView->GetDEditor();
	CSortListCtrl& ctrl = m_lsControl;

	CUIntArray arOrder;
	arOrder.SetSize(ctrl.GetItemCount());

	for (int i =0; i < ctrl.GetItemCount(); i++)
		arOrder[ctrl.GetItemData(i)] = atoi(ctrl.GetItemText(i, ORDER_COLUMN));

	editor.Reorder(arOrder);

	editor.SetModified(TRUE);
	editor.RedrawWindow();

	OnOK();
}


#if 0
	swap item

	for (int i = 0; i < ctrl.GetItemCount(); i++) {
		CString strText = ctrl.GetItemText(iItem,i);
		ctrl.SetItemText(iItem,i,ctrl.GetItemText(iItem-1,i));
		ctrl.SetItemText(iItem-1,i,strText);
	}
	DWORD data = ctrl.GetItemData(iItem);
	ctrl.SetItemData(iItem,ctrl.GetItemData(iItem-1));
	ctrl.SetItemData(iItem-1,data);
	
	ctrl.SetItemState(iItem-1, LVNI_SELECTED, LVNI_SELECTED);
	ctrl.SetItemState(iItem, 0, LVNI_SELECTED);
#endif