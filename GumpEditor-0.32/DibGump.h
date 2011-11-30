#pragma once
#include "cdib.h"

class CDibGump : public CDib
{
public:
	CDibGump() : m_iGumpID(-1) {}
	CDibGump(int iGumpID, CSize sizeDib, LPBYTE pData);
	virtual ~CDibGump(void);

	int m_iGumpID;
	int GetGumpID() { return m_iGumpID; }
	void DrawGump(CDC *dc, CRect rect, double zoom);
	void DrawGump(CDC *dc, CPoint pt, double zoom);
};
