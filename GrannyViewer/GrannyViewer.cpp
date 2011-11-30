// GrannyViewer.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "GrannyViewer.h"
#include "MainFrm.h"

#include "GrannyViewerDoc.h"
#include "GrannyViewerView.h"

#include "iris/config.h"
#include "iris/debug.h"
#include ".\grannyviewer.h"

#include "SetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int polycount;

// CGrannyViewerApp

BEGIN_MESSAGE_MAP(CGrannyViewerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_LOG_SETUPLOG, &CGrannyViewerApp::OnLogSetuplog)
END_MESSAGE_MAP()


// CGrannyViewerApp 생성

CGrannyViewerApp::CGrannyViewerApp() //: m_iModelID(-1)
{

}


// 유일한 CGrannyViewerApp 개체입니다.

CGrannyViewerApp theApp;

// CGrannyViewerApp 초기화

int GetCmdOpt(LPCTSTR szCmdLine, CStringArray &arOpt)
{
	int iPos = 0;
	CString strCmd(szCmdLine), strTok;

	arOpt.RemoveAll();
	strTok = strCmd.Tokenize(" ", iPos);
	while (strTok != "")
	{
		arOpt.Add(strTok);
		strTok = strCmd.Tokenize(" ", iPos);
	}

	return arOpt.GetSize();
}

#include "granny/grannyfile.h"

BOOL CGrannyViewerApp::InitInstance()
{
	try {
		std::string config_file = "xml\\config.xml";

		if(!nConfig::Init(config_file)) {
			MessageBox(NULL, config_file.c_str(), "Can't Open File", MB_OK|MB_ICONERROR);
			return FALSE;
		}
	} catch (...) {
		return FALSE;
	}

	InitCommonControls();

	CWinApp::InitInstance();

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	SetRegistryKey(_T("Harkon"));
	LoadStdProfileSettings(4);  
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGrannyViewerDoc),
		RUNTIME_CLASS(CMainFrame),
		RUNTIME_CLASS(CGrannyViewerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	ReadConfig();

	std::string granny_file = nConfig::aos ? "xml/granny_aos.xml" : "xml/granny.xml";

	CStringArray arOpt;
	GetCmdOpt(m_lpCmdLine, arOpt);

	for (int i = 0; i < arOpt.GetSize(); i++)
	{
		//if (arOpt[i] == "-id" && (i+1) < arOpt.GetSize())
		if (arOpt[i] == "-imm")
			nConfig::useVertexArray = false;
		else
			granny_file = arOpt[i];
	}

	if (!OpenDocumentFile(granny_file.c_str()))
		return FALSE;

//	m_iModelID = -1;

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	GfxLog("OpenGL Render Mode : %s\n", nConfig::useVertexArray ? "vertex array" : "immediate");

#if 0
	cGrannyFile f;
	if (!f.import("data/aa.smd"))
		return FALSE;
#endif

	return TRUE;
}

void CGrannyViewerApp::ReadConfig()
{
	pDebug.SetLoglevel(GetProfileInt("log", "level", LEVEL_INFO3));

	nConfig::logTextChunk = (bool)GetProfileInt("log", "textchunk", FALSE);
	nConfig::logMesh = (bool)GetProfileInt("log", "mesh", FALSE);
	nConfig::logObject = (bool)GetProfileInt("log", "object", FALSE);
	nConfig::logObjPtr = (bool)GetProfileInt("log", "objptr", FALSE);
	nConfig::logBone = (bool)GetProfileInt("log", "bone", TRUE);
	nConfig::logBoneObj = (bool)GetProfileInt("log", "boneobj", FALSE);
	nConfig::logBoneAnim = (bool)GetProfileInt("log", "boneanim", FALSE);
}

void CGrannyViewerApp::SaveConfig()
{
	WriteProfileInt("log", "level", pDebug.GetLoglevel());

	WriteProfileInt("log", "textchunk", nConfig::logTextChunk);
	WriteProfileInt("log", "mesh", nConfig::logMesh);
	WriteProfileInt("log", "object", nConfig::logObject);
	WriteProfileInt("log", "objptr", nConfig::logObjPtr);
	WriteProfileInt("log", "bone", nConfig::logBone);
	WriteProfileInt("log", "boneobj", nConfig::logBoneObj);
	WriteProfileInt("log", "boneanim", nConfig::logBoneAnim);
}

int CGrannyViewerApp::ExitInstance()
{
	SaveConfig();

	return CWinApp::ExitInstance();
}


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

void CGrannyViewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CGrannyViewerApp::OnLogSetuplog()
{
	CSetupDlg dlg;

	dlg.m_bTextChunk = nConfig::logTextChunk;
	dlg.m_bMesh = nConfig::logMesh;
	dlg.m_bObject = nConfig::logObject;
	dlg.m_bObjPtr = nConfig::logObjPtr;
	dlg.m_bBone = nConfig::logBone;
	dlg.m_bBoneObj = nConfig::logBoneObj;
	dlg.m_bBoneAnim = nConfig::logBoneAnim;

	if (dlg.DoModal() != IDOK) return;

	nConfig::logTextChunk = (bool)dlg.m_bTextChunk;
	nConfig::logMesh = (bool)dlg.m_bMesh ;
	nConfig::logObject = (bool)dlg.m_bObject;
	nConfig::logObjPtr = (bool)dlg.m_bObjPtr;
	nConfig::logBone = (bool)dlg.m_bBone;
	nConfig::logBoneObj = (bool)dlg.m_bBoneObj;
	nConfig::logBoneAnim = (bool)dlg.m_bBoneAnim;
}

