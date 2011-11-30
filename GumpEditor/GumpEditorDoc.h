// GumpEditorDoc.h : interface of the CGumpEditorDoc class
//


#pragma once

#include "iris/GumpLoader.h"
#include "iris/HueLoader.h"
#include "diagram/DialogEditorDemoDoc.h"
#include <map>

class CGumpView;
class CGumpEditorView;
class CLeftVIew;
class CDynControl;

class CGumpEditorDoc : public CDialogEditorDemoDoc
{
protected: // create from serialization only
	CGumpEditorDoc();
	DECLARE_DYNCREATE(CGumpEditorDoc)

// Attributes
public:
	CView* FindView(CRuntimeClass* pClass);

	cGumpLoader* GetGumpLoader()  { return m_pGumpLoader; }
	
	CGumpPtr GetGump(int iGumpID) { return FindGump(iGumpID,0,0,1); }
	// if w,h is 0, ignore w, h
	CGumpPtr FindGump(int iStartGumpID, int w, int h, int iTryCount=10);

	cHueLoader* GetHueLoader() { return m_pHueLoader; }
	const stHue* GetHue(int iHueId);
	// if iHudeId=0 then use default font color
	COLORREF GetHueColor(int iHueId, int iFontId=-1);

	CFont* GetFont(int iFontId);

	CString GetName() const { return m_strName; }
	void SetName(LPCTSTR szName) { m_strName = szName; }
	
	enum EVENT { ONCLICK, ONCLOSE, ONMOUSEDOWN, ONMOUSEUP, ONKEYPRESSED, NUM_EVENT };
	
	CString GetEventHandler(EVENT e) const { return m_strEventHandler[e]; }
	void SetEventHandler(EVENT e, LPCTSTR szEventHandler) { m_strEventHandler[e] = szEventHandler; SetModifiedFlag(); }
	void GetEventHandler(CString& strEvClick, CString& strEvClose, CString& strEvMouseUp, CString& strEvMouseDown, CString& strEvKeyPressed) const;
	void SetEventHandler(LPCTSTR szEvClick, LPCTSTR szEvClose, LPCTSTR szEvMouseUp, LPCTSTR szEvMouseDown, LPCTSTR szEvKeyPressed);
	
	int GetAlpha() const { return m_iAlpha; }
	void SetAlpha(int iAlpha) { m_iAlpha = iAlpha; SetModifiedFlag(); }

	int GetFlags() const { return m_iFlags; }
	void SetFlags(int iFlags) { m_iFlags = iFlags; SetModifiedFlag(); }

	void SetFade(int iAlpha, int iTime) { m_iFadeAlpha = iAlpha; m_iFadeTime = iTime; SetModifiedFlag(); }
	void GetFade(int& iAlpha, int& iTime) const { iAlpha = m_iFadeAlpha; iTime = m_iFadeTime; }
	int GetFadeAlpha() const { return m_iFadeAlpha; }
	int GetFadeTime() const { return m_iFadeTime; }
	
	void SetShapeName(LPCTSTR szShapeName) { m_strShapeName = szShapeName; SetModifiedFlag(); }
	CString GetShapeName() const { return m_strShapeName; }

	void SelectGump(int iGumpID);
	CGumpPtr GetSelectedGump(void);

	void SetBackgroundImage(CGumpPtr pDib);
	
	void SelectGumpList(int iGumpID);
	void InsertGump(int iGumpID);
	void UpdateGumpEditorView(void);

	void ShowCode(BOOL bShow, CDiagramEntity *entity=NULL);
	void ShowControlList(void);

	CString GetDocVersion() const { return m_strDocVersion; }
	CString GetLoadDocVersion() const { return m_strLoadDocVersion; }

	CString GetDocName() const;
protected:
	CString m_strDocVersion;
	CString m_strLoadDocVersion;
	void    SetLoadDocVersion(LPCTSTR szVersion) { m_strLoadDocVersion; }

	static const int m_iDefWidth  = 640;
	static const int m_iDefHeight = 480;

	CString m_strName;
	CString m_strEventHandler[NUM_EVENT];
	int m_iAlpha;
	int m_iFlags;
	int m_iFadeAlpha, m_iFadeTime;
	CString m_strShapeName;

	cHueLoader* m_pHueLoader;
	struct sMyFont {
		std::string file;
		COLORREF color;
		CFont* font;
	};
	std::map<int,sMyFont> m_fonts;
	typedef std::map<int,sMyFont>::iterator font_iter;
	
	cGumpLoader* m_pGumpLoader;

	bool Init();
	void UnInit();

	bool m_bInitUOData;

	CString m_strIrisPath;
	CString m_strXmlViewerPath;

	bool LoadGfmFile(LPCTSTR lpszPathName, bool bLoadForm);
	// Operations
public:

// Implementation
public:
	virtual ~CGumpEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnSettings();
	afx_msg void OnViewSwitch();
	afx_msg void OnViewViewxml();
protected:
	virtual BOOL SaveModified();
public:
	afx_msg void OnFileRun();
public:
	afx_msg void OnViewSetting();
public:
	afx_msg void OnFileImport();
};

CGumpEditorDoc* GfxGetGumpDocument();
