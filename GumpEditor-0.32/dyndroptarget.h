// Copyright ?2002 Oink!
// FILE NAME	: DynDropTarget.h
// DESCRIPTION	: Provides basic member variables and functions for all
//				: CWnd objects to act as drop target for control dragged from
//				: toolbar
//
// PROGRAMMER	DATE		PATCH	DESCRIPTION
// ============	===========	=======	===============================================

#pragma once

#define WM_SCREENDESIGNER	(WM_USER + 100)
#define WM_ADDCONTROL		(WM_USER + 101)

enum DYN_CONTROL_TYPE 
{
	DYN_EDIT,
	DYN_EDITMULTI,
	DYN_LABEL,
	DYN_GROUP,
	DYN_COMBO,
	DYN_CHECK,
	DYN_DATE,
	DYN_RADIO,
	DYN_PICTURE,
	DYN_LIST,
	DYN_BUTTON
};

class CDynDropSource : public COleDropSource
{
public:
	DYN_CONTROL_TYPE nControlType;

	SCODE GiveFeedback(DROPEFFECT dropEffect);
	CDynDropSource() : nControlType(DYN_EDIT) {}
};

class CDynDropTarget : public COleDropTarget
{
	DECLARE_DYNCREATE(CDynDropTarget)

	CDynDropTarget();           // protected constructor used by dynamic creation

// Attributes
public:
	UINT m_nIDClipFormat;

// Operations
public:
	virtual DROPEFFECT OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	virtual BOOL OnDrop( CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point );
	virtual DROPEFFECT OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
  
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynDropTarget)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDynDropTarget();

	// Generated message map functions
	//{{AFX_MSG(CDynDropTarget)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
