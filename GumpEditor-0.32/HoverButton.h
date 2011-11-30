#if !defined(AFX_HOVERBUTTON_H__16C6D980_BD45_11D3_BDA3_00104B133581__INCLUDED_)
#define AFX_HOVERBUTTON_H__16C6D980_BD45_11D3_BDA3_00104B133581__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HoverButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHoverButton by Niek Albers
// Thanks to some people for the tooltip.
// A cool CBitmapButton derived class with 3 states,
// Up/Down/Hover.

class CDib;

class CHoverButton : public CButton
{
	DECLARE_DYNAMIC(CHoverButton);

	// Construction
public:
	enum STATE { NORMAL=0, HOVER, PRESSED, NUM_STATE };

	CHoverButton();
	virtual ~CHoverButton();
	void SetToolTipText(CString* spText, BOOL bActivate = TRUE);
	void SetToolTipText(int nId, BOOL bActivate = TRUE);
	int GetGumpID(STATE state);
	CGumpPtr GetGump(STATE state);

	BOOL IsCheckbox() { return m_bCheckbox; }
	void SetCheckbox(BOOL bCheckBox) { m_bCheckbox = bCheckBox; }

// Attributes
protected:
	void ActivateTooltip(BOOL bActivate = TRUE);
	BOOL m_bHover;						// indicates if mouse is over the button
	CSize m_ButtonSize;					// width and height of the button
	BOOL m_bTracking;
	BOOL m_bClick;
	BOOL m_bCheckbox;

	CGumpPtr m_pDib[NUM_STATE];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoverButton)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL LoadBitmap(LPCTSTR szNormal, LPCTSTR szHover, LPCTSTR szPressed);
	BOOL SetBitmap(CGumpPtr pNormal, CGumpPtr pHover, CGumpPtr pPressed);
	void GetBitmap(CGumpPtr& pNormal, CGumpPtr& pHover, CGumpPtr& pPressed);

	// Generated message map functions
protected:
	CToolTipCtrl m_ToolTip;
	void InitToolTip();
	//{{AFX_MSG(CHoverButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam) ;
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	bool SetGump(STATE state, CGumpPtr pDib);
	afx_msg void OnBnClicked();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOVERBUTTON_H__16C6D980_BD45_11D3_BDA3_00104B133581__INCLUDED_)
