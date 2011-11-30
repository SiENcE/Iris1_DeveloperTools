#if !defined(AFX_DEDITOR_H__C84FB781_CCDF_4AC5_A2A2_F5AD2648F0C4__INCLUDED_)
#define AFX_DEDITOR_H__C84FB781_CCDF_4AC5_A2A2_F5AD2648F0C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DEditor.h : header file
//
#include "DiagramEditor\DiagramEditor.h"

/////////////////////////////////////////////////////////////////////////////
// CDEditor window

class CDEditor : public CDiagramEditor
{
public:
	virtual void DrawBackground( CDC* dc, CRect rect, double zoom ) const;
	virtual void DrawGrid( CDC* dc, CRect rect, double zoom ) const;
	bool AdjustRect(CRect& rect);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEDITOR_H__C84FB781_CCDF_4AC5_A2A2_F5AD2648F0C4__INCLUDED_)
