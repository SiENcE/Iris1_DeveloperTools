// LeftView.cpp : implementation of the CGumpListView class
//

#include "stdafx.h"
#include "GumpEditor.h"

#include "GumpEditorDoc.h"
#include "GumpListView.h"

#include <afxadv.h>
#include ".\gumplistview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const TCHAR GUMP_DESC_FILE[] = _T("gumpid.txt");

// CGumpListView

IMPLEMENT_DYNCREATE(CGumpListView, CListView)

BEGIN_MESSAGE_MAP(CGumpListView, CListView)
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnLvnBegindrag)
	ON_WM_LBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POPUP_COPYGUMPID, OnPopupCopygumpid)
	ON_COMMAND(ID_POPUP_INSERTGUMP, OnPopupInsertgump)
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_POPUP_EXPORTGUMPDESC, &CGumpListView::OnPopupExportgumpdesc)
END_MESSAGE_MAP()


// CGumpListView construction/destruction

CGumpListView::CGumpListView()
: m_bInit(false)
{
	// TODO: add construction code here
	m_nIDClipFormat = RegisterClipboardFormat("GumpEditor");
}

CGumpListView::~CGumpListView()
{
}

BOOL CGumpListView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS ;
	return CListView::PreCreateWindow(cs);
}

bool CGumpListView::LoadGumpDesc(LPCTSTR szDescFile)
{
	CStdioFile file;
	if (!file.Open(szDescFile, CFile::modeRead | CFile::typeText))
		return false;

	int len = 0, pos = 0;
	CString str, id;
	std::string desc;
	while (file.ReadString(str)) {
		pos = str.Find(_T('='));
		if (pos < 0) continue;
		id = str.Left(pos).Trim();
		desc = str.Mid(pos+1).Trim();

		// comments
		if (id.Left(1) == "#" || id.IsEmpty() || desc.empty()) continue; 

		m_mapGumpDesc[GfxAtoX(id)] = desc;
	}

	file.Close();
}

bool CGumpListView::SaveGumpDesc(LPCTSTR szDescFile)
{
	CListCtrl& ctrl = GetListCtrl();

	CStdioFile file;
	if (!file.Open(szDescFile, CFile::modeCreate | CFile::modeWrite)) return false;

	int id = 0;
	LPCTSTR desc;
	for (int i = 0; i < ctrl.GetItemCount(); i++)
	{
		id = ctrl.GetItemData(i);
		desc = GetGumpDesc(id);
		file.WriteString(GfxSprintf("0x%04X=%s\n",id, desc ? desc : ""));
	}

	file.Close();

	return true;
}

LPCTSTR CGumpListView::GetGumpDesc(int iGumpId)
{
	std::map<int, std::string>::const_iterator iter = m_mapGumpDesc.find(iGumpId);
	if (m_mapGumpDesc.end() != iter)
		return iter->second.c_str();

	return NULL;
}

void CGumpListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	if (m_bInit) return;
	m_bInit = true;

	LoadGumpDesc(GUMP_DESC_FILE);

	CListCtrl& ctrl = GetListCtrl();

	ctrl.ModifyStyle(0, LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS);
	ctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	HGDIOBJ hFont = ::GetStockObject(OEM_FIXED_FONT);
	::GetObject(hFont, sizeof(lf), &lf);

	lf.lfHeight = 100;
	lf.lfPitchAndFamily = FIXED_PITCH;

	CFont font;

	font.CreatePointFontIndirect(&lf);

	ctrl.SetFont(&font);
	font.Detach();

	ctrl.InsertColumn(0, "Gump ID", LVCFMT_RIGHT,60);
	ctrl.InsertColumn(1, "Size",	LVCFMT_RIGHT,60);
	ctrl.InsertColumn(2, "Desc",	LVCFMT_LEFT,100);

	CString strText;
	cGumpLoader* pGumpLoader = GetDocument()->GetGumpLoader(); ASSERT(pGumpLoader);

	int w=0,h=0,iItem=0;
	for (int i = 0; i < pGumpLoader->GetGumpCount(); i++)
	{
		pGumpLoader->GetGumpSize(i, w, h);
		if (w==0 || h==0) continue;
		
		strText.Format("0x%04X", i);
		iItem = ctrl.InsertItem(i, strText);
		strText.Format("%dx%d", w,h);
		ctrl.SetItemText(iItem, 1, strText);
		ctrl.SetItemData(iItem, i);

		LPCTSTR desc = GetGumpDesc(i);
		if (desc)
			ctrl.SetItemText(iItem, 2, desc);

		//if (i > 100) break;
	}
	
}


// CGumpListView diagnostics

#ifdef _DEBUG
void CGumpListView::AssertValid() const
{
	CListView::AssertValid();
}

void CGumpListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CGumpEditorDoc* CGumpListView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGumpEditorDoc)));
	return (CGumpEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CGumpListView message handlers

void CGumpListView::OnSize(UINT nType, int cx, int cy)
{
	CListView::OnSize(nType, cx, cy);

}

void CGumpListView::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	UpdateGump();
}

int CGumpListView::GetSelectedGumpID(void)
{
	int iItem = GetSelectedItem();
	if (iItem < 0) return -1;

	return GetListCtrl().GetItemData(iItem);
}

void CGumpListView::UpdateGump(void)
{
	int iGumpID = GetSelectedGumpID();
	if (iGumpID < 0) return;

	GetDocument()->SelectGump(iGumpID);
}

void CGumpListView::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	
	COleDataSource srcItem;
	CString sType = _T("");
	HGLOBAL hTextData = 0;	

	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	dynDropSource.nControlType = DYN_PICTURE;

	CString strText;
	strText.Format("%d",0);
	sType = strText;

	clipb.Write(sType, sType.GetLength()*sizeof(TCHAR));
	hTextData = clipb.Detach();

	srcItem.CacheGlobalData(m_nIDClipFormat, hTextData);
	srcItem.DoDragDrop(DROPEFFECT_COPY,NULL,&dynDropSource);
}

void CGumpListView::OnLButtonUp(UINT nFlags, CPoint point)
{

	CListView::OnLButtonUp(nFlags, point);
}

void CGumpListView::SelectGump(int iGumpID)
{
	LVFINDINFO info={LVFI_PARAM, NULL, iGumpID, 0 };
	CListCtrl& ctrl = GetListCtrl();
	int iItem = ctrl.FindItem(&info);
	if (iItem == -1) return;
	ctrl.SetItemState(iItem, LVNI_SELECTED, LVNI_SELECTED);
	ctrl.EnsureVisible(iItem, FALSE);
}

void CGumpListView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	if (GetSelectedGumpID() < 0) return;

	CMenu menu;
	menu.LoadMenu(IDR_GUMPLIST_MENU);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, point.x,point.y,this);
}

void CGumpListView::OnPopupCopygumpid()
{
	int iGumpID = GetSelectedGumpID();
	CString strText = GfxXtoA(iGumpID);
	
	if  (!OpenClipboard()) return;
	
	EmptyClipboard();
	HGLOBAL hClipboardData;
	hClipboardData = GlobalAlloc(GMEM_DDESHARE, strText.GetLength()+1);

	char * pchData = (char*)GlobalLock(hClipboardData);
	strcpy(pchData, strText);
	GlobalUnlock(hClipboardData);
	SetClipboardData(CF_TEXT,hClipboardData);

	CloseClipboard();

}

void CGumpListView::OnPopupInsertgump()
{
	int iGumpID = GetSelectedGumpID();

	GetDocument()->InsertGump(iGumpID);
}

int CGumpListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void CGumpListView::OnEditCopy()
{
	OnPopupCopygumpid();
}

void CGumpListView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetSelectedGumpID() != -1);
}

void CGumpListView::OnPopupExportgumpdesc()
{
	// TODO: Add your command handler code 
	CFileDialog dlg(FALSE, NULL, "gumpid.tmp.txt");
	if (dlg.DoModal() != IDOK) return;

	if (!SaveGumpDesc(dlg.GetPathName())) {
		AfxMessageBox("Export failed " + dlg.GetFileName());
		return;
	}	
}
