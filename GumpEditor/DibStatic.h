#pragma once


// CDibStatic

class CDibStatic : public CStatic
{
	DECLARE_DYNAMIC(CDibStatic)
public:
	CGumpPtr m_pDib;
public:
	CDibStatic(CGumpPtr pDib);
	virtual ~CDibStatic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPaint();
	int GetGumpID(void);
	void SetGump(CGumpPtr pDib);
};


