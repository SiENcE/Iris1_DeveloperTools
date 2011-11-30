// GumpEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GumpEditor.h"
#include "MainFrm.h"

#include "GumpEditorDoc.h"
#include "GumpListView.h"
#include "iris/Debug.h"
#include "iris/Exception.h"
#include "iris/Config.h"
#include "codeedit/ScintillaWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGumpEditorApp

BEGIN_MESSAGE_MAP(CGumpEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CGumpEditorApp construction

CGumpEditorApp::CGumpEditorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CGumpEditorApp object

CGumpEditorApp theApp;

// CGumpEditorApp initialization

BOOL CGumpEditorApp::InitInstance()
{
	GfxResetWorkingDirectory();	

	if (!CScintillaWnd::LoadScintillaDll()) {
		AfxMessageBox("Can't Open "STR_LEXERDLL" File", MB_OK|MB_ICONERROR);
		return FALSE;
	}

	try {
		std::string config_file = "xml\\config.xml";

		if(!nConfig::Init(config_file)) {
			pDebug.Log("Unable to load configuration file - Using defaults ", __FILE__, __LINE__, LEVEL_WARNING);
			MessageBox(NULL, config_file.c_str(), "Can't Open File", MB_OK|MB_ICONERROR);
			return FALSE;
		}
	} catch (...) {
		return FALSE;
	}

	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGumpEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CGumpListView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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

// App command to run the dialog
void CGumpEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CGumpEditorApp message handlers


