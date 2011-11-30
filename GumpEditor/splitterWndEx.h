//============================================================================
//
// Filename:         splitterWndEx.h
// Description:      Interface for a splitter class that can have 2 panes
//                   with multiple different views
//
//============================================================================

// Only include this file once
#pragma once

// Required headers
#include <vector>
using namespace std;

// Special types
typedef enum tagESplitDirection { Unknown, Vertical, Horizontal } 
ESplitDirection;
typedef enum tagESplitPane { Top, Bottom, Left, Right } ESplitPane;
typedef vector<CWnd *> CViewList;
typedef CViewList::iterator CViewListIter;

// Class definition
class CSplitterWndEx : public CSplitterWnd  
{
public:
	CSplitterWndEx();
	virtual ~CSplitterWndEx();

	// Overridden functions
	BOOL Create(CWnd *pParent, CCreateContext *pContext, 
		ESplitDirection eDirection = Vertical, 
		int iChildID = AFX_IDW_PANE_FIRST);

	// Member access
	ESplitDirection GetSplitDirection() { return m_eDirection; }
	BOOL IsPaneHidden(ESplitPane ePane);
	void ShowPane(ESplitPane ePane, BOOL fShow);
	CSplitterWndEx *GetPaneSubSplitter(ESplitPane ePane);
	CViewList *GetPaneViews(ESplitPane ePane);

	// Actions
	BOOL AddView(ESplitPane ePane, CRuntimeClass *pViewClass, 
		CSize size, CCreateContext *pContext);
	BOOL SwitchView(ESplitPane ePane, UINT uiViewIndex);
	CSplitterWndEx *Split(ESplitPane ePane, CCreateContext *pContext,
		ESplitDirection eDirection = Vertical);

protected:
	ESplitDirection m_eDirection;          // Split direction
	BOOL m_fPaneVisible[2];                // Is the pane visible?
	CSplitterWndEx *m_pSubSplitter[2];     // Sub-splitter windows
	CViewList m_listViews[2];              // List of associated views

	// Helper functions
	void PaneToRowCol(ESplitPane ePane, int& iRow, int& iCol);
	BOOL HideView(int iRow, int iCol);
	BOOL ShowView(int iRow, int iCol, CWnd *pWnd);
	void ShowPane(ESplitPane ePane);
	void HidePane(ESplitPane ePane);
};

