#pragma once

// CGrannyListView

class CGrannyViewerDoc;
class cGrannyModel;
class cGrannyModelTD;
class cGrannyModelAOS;
class cGrannyLoader;

class CGrannyListView : public CTreeView
{
	DECLARE_DYNCREATE(CGrannyListView)
protected:
	const static UINT nMask = TVIF_TEXT | TVIF_PARAM;

	// xy yy yy yy - x : type info, y : data
	UINT ISSET(UINT c)		{ return c & 0xC0000000; }
	UINT ISCHAR(UINT c)		{ return c & 0x80000000; }
	UINT ISANIM(UINT c)		{ return c & 0x40000000; }
	UINT SETCHAR(UINT c)	{ return c | 0x80000000; }
	UINT SETANIM(UINT c )	{ return c | 0x40000000; }
	UINT GETDATA(UINT c)	{ return c & 0x3FFFFFFF; }

	// equip item
	UINT GETASSIGN(UINT c)  { return c >> 16;    }
	UINT GETID(UINT c)		{ return c & 0xFFFF; }
	UINT ISPART(UINT c)		{ return c & 0xC000; }

	CImageList m_imageList;

	HTREEITEM InsertModelTD(CTreeCtrl &tree, HTREEITEM htRoot, Uint32 id, cGrannyModelTD *model);
	HTREEITEM InsertModelAOS(CTreeCtrl &tree, HTREEITEM htRoot, Uint32 id, cGrannyModelAOS *model);
	HTREEITEM InsertEquipItem(CTreeCtrl &tree, HTREEITEM htRoot, Uint32 id, cGrannyModelTD *model);
	void InsertAnimation(CTreeCtrl &tree, HTREEITEM htRoot, cGrannyLoader *pLoader);
	bool IsPlayerModelID(int id) { return id == 400 || id == 401; }
public:
	CGrannyListView();
	virtual ~CGrannyListView();

	CGrannyViewerDoc* GetDocument() const;

	// 트리에서 선택된 아이템 lParam 반환
	UINT GetSelectedItemData();
	// 트리에서 선택된 아이템 이름 반환
	CString GetSelectedItemName();
protected:
	enum { ROOT,  MALE, FEMALE, MONSTER, ANIMAL, SEACRET,  
		   ELEFT, ERIGHT, EMCOL, EFCOL, EOTHER,
		   OTHER, NUM_ITEM };
	HTREEITEM m_htItem[NUM_ITEM];

	HTREEITEM GetHtItem(Uint32 id, cGrannyModel *model);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnModelRegistermodel();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnModelClearallmodels();
	afx_msg void OnModelUnregistermodel();
};

#ifndef _DEBUG  // GrannyViewerView.cpp의 디버그 버전
inline CGrannyViewerDoc* CGrannyListView::GetDocument() const
   { return reinterpret_cast<CGrannyViewerDoc*>(m_pDocument); }
#endif
