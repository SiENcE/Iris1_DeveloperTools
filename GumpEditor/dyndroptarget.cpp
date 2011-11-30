// Copyright ?2002 Oink!
// FILE NAME	: DynDropTarget.cpp
// DESCRIPTION	: Provides basic member variables and functions for all
//				: CWnd objects to act as drop target for control dragged from
//				: toolbar
//
// PROGRAMMER	DATE		PATCH	DESCRIPTION
// ============	===========	=======	===============================================

#include "stdafx.h"
#include "gumpeditor.h"
#include "dyndroptarget.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static UINT uiCursorID[] =
{
	IDC_EDITCURSOR, //DYN_EDIT,
	IDC_EDITCURSOR, //DYN_MULTIEDIT,
	IDC_LABELCURSOR,//DYN_LABEL,
	IDC_GROUPCURSOR,//DYN_GROUP,
	IDC_COMBOCURSOR,//DYN_COMBO,
	IDC_CHECKCURSOR,//DYN_CHECK,
	IDC_CHECKCURSOR,//DYN_DATE,
	IDC_DATECURSOR, //DYN_RADIO,
	IDC_PICTURECURSOR,//DYN_PICTURE,
	IDC_LISTCURSOR,//DYN_LIST,
	IDC_BUTTONCURSOR,//DYN_BUTTON,
};

SCODE CDynDropSource::GiveFeedback(DROPEFFECT dropEffect)
{
	UINT cursorID = IDC_EDITCURSOR;
	if (0 <= nControlType && nControlType < sizeof(uiCursorID)/sizeof(uiCursorID[0]))
		cursorID = uiCursorID[nControlType];
	
	SetCursor(AfxGetApp()->LoadCursor(cursorID));

	return S_OK;
} 


/////////////////////////////////////////////////////////////////////////////
// CDynDropTarget

IMPLEMENT_DYNCREATE(CDynDropTarget, CCmdTarget)

CDynDropTarget::CDynDropTarget()
{
}

CDynDropTarget::~CDynDropTarget()
{
}


BEGIN_MESSAGE_MAP(CDynDropTarget, COleDropTarget)
	//{{AFX_MSG_MAP(CDynDropTarget)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynDropTarget message handlers

BOOL CDynDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect,CPoint point)
{
	// Detects a drop and adds a new control to the screen
	
	if (!(pDataObject->IsDataAvailable(m_nIDClipFormat)))
		return FALSE; 
	
	STGMEDIUM stgmedium;
	int nType;

	if (pDataObject->GetData(m_nIDClipFormat, &stgmedium))
	{
		HGLOBAL hGlobal = stgmedium.hGlobal;
		LPCTSTR pText = (LPCTSTR)GlobalLock(hGlobal);
		nType = atoi(pText);

		// free memory
		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);

		pWnd->SendMessage(WM_ADDCONTROL,(WPARAM)nType,(LPARAM)MAKELONG(point.y,point.x));
	}

	return FALSE;
}

DROPEFFECT CDynDropTarget::OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	// Determines if data is available to drop

	if (pDataObject->IsDataAvailable(m_nIDClipFormat))
			return DROPEFFECT_COPY;

	return DROPEFFECT_NONE; 
}

DROPEFFECT CDynDropTarget::OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	// Determines if data is available to drop

	CString strPos;
	strPos.Format(" x: %d, y: %d ",point.x,point.y);

	if (pDataObject->IsDataAvailable(m_nIDClipFormat))
		return DROPEFFECT_COPY;

	return DROPEFFECT_NONE; 
}
