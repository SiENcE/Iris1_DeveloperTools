//============================================================================
//
// Filename:         splitterWndEx.cpp
// Description:      Implementation of a splitter class that can have 2 panes
//                   with multiple different views
//
//============================================================================

// Required headers
//#include <afxwin.h>
//#include <afxext.h>
#include "stdafx.h"
#include "splitterWndEx.h"

//----------------------------------------------------------------------------
//
// Function:         CSplitterWndEx::CSplitterWndEx
//
// Description:      Class constructor function
//
// Scope:            public
//
// Parameters:
//    None
//
// Return Values:
//    N/A
//
// Remarks:
//    None
//
//----------------------------------------------------------------------------
CSplitterWndEx::CSplitterWndEx()
{
   // Initialize members
   m_eDirection = Unknown;
   m_pSubSplitter[0] = NULL;
   m_pSubSplitter[1] = NULL;
   m_fPaneVisible[0] = TRUE;
   m_fPaneVisible[1] = TRUE;
}

//----------------------------------------------------------------------------
//
// Function:         CSplitterWndEx::~CSplitterWndEx
//
// Description:      Class destructor function
//
// Scope:            public
//
// Parameters:
//    None
//
// Return Values:
//    N/A
//
// Remarks:
//    None
//
//----------------------------------------------------------------------------
CSplitterWndEx::~CSplitterWndEx()
{
   if (m_pSubSplitter[0] != NULL)
      delete m_pSubSplitter[0];
   if (m_pSubSplitter[1] != NULL)
      delete m_pSubSplitter[1];
}

//----------------------------------------------------------------------------
//
// Function:         CSplitterWndEx::Create
//
// Description:      Creates the splitter window
//
// Scope:            public
//
// Parameters:
//    [in] CWnd *pParent                  Pointer to the parent window
//    [in] CCreateContext *pContext       Pointer to the context in which to
//                                        create the splitter
//    [in] ESplitDirection eDirection     Direction to split the window
//    [in] int iChildID                   Child window ID
//
// Return Values:
//    BOOL                 TRUE on success or FALSE on failure
//
// Remarks:
//    None
//
//----------------------------------------------------------------------------
BOOL CSplitterWndEx::Create(CWnd *pParent, CCreateContext *pContext, 
   ESplitDirection eDirection, int iChildID)
{
   // Vertical split
   int iRows, iCols;
   if (eDirection == Vertical)
   {
      m_eDirection = Vertical;
      iRows = 1;
      iCols = 2;
   }
   
   // Horizontal split
   else if (eDirection == Horizontal)
   {
      m_eDirection = Horizontal;
      iRows = 2;
      iCols = 1;
   }

   // Create the split window
   if (!CreateStatic(pParent, iRows, iCols, WS_CHILD | WS_VISIBLE | WS_BORDER,
         iChildID))
   {
      return FALSE;
   }
   return TRUE;
}

//----------------------------------------------------------------------------
//
// Function:         CSplitterWndEx::IsPaneHidden
//
// Description:      Determines if the given pane is hidden or not
//
// Scope:            public
//
// Parameters:
//    [in] ESplitPane ePane               Pane to check
//
// Return Values:
//    BOOL                 TRUE if the pane is hidden or FALSE if not
//
// Remarks:
//    If the window is split horizontally, the split pane must be Top or
//    Bottom.  If split vertically, the pane must be Left or Right.
//
//----------------------------------------------------------------------------
BOOL CSplitterWndEx::IsPaneHidden(ESplitPane ePane)
{
   if (m_eDirection == Horizontal)
   {
      ASSERT((ePane == Top || ePane == Bottom));
      if (ePane == Top && m_fPaneVisible[0])
         return FALSE;
      else if (ePane == Bottom && m_fPaneVisible[1])
         return FALSE;
   }
   else if (m_eDirection == Vertical)
   {
      ASSERT((ePane == Left || ePane == Right));
      if (ePane == Left && m_fPaneVisible[0])
         return FALSE;
      else if (ePane == Right && m_fPaneVisible[1])
         return FALSE;
   }
   return TRUE;
}

//----------------------------------------------------------------------------
//
// Function:         CSplitterWndEx::ShowPane
//
// Description:      Shows or hides the given pane
//
// Scope:            public
//
// Parameters:
//    [in] ESplitPane ePane      Pane to show/hide
//    [in] BOOL fShow            Show the pane
//
// Return Values:
//    None
//
// Remarks:
//    If the window is split horizontally, the split pane must be Top or
//    Bottom.  If split vertically, the pane must be Left or Right.
//
//----------------------------------------------------------------------------
void CSplitterWndEx::ShowPane(ESplitPane ePane, BOOL fShow)
{
   // Make sure a proper pane was passed
   int iIndex;
   if (m_eDirection == Horizontal)
   {
      ASSERT((ePane == Top || ePane == Bottom));
      if (ePane == Top)
         iIndex = 0;
      else
         iIndex = 1;
   }
   else if (m_eDirection == Vertical)
   {
      ASSERT((ePane == Left || ePane == Right));
      if (ePane == Left)
         iIndex = 0;
      else
         iIndex = 1;
   }

   // If the pane's visibility is changing, make sure to update the layout
   if (m_fPaneVisible[iIndex] != fShow)
   {
      if (!fShow)
         HidePane(ePane);
      else
         ShowPane(ePane);
      m_fPaneVisible[iIndex] = fShow;
   }
}

//----------------------------------------------------------------------------
//
// Function:         CSplitterWndEx::GetPaneSubSplitter
//
// Description:      Returns the CSplitterWndEx pointer for the given pane
//
// Scope:            public
//
// Parameters:
//    [in] ESplitPane ePane               Pane to get
//
// Return Values:
//    CSplitterWndEx *     A pointer to the sub-splitter window on success
//                         or a NULL pointer if there is no sub-splitter or
//                         an error occurs
//
// Remarks:
//    If the window is split horizontally, the split pane must be Top or
//    Bottom.  If split vertically, the pane must be Left or Right.
//
//----------------------------------------------------------------------------
CSplitterWndEx *CSplitterWndEx::GetPaneSubSplitter(ESplitPane ePane)
{
   if (m_eDirection == Horizontal)
   {
      ASSERT((ePane == Top || ePane == Bottom));
      if (ePane == Top)
         return m_pSubSplitter[0];
      else
         return m_pSubSplitter[1];
   }
   else if (m_eDirection == Vertical)
   {
      ASSERT((ePane == Left || ePane == Right));
      if (ePane == Left)
         return m_pSubSplitter[0];
      else
         return m_pSubSplitter[1];
   }
   return NULL;
}

//----------------------------------------------------------------------------
//
// Function:         CSplitterWndEx::GetPaneViews
//
// Description:      Gets the list of available views for the given pane
//
// Scope:            public
//
// Parameters:
//    [in] ESplitPane ePane               Pane to get
//
// Return Values:
//    CViewList *          A pointer to the available views list on success or
//                         NULL on failure
//
// Remarks:
//    If the window is split horizontally, the split pane must be Top or
//    Bottom.  If split vertically, the pane must be Left or Right.
//
//----------------------------------------------------------------------------
CViewList *CSplitterWndEx::GetPaneViews(ESplitPane ePane)
{
   if (m_eDirection == Horizontal)
   {
      ASSERT((ePane == Top || ePane == Bottom));
      if (ePane == Top)
         return &m_listViews[0];
      else
         return &m_listViews[1];
   }
   else if (m_eDirection == Vertical)
   {
      ASSERT((ePane == Left || ePane == Right));
      if (ePane == Left)
         return &m_listViews[0];
      else
         return &m_listViews[1];
   }
   return NULL;
}

//----------------------------------------------------------------------------
//
// Function:         CSplitterWndEx::AddView
//
// Description:      Adds a new view to a splitter pane by calling the
//                   CreateView API
//
// Scope:            public
//
// Parameters:
//    [in] ESplitPane ePane               Pane to add the view to
//    [in] CRuntimeClass *pViewClass      The CView-derived class to add
//    [in] CSize size                     Initial window size
//    [in] CCreateContext *pContext       Pointer to the context in which to
//                                        create the window
//
// Return Values:
//    BOOL                 TRUE on success or false on failure
//
// Remarks:
//    If the window is split horizontally, the split pane must be Top or
//    Bottom.  If split vertically, the pane must be Left or Right.
//
//----------------------------------------------------------------------------
BOOL CSplitterWndEx::AddView(ESplitPane ePane, CRuntimeClass *pViewClass,
   CSize size, CCreateContext *pContext)
{
   // Make sure a proper pane was passed
   int iViewsIndex = -1;
   if (m_eDirection == Horizontal)
   {
      ASSERT((ePane == Top || ePane == Bottom));
      if (ePane == Top)
         iViewsIndex = 0;
      else
         iViewsIndex = 1;
   }
   else if (m_eDirection == Vertical)
   {
      ASSERT((ePane == Left || ePane == Right));
      if (ePane == Left)
         iViewsIndex = 0;
      else
         iViewsIndex = 1;
   }

   // Find out which row/column we're adding the view to
   int iRow, iCol;
   PaneToRowCol(ePane, iRow, iCol);

   // Hide the current view of the pane if there is a view attached already
   if (GetDlgItem(IdFromRowCol(iRow, iCol)))
      HideView(iRow, iCol);

   // Create the new view
   if (!CreateView(iRow, iCol, pViewClass, size, pContext))
      return FALSE;

   // Store the new view
   CWnd *pWnd = GetPane(iRow, iCol);
   m_listViews[iViewsIndex].push_back(pWnd);
   
   // Show the window
   ShowView(iRow, iCol, pWnd);
   RedrawWindow();
	return TRUE;
}

//----------------------------------------------------------------------------
//
// Function:         CSplitterWndEx::SwitchView
//
// Description:      Switches the active view in one of the splitter panes
//
// Scope:            public
//
// Parameters:
//    [in] ESplitPane ePane      Pane to switch the view in
//    [in] UINT uiViewIndex      The index of the view to switch to
//
// Return Values:
//    BOOL                 TRUE on success or false on failure
//
// Remarks:
//    If the window is split horizontally, the split pane must be Top or
//    Bottom.  If split vertically, the pane must be Left or Right.
//
//----------------------------------------------------------------------------
BOOL CSplitterWndEx::SwitchView(ESplitPane ePane, UINT uiViewIndex)
{
   // Make sure a proper pane was passed
   int iIndex = -1;
   if (m_eDirection == Horizontal)
   {
      ASSERT((ePane == Top || ePane == Bottom));
      if (ePane == Top)
         iIndex = 0;
      else
         iIndex = 1;
   }
   else if (m_eDirection == Vertical)
   {
      ASSERT((ePane == Left || ePane == Right));
      if (ePane == Left)
         iIndex = 0;
      else
         iIndex = 1;
   }

   // Make sure the index is valid
   if (uiViewIndex >= (UINT)m_listViews[iIndex].size())
      return FALSE;
   
   // Switch the view
   CWnd *pWnd = m_listViews[iIndex][uiViewIndex];
   int iRow, iCol;
   PaneToRowCol(ePane, iRow, iCol);
   if (!HideView(iRow, iCol) || !ShowView(iRow, iCol, pWnd))
      return FALSE;
   RecalcLayout();
   RedrawWindow();
   return TRUE;
}

//----------------------------------------------------------------------------
//
// Function:         CSplitterWndEx::Split
//
// Description:      Splits a window horizontally or vertically
//
// Scope:            public
//
// Parameters:
//    [in] ESplitPane ePane               Pane to split
//    [in] CCreateContext *pContext       Pointer to the context in which to
//                                        create the window
//    [in] ESplitDirection eDirection     Direction to split the window
//
// Return Values:
//    CSplitterWndEx *     A pointer to the newly created splitter on success
//                         or NULL on failure
//
// Remarks:
//    If the window is split horizontally, the split pane must be Top or
//    Bottom.  If split vertically, the pane must be Left or Right.
//
//    If the window is already split, a pointer to the existing splitter is
//    returned instead.
//
//----------------------------------------------------------------------------
CSplitterWndEx *CSplitterWndEx::Split(ESplitPane ePane, 
   CCreateContext *pContext, ESplitDirection eDirection)
{
   // Make sure a proper pane was passed
   int iIndex = -1;
   if (m_eDirection == Horizontal)
   {
      ASSERT((ePane == Top || ePane == Bottom));
      if (ePane == Top)
         iIndex = 0;
      else
         iIndex = 1;
   }
   else if (m_eDirection == Vertical)
   {
      ASSERT((ePane == Left || ePane == Right));
      if (ePane == Left)
         iIndex = 0;
      else
         iIndex = 1;
   }

   // If the pane is already split, just return the existing pointer
   if (m_pSubSplitter[iIndex] != NULL)
      return m_pSubSplitter[iIndex];

   // Get the ID for the pane
	int iRow, iCol;
	PaneToRowCol(ePane, iRow, iCol);
   int iID = IdFromRowCol(iRow, iCol);
   
   // Create the new splitter
   m_pSubSplitter[iIndex] = new CSplitterWndEx();
   m_pSubSplitter[iIndex]->Create(this, pContext, eDirection , iID);
   return m_pSubSplitter[iIndex];
}

//----------------------------------------------------------------------------
//
// Function:         CSplitterWndEx::PaneToRowCol
//
// Description:      Finds the row and column of the specified pane
//
// Scope:            protected
//
// Parameters:
//    [in] ESplitPane ePane      Pane to get row/col info for
//    [in/out] int& iRow         Row number
//    [in/out] int& iCol         Column number
//
// Return Values:
//    None
//
// Remarks:
//    If the window is split horizontally, the split pane must be Top or
//    Bottom.  If split vertically, the pane must be Left or Right.
//
//----------------------------------------------------------------------------
void CSplitterWndEx::PaneToRowCol(ESplitPane ePane, int& iRow, int& iCol)
{
   if (m_eDirection == Horizontal)
   {
      ASSERT((ePane == Top || ePane == Bottom));
      if (ePane == Top)
      {
         iRow = 0;
         iCol = 0;
      }
      else
      {
         iRow = 1;
         iCol = 0;
      }
   }
   else if (m_eDirection == Vertical)
   {
      ASSERT((ePane == Left || ePane == Right));
      if (ePane == Left)
      {
         iRow = 0;
         iCol = 0;
      }
      else
      {
         iRow = 0;
         iCol = 1;
      }
   }
}

//----------------------------------------------------------------------------
//
// Function:         CSplitterWndEx::HideView
//
// Description:      Hides the view on a pane; Used by SwitchView() function
//
// Scope:            protected
//
// Parameters:
//    [in] int iRow           Row number of pane
//    [in] int iCol           Column number of pane
//
// Return Values:
//    BOOL                 TRUE on success or FALSE on failure
//
// Remarks:
//    None
//
//----------------------------------------------------------------------------
BOOL CSplitterWndEx::HideView(int iRow, int iCol)
{
   CWnd *pWnd = GetPane(iRow, iCol);
   if (pWnd == NULL)
      return FALSE;
   pWnd->SetDlgCtrlID(0);
   pWnd->ShowWindow(SW_HIDE);
   return TRUE;
}

//----------------------------------------------------------------------------
//
// Function:         CSplitterWndEx::ShowView
//
// Description:      Shows the view on a pane; Used by SwitchView() function
//
// Scope:            protected
//
// Parameters:
//    [in] int iRow           Row number of pane
//    [in] int iCol           Column number of pane
//    [in] CWnd *pWnd         Pointer to the view to show
//
// Return Values:
//    BOOL                 TRUE on success or FALSE on failure
//
// Remarks:
//    None
//
//----------------------------------------------------------------------------
BOOL CSplitterWndEx::ShowView(int iRow, int iCol, CWnd *pWnd)
{
   if (pWnd == NULL)
      return FALSE;
   pWnd->SetDlgCtrlID(IdFromRowCol(iRow, iCol));
   pWnd->ShowWindow(SW_SHOW);
   return TRUE;
}

//----------------------------------------------------------------------------
//
// Function:         CSplitterWndEx::ShowPane
//
// Description:      Shows a pane; Used by ShowPane() public function
//
// Scope:            protected
//
// Parameters:
//    [in] ESplitPane ePane      Pane to show
//
// Return Values:
//    None
//
// Remarks:
//    If the window is split horizontally, the split pane must be Top or
//    Bottom.  If split vertically, the pane must be Left or Right.
//
//----------------------------------------------------------------------------
void CSplitterWndEx::ShowPane(ESplitPane ePane)
{
   // Make sure a proper pane was passed
   int iIndex, iOuterMax, iInnerStart;
   if (m_eDirection == Horizontal)
   {
      ASSERT((ePane == Top || ePane == Bottom));
      if (ePane == Top)
         iIndex = 0;
      else
         iIndex = 1;
      m_nRows++;
      iOuterMax = m_nCols;
      iInnerStart = m_nRows - 2;
   }
   else if (m_eDirection == Vertical)
   {
      ASSERT((ePane == Left || ePane == Right));
      if (ePane == Left)
         iIndex = 0;
      else
         iIndex = 1;
      m_nCols++;
      iOuterMax = m_nRows;
      iInnerStart = m_nCols - 2;
   }
   
   // Show the row/column
   for (int i = 0; i < iOuterMax; i++)
   {
      CWnd *pPaneShow;
      if (m_eDirection == Horizontal)
         pPaneShow = GetDlgItem(AFX_IDW_PANE_FIRST + m_nRows * 16 + i);
      else
         pPaneShow = GetDlgItem(AFX_IDW_PANE_FIRST + i * 16 + m_nCols);
      if (pPaneShow == NULL)
         continue;
      pPaneShow->ShowWindow(SW_SHOWNA);
      
      for (int j = iInnerStart; j >= iIndex; j--)
      {
         if (m_eDirection == Horizontal)
         {
            CWnd *pPane = GetPane(j, i);
            if (pPane != NULL)
               pPane->SetDlgCtrlID(IdFromRowCol(j + 1, i));
         }
         else
         {
            CWnd *pPane = GetPane(i, j);
            if (pPane != NULL)
               pPane->SetDlgCtrlID(IdFromRowCol(j, i + 1));
         }
      }
      if (m_eDirection == Horizontal)
         pPaneShow->SetDlgCtrlID(IdFromRowCol(iIndex, i));
      else
         pPaneShow->SetDlgCtrlID(IdFromRowCol(i, iIndex));
   }
   RecalcLayout();
}

//----------------------------------------------------------------------------
//
// Function:         CSplitterWndEx::HidePane
//
// Description:      Hides a pane; Used by ShowPane() public function
//
// Scope:            protected
//
// Parameters:
//    [in] ESplitPane ePane      Pane to hide
//
// Return Values:
//    None
//
// Remarks:
//    If the window is split horizontally, the split pane must be Top or
//    Bottom.  If split vertically, the pane must be Left or Right.
//
//----------------------------------------------------------------------------
void CSplitterWndEx::HidePane(ESplitPane ePane)
{
   // Make sure a proper pane was passed
   int iIndex, iOuterMax, iInnerMax;
   if (m_eDirection == Horizontal)
   {
      ASSERT((ePane == Top || ePane == Bottom));
      if (ePane == Top)
         iIndex = 0;
      else
         iIndex = 1;
      iOuterMax = m_nCols;
      iInnerMax = m_nRows;
   }
   else if (m_eDirection == Vertical)
   {
      ASSERT((ePane == Left || ePane == Right));
      if (ePane == Left)
         iIndex = 0;
      else
         iIndex = 1;
      iOuterMax = m_nRows;
      iInnerMax = m_nCols;
   }
   
   // Change the active window if there is one
   int iActiveRow, iActiveCol;
   if (GetActivePane(&iActiveRow, &iActiveCol) != NULL)
   {
      if (m_eDirection == Horizontal && iActiveRow == iIndex)
      {
         if (++iActiveRow >= m_nRows)
            iActiveRow = 0;
      }
      else if (m_eDirection == Vertical && iActiveCol == iIndex)
      {
         if (++iActiveCol >= m_nCols)
            iActiveCol = 0;
      }
      SetActivePane(iActiveRow, iActiveCol);
   }
   
   // Hide the row/column
   for (int i = 0; i < iOuterMax; i++)
   {
      CWnd *pPaneHide;
      int iCtrlID;
      if (m_eDirection == Horizontal)
      {
         pPaneHide = GetPane(iIndex, i);
         iCtrlID = AFX_IDW_PANE_FIRST + m_nRows * 16;
      }
      else
      {
         pPaneHide = GetPane(i, iIndex);
         iCtrlID = AFX_IDW_PANE_FIRST + i * 16 + m_nCols;
      }
      pPaneHide->ShowWindow(SW_HIDE);
      pPaneHide->SetDlgCtrlID(iCtrlID);
      
      for (int j = iIndex + 1; j < iInnerMax; j++)
      {
         CWnd *pPane;
         if (m_eDirection == Horizontal)
         {
            pPane = GetPane(j, i);
            if (pPane != NULL)
               pPane->SetDlgCtrlID(IdFromRowCol(j - 1, i));
         }
         else
         {
            pPane = GetPane(i, j);
            if (pPane != NULL)
               pPane->SetDlgCtrlID(IdFromRowCol(i, j - 1));
         }
      }
   }
   
   // Update the layout
   if (m_eDirection == Horizontal)
   {
      m_nRows--;
      m_pRowInfo[m_nRows].nCurSize = m_pRowInfo[iIndex].nCurSize;
   }
   else
   {
      m_nCols--;
      m_pColInfo[m_nCols].nCurSize = m_pColInfo[iIndex].nCurSize;
   }
   RecalcLayout();
}

