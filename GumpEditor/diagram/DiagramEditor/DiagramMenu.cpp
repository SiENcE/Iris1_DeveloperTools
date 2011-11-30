/* ==========================================================================
	CDiagramMenu

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-30

	Purpose :		CDiagramMenu is a simple class to allow easy 
					customization of the popup menu of the CDiagramEditor 
					without deriving a new class.

	Description :	Contains a CMenu that is exposed through GetPopupMenu. 

	Usage :			Derive a class from CDiagramMenu, and implement 
					GetPopupMenu. This function is expected to return a 
					pointer to a CMenu. The menu alternatives on the menu 
					should have menu ids between CMD_START and CMD_END 
					inclusively (some are predefined). The constants are 
					defined in DiagramEntity.h. The CDiagramMenu should 
					be added to the CDiagramEditor class with a call to 
					SetPopupMenu. Note that the editor then owns the menu 
					and will delete it as soon as a new menu is set and 
					in the dtor, so either heap allocate it or allocate and 
					add it so:

						m_editor.SetPopupMenu( new CMyDiagramMenuDerivedClass );

					Whenever a menu alternative is selected from the popup 
					menu, the command is sent to all selected objects. The 
					objects will then have to handle the messages they are 
					interested in in their DoCommand functions.

					The class is a friend class to CDiagramEditor, so all 
					members are accessible.

   ========================================================================*/
#include "stdafx.h"
#include "../GumpEditor.h"
#include "DiagramMenu.h"
#include "DiagramEditor.h"

CDiagramMenu::CDiagramMenu()
/* ============================================================
	Function :		CDiagramMenu::CDiagramMenu
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

CDiagramMenu::~CDiagramMenu()
/* ============================================================
	Function :		CDiagramMenu::~CDiagramMenu
	Description :	destructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	if( m_menu.m_hMenu != NULL )
		m_menu.DestroyMenu();

}

CMenu* CDiagramMenu::GetPopupMenu( CDiagramEditor* editor )
/* ============================================================
	Function :		CDiagramMenu::GetPopupMenu
	Description :	Gets a menu pointer to the desired popup 
					menu.
					
	Return :		CMenu*					-	A pointer to 
												the popup menu
	Parameters :	CDiagramEditor* editor	-	The editor 
												calling for a 
												menu.
					
	Usage :			Call to get the popup menu for the editor.

   ============================================================*/
{

	if( m_menu.m_hMenu == NULL )
	{
		if( m_menu.CreatePopupMenu() )
		{
			m_menu.AppendMenu( MF_STRING, ID_EDIT_CUT, _T( "Cu&t" ) );
			m_menu.AppendMenu( MF_STRING, ID_EDIT_COPY, _T( "C&opy" ) );
			m_menu.AppendMenu( MF_STRING, ID_EDIT_PASTE, _T( "&Paste" ) );
			m_menu.AppendMenu( MF_SEPARATOR );
			m_menu.AppendMenu( MF_STRING, CMD_FREEZE, _T( "Freeze Selection" ) );
			m_menu.AppendMenu( MF_STRING, CMD_UNFREEZE_ALL, _T( "Unfreeze All" ) );
			m_menu.AppendMenu( MF_SEPARATOR );
			m_menu.AppendMenu( MF_STRING, CMD_HIDE, _T( "Hide Selection" ) );	
			m_menu.AppendMenu( MF_STRING, CMD_UNHIDE_ALL, _T( "Unhide All" ) );
			m_menu.AppendMenu( MF_STRING, CMD_HIDE_UNSEL, _T( "Hide Unselected" ) );
			m_menu.AppendMenu( MF_SEPARATOR );
			m_menu.AppendMenu( MF_STRING, CMD_CONTROL_LIST, _T( "Control List" ) );
			m_menu.AppendMenu( MF_STRING, CMD_SHOW_CODE, _T( "Code View" ) );
		}
	}

	UINT cut = MF_GRAYED;
	UINT copy = MF_GRAYED;
	UINT paste = MF_GRAYED;
	if( editor->IsAnyObjectSelected() )
	{
		cut = 0;
		copy = 0;
	}

	if( editor->GetDiagramEntityContainer() && editor->GetDiagramEntityContainer()->ObjectsInPaste() )
		paste = 0;

	m_menu.EnableMenuItem( ID_EDIT_CUT, MF_BYCOMMAND | cut );
	m_menu.EnableMenuItem( ID_EDIT_COPY, MF_BYCOMMAND | copy );
	m_menu.EnableMenuItem( ID_EDIT_PASTE, MF_BYCOMMAND | paste );

	return &m_menu;

}