// PropView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "PropView.h"
#include ".\propview.h"
#include "gumpeditordoc.h"
#include "HoverButton.h"
#include "DibStatic.h"
#include "dynscreen/dyndroptarget.h"
#include "dynscreen/DynControl.h"

// CPropView

#define IDC_PROPERTYTREE 100

IMPLEMENT_DYNCREATE(CPropView, CView)

CPropView::CPropView() : m_pControl(NULL)
{
}

CPropView::~CPropView()
{
}

BEGIN_MESSAGE_MAP(CPropView, CView)
	ON_WM_SIZE()
	ON_NOTIFY(PTN_ITEMCHANGED, IDC_PROPERTYTREE, OnItemChanged)
END_MESSAGE_MAP()


// CPropView 그리기입니다.

void CPropView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CPropView 진단입니다.

#ifdef _DEBUG
void CPropView::AssertValid() const
{
	CView::AssertValid();
}

void CPropView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CPropView 메시지 처리기입니다.



void CPropView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	DWORD dwStyle;
	CRect rc;

	// PTS_NOTIFY - CPropTree will send notification messages to the parent window
	dwStyle = WS_CHILD|WS_VISIBLE|PTS_NOTIFY;

	// Init the control's size to cover the entire client area
	GetClientRect(rc);

	// Create CPropTree control
	m_Tree.Create(dwStyle, rc, this, IDC_PROPERTYTREE);

	//
	// Create some tree items
	//

	// Create a root item (root items should always be CPropTreeItem object since they
	// can not have properties
	CPropTreeItem* pRoot;

	pRoot = m_Tree.InsertItem(new CPropTreeItem());
	pRoot->SetLabelText(_T("Properties"));
	pRoot->SetInfoText(_T("This is a root level item"));
	pRoot->Expand(); // have this item expanded by default

	// Create a static item
	CPropTreeItem* pItem;

	pItem = m_Tree.InsertItem(new CPropTreeItem(), pRoot);
	pItem->SetLabelText(_T("Sub Item"));
	pItem->SetInfoText(_T("This is a simple subitem"));

	// Create a dropdown combolist box
	CPropTreeItemCombo* pCombo;

	pCombo = (CPropTreeItemCombo*)m_Tree.InsertItem(new CPropTreeItemCombo(), pRoot);
	pCombo->SetLabelText(_T("Combo Item"));
	pCombo->SetInfoText(_T("This is a TRUE/FALSE dropdown combo list"));
	pCombo->CreateComboBoxBool();	// create the ComboBox control and auto fill with TRUE/FALSE values
	pCombo->SetItemValue(TRUE);		// set the combo box to default as TRUE

	// Create another item
	pItem = m_Tree.InsertItem(new CPropTreeItemStatic(), pRoot);
	pItem->SetLabelText(_T("Sub Item 2"));
	pItem->SetInfoText(_T("This is item has child items"));
	pItem->SetItemValue((LPARAM)_T("Text Info"));

	// Create a child item
	pItem = m_Tree.InsertItem(new CPropTreeItem(), pItem);
	pItem->SetLabelText(_T("SubSub"));
	pItem->SetInfoText(_T("This is item has a check box"));
	pItem->HasCheckBox();		// we want this item to have a checkbox
	pItem->Check();				// have the checkbox initially checked

	// Create another item
	pItem = m_Tree.InsertItem(new CPropTreeItem(), pRoot);
	pItem->SetLabelText(_T("Sub Item 3"));

	// Create another root item
	pRoot = m_Tree.InsertItem(new CPropTreeItem());
	pRoot->SetLabelText(_T("Styles"));

	// Create a color item
	CPropTreeItemColor* pColor;
	pColor = (CPropTreeItemColor*)m_Tree.InsertItem(new CPropTreeItemColor(), pRoot);
	pColor->SetLabelText(_T("Color"));
	pColor->SetInfoText(_T("Simple color picker"));
	pColor->SetItemValue((LPARAM)RGB(0xff, 0xff, 0x00)); // default as color yellow

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_Tree.InsertItem(new CPropTreeItemEdit(), pRoot);
	pEdit->SetLabelText(_T("Name"));
	pEdit->SetInfoText(_T("Edit text attribute"));
	pEdit->SetItemValue((LPARAM)_T("This text is editable"));

	pEdit = (CPropTreeItemEdit*)m_Tree.InsertItem(new CPropTreeItemEdit(), pRoot);
	pEdit->SetLabelText(_T("Number"));
	pEdit->SetInfoText(_T("Number edit box"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);	// this allows you to
	// pass in a number in SetItemValue
	pEdit->SetItemValue((LPARAM)56);
}

void CPropView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (IsWindow(m_Tree.GetSafeHwnd()))
		m_Tree.MoveWindow(0,0,cx,cy);
}

void CPropView::UpdatePropView(void)
{
	CGumpEditorDoc* pDoc = GetDocument();
	if (!pDoc) return;
	CDynControl* pControl = pDoc->GetSelectedControl();
	if (pControl == m_pControl) return;

	m_Tree.DeleteAllItems();
	m_pControl = pControl;
	if (!m_pControl) return;

	CWnd* pWnd = m_pControl->GetWindow();
	if (!pWnd) return;

	CPropTreeItem* pRoot;

	pRoot = m_Tree.InsertItem(new CPropTreeItem());
	pRoot->SetLabelText("Properties");
	pRoot->SetInfoText("Control Properties");
	pRoot->Expand(); // have this item expanded by default

	int iControlType = m_pControl->GetControlType();
	if (pWnd->IsKindOf(RUNTIME_CLASS(CHoverButton)))
	{
		CHoverButton* pButton = (CHoverButton*)pWnd;

		CString szLabel[] = {"Normal", "Hover", "Pressed" };
		CHoverButton::STATE btnState[] = {CHoverButton::NORMAL, CHoverButton::HOVER, CHoverButton::PRESSED };

		int iGumpID=-1;
		CPropTreeItemEdit* pEdit;
		CString strInfo;

		for (int i = 0; i < 3; i++)	
		{
			pEdit = (CPropTreeItemEdit*)m_Tree.InsertItem(new CPropTreeItemEdit(), pRoot);
			pEdit->SetLabelText(szLabel[i]);
			pEdit->SetInfoText(szLabel[i] + " State Gump ID");
			pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
			iGumpID = pButton->GetGumpID(btnState[i]);
			if (iGumpID >=0) pEdit->SetItemValue((LPARAM)iGumpID);
		}	
	}
	else
	if (pWnd->IsKindOf(RUNTIME_CLASS(CDibStatic)))
	{
		CDibStatic* pStatic= (CDibStatic*)pWnd;

		int iGumpID=-1;
		CPropTreeItemEdit* pEdit;
		CString strInfo;

		pEdit = (CPropTreeItemEdit*)m_Tree.InsertItem(new CPropTreeItemEdit(), pRoot);
		pEdit->SetLabelText("Gump ID");
		pEdit->SetInfoText("Gump ID");
		pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
		iGumpID = pStatic->GetGumpID();
		if (iGumpID >=0) pEdit->SetItemValue((LPARAM)iGumpID);
		
	}
}

void CPropView::OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult)
{
	LPNMPROPTREE pNMPropTree = (LPNMPROPTREE)pNotifyStruct;
	CPropTreeItem* pItem = pNMPropTree->pItem;
	if (pItem)
	{
		// retrieve pItem's changed data
		CPropTreeItemEdit* pEdit = (CPropTreeItemEdit*)pItem;
		int iGumpID = pEdit->GetItemValue();
		CDibPtr pDib = GetDocument()->LoadGump(iGumpID);
		CDynControl* pControl = GetDocument()->GetSelectedControl();
		CDibStatic* pStatic = (CDibStatic*)pControl->GetWindow();
		pStatic->SetGump(pDib);
	}

	*plResult = 0;
}

