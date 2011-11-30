// GumpEditorDoc.cpp : implementation of the CGumpEditorDoc class
//

#include "stdafx.h"
#include "GumpEditor.h"

#include "GumpEditorDoc.h"
#include "GumpListView.h"
#include "GumpView.h"
#include "GumpEditorView.h"
#include ".\gumpeditordoc.h"
#include "mainfrm.h"
#include "iris/Config.h"
#include "iris/VerdataLoader.h"
#include "DiagramControlFactory.h"
#include "iris/xml.h"
#include "DialogSettings.h"

#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
END_MESSAGE_MAP()

CGumpEditorDoc::CGumpEditorDoc()
: m_bInitUOData(false), m_iAlpha(0), m_iFlags(0), m_iFadeAlpha(0), m_iFadeTime(0)
{
	// TODO: add one-time construction code here
	m_pGumpLoader = NULL;
	m_pHueLoader = NULL;
	
	if (!Init()) AfxThrowUserException();
}

CGumpEditorDoc::~CGumpEditorDoc()
{
	UnInit();
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

	//fix Mulpath if wrong
	if (nConfig::mulpath.size ())
    {
		char last_char = nConfig::mulpath[nConfig::mulpath.length () - 1];
		if ((last_char != '/') && (last_char != '\\'))
		  nConfig::mulpath +="/";
	}

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

CGumpPtr CGumpEditorDoc::LoadGump(int iGumpID)
{
	if (!m_pGumpLoader) return NULL;

	return m_pGumpLoader->LoadGump(iGumpID);
}

void CGumpEditorDoc::SelectGump(int iGumpID)
{	
	CGumpView* pView = (CGumpView*)FindView(RUNTIME_CLASS(CGumpView));	
	if (!pView)  return;

	pView->SetGump(LoadGump(iGumpID));
}

CGumpPtr CGumpEditorDoc::GetSelectedGump(void)
{
	CGumpPtr pDib;
	CGumpView* pView = (CGumpView*)FindView(RUNTIME_CLASS(CGumpView));	
	if (pView) pDib = pView->GetGump();
	
	return pDib;
}


CGumpPtr CGumpEditorDoc::FindGump(int iStartGumpID, int w, int h, int iTryCount/*=10*/)
{
	if (!m_pGumpLoader) return NULL;

	int gw=0, gh=0;
	CGumpPtr pDib;

	for (int i =0; i < iTryCount; i++)
	{
		if (!m_pGumpLoader->GetGumpSize(iStartGumpID, gw, gh)) break;
		if (gw==w && gh==h) return m_pGumpLoader->LoadGump(iStartGumpID);
		iStartGumpID++;
	}

	return NULL;
}

void CGumpEditorDoc::SelectGumpList(int iGumpID)
{
	CGumpListView* pView = (CGumpListView*)FindView(RUNTIME_CLASS(CGumpListView));
	if (!pView) return;

	pView->SelectGump(iGumpID);
}

void CGumpEditorDoc::InsertGump(int iGumpID)
{
	CGumpEditorView* pView = (CGumpEditorView*)FindView(RUNTIME_CLASS(CGumpEditorView));
	if (!pView) return;

	pView->PostMessage(WM_ADDCONTROL);
}

void CGumpEditorDoc::UpdateGumpEditorView(void)
{
	CGumpEditorView* pView = (CGumpEditorView*)FindView(RUNTIME_CLASS(CGumpEditorView));
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

	return TRUE;
}


BOOL CGumpEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	XML::Parser parser;
	XML::Node* form=NULL,* document=NULL;
	XML::Node* section = NULL,* value = NULL;
	CSize sizeForm(m_iDefWidth,m_iDefHeight);
	
	try	{
		parser.loadData(lpszPathName);
		document = parser.parseDocument();

		form = document->findNode("form");
		if(!form ) throw;
		form->lookupAttribute("width",  (int&)sizeForm.cx);
		form->lookupAttribute("height", (int&)sizeForm.cy);

		int alpha=0,flags=0;
		std::string shape;

		form->lookupAttribute("alpha", alpha);
		form->lookupAttribute("flags", flags);
		form->lookupAttribute("shape", shape);

		m_objs.Clear();
		m_objs.SetVirtualSize(sizeForm);

		SetAlpha(alpha);
		SetFlags(flags);
		SetShapeName(shape.c_str());
	} catch (...) {
		return FALSE;
	}

	
	int alpha=0,time=0;
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

	int idx=0;
	XML::Node* control = NULL;	

	while (control = form->findNode("control", idx++)) {
		CDiagramEntity* obj = CDiagramControlFactory::CreateFromString(control);
		if (obj) m_objs.Add(obj);
	}
	
	m_objs.SetModified( FALSE );
	
	delete document; document=NULL;

	return TRUE;
}

BOOL CGumpEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CStdioFile file;
	if (!file.Open(lpszPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
		return FALSE;
	
	CString str;
	CSize size = m_objs.GetVirtualSize();
	str = "<?xml version='1.0'?>\n";
	file.WriteString(str);

	str.Format("<form width='%d' height='%d' alpha='%d' flags='%d' shape='%s'>\n", size.cx, size.cy, GetAlpha(), GetFlags(), GetShapeName());
	file.WriteString( str );

	str.Format(" <fade alpha='%d' time='%d'/>\n", GetFadeAlpha(), GetFadeTime());
	file.WriteString(str);
	str.Format(" <event onclick='%s' onclose='%s' onmouseup='%s' onmousedown='%s' onkeypressed='%s'/>\n",
			GetEventHandler(ONCLICK), GetEventHandler(ONCLOSE), GetEventHandler(ONMOUSEUP), GetEventHandler(ONMOUSEDOWN), GetEventHandler(ONKEYPRESSED));
	file.WriteString(str);

	int count = 0;
	CDiagramEntity* obj;
	while( ( obj = m_objs.GetAt( count++ ) ) ) {
		file.WriteString( obj->GetString(TRUE) + _T( "\n" ) );
		file.WriteString( obj->GetString(FALSE) + _T( "\n" ) );
	}

	file.WriteString("</form>\n");

	file.Close();

	m_objs.SetModified( FALSE );

	SetModifiedFlag(FALSE);

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
	CGumpEditorView* pView = (CGumpEditorView*)FindView(RUNTIME_CLASS(CGumpEditorView));
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

	if( dlg.DoModal() == IDOK )
	{
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

void CGumpEditorDoc::ShowControlList(void)
{
	CGumpEditorView* pView = (CGumpEditorView*)FindView(RUNTIME_CLASS(CGumpEditorView));
	if (!pView) return;

	pView->OnViewControllist();
}
