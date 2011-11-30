// GrannyListView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Grannyviewer.h"
#include "GrannyViewerDoc.h"
#include "grannylistview.h"
#include "FileString.h"
#include "granny/GrannyLoader.h"
#include "granny/GrannyModelTD.h"
#include "granny/GrannyModelAOS.h"
#include "debug.h"
#include ".\grannylistview.h"



// CGrannyListView

IMPLEMENT_DYNCREATE(CGrannyListView, CTreeView)
CGrannyListView::CGrannyListView()
{
}

CGrannyListView::~CGrannyListView()
{
}


BEGIN_MESSAGE_MAP(CGrannyListView, CTreeView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMRclick)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	ON_COMMAND(ID_MODEL_REGISTERMODEL, OnModelRegistermodel)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MODEL_UNREGISTERMODEL, OnModelUnregistermodel)
END_MESSAGE_MAP()

#ifdef _DEBUG
CGrannyViewerDoc* CGrannyListView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGrannyViewerDoc)));
	return (CGrannyViewerDoc*)m_pDocument;
}
#endif //_DEBUG

// CGrannyListView 메시지 처리기입니다.

std::string getFileName(const std::string& path_name, char split)
{
	int pos = path_name.find_last_of(split);
	if (pos != std::string::npos) {
		int len = path_name.length()-pos-5;
		return path_name.substr(pos+1, len);
	}

	return path_name;
}

std::string getHandName(int hand)
{
	switch (hand) {
		case HAND_NONE :  return "none";
		case HAND_LEFT :  return "left";
		case HAND_RIGHT : return "right";
		case HAND_OWNER : return "owner";
	}
	return "";
}

bool isFileExist(std::string file)
{
	CFile f;
	return f.Open(file.c_str(), CFile::modeRead | CFile::shareDenyNone);
}

HTREEITEM CGrannyListView::InsertModelTD(CTreeCtrl& tree, HTREEITEM htRoot, Uint32 id, cGrannyModelTD *model)
{
	const UINT nMask = TVIF_TEXT | TVIF_PARAM;

	if (!model) return NULL;

	CString strText;
	strText.Format("%04d %s", GETID(id), getFileName(model->m_filename).c_str());
	
	HTREEITEM htChar  = tree.InsertItem(nMask, strText, 0,0,0,0, SETCHAR(id), htRoot, 0);
	//HTREEITEM htAnim  = tree.InsertItem("anim", htChar);
	HTREEITEM htEquip = IsPlayerModelID(id) ? tree.InsertItem("equip", htChar) : NULL;
	
	strText.Format("file: %s%s", isFileExist(model->m_filename) ? "":"(x) ",model->m_filename.c_str());
	tree.InsertItem(strText, htChar);

	if (!model->desc.empty()) {
		strText.Format("desc: %s", model->desc.c_str());
		tree.InsertItem(strText, htChar);
	}

	strText.Format("hand: %s", getHandName(model->hand).c_str());
	tree.InsertItem(strText, htChar);

	strText.Format("bone left: %d, right: %d", model->left_hand_bone, model->right_hand_bone);
	tree.InsertItem(strText, htChar);

#if 0
	strText.Format("%s (default)", getFileName(model->defaultanimname).c_str());
	HTREEITEM htItem = tree.InsertItem(nMask, strText, 0,0,0,0, SETANIM(AnimTypeData::GetAnimType("default.animation")), htAnim, 0);
	strText.Format("file: %s%s", isFileExist(model->defaultanimname) ? "":"(x) ",model->defaultanimname.c_str());
	tree.InsertItem(strText, htItem);
	

	std::map<int, std::string>& anim_names = model->animation_names;
	for (std::map<int, std::string>::iterator i = anim_names.begin(); i != anim_names.end(); i++) {
		strText.Format("%s (%s,%d)", getFileName(i->second).c_str(), 
			AnimTypeData::GetAnimTypeName(i->first).c_str(),i->first);
		HTREEITEM htItem = tree.InsertItem(nMask, strText, 0,0,0,0, SETANIM(i->first), htAnim,0);
		strText.Format("file: %s%s", isFileExist(i->second) ? "":"(x) ",i->second.c_str());
		tree.InsertItem(strText, htItem);
	}
#endif

	return htEquip;
}

HTREEITEM CGrannyListView::InsertModelAOS(CTreeCtrl& tree, HTREEITEM htRoot, Uint32 id, cGrannyModelAOS *model)
{
	const UINT nMask = TVIF_TEXT | TVIF_PARAM;

	if (!model) return NULL;

	CString strText;
	strText.Format("%04d %s", GETID(id), model->desc.c_str());

	HTREEITEM htChar  = tree.InsertItem(nMask, strText, 0,0,0,0, SETCHAR(id), htRoot, 0);
	HTREEITEM htPart  = tree.InsertItem("part", htChar);
	//HTREEITEM htAnim  = tree.InsertItem("anim", htChar);
	HTREEITEM htEquip = tree.InsertItem("equip", htChar);

	std::map <int, cGrannyModelTD *> models = model->models;

	if (models.empty()) return htEquip;

	for (std::map<int, cGrannyModelTD*>::iterator i = models.begin(); i != models.end(); i++) {
		cGrannyModelTD *td = i->second;
		if (!td) continue;

		UINT id2 = 0xC000 | id<< 4 | i->first;
		strText.Format("%2d %s", i->first, cGrannyLoader::GetAosBodyName(i->first).c_str());
		HTREEITEM htPart2 = tree.InsertItem(nMask, strText, 0,0,0,0, SETCHAR(id2), htPart, 0);
		strText.Format("file: %s", td->m_filename.c_str());
		tree.InsertItem(strText, htPart2);
	}

	if (!model->desc.empty()) {
		strText.Format("desc: %s", model->desc.c_str());
		tree.InsertItem(strText, htChar);
	}

	strText.Format("hand: %s", getHandName(model->hand).c_str());
	tree.InsertItem(strText, htChar);

	strText.Format("bone left: %d, right: %d", model->left_hand_bone, model->right_hand_bone);
	tree.InsertItem(strText, htChar);

#if 0
	// part animation is same.
	std::map<int, std::string>& anim_names = models.begin()->second->animation_names;
	for (std::map<int, std::string>::iterator i = anim_names.begin(); i != anim_names.end(); i++) {
		strText.Format("%s (%s,%d)", getFileName(i->second).c_str(), 
			AnimTypeData::GetAnimTypeName(i->first).c_str(),i->first);
		HTREEITEM htItem = tree.InsertItem(nMask, strText, 0,0,0,0, SETANIM(i->first), htAnim,0);
		strText.Format("file: %s%s", isFileExist(i->second) ? "":"(x) ",i->second.c_str());
		tree.InsertItem(strText, htItem);
	}
#endif

	return htEquip;
}



HTREEITEM CGrannyListView::InsertEquipItem(CTreeCtrl& tree, HTREEITEM htRoot, Uint32 id, cGrannyModelTD *model)
{
	const UINT nMask = TVIF_TEXT | TVIF_PARAM;

	if (!model) return NULL;

	CString strText;
	strText.Format("%04d %s", GETID(id), getFileName(model->m_filename).c_str());
	
	HTREEITEM htChar = tree.InsertItem(nMask, strText, 0,0,0,0, SETCHAR(id), htRoot, 0);

	strText.Format("file: %s%s", isFileExist(model->m_filename) ? "":"(x) ",model->m_filename.c_str());
	tree.InsertItem(strText, htChar);

	strText.Format("id: %d (0x%08x)", id, id);
	tree.InsertItem(strText, htChar);

	if (!model->desc.empty()) {
		strText.Format("desc: %s", model->desc.c_str());
		tree.InsertItem(strText, htChar);
	}

	strText.Format("hand: %s", getHandName(model->hand).c_str());
	tree.InsertItem(strText, htChar);

	strText.Format("bone left: %d, right: %d", model->left_hand_bone, model->right_hand_bone);
	tree.InsertItem(strText, htChar);

	return htChar;
}



HTREEITEM CGrannyListView::GetHtItem(Uint32 id, cGrannyModel *model)
{
	if (!model) return m_htItem[OTHER];

	Uint32 assign = GETASSIGN(id);
	int anim = model->animset();

	if (assign == 0)
	{
		if (anim == 0) return m_htItem[MALE];
		if (anim == 1) return m_htItem[FEMALE];
		if (anim == 2) return m_htItem[ANIMAL];
		if (anim == 3) return m_htItem[MONSTER];
		if (anim == 4) return m_htItem[SEACRET];
	}

	int hand = model->GetHand();
	if (assign == 400)			// Male
	{
		if (hand == HAND_LEFT)  return m_htItem[ELEFT];
		if (hand == HAND_RIGHT) return m_htItem[ERIGHT];
		if (hand == HAND_NONE)  return m_htItem[EMCOL];
	}
	else if (assign == 401)		// Female
	{
		if (hand == HAND_NONE)  return m_htItem[EFCOL];
	}
	else if (assign > 0)		// Other
	{
		return m_htItem[EOTHER];
	}

	return m_htItem[OTHER];
}

void CGrannyListView::InsertAnimation(CTreeCtrl &tree, HTREEITEM htRoot, cGrannyLoader *pLoader)
{
	ASSERT(pLoader);
	const GrannyAnimMap &mapAnim = pLoader->GetAnimsetMap();

	CString strText;

	LPCTSTR szNames[SEACRET-MALE+1] = { "Male", "Female", "Animals", "Mosters", "SeaCreatures" };

	for (int i = MALE; i <= SEACRET; i++)
	{
		int a = i - MALE;
		GrannyAnimMap::const_iterator iter = mapAnim.find(a);
		if (iter == mapAnim.end()) continue;

		HTREEITEM htAnim = tree.InsertItem(szNames[a], htRoot);
		cAnimSet *animset = iter->second;

		std::map<int, std::string>::iterator it = animset->anim_names.begin();
		for (; it != animset->anim_names.end(); it++)
		{
			int aid = it->first;
			strText.Format("%3d %s", aid, cGrannyLoader::GetAnimName(aid).c_str());
			HTREEITEM htItem = tree.InsertItem(nMask, strText, 0,0,0,0, SETANIM(aid), htAnim, 0);
			strText.Format("file: %s", it->second.c_str());
			tree.InsertItem(strText, htItem);
		}
	}

	extern TypeData AnimTypeInfo[];

	HTREEITEM htAlls = tree.InsertItem("Alls", htRoot);

	for (int i = 0; AnimTypeInfo[i].blockid != -1; i++)
	{
		int aid = AnimTypeInfo[i].blockid;
		strText.Format("%3d %s", aid, cGrannyLoader::GetAnimName(aid).c_str());
		HTREEITEM htItem = tree.InsertItem(nMask, strText, 0,0,0,0, SETANIM(aid), htAlls, 0);
	}

}

void CGrannyListView::OnInitialUpdate()
{
	const UINT nMask = TVIF_TEXT | TVIF_PARAM;

	CTreeView::OnInitialUpdate();
	
	DEBUG_TIMER_START("CGrannyListView::OnInitialUpdate()");

	CTreeCtrl& tree=GetTreeCtrl();
	tree.DeleteAllItems();

	m_htItem[ROOT]     = tree.InsertItem("Models");
	m_htItem[MALE]     = tree.InsertItem("Male",         m_htItem[ROOT]); // animset id 0
	m_htItem[FEMALE]   = tree.InsertItem("Female",       m_htItem[ROOT]); // animset id 1
	m_htItem[ANIMAL]   = tree.InsertItem("Animals",      m_htItem[ROOT]); // animset id 2
	m_htItem[MONSTER]  = tree.InsertItem("Monsters",     m_htItem[ROOT]); // animset id 3
	m_htItem[SEACRET]  = tree.InsertItem("SeaCreatures", m_htItem[ROOT]); // animset id 4
	HTREEITEM htEquip  = tree.InsertItem("Equipments",   m_htItem[ROOT]);
	m_htItem[OTHER]	   = tree.InsertItem("Others",       m_htItem[ROOT]);
	m_htItem[ELEFT]	   = tree.InsertItem("Left",              htEquip);
	m_htItem[ERIGHT]   = tree.InsertItem("Right",             htEquip);
	m_htItem[EMCOL]	   = tree.InsertItem("Male Clothes",      htEquip);
	m_htItem[EFCOL]	   = tree.InsertItem("Female Clothes",    htEquip);
	m_htItem[EOTHER]   = tree.InsertItem("Other",             htEquip);
	
	CGrannyViewerDoc* pDoc = GetDocument(); ASSERT(pDoc);
	cGrannyLoader* pLoader = pDoc->GetGrannyLoader(); 
	if (!pLoader) 
	{
		DEBUG_TIMER_STOP("CGrannyListView::OnInitialUpdate()");
		return;
	}

	//InsertAnimation(tree, htAnim, pLoader);

	GrannyModelMap &models = pLoader->GetGrannyModels();
	
	std::map<Uint32, HTREEITEM> mapEquip;
	
	for (GrannyModelMapIter iter = models.begin(); iter != models.end(); iter++) 
	{
		Uint32 id = iter->first;
		Uint32 assign = GETASSIGN(id);
		if (!assign) 
		{
			// aos part model
			if (ISPART(id)) continue;

			// insert char model
			HTREEITEM htEquip;

			cGrannyModelTD *td = dynamic_cast<cGrannyModelTD*>(iter->second);
			if (td) 
			{
				htEquip = InsertModelTD(tree, GetHtItem(assign,td), id, td);
			}
			else
			{
				cGrannyModelAOS *aos = dynamic_cast<cGrannyModelAOS*>(iter->second);
				if (!aos) continue;
				htEquip = InsertModelAOS(tree,  GetHtItem(assign,aos), id, aos);
			}
		}
		else 
		{
			// insert equip item
			cGrannyModelTD* model = reinterpret_cast<cGrannyModelTD*>(iter->second);
			
			InsertEquipItem(tree, GetHtItem(id, model), id, model);
		}
	}

	tree.Expand(m_htItem[ROOT], TVE_EXPAND);

	DEBUG_TIMER_STOP("CGrannyListView::OnInitialUpdate()");
}


int CGrannyListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CTreeCtrl& tree=GetTreeCtrl();
	tree.ModifyStyle(0,TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS);
	
	return 0;
}

UINT CGrannyListView::GetSelectedItemData()
{
	CTreeCtrl &TreeCtrl = GetTreeCtrl();
	HTREEITEM htItem = TreeCtrl.GetSelectedItem();
	
	if (htItem == NULL) 
		return 0;
	else
		return TreeCtrl.GetItemData(htItem);
}

CString CGrannyListView::GetSelectedItemName()
{
	CTreeCtrl &TreeCtrl = GetTreeCtrl();
	HTREEITEM htItem = TreeCtrl.GetSelectedItem();

	return TreeCtrl.GetItemText(htItem);
}

void CGrannyListView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CGrannyListView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CGrannyListView::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
#if 0
	UINT iData = GetSelectedItemData();
	CGrannyViewerDoc *pDoc = GetDocument();
	if (!pDoc) return;
	pDoc->SetCurEquipId(INVALID_GRN_ID);
#endif


}

void CGrannyListView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	UINT iData = GetSelectedItemData();
	CGrannyViewerDoc *pDoc = GetDocument();
	if (!pDoc) return;

	if (ISSET(iData))
	{
		if (ISANIM(iData)) 
		{
			pDoc->SetCurAnimId(GETDATA(iData));
		} 
		else if (ISCHAR(iData)) 
		{
			UINT id = GETDATA(iData);
		/*	if (GETASSIGN(id)) 
				pDoc->SetCurEquipId(id);
			else */
			
			pDoc->SetCurModelId(id);
		}
	}
	//else
	//{
	//	pDoc->SetCurModelId(INVALID_GRN_ID);
	//}

	*pResult = 0;
}

void CGrannyListView::OnModelRegistermodel()
{
	CGrannyViewerDoc *pDoc = GetDocument();
	UINT iData = GetSelectedItemData();
	if (!pDoc || !ISCHAR(iData)) return;

	UINT id = GETDATA(iData);
	pDoc->RegisterModel(id);
}

void CGrannyListView::OnModelUnregistermodel()
{
	UINT iData = GetSelectedItemData();
	CGrannyViewerDoc *pDoc = GetDocument();
	if (!pDoc) return;

	if (ISCHAR(iData))
	{	
		UINT id = GETDATA(iData);
		pDoc->UnregisterModel(id);
	}
}


void CGrannyListView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMenu menu;
	if (!menu.LoadMenu(IDM_POPUP_MENU)) return;
	ClientToScreen(&point);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}



