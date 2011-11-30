#include "stdafx.h"
#include "GumpEditor.h"
#include "ToolBarEx.h"
#include "GumpEditorDoc.h"
#include ".\toolbarex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarWithCombo

BEGIN_MESSAGE_MAP(CToolBarWithCombo, CToolBar)
	//{{AFX_MSG_MAP(CToolBarWithCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CToolBarWithCombo::CreateComboBox(CComboBox& comboBox, UINT nIndex, UINT nID, 
								int nWidth, int nDropHeight)
{
	// Create the combo box
	SetButtonInfo(nIndex, nID, TBBS_SEPARATOR, nWidth);

	CRect rect;
	GetItemRect(nIndex, &rect);
	rect.top = 1;
	rect.bottom = rect.top + nDropHeight;
	if (!comboBox.Create(
			CBS_DROPDOWN|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL,
			rect, this, nID))
	{
		TRACE("Failed to create combo-box\n");
		return FALSE;
	}

	return TRUE;
}

BOOL CToolBarWithCombo::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if( wParam == IDOK && lParam == 0 )
	{
		CString strText;
		CString strCommand;
		CComboBox* rgComboBox[] = {&m_comboBox1};
		for( int index = 0; index < sizeof(rgComboBox) / sizeof(rgComboBox[0]); ++index )
		{
			if( rgComboBox[index]->GetEditSel() != 0 )
			{
				rgComboBox[index]->GetWindowText(strText);
				rgComboBox[index]->AddString(strText);
				//rgComboBox[index]->SetWindowText(_T(""));

				GfxGetGumpDocument()->SelectGumpList(GfxAtoX(strText));
			}
		}
	}
	
	return CToolBar::OnCommand(wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CSmartComboBox

CSmartComboBox::CSmartComboBox()
{
	m_nMaxStrings = 10;
}

BEGIN_MESSAGE_MAP(CSmartComboBox, CComboBox)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CSmartComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	SendMessage(WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT));

	return 0;
}


int CSmartComboBox::AddString(LPCTSTR str)
{
	if( _tcslen(str) == 0 )
		return -1;

	int oldIndex = FindStringExact(-1, str);
	if( oldIndex >= 0 )
		DeleteString(oldIndex);

	if( GetCount() == m_nMaxStrings )
		DeleteString(m_nMaxStrings-1);

	return CComboBox::InsertString(0, str);
}

int CSmartComboBox::InsertString(int index, LPCTSTR str)
{
	if( _tcslen(str) == 0 )
		return -1;

	int oldIndex = FindStringExact(-1, str);
	if( oldIndex >= 0 )
	{
		DeleteString(oldIndex);
		if( index >= oldIndex )
			--index;
	}

	if( GetCount() == m_nMaxStrings )
		DeleteString(m_nMaxStrings-1);

	return CComboBox::InsertString(index, str);
}

/////////////////////////////////////////////////////////////////////////////

void CSmartComboBox::OnEditPaste()
{
	char * buffer = NULL;
	//open the clipboard
	CString fromClipboard;
	if ( OpenClipboard() ) 
	{
		HANDLE hData = GetClipboardData( CF_TEXT );
		char * buffer = (char*)GlobalLock( hData );
		fromClipboard = buffer;
		GlobalUnlock( hData );
		CloseClipboard();
	}

	if (!fromClipboard.IsEmpty()) {
		SetWindowText(fromClipboard);
	}
}

BOOL CSmartComboBox::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (GetAsyncKeyState(VK_CONTROL)) {
			if (pMsg->wParam == VkKeyScan('v')) {
				Paste();
			} else if (pMsg->wParam == VkKeyScan('c')) {
				Copy();
			} else if (pMsg->wParam == VkKeyScan('x')) {
				Cut();
			} 

			return TRUE;
		}
	}

	return CComboBox::PreTranslateMessage(pMsg);
}


