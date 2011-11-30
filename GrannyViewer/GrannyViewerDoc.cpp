// GrannyViewerDoc.cpp : CGrannyViewerDoc 클래스의 구현
//

#include "stdafx.h"
#include "GrannyViewer.h"
#include "granny/GrannyLoader.h"
#include "granny/GrannyModelTD.h"
#include "loaders/StitchinLoader.h"
#include "loaders/ModelInfoLoader.h"
#include "iris/config.h"
#include "iris/exception.h"
#include "iris/Debug.h"
#include "grannyviewerdoc.h"
#include "grannyviewerview.h"
#include "LogView.h"
#include "ModelListView.h"
#include ".\grannyviewerdoc.h"
#include "FileString.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGrannyViewerDoc

IMPLEMENT_DYNCREATE(CGrannyViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CGrannyViewerDoc, CDocument)
	ON_COMMAND(ID_FILE_VIEWXML, OnFileViewxml)
	ON_COMMAND(ID_FILE_CLEARLOG, OnFileClearlog)
	ON_COMMAND(ID_FILE_EXPORTMODEL, OnFileExportmodel)
	ON_COMMAND(ID_LOG_LOGLEVEL1, OnLogLoglevel1)
	ON_UPDATE_COMMAND_UI(ID_LOG_LOGLEVEL1, OnUpdateLogLoglevel1)
	ON_COMMAND(ID_LOG_LOGLEVEL2, OnLogLoglevel2)
	ON_UPDATE_COMMAND_UI(ID_LOG_LOGLEVEL2, OnUpdateLogLoglevel2)
	ON_COMMAND(ID_LOG_LOGLEVEL3, OnLogLoglevel3)
	ON_UPDATE_COMMAND_UI(ID_LOG_LOGLEVEL3, OnUpdateLogLoglevel3)
	ON_COMMAND(ID_LOG_SETFONT, &CGrannyViewerDoc::OnLogSetfont)
	ON_COMMAND(ID_FILE_DUMPMODEL, &CGrannyViewerDoc::OnFileDumpmodel)
	ON_COMMAND(ID_FILE_SHELLOPEN, &CGrannyViewerDoc::OnFileShellopen)
	ON_COMMAND(ID_LOG_LOGLEVEL4, &CGrannyViewerDoc::OnLogLoglevel4)
	ON_UPDATE_COMMAND_UI(ID_LOG_LOGLEVEL4, &CGrannyViewerDoc::OnUpdateLogLoglevel4)
	ON_COMMAND(ID_FILE_RELOAD, &CGrannyViewerDoc::OnFileReload)
	ON_COMMAND(ID_MODEL_CLEARMODEL, &CGrannyViewerDoc::OnModelClear)
	ON_COMMAND(ID_MODEL_CLEARALL, OnModelClearall)
	ON_COMMAND(ID_MODEL_CLEARREGISTERED, OnModelClearRegistered)
	ON_COMMAND(ID_MODEL_COLOR, OnModelColor)
	ON_COMMAND(ID_MODEL_RESETCOLOR, OnModelResetcolor)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RESETCOLOR, OnUpdateModelResetcolor)
	ON_UPDATE_COMMAND_UI(ID_MODEL_COLOR, OnUpdateModelColor)
	ON_COMMAND(ID_MODEL_CLEARMODEL, OnModelClearmodel)
END_MESSAGE_MAP()


// CGrannyViewerDoc 생성/소멸

CGrannyViewerDoc* g_pGrannyViewerDoc = NULL;

CGrannyViewerDoc* GfxGetDocument()
{
	return g_pGrannyViewerDoc;
}

CGrannyViewerDoc::CGrannyViewerDoc()
: m_bInit(false), m_lastSelect(SEL_NONE)
{
	m_iCurModelId = m_iCurAnimId = m_iLastId = INVALID_GRN_ID;
	g_pGrannyViewerDoc = this;
	
	m_pGrannyLoader    = NULL;
	m_pStitchinLoader  = NULL;
	m_pModelInfoLoader = NULL;
}

CGrannyViewerDoc::~CGrannyViewerDoc()
{
	g_pGrannyViewerDoc = NULL;

	delete m_pGrannyLoader;    m_pGrannyLoader    = NULL;
	delete m_pStitchinLoader;  m_pStitchinLoader  = NULL;
	delete m_pModelInfoLoader; m_pModelInfoLoader = NULL;
}

CView* CGrannyViewerDoc::FindView(CRuntimeClass* pClass)
{
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(pClass)) return pView;
	}  

	return NULL;
}

BOOL CGrannyViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	if (m_bInit) return TRUE;
	m_bInit = true;
	
	return TRUE;
}

// CGrannyViewerDoc serialization

void CGrannyViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CGrannyViewerDoc 진단

#ifdef _DEBUG
void CGrannyViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGrannyViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGrannyV


cGrannyModel *CGrannyViewerDoc::GetModel(UINT id)
{ 
	return m_pGrannyLoader ? m_pGrannyLoader->GetModel(id) : NULL; 
}

COLORREF CGrannyViewerDoc::GetModelColor(UINT id)
{
	cGrannyModel *pModel = GetModel(id);
	if (!pModel) return 0;

	return pModel->GetColor();
}

void CGrannyViewerDoc::SetModelColor(UINT id, COLORREF color)
{
	cGrannyModel *pModel = GetModel(id);
	if (!pModel) return;

	pModel->SetColor(color);

	/*CGrannyViewerView *pView = dynamic_cast<CGrannyViewerView*>(FindView(RUNTIME_CLASS(CGrannyViewerView))); ASSERT(pView);
	pView->InvalidateScene();*/
	UpdateAllViews(NULL);
}

void CGrannyViewerDoc::SetCurModelId(UINT iModelId) 
{ 
	if (m_iCurModelId == iModelId) return;

	if (m_pGrannyLoader && !IsRegisterModel(m_iCurModelId))
	{
		m_pGrannyLoader->Release(m_iCurModelId);
	}
	
	m_lastSelect = SEL_MODEL;
	m_iLastId = iModelId;

	m_iCurModelId = iModelId; 
	//m_iCurAnimId  = INVALID_GRN_ID;
	//ResetEquip();
	CGrannyViewerView* pView = dynamic_cast<CGrannyViewerView*>(FindView(RUNTIME_CLASS(CGrannyViewerView))); ASSERT(pView);
	/*if (!ISEQUIP(iModelId))
		pView->ResetTrans(false);*/
	pView->InvalidateScene();
}

void CGrannyViewerDoc::SetCurAnimId(UINT iAnimId) 
{ 
	m_lastSelect = SEL_ANIM;
	m_iLastId = iAnimId;

	m_iCurAnimId = iAnimId; 
	CGrannyViewerView* pView = dynamic_cast<CGrannyViewerView*>(FindView(RUNTIME_CLASS(CGrannyViewerView))); ASSERT(pView);
	pView->InvalidateScene();
}

bool CGrannyViewerDoc::IsRegisterModel(UINT id)
{
	return std::find(m_arRegister.begin(), m_arRegister.end(), id) != m_arRegister.end();
}

void CGrannyViewerDoc::RegisterModel(UINT id, bool first) 
{ 
	if (IsRegisterModel(id)) return;
	
	if (first)
		m_arRegister.insert(m_arRegister.begin(), id);
	else
		m_arRegister.push_back(id);
	
	UpdateAllViews(NULL);
#if 0
	{
	CGrannyViewerView *pView = dynamic_cast<CGrannyViewerView*>(FindView(RUNTIME_CLASS(CGrannyViewerView))); ASSERT(pView);
	pView->InvalidateRect(NULL,FALSE);
	}

	{
	CModelListView *pView = dynamic_cast<CModelListView*>(FindView(RUNTIME_CLASS(CModelListView))); ASSERT(pView);
	pView->UpdateModel();
	}
#endif
}
 
void CGrannyViewerDoc::UnregisterModel(UINT id)
{
	std::vector<UINT>::iterator i = std::find(m_arRegister.begin(), m_arRegister.end(), id);
	if (i == m_arRegister.end()) return;
	m_arRegister.erase(i);
	
	UpdateAllViews(NULL);
	//CGrannyViewerView *pView = dynamic_cast<CGrannyViewerView*>(FindView(RUNTIME_CLASS(CGrannyViewerView))); ASSERT(pView);
	//pView->InvalidateScene();
}


BOOL CGrannyViewerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	GfxLog("Open %s...\n", lpszPathName);

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	BOOL bRet = TRUE;

	CGrannyViewerApp *pApp = dynamic_cast<CGrannyViewerApp*>(AfxGetApp());

	CFileString strFile = lpszPathName;

	DEBUG_TIMER_START("CGrannyViewerDoc::OnOpenDocument(LPCTSTR lpszPathName)");
	
	try {
		GfxLog(" grannyloader    : %s\n", lpszPathName);
		delete m_pGrannyLoader;
		m_pGrannyLoader = new cGrannyLoader;
		m_pGrannyLoader->Load(lpszPathName, nConfig::mulpath);

		std::string mfile = nConfig::mulpath + "Models/models.txt", 
					sfile = nConfig::mulpath + "stitchin.def";

		GfxLog(" stitchinloader  : %s, %s\n", mfile.c_str(), sfile.c_str());
		delete m_pStitchinLoader;
		m_pStitchinLoader = new cStitchinLoader();
		if (nConfig::aos) 
			m_pStitchinLoader->Init(mfile, sfile);

		std::string minfofile = strFile.GetDirName() + "\\ModelsInfo.xml";

		GfxLog(" modelinfoloader : %s\n", minfofile.c_str());
		delete m_pModelInfoLoader;
		m_pModelInfoLoader = new cModelInfoLoader;
		m_pModelInfoLoader->Init(minfofile);
	}
	catch (cException ex) {
		AfxMessageBox(GfxSprintf("Open Error %s\n", ex.debug_message().c_str()));
		bRet = FALSE;
	}
	catch (...) {
		AfxMessageBox("Unknown Error!");
		bRet = FALSE;
	}

	DEBUG_TIMER_STOP("CGrannyViewerDoc::OnOpenDocument(LPCTSTR lpszPathName)");

	return bRet;
}


void CGrannyViewerDoc::OnFileReload()
{
	CString strFile = GetPathName();
	DeleteContents();
	GetDocTemplate()->OpenDocumentFile(strFile);
	UpdateAllViews(NULL);
}


void CGrannyViewerDoc::Log(LPCTSTR lpszLog)
{
	CLogView *pView = dynamic_cast<CLogView*>(FindView(RUNTIME_CLASS(CLogView)));
	if (!pView) return;

	pView->Log(lpszLog);
	SetModifiedFlag(FALSE);
}

void CGrannyViewerDoc::OnFileViewxml()
{
	if (GetPathName().IsEmpty()) return;

#if 0
	char szPath[MAX_PATH+1]={0,};
	LONG cbValue=sizeof(szPath);
	RegQueryValue(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE", szPath, &cbValue);

	CString strExec;
	strExec.Format("%s %s", szPath, GetPathName());
	WinExec(strExec,SW_SHOW);
#endif
	ShellExecute(NULL, "open", GetPathName(), 0, 0, SW_SHOW);
}

void CGrannyViewerDoc::OnFileClearlog()
{
	CLogView *pView = dynamic_cast<CLogView*>(FindView(RUNTIME_CLASS(CLogView)));
	if (!pView) return;

	pView->Clear();
	SetModifiedFlag(FALSE);
}

void CGrannyViewerDoc::OnLogSetfont()
{
	// TODO: Add your command handler code here
	CFontDialog dlg;
	if (dlg.DoModal() != IDOK) return;

	CLogView *pView = dynamic_cast<CLogView*>(FindView(RUNTIME_CLASS(CLogView)));
	if (!pView) return;
	pView->SetFont(dlg.GetFaceName(), dlg.GetSize());

	SetModifiedFlag(FALSE);
}

void CGrannyViewerDoc::OnFileDumpmodel()
{
	if (!m_pGrannyLoader || m_iLastId == INVALID_GRN_ID || m_lastSelect == SEL_NONE) return;
	
	cGrannyModel* model = NULL;
	cGrannyFile* file = NULL;

	if (m_lastSelect != SEL_ANIM) {
		model = m_pGrannyLoader->GetModel(m_iLastId);
		if (model) file = model->GetModel();
	} else {
		model = m_pGrannyLoader->GetModel(m_iCurModelId);
		if (model) file = model->GetAnimation(m_iCurAnimId);
	}

	if (file) file->print();
}



void CGrannyViewerDoc::OnFileExportmodel()
{
	if (!m_pGrannyLoader || m_iLastId == INVALID_GRN_ID || m_lastSelect == SEL_NONE) return;
	
	cGrannyModel* model = NULL;
	std::string filename;

	if (m_lastSelect != SEL_ANIM) {
		model = m_pGrannyLoader->GetModel(m_iLastId);
		filename = getFileName(model->GetFileName());
	} else {
		model = m_pGrannyLoader->GetModel(m_iCurModelId);
		cGrannyAnimation *anm = model->GetAnimation(m_iCurAnimId);
		if (!anm) return;
		filename = anm->getFileName();
	}

	if (!model) return;

	
	//filename += ".smd";

#if 1
	static char BASED_CODE szFilter[] = "SMD Files (*.smd)|*.smd|All Files (*.*)|*.*||";

	CFileDialog dlg(FALSE, "smd", filename.c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (dlg.DoModal()==IDCANCEL) return;

	if (m_lastSelect != SEL_ANIM)
		model->ExportModel((LPCTSTR)dlg.GetPathName());
	else
		model->ExportAnimation((LPCTSTR)dlg.GetPathName(), m_iLastId);
#else
	model->Export("test.smd");
#endif
}

void CGrannyViewerDoc::OnFileShellopen()
{
	if (!m_pGrannyLoader || m_iLastId == INVALID_GRN_ID || m_lastSelect == SEL_NONE) return;
	
	cGrannyModel* model = NULL;
	std::string filename;

	if (m_lastSelect != SEL_ANIM) {
		model = m_pGrannyLoader->GetModel(m_iLastId);
		filename = model->GetFileName(true);
	} else {
		model = m_pGrannyLoader->GetModel(m_iCurModelId);
		cGrannyAnimation *anm = model->GetAnimation(m_iCurAnimId);
		filename = anm->getPathName();
	}

	if (!model) return;

	//AfxMessageBox(filename.c_str());
	ShellExecute(NULL, "open", filename.c_str(), 0, 0, SW_SHOW);
}


void CGrannyViewerDoc::OnLogLoglevel1()
{
	pDebug.SetLoglevel(LEVEL_INFO1);
}

void CGrannyViewerDoc::OnUpdateLogLoglevel1(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(pDebug.GetLoglevel()==LEVEL_INFO1);
}

void CGrannyViewerDoc::OnLogLoglevel2()
{
	pDebug.SetLoglevel(LEVEL_INFO2);
}

void CGrannyViewerDoc::OnUpdateLogLoglevel2(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(pDebug.GetLoglevel()==LEVEL_INFO2);
}

void CGrannyViewerDoc::OnLogLoglevel3()
{
	pDebug.SetLoglevel(LEVEL_INFO3);
}

void CGrannyViewerDoc::OnUpdateLogLoglevel3(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(pDebug.GetLoglevel()==LEVEL_INFO3);
}


void CGrannyViewerDoc::OnLogLoglevel4()
{
	pDebug.SetLoglevel(LEVEL_INFO4);
}

void CGrannyViewerDoc::OnUpdateLogLoglevel4(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(pDebug.GetLoglevel()==LEVEL_INFO4);
}

void CGrannyViewerDoc::ClearModels(MODEL model)
{
	if (model != MCURRENT)
		m_arRegister.clear();
	
	if (model != MREGISTER)
		m_iCurModelId = INVALID_GRN_ID;

	UpdateAllViews(NULL);
}

void CGrannyViewerDoc::ShowModel(UINT id, int show)
{
	cGrannyModel *pModel = GetModel(id);
	if (!pModel || pModel->show == show) return;
		
	if (show == -1)
		pModel->show = !pModel->show;
	else
		pModel->show = show;

	UpdateAllViews(NULL);
}

void CGrannyViewerDoc::ShowAllModels(int show)
{
	for (int i = 0; i < m_arRegister.size(); i++)
	{
		UINT id = m_arRegister[i];
		cGrannyModel *pModel = GetModel(id);
		if (!pModel || pModel->show == show) return;
		
		if (show == -1)
			pModel->show = !pModel->show;
		else
			pModel->show = show;
	}

	UpdateAllViews(NULL);
}



bool CGrannyViewerDoc::IsShowModel(UINT id)
{
	cGrannyModel *pModel = GetModel(id);
	return pModel && pModel->show;
}

void CGrannyViewerDoc::OnModelClear()
{
	ClearModels(MCURRENT);
}

void CGrannyViewerDoc::OnModelClearall()
{
	ClearModels(MALL);
}

void CGrannyViewerDoc::OnModelClearRegistered()
{
	ClearModels(MREGISTER);
}

void CGrannyViewerDoc::OnModelColor()
{
	if (m_iCurModelId == INVALID_GRN_ID) return;

	COLORREF color = GetModelColor(m_iCurModelId);

	CColorDialog dlg(color);

	if (dlg.DoModal() != IDOK) return;

	color = dlg.GetColor();

	SetModelColor(m_iCurModelId, color);

	UpdateAllViews(NULL);
}

void CGrannyViewerDoc::OnModelResetcolor()
{
	if (m_iCurModelId == INVALID_GRN_ID) return;

	SetModelColor(m_iCurModelId, RGB(255,255,255));

	UpdateAllViews(NULL);
}

void CGrannyViewerDoc::OnUpdateModelResetcolor(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_iCurModelId != INVALID_GRN_ID);
}

void CGrannyViewerDoc::OnUpdateModelColor(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_iCurModelId != INVALID_GRN_ID);
}

void CGrannyViewerDoc::OnModelClearmodel()
{
	ClearModels(MCURRENT);
}
