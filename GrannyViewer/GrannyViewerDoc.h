// GrannyViewerDoc.h : CGrannyViewerDoc 클래스의 인터페이스
//
#pragma once

#include <vector>

class cGrannyModel;
class cGrannyLoader;
class cStitchinLoader;
class cModelInfoLoader;


inline UINT GETID(UINT c)	   { return c & 0xFFFF; }
inline UINT ISEQUIP(UINT c)    { return c >> 16;    }

class CGrannyViewerDoc : public CDocument
{
protected:
	CGrannyViewerDoc();
	DECLARE_DYNCREATE(CGrannyViewerDoc)


public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);

public:
	virtual ~CGrannyViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	cGrannyLoader     *m_pGrannyLoader;
	cStitchinLoader   *m_pStitchinLoader;
	cModelInfoLoader  *m_pModelInfoLoader;

	bool m_bInit;
	enum LAST_SELECT { SEL_NONE, SEL_ANIM, SEL_MODEL, SEL_EQUIP } m_lastSelect;
	UINT m_iCurAnimId, m_iCurModelId, m_iLastId;
	std::vector<UINT> m_arRegister;

	bool IsRegisterModel(UINT id);

public:
	enum MODEL { MCURRENT, MREGISTER, MALL };
	void ClearModels(MODEL model=MALL);
	
	void SetCurModelId(UINT iModelId);
	void SetCurAnimId(UINT iAnimId);

	UINT  GetCurModelId()                           { return m_iCurModelId;        }
	UINT  GetCurAnimId()                            { return m_iCurAnimId;         }
	UINT  GetReigsterModelCount()                   { return m_arRegister.size();  }
	UINT  GetReigsterModelId(UINT i)                { return m_arRegister[i];      }

	COLORREF GetModelColor(UINT id);
	void     SetModelColor(UINT id, COLORREF color);
	
	bool IsShowModel(UINT id);
	void ShowModel(UINT id, int show); // show 0: hide, 1: show, -1: toggle
	void ShowAllModels(int show);

	void ResetReigsterModel()                       { m_arRegister.clear();        }
	void RegisterModel(UINT id, bool first=false);
	void UnregisterModel(UINT id);
	std::vector<UINT> GetReisterModels()            { return m_arRegister;         }

	void Log(LPCTSTR lpszLog);
	bool Load(LPCTSTR lpszPathName);

	CView            *FindView(CRuntimeClass* pClass);
	cGrannyLoader    *GetGrannyLoader()             { return m_pGrannyLoader;      }
	cStitchinLoader  *GetStitchinLoader()           { return m_pStitchinLoader;    }
	cModelInfoLoader *GetModelInfoLoader()          { return m_pModelInfoLoader;   }
	cGrannyModel     *GetModel(UINT id);
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileViewxml();
	afx_msg void OnFileClearlog();
	afx_msg void OnFileExportmodel();
	afx_msg void OnLogLoglevel1();
	afx_msg void OnUpdateLogLoglevel1(CCmdUI *pCmdUI);
	afx_msg void OnLogLoglevel2();
	afx_msg void OnUpdateLogLoglevel2(CCmdUI *pCmdUI);
	afx_msg void OnLogLoglevel3();
	afx_msg void OnUpdateLogLoglevel3(CCmdUI *pCmdUI);
	afx_msg void OnLogLoglevel4();
	afx_msg void OnUpdateLogLoglevel4(CCmdUI *pCmdUI);
	afx_msg void OnLogSetfont();
	afx_msg void OnFileDumpmodel();
	afx_msg void OnFileShellopen();
	afx_msg void OnFileReload();
	afx_msg void OnModelClear();
	afx_msg void OnModelClearall();
	afx_msg void OnModelClearRegistered();
	afx_msg void OnModelColor();
	afx_msg void OnModelResetcolor();
	afx_msg void OnUpdateModelResetcolor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModelColor(CCmdUI *pCmdUI);
	afx_msg void OnModelClearmodel();
};


