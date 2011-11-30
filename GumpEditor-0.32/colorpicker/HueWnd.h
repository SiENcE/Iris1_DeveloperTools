#pragma once

class cHueLoader;

const int WM_HUE_CHANGED = WM_USER + 0x400; // wParam : hue id, lParam : rgb color

class CHueWnd : public CWnd
{
// Construction
public:
	CHueWnd();

// Attributes
public:
	cHueLoader* m_pHueLoader;

	
	CPoint m_ptCursor; // caret pos
	int m_iColorSize;  // cell size
	CSize m_sizePage;
	CSize m_sizeTotal;
	int m_iTotalSize;
	
	CRect m_rectColor;

	void UpdateBars();
	// 스크롤바의 상태는 변경없이 관련 정보만 알려준다.
	void GetScrollBarState(CSize& sizeRange, CSize& sizePage, CPoint& ptMove);
	BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);

	CPoint ClientToColor(CPoint point) { return CPoint(point.x / m_iColorSize,  point.y / m_iColorSize + GetScrollPos(SB_VERT));  }
	CPoint ColorToClient(CPoint point) { return CPoint(point.x * m_iColorSize,  point.y * m_iColorSize); }

	COLORREF GetColor(CPoint pt);
	COLORREF GetCurrentColor() { return GetColor(m_ptCursor); }
	DWORD GetCurrentHueId() { return m_ptCursor.x + m_ptCursor.y * m_sizePage.cx + 1; }

	void SelectHue(DWORD hueId);
	void SelectNearHue(COLORREF color);
protected:
	void SelectHue(CPoint point);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHueWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHueWnd();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

