// DialogEditorDemoDoc.h : interface of the CDialogEditorDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOGEDITORDEMODOC_H__123376D9_1F69_4731_8BA4_50D6E0449623__INCLUDED_)
#define AFX_DIALOGEDITORDEMODOC_H__123376D9_1F69_4731_8BA4_50D6E0449623__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// --- DiagramEditor ---
#include "DiagramEditor\DiagramEntityContainer.h"

class CDialogEditorDemoDoc : public CDocument
{
protected: // create from serialization only
	CDialogEditorDemoDoc();
	DECLARE_DYNCREATE(CDialogEditorDemoDoc)

// Attributes
public:

	// --- DiagramEditor ---
	CDiagramEntityContainer*	GetData();

// Operations
public:

	// --- DiagramEditor ---
	void	Export();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEditorDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDialogEditorDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDialogEditorDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// --- DiagramEditor ---
protected:
	CDiagramEntityContainer	m_objs; // The object data

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGEDITORDEMODOC_H__123376D9_1F69_4731_8BA4_50D6E0449623__INCLUDED_)
