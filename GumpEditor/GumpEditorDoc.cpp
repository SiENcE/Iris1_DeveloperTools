// GumpEditorDoc.cpp : implementation of the CGumpEditorDoc class
//

#include "stdafx.h"
#include "gfx.h"
#include "GumpEditor.h"
#include "GumpEditorDoc.h"
#include "GumpListView.h"
#include "GumpView.h"
#include "GumpEditorView.h"
#include "mainfrm.h"
#include "iris/Config.h"
#include "iris/VerdataLoader.h"
#include "iris/xml.h"
#include "DialogSettings.h"
#include "codeview.h"
#include "GumpEntity.h"
#include "GumpEntityFactory.h"
#include "ConfigDlg.h"
#include ".\gumpeditordoc.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CString GFM_DOC_VERSION = _T("0.4.0");

static CGumpEditorDoc* g_pGumpEditorDoc = NULL;

CGumpEditorDoc* GfxGetGumpDocument()
{
	return g_pGumpEditorDoc;
	/*CWnd* pWnd = AfxGetMainWnd();
	if (!pWnd || !pWnd->IsKindOf(RUNTIME_CLASS(CMainFrame))) return NULL;
	return ((CMainFrame*)pWnd)->GetGumpDocument();*/
}

// CGumpEditorDoc

IMPLEMENT_DYNCREATE(CGumpEditorDoc, CDialogEditorDemoDoc)

BEGIN_MESSAGE_MAP(CGumpEditorDoc, CDialogEditorDemoDoc)
	ON_COMMAND(IDC_SETTINGS, OnSettings)
	ON_COMMAND(ID_VIEW_SWITCH, OnViewSwitch)
	ON_COMMAND(ID_FILE_RUN, OnFileRun)
	ON_COMMAND(ID_VIEW_VIEWXML, OnViewViewxml)
	ON_COMMAND(ID_VIEW_SETTING, &CGumpEditorDoc::OnViewSetting)
	ON_COMMAND(ID_FILE_IMPORT, &CGumpEditorDoc::OnFileImport)
END_MESSAGE_MAP()

CGumpEditorDoc::CGumpEditorDoc()
: m_bInitUOData(false), m_iAlpha(0), m_iFlags(0), m_iFadeAlpha(0), m_iFadeTime(0)
{
	// TODO: add one-time construction code here
	m_pGumpLoader = NULL;
	m_pHueLoader = NULL;

	m_strDocVersion = GFM_DOC_VERSION;

	if (!Init()) AfxThrowUserException();

	char szPath[MAX_PATH+1]={0,};
	LONG cbValue=sizeof(szPath);
	RegQueryValue(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE", 
		szPath, &cbValue);

	m_strXmlViewerPath = AfxGetApp()->GetProfileString("config", "xmlviewer", szPath);
	m_strIrisPath = AfxGetApp()->GetProfileString("config", "iris", "iris.exe");
}

CGumpEditorDoc::~CGumpEditorDoc()
{
	UnInit();

	AfxGetApp()->WriteProfileString("config", "xvmlviewer", m_strXmlViewerPath);
	AfxGetApp()->WriteProfileString("config", "iris", m_strIrisPath);
}

// CGumpEditorDoc construction/destruction

CFont* CGumpEditorDoc::GetFont(int iFontId)
{ 
	font_iter iter = m_fonts.find(iFontId);
	return iter != m_fonts.end() ? iter->second.font : NULL;
}

bool CGumpEditorDoc::Init()
{
	if (m_bInitUOData) return true;

	UnInit();

	std::string mulpath = nConfig::mulpath;
	std::string gumpartfile = mulpath + "gumpart.mul";
	std::string gumpidxfile = mulpath + "gumpidx.mul";
	std::string huesfile = mulpath + "hues.mul";
	//flo:
	pVerdataLoader = NULL;

	m_pGumpLoader = new cGumpLoader;
	if (!m_pGumpLoader->load(gumpartfile, gumpidxfile))
	{
		MessageBox(NULL, (gumpartfile + " or\n" + gumpidxfile).c_str(), "Can't Open File", MB_OK|MB_ICONERROR);
		delete m_pGumpLoader;
		m_pGumpLoader = NULL;
		return false;
	}

	//flo: New
	std::string filename=mulpath + "verdata.mul";
	std::ifstream verdata; 
	verdata.open(filename.c_str (),  std::ios::in | std::ios::binary);
	if (verdata)
	{
		verdata.close();
		pVerdataLoader = new cVerdataLoader (mulpath + "verdata.mul");
	}
	//--------
	m_pHueLoader = new cHueLoader;
	if (!m_pHueLoader->load(huesfile))
	{
		MessageBox(NULL, huesfile.c_str(), "Can't Open File", MB_OK|MB_ICONERROR);
		delete m_pHueLoader;
		m_pHueLoader = NULL;
		return false;
	}


	LOGFONT lf;
	memset(&lf,0,sizeof(lf));

	sMyFont font;

	for (std::vector<nConfig::font_info>::iterator iter = nConfig::fonts.begin(); iter != nConfig::fonts.end(); iter++)
	{	
		if (!AddFontResource(iter->file.c_str())) continue;

		strcpy(lf.lfFaceName, iter->name.c_str());
		lf.lfHeight = iter->size;
		//lf.lfWeight = FW_BOLD;
		//lf.lfCharSet = ANSI_CHARSET;
		//lf.lfOutPrecision  = OUT_TT_ONLY_PRECIS;

		CFont* pFont = new CFont;
		//if (!pFont->CreateFontIndirect(&lf)) {
		if (!pFont->CreatePointFont(lf.lfHeight*8, lf.lfFaceName)) {
			delete pFont;
			RemoveFontResource(iter->file.c_str());
			continue;
		}

		font.file = iter->file;
		font.font = pFont;
		font.color = GetHueColor(iter->hue, iter->id);

		m_fonts.insert(std::make_pair(iter->id, font));
	}

	m_bInitUOData = true;

	g_pGumpEditorDoc = this;

	return true;
}



void CGumpEditorDoc::UnInit()
{
	delete m_pGumpLoader;
	m_pGumpLoader = NULL;
	//flo:
	delete pVerdataLoader;
	pVerdataLoader = NULL;

	delete m_pHueLoader;
	m_pHueLoader = NULL;

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));

	CString strFontName;
	for (font_iter iter = m_fonts.begin(); iter != m_fonts.end(); iter++)
	{
		delete iter->second.font;
		RemoveFontResource(iter->second.file.c_str());
	}
	m_fonts.clear();

	m_bInitUOData = false;
	g_pGumpEditorDoc = NULL;
}



// CGumpEditorDoc diagnostics

#ifdef _DEBUG
void CGumpEditorDoc::AssertValid() const
{
	CDialogEditorDemoDoc::AssertValid();
}

void CGumpEditorDoc::Dump(CDumpContext& dc) const
{
	CDialogEditorDemoDoc::Dump(dc);
}
#endif //_DEBUG


// CGumpEditorDoc commands

CView* CGumpEditorDoc::FindView(CRuntimeClass* pClass)
{
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(pClass)) return pView;
	}  

	return NULL;
}

CGumpPtr CGumpEditorDoc::FindGump(int iStartGumpID, int w, int h, int iTryCount/*=10*/)
{
	if (!m_pGumpLoader) return NULL;

	CGumpPtr pDib;
	int gw = 0, gh = 0;

	if (w <= 0 && h <= 0) 
		return m_pGumpLoader->LoadGump(iStartGumpID);

	for (int i =0; i < iTryCount; i++)
	{
		if (!m_pGumpLoader->GetGumpSize(iStartGumpID, gw, gh)) break;
		if ((gw == w && gh == h) || (gw == w && gh <= 0) || (gw <= 0 && gh == h))
			return m_pGumpLoader->LoadGump(iStartGumpID);
		iStartGumpID++;
	}

	return NULL;
}

void CGumpEditorDoc::SelectGump(int iGumpID)
{	
	CGumpView* pView = dynamic_cast<CGumpView*>(FindView(RUNTIME_CLASS(CGumpView)));	
	if (!pView)  return;

	pView->SetGump(GetGump(iGumpID));
}

CGumpPtr CGumpEditorDoc::GetSelectedGump(void)
{
	CGumpPtr pDib;
	CGumpView* pView = dynamic_cast<CGumpView*>(FindView(RUNTIME_CLASS(CGumpView)));	
	if (pView) pDib = pView->GetGump();

	return pDib;
}


void CGumpEditorDoc::SelectGumpList(int iGumpID)
{
	CGumpListView* pView = dynamic_cast<CGumpListView*>(FindView(RUNTIME_CLASS(CGumpListView)));
	if (!pView) return;

	pView->SelectGump(iGumpID);
}

void CGumpEditorDoc::InsertGump(int iGumpID)
{
	CGumpEditorView* pView = dynamic_cast<CGumpEditorView*>(FindView(RUNTIME_CLASS(CGumpEditorView)));
	if (!pView) return;

	pView->PostMessage(WM_ADDCONTROL);
}

void CGumpEditorDoc::UpdateGumpEditorView(void)
{
	CGumpEditorView* pView = dynamic_cast<CGumpEditorView*>(FindView(RUNTIME_CLASS(CGumpEditorView)));
	if (!pView) return;

	pView->RedrawWindow();;
}


const stHue* CGumpEditorDoc::GetHue(int iHueId)
{
	return m_pHueLoader ? m_pHueLoader->getHue(iHueId) : NULL;
}

COLORREF CGumpEditorDoc::GetHueColor(int iHueId, int iFontId)
{	
	if (iFontId >= 0 && iHueId == 0) {
		font_iter iter = m_fonts.find(iFontId);
		return iter != m_fonts.end() ? iter->second.color : 0;
	}

	return m_pHueLoader ? m_pHueLoader->getColor(iHueId) : 0;
}


BOOL CGumpEditorDoc::OnNewDocument()
{
	if (!CDialogEditorDemoDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	SetName("");
	SetAlpha(0);
	SetFade(0,0);
	SetEventHandler("","","","","");
	SetShapeName("");

	m_objs.Clear();
	m_objs.SetVirtualSize(CSize(m_iDefWidth,m_iDefHeight));

	CCodeView* pCodeView = dynamic_cast<CCodeView*>(FindView(RUNTIME_CLASS(CCodeView)));
	if (pCodeView) pCodeView->SetText("");


	return TRUE;
}


// 0.3.1
std::string LoadFormV1(XML::Node *form)
{
	std::string buf;
	CString str;
	
	int width=640,height=480, alpha=0, flags=0, fade_alpha=0, fade_time=0;
	std::string shape, onclick, onclose, onmouseup, onmousedown, onkeypressed;
	
	form->lookupAttribute("width", width);
	form->lookupAttribute("height", height);
	form->lookupAttribute("alpha", alpha);
	form->lookupAttribute("flags", flags);
	form->lookupAttribute("shape", shape);
	
	str = _T("<?xml version='1.0'?>\n");
	buf += str;

	str.Format(_T("<form width='%d' height='%d' alpha='%d' flags='%d' shape='%s'>\n"), 
		width, height, alpha, flags, shape.c_str());
	buf += str;

	XML::Node *fade_node = form->findNode("fade");
	if (fade_node) {
		fade_node->lookupAttribute("alpha", fade_alpha);
		fade_node->lookupAttribute("time", fade_time);
	}

	str.Format(_T(" <fade alpha='%d' time='%d'/>\n"), fade_alpha, fade_time);
	buf += str;

	XML::Node *event_node = form->findNode("event");

	if (event_node) {
		event_node->lookupAttribute("onclick", onclick);
		event_node->lookupAttribute("onclose", onclose);
		event_node->lookupAttribute("onmouseup", onmouseup);
		event_node->lookupAttribute("onmousedown", onmousedown);
		event_node->lookupAttribute("onkeypressed", onkeypressed);
	}

	str.Format(_T(" <event onclick='%s' onclose='%s' onmouseup='%s' onmousedown='%s' onkeypressed='%s'/>\n"),
		onclick.c_str(), onclose.c_str(), onmouseup.c_str(), onmousedown.c_str(), onkeypressed.c_str());
	buf += str;

	int idx=0;
	XML::Node* control_node = NULL;			
	
	while ((control_node = form->findNode("control", idx++))) {
		XML::Node *event_node=NULL, *gump_node=NULL, *passwd_node=NULL;
		XML::Node *check_node=NULL, *group_node=NULL,*text_node=NULL;
		
		std::string type, name;
		std::string onclick, onclose, onmouseup, onmousedown, onkeypressed;

		int left=0,top=0,width=0,height=0,freezed=0,visible=1;			
		int alpha=0,flags=0;

		if (!control_node->lookupAttribute("class", type)) continue;

		control_node->lookupAttribute("name", name);
		control_node->lookupAttribute("left", left);
		control_node->lookupAttribute("top", top);
		control_node->lookupAttribute("width", width);
		control_node->lookupAttribute("height", height);
		control_node->lookupAttribute("freezed", freezed);
		control_node->lookupAttribute("visible", visible);
		control_node->lookupAttribute("alpha", alpha);
		control_node->lookupAttribute("flags", flags);

		str.Format("<control class='%s' name='%s' left='%d' top='%d' width='%d' height='%d' alpha='%d' flags='%d'>\n", 
			type.c_str(), name.c_str(), left, top, width, height, alpha, flags);
		buf += str;

		str.Format("<meta freezed='%d' visible='%d'/>", freezed, visible);
		buf += str;
			
		if ((event_node = control_node->findNode("event"))) {
			event_node->lookupAttribute("onclick", onclick);
			event_node->lookupAttribute("onclose", onclose);
			event_node->lookupAttribute("onmouseup", onmouseup);
			event_node->lookupAttribute("onmousedown", onmousedown);
			event_node->lookupAttribute("onkeypressed", onkeypressed);
		}	

		str.Format(" <event onclick='%s' onclose='%s' onmouseup='%s' onmousedown='%s' onkeypressed='%s'/>\n",
			onclick.c_str(), onclose.c_str(), onmouseup.c_str(), onmousedown.c_str(), onkeypressed.c_str());
		//buf += str;

		if (type=="picture" || type=="border" || type=="paperdoll") {			
			int gump=0;
			if ((gump_node = control_node->findNode("gump"))) 
				gump = gump_node->asInteger();

			str.Format("  <picture type='gump' hue='0x1' gump='0x%X' /> ", gump);
			//str.Format(" <gump>0x%X</gump>\n", gump);
			buf += str;
		} else if (type=="button") {
			int normal=-1, over=-1, pressed=-1;
			if ((gump_node = control_node->findNode("gump"))) { 
				gump_node->lookupAttribute("normal", normal);
				gump_node->lookupAttribute("over", over);
				gump_node->lookupAttribute("pressed", pressed);
			}

			str.Format(" <gump normal='0x%X' over='0x%X' pressed='0x%X'/>\n", normal, over, pressed);
			buf += str;

			if (!onclick.empty()) {
				str.Format(" <event type='%s'>%s</event>", "onclick", onclick.c_str());
				buf += str;
			}
		} else if (type=="checkbox" || type=="radio") {
			int normal=-1, checked=-1, group=0, ischecked=0;
			gump_node = control_node->findNode("gump");
			if (gump_node) { 
				gump_node->lookupAttribute("normal", normal);
				gump_node->lookupAttribute("checked", checked);
			}
			if ((check_node = control_node->findNode("checked"))) {
				ischecked = check_node->asBool();
			}
			if ((group_node = control_node->findNode("group"))) {
				group = group_node->asInteger();
			}

			str.Format(" <gump normal='0x%X' checked='0x%X'/>\n", normal, checked);
			buf += str;
			
			str.Format(" <checked>%d</checked>\n", ischecked);
			buf += str;

			str.Format(" <group>%d</group>\n", group);
			buf += str;

		} else if (type=="label" || type=="edit") {
			std::string text, align;
			int font=-1,hue=-1,password=0, label_align=-1;
			if ((text_node = control_node->findNode("text"))) {
				text = text_node->asString();
				text_node->lookupAttribute("font", font);
				text_node->lookupAttribute("hue", hue);
				text_node->lookupAttribute("align", align);
			}

			str.Format(" <text font='%d' hue='0x%X' align='%s'>%s</text>", 
				font, hue, align.c_str(), text.c_str());
			buf += str;

			if ((passwd_node = control_node->findNode("password"))) {
				password = passwd_node->asBool();
			}

			str.Format(" <password>%d</password>\n", password);
			buf += str;
			
			if (type=="label") {
			} else {
			//	input->SetScriptFunction(GUI::FUNC_ONKEYPRESSED, (char*)onkeypressed.c_str());
			}
		}

		str.Format(" </control>\n");
		buf += str;
	}

	str.Format("</form>\n");
	buf += str;

	return buf;
}

bool CGumpEditorDoc::LoadGfmFile(LPCTSTR lpszPathName, bool bLoadForm)
{
	

	XML::Parser parser;
	XML::Node* form=NULL,* document=NULL;
	//XML::Node* section = NULL,* value = NULL;
	CSize sizeForm(m_iDefWidth,m_iDefHeight);

	std::string version = "0.3.1";

	try	{
		parser.loadData(lpszPathName);
		document = parser.parseDocument();
		
		form = document->findNode("form");
		if (!form) throw;
		
		XML::Node *ver_node = form->findNode("version");
		if (ver_node)
			version = ver_node->asString();

		if (GetDocVersion().CompareNoCase(version.c_str()) > 0) {
			// load old version
			std::string buf = LoadFormV1(form);
			parser.setData(buf);
			document = parser.parseDocument();

			form = document->findNode("form");
			if (!form) throw;
		}
	} catch (...) {
		delete document; document = NULL;
		return false;
	}

	if (bLoadForm) {
		int alpha=0,flags=0,time=0;
		std::string shape;

		XML::Node *ver_node = form->findNode("version");
		if (ver_node)
			version = ver_node->asString();

		form->lookupAttribute("width",  (int&)sizeForm.cx);
		form->lookupAttribute("height", (int&)sizeForm.cy);

		form->lookupAttribute("alpha", alpha);
		form->lookupAttribute("flags", flags);
		form->lookupAttribute("shape", shape);

		m_objs.Clear();
		m_objs.SetVirtualSize(sizeForm);

		SetAlpha(alpha);
		SetFlags(flags);
		SetShapeName(shape.c_str());
		SetLoadDocVersion(version.c_str());

		std::string onclick, onclose, onmouseup, onmousedown, onkeypressed;

		XML::Node* fade_node = form->findNode("fade");
		if (fade_node) {
			fade_node->lookupAttribute("alpha", alpha);
			fade_node->lookupAttribute("time", time);
		}

		XML::Node* event_node = form->findNode("event");
		if (event_node) {
			event_node->lookupAttribute("onclick", onclick);
			event_node->lookupAttribute("onclose", onclose);
			event_node->lookupAttribute("onmouseup", onmouseup);
			event_node->lookupAttribute("onmousedown", onmousedown);
			event_node->lookupAttribute("onkeypressed", onkeypressed);
		}

		SetFade(alpha, time);
		SetEventHandler(onclick.c_str(), onclose.c_str(), onmouseup.c_str(), onmousedown.c_str(), onkeypressed.c_str());
	}

	int idx=0;
	XML::Node* control = NULL;	

	while (control = form->findNode("control", idx++)) {
		CDiagramEntity* obj = CGumpEntityFactory::CreateFromNode(control);
		if (obj) m_objs.Add(obj);
	}

	delete document; document=NULL;

	return true;
}

BOOL CGumpEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	//
	// open form(.gfm) file
	//
	
	if (!LoadGfmFile(lpszPathName, true)) return FALSE;

	//
	// open script(.csl) file
	//

	CCodeView* pCodeView = dynamic_cast<CCodeView*>(FindView(RUNTIME_CLASS(CCodeView))); ASSERT(pCodeView);

	CFile file;
	CString strCslFileName(lpszPathName), str;
	strCslFileName.Replace(_T(".gfm"), _T(".csl"));

	if (file.Open(strCslFileName, CFile::modeRead)) {	
		ULONGLONG iLen = file.GetLength();

		if (iLen > 0)
			file.Read(str.GetBufferSetLength(iLen), iLen);

		pCodeView->SetText(str);

		file.Close();
	}

	m_objs.SetModified( FALSE );

	GfxResetWorkingDirectory();

	return TRUE;
}

void CGumpEditorDoc::OnFileImport()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE, ".gfm");
	if (dlg.DoModal() != IDOK) return;

	if (!LoadGfmFile(dlg.GetPathName(), false)) {
		AfxMessageBox("Import failed " + dlg.GetFileName());
		return;
	}

	m_objs.SetModified( FALSE );

	GfxResetWorkingDirectory();
}

BOOL CGumpEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	CStdioFile file;

	CString str;

	CString strCreateFileName;
	CString strDirName  = GfxGetDirName(lpszPathName);
	CString strFileName = GfxGetFileName(lpszPathName, false);
	CString strGfmFileName = strFileName + _T(".gfm");
	CString strCslFileName = strFileName + _T(".csl");
	CString strPotFileName = strFileName + _T(".pot");


	//
	// save form(.gfm) file
	//

	strCreateFileName = strDirName + _T("\\") + strGfmFileName;
	if (!file.Open(strCreateFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
		return FALSE;

	CSize size = m_objs.GetVirtualSize();
	str = _T("<?xml version='1.0'?>\n");
	file.WriteString(str);

	str.Format(_T("<form width='%d' height='%d' alpha='%d' flags='%d' shape='%s'>\n"), size.cx, size.cy, GetAlpha(), GetFlags(), GetShapeName());
	file.WriteString( str );

	str.Format(_T("<version>%s</version>\n"), GetDocVersion());
	file.WriteString(str);

	str.Format(_T(" <fade alpha='%d' time='%d'/>\n"), GetFadeAlpha(), GetFadeTime());
	file.WriteString(str);
	str.Format(_T(" <event onclick='%s' onclose='%s' onmouseup='%s' onmousedown='%s' onkeypressed='%s'/>\n"),
		GetEventHandler(ONCLICK), GetEventHandler(ONCLOSE), GetEventHandler(ONMOUSEUP), GetEventHandler(ONMOUSEDOWN), GetEventHandler(ONKEYPRESSED));
	file.WriteString(str);

	int count = 0;
	CDiagramEntity* obj = NULL;
	while (obj = m_objs.GetAt(count++)) {
		file.WriteString(obj->GetString(TRUE)  + _T("\n"));
		file.WriteString(obj->GetString(FALSE) + _T("\n"));
	}

	file.WriteString(_T("</form>\n"));
	file.Close();

	//
	// save script(.csl) file
	//

	CCodeView* pCodeView = dynamic_cast<CCodeView*>(FindView(RUNTIME_CLASS(CCodeView))); ASSERT(pCodeView);
	str = pCodeView->GetText();

	if (!str.IsEmpty()) {
		strCreateFileName = strDirName + _T("\\") + strCslFileName;
		if (!file.Open(strCreateFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
			return FALSE;

		file.WriteString(str);
		file.Close();
	}

	m_objs.SetModified( FALSE );
	SetModifiedFlag(FALSE);

	//
	// save message(.pot) file
	//

	strCreateFileName = strDirName + _T("\\") + strPotFileName;

	DeleteFile(strCreateFileName);

	CString strCmdLine;
	strCmdLine.Format(_T("-C -a -s --omit-header --add-location %s\\%s -o %s"), 
		(LPCTSTR)strDirName, (LPCTSTR)strCslFileName, strCreateFileName);

	CString strError = GfxExecuteExternalFile(_T("xgettext.exe"), strCmdLine, true);

	if (!file.Open(strCreateFileName, CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText)) {
		//AfxMessageBox(strError);
		if (!file.Open(strCreateFileName, CFile::modeCreate| CFile::modeWrite | CFile::typeText)) {
			return FALSE;
		}
	} else {
		file.SeekToEnd();
		file.WriteString(_T("\n\n"));
	}

	CString strType, strTitle;

	count = 0;
	obj = NULL;
	while (obj = m_objs.GetAt(count++)) {
		strType  = obj->GetType();
		strTitle = obj->GetTitle();
		if (strTitle.IsEmpty()) continue;
		if ((strType == _T("label")  && strTitle != _T("label")) || 
			(strType == _T("edit")   && strTitle != _T("edit"))  ||
			(strType == _T("button") && strTitle != _T("button"))) {
			//#: test.gfm:label1
			//msgid "what do you want to do today?"
			//msgstr ""
			str.Format(_T("#:%s:%s\nmsgid \"%s\"\n"), 
				(LPCTSTR)strGfmFileName, (LPCTSTR)obj->GetName(), (LPCTSTR)strTitle);
			file.WriteString(str);
			file.WriteString(_T("msgstr \"\"\n\n"));
		}
	}

	ULONGLONG iSize = file.GetLength();
	file.Close();

	if (iSize == 0)
		DeleteFile(strCreateFileName);

	return TRUE;
}

void CGumpEditorDoc::GetEventHandler(CString& strEvClick, CString& strEvClose, 
									 CString& strEvMouseUp, CString& strEvMouseDown, CString& strEvKeyPressed) const
{
	strEvClick = GetEventHandler(ONCLICK);
	strEvClose = GetEventHandler(ONCLOSE);
	strEvMouseUp = GetEventHandler(ONMOUSEUP);
	strEvMouseDown = GetEventHandler(ONMOUSEDOWN);
	strEvKeyPressed = GetEventHandler(ONKEYPRESSED);
}

void CGumpEditorDoc::SetEventHandler(LPCTSTR szEvClick, LPCTSTR szEvClose, 
									 LPCTSTR szEvMouseUp, LPCTSTR szEvMouseDown, LPCTSTR szEvKeyPressed)
{
	SetEventHandler(ONCLICK, szEvClick);
	SetEventHandler(ONCLOSE, szEvClose);
	SetEventHandler(ONMOUSEUP, szEvMouseUp);
	SetEventHandler(ONMOUSEDOWN, szEvMouseDown);
	SetEventHandler(ONKEYPRESSED, szEvKeyPressed);
}



void CGumpEditorDoc::OnSettings()
{
	CGumpEditorView* pView = dynamic_cast<CGumpEditorView*>(FindView(RUNTIME_CLASS(CGumpEditorView)));
	if (!pView) return;

	CDEditor& editor = pView->GetDEditor();

	CDialogSettings	dlg;

	dlg.m_width = editor.GetVirtualSize().cx;
	dlg.m_height = editor.GetVirtualSize().cy;
	dlg.m_gridWidth = editor.GetGridSize().cx;
	dlg.m_gridHeight = editor.GetGridSize().cy;
	editor.GetMargins( dlg.m_marginLeft, dlg.m_marginTop, dlg.m_marginRight, dlg.m_marginBottom );

	dlg.m_strName = GetName();
	dlg.m_iAlpha = GetAlpha();
	dlg.m_iFlags = GetFlags();
	dlg.m_iFadeAlpha = GetFadeAlpha();
	dlg.m_iFadeTime = GetFadeTime();
	dlg.m_strShapeName = GetShapeName();
	GetEventHandler(dlg.m_strEvClick, dlg.m_strEvClose, dlg.m_strEvMouseUp, dlg.m_strEvMouseDown, dlg.m_strEvKeyPressed);

	if( dlg.DoModal() == IDOK )	{
		editor.SetGridSize( CSize( dlg.m_gridWidth, dlg.m_gridHeight ) );
		editor.SetVirtualSize( CSize( dlg.m_width, dlg.m_height ) );
		editor.SetMargins( dlg.m_marginLeft, dlg.m_marginTop, dlg.m_marginRight, dlg.m_marginBottom );

		SetName(dlg.m_strName);
		SetAlpha(dlg.m_iAlpha);
		SetFlags(dlg.m_iFlags);
		SetFade(dlg.m_iFadeAlpha, dlg.m_iFadeTime);
		SetShapeName(dlg.m_strShapeName);
		SetEventHandler(dlg.m_strEvClick, dlg.m_strEvClose, dlg.m_strEvMouseUp, dlg.m_strEvMouseDown, dlg.m_strEvKeyPressed);

		editor.RedrawWindow();
	}
}

void CGumpEditorDoc::OnViewSwitch()
{
	ShowCode(TRUE, NULL);
}

void CGumpEditorDoc::ShowControlList(void)
{
	CGumpEditorView* pView = dynamic_cast<CGumpEditorView*>(FindView(RUNTIME_CLASS(CGumpEditorView)));
	if (!pView) return;

	pView->OnViewControllist();
}



void CGumpEditorDoc::ShowCode(BOOL bShow, CDiagramEntity *entity)
{
	CGumpEntity* e = dynamic_cast<CGumpEntity*>(entity);
	CCodeView* pCodeView = dynamic_cast<CCodeView*>(FindView(RUNTIME_CLASS(CCodeView)));
	CGumpEditorView* pGumpView = dynamic_cast<CGumpEditorView*>(FindView(RUNTIME_CLASS(CGumpEditorView)));

	if (pGumpView) 
		pGumpView->GetDEditor().RemoveUnselectedPropertyDialogs(TRUE);

	if (e && pCodeView) {
		// insert event handler code
		if (e->GetEventHandler().IsEmpty()) 
			e->SetDefaultEventHandler(GetDocName());
		pCodeView->GotoEventHandler(e->GetEventHandler(), e->GetEventHandlerParams());
		pCodeView->SetFocus();
	}

	CMainFrame* pFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (pFrame) pFrame->SwitchView();
}

BOOL CGumpEditorDoc::SaveModified()
{
	CCodeView* pView = dynamic_cast<CCodeView*>(FindView(RUNTIME_CLASS(CCodeView))); ASSERT(pView);

	SetModifiedFlag(pView->IsModified() || m_objs.IsModified());

	return CDocument::SaveModified();
}

void CGumpEditorDoc::OnFileRun()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_SAVE);
	//AfxMessageBox(GetPathName());
	//ExecuteExternalFile("iris.exe", "", false);
	GfxResetWorkingDirectory();
	GfxExecuteExternalFile(m_strIrisPath + " -gui ", GetPathName(), false);
}

void CGumpEditorDoc::OnViewViewxml()
{
	if (GetPathName().IsEmpty()) return;
	
	CString strExec;
	strExec.Format("%s %s", m_strXmlViewerPath, GetPathName());
	WinExec(strExec,SW_SHOW);
}

void CGumpEditorDoc::OnViewSetting()
{
	// TODO: Add your command handler code here
	CConfigDlg dlg;
	dlg.m_strIrisPath = m_strIrisPath;
	dlg.m_strXmlViewerPath  = m_strXmlViewerPath;
	if (dlg.DoModal()!=IDOK) return;

	m_strIrisPath = dlg.m_strIrisPath;
	m_strXmlViewerPath = dlg.m_strXmlViewerPath;
}



CString CGumpEditorDoc::GetDocName() const
{
	CString strPathName = GetPathName();
	if (strPathName.IsEmpty()) return _T("noname");

	return GfxGetFileName(strPathName, false);
}