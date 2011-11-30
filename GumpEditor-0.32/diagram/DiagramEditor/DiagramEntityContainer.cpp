/* ==========================================================================
	CDiagramEntityContainer

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-29

	Purpose :		CDiagramEntityContainer contains the data for a 
					CDiagramEditor.	It manages mass operations such as 
					copying, pasting and undo. It is completely separated 
					from CDiagramEditor to allow the package to be used 
					in a doc/view app. This is also the reason why some 
					functionality is accessible in both this class and ?
					CDiagramEditor.

	Description :	The class contains a CObArray with the instances of 
					CDiagramEntity-derived classes that is the current data 
					for an editor. It also contains a pointer to a 
					CDiagramClipboardHandler-instance that works as the 
					"clipboard" for an editor. Furthermore, It contains an
					CObArray of CObArrays that is the undo stack.

	Usage :			Normally, this class need not be derived from. A 
					CDiagramEditor needs an instance of 
					CDiagramEntityContainer to hold the object data. This 
					instance can either be external, as for a doc/view app 
					where the container belongs to the document, or 
					internal, as for a dialog application where the editor 
					will manage all of the data. In the first case, a 
					CDiagramEntityContainer member should be declared in 
					the document class, and a pointer to it submitted to 
					the Create-call of the CDiagramEditor (or by calling 
					CDiagramEditor::SetCDiagramEntityContainer). In the 
					second case, nothing special need to be done - the 
					CDiagramEntityContainer will be created during the 
					CDiagramEditor::Create call automatically if no pointer 
					is submitted.
					
					The container is not using the Windows clipboard 
					(because of instantiation questions on derived 
					entities), but rather an external clipboard handler 
					derived from CDiagramClipboardHandler. This handler is 
					set calling SetClipboardHandler, and several containers 
					can share the same handler. If no clipboard handler is 
					set, a default internal one will be used.

					CDiagramEntityContainer manages all data internally, 
					all internal objects are deleted in the class dtor.

	Changes :		19/4 2004	Made RemoveAt virtual.
					20/4 2004	Made several Undo- and Copy/Paste functions 
								virtual. Added array accessors for derived 
								classes. Moved the member function Find to 
								protected.
					30/4 2004	Copy/paste-handling removed to a separate 
								class to allow several containers to share 
								the same clipboard.
					30/4 2004	Changed c-style casts to static_cast
   ========================================================================*/

#include "stdafx.h"
#include "DiagramEntityContainer.h"
#include "DiagramEntity.h"
#include "Tokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning( disable : 4706 )

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer construction/destruction/initialization

CDiagramEntityContainer::CDiagramEntityContainer( CDiagramClipboardHandler* clip )
/* ============================================================
	Function :		CDiagramEntityContainer::CDiagramEntityContainer
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
	m_lastSelObj=NULL;
	m_clip = clip;

	SetUndoStackSize( 0 );
	Clear();
	SetVirtualSize( CSize( 0, 0 ) );

}

CDiagramEntityContainer::~CDiagramEntityContainer()
/* ============================================================
	Function :		CDiagramEntityContainer::~CDiagramEntityContainer
	Description :	destructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	Clear();

}

void CDiagramEntityContainer::Clear()
/* ============================================================
	Function :		CDiagramEntityContainer::Clear
	Description :	Removes all data from the data and undo.
					
	Return :		void
	Parameters :	none

	Usage :			Call to remove data from the container. The 
					Paste-array will be kept.

   ============================================================*/
{

	RemoveAll();
	ClearUndo();
	SetModified( FALSE );

}

CString CDiagramEntityContainer::GetString() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetString
	Description :	Returns a string representation of the 
					virtual paper size
					
	Return :		CString	-	Resulting string
	Parameters :	none

	Usage :			Call to get a string representing the paper 
					size of the container. The format is 
					"paper:[x],[y];" where [x] and [y] are the 
					horisintal and vertical sizes.

   ============================================================*/
{

	CString str;
	str.Format( _T( "paper:%i,%i;" ), GetVirtualSize().cx, GetVirtualSize().cy );
	return str;

}

BOOL CDiagramEntityContainer::FromString( const CString& str )
/* ============================================================
	Function :		CDiagramEntityContainer::FromString
	Description :	Sets the virtual paper size from a string.
					
	Return :		BOOL				-	TRUE if the string 
											represented a 
											paper.
	Parameters :	const CString& str	-	The string 
											representation.
					
	Usage :			Call to set the paper size of the container 
					from a string. The format is "paper:[x],[y];" 
					where [x] and [y] are the horisontal and 
					vertical sizes.

   ============================================================*/
{

	BOOL result = FALSE;

	CTokenizer main( str, _T( ":" ) );
	CString header;
	CString data;
	if( main.GetSize() == 2 )
	{
		main.GetAt( 0, header );
		main.GetAt( 1, data );
		header.TrimLeft();
		header.TrimRight();
		data.TrimLeft();
		data.TrimRight();
		if( header == _T( "paper" ) )
		{
			CTokenizer tok( data.Left( data.GetLength() - 1 ) );
			int size = tok.GetSize();
			if( size == 2 )
			{
				int right;
				int bottom;

				tok.GetAt(0, right );
				tok.GetAt(1, bottom );

				SetVirtualSize( CSize( right, bottom ) );
				result = TRUE;
			}
		}
	}

	return result;

}

void CDiagramEntityContainer::Export( CStringArray& stra, UINT format ) const
/* ============================================================
	Function :		CDiagramEntityContainer::Export
	Description :	Exports all objects to format format.
					
	Return :		void
	Parameters :	CStringArray& stra	-	CStingArray that 
											will be filled with 
											data on return. 
					UINT format			-	Format to save to.
					
	Usage :			Call to export the contents of the container 
					to a CStringArray. Export will - of course - 
					have to be defined for the derived objects.

   ============================================================*/
{

	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CDiagramEntity* obj = GetAt( t );
		stra.Add( obj->Export( format ) );
	}

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer data access

int CDiagramEntityContainer::GetSize() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetSize
	Description :	Returns the number of objects in the data
					container.
					
	Return :		int		-	The number of objects.
	Parameters :	none

	Usage :			Call to get the number of objects currently 
					in the data array of the container.

   ============================================================*/
{

	return m_objs.GetSize();

}

int	CDiagramEntityContainer::GetSelectCount() const
{
	int count=0;
	for (int i = 0; i < GetSize(); i++)
		if (GetAt(i)->IsSelected()) count++;
	return count;
}

void CDiagramEntityContainer::Add( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Add
	Description :	Add an object to the data.
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to add.
					
	Usage :			Call to add a new object to the container.

   ============================================================*/
{

	obj->SetParent( this );
	m_objs.Add( obj );
	SetModified( TRUE );

}

CDiagramEntity* CDiagramEntityContainer::GetSelectedObject() 
{
	if (m_lastSelObj) return m_lastSelObj;

	for (int i = GetSize()-1; i >= 0; i--)  {
		CDiagramEntity* obj= GetAt(i);
		if (obj->IsSelected()) {
			m_lastSelObj = obj;
			return obj;
		}
	}

	return NULL;
}

CDiagramEntity* CDiagramEntityContainer::GetAt( int index ) const
/* ============================================================
	Function :		CDiagramEntityContainer::GetAt
	Description :	Gets the object at position index.
					
	Return :		CDiagramEntity*	-	The object of NULL if 
										out of range.
	Parameters :	int index		-	The index to get data 
										.from
					
	Usage :			Call to get a specific object from the 
					container.

   ============================================================*/
{

	CDiagramEntity* result = NULL;
	if( index < m_objs.GetSize() && index >= 0 )
		result = static_cast< CDiagramEntity* >( m_objs.GetAt( index ) );
	return result;

}

void CDiagramEntityContainer::SetAt( int index, CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::SetAt
	Description :	Sets an object at poisition index.
					
	Return :		void
	Parameters :	int index			-	Index to set data 
											at.
					CDiagramEntity* obj	-	Object to set.
					
	Usage :			Internal function. Used by Swap.

   ============================================================*/
{

	m_objs.SetAt( index, obj );
	SetModified( TRUE );

}

void CDiagramEntityContainer::RemoveAt( int index )
/* ============================================================
	Function :		CDiagramEntityContainer::RemoveAt
	Description :	Removes the object at index.
					
	Return :		void
	Parameters :	int index	-	The index of the object 
									to remove.
					
	Usage :			Call to remove a specific object. Memory is 
					freed.

   ============================================================*/
{

	CDiagramEntity* obj = GetAt( index );
	if( obj )
	{
		if (obj == m_lastSelObj) m_lastSelObj = NULL;
		delete obj;
		m_objs.RemoveAt( index );
		SetModified( TRUE );
	}

}

void CDiagramEntityContainer::RemoveAll()
/* ============================================================
	Function :		CDiagramEntityContainer::RemoveAll
	Description :	Removes all data objects
					
	Return :		void
	Parameters :	none

	Usage :			Call to remove all data objects in the 
					container. Undo- and paste arrays are not 
					emptied.
					Allocated memory is released. Undo and 
					paste not deleted.

   ============================================================*/
{
	m_lastSelObj = NULL;
	int max = m_objs.GetSize() - 1;
	for( int t = max ; t >= 0 ; t-- )
		RemoveAt( t );

}

void CDiagramEntityContainer::RemoveAllSelected()
/* ============================================================
	Function :		CDiagramEntityContainer::RemoveAllSelected
	Description :	Removes all selected objects
					
	Return :		void
	Parameters :	none

	Usage :			Call to remove all selected objects from the 
					container. Releases allocated data

   ============================================================*/
{
	m_lastSelObj = NULL;
	int max = m_objs.GetSize() - 1;
	for( int t = max ; t >= 0 ; t-- )
		if( GetAt( t )->IsSelected() )
			RemoveAt( t );

}

void CDiagramEntityContainer::Select( CDiagramEntity* obj, BOOL selected)
{
	if (!obj) return;

	obj->m_selected = selected;

	if (selected) {
		m_lastSelObj = obj;	
	} else {
		if (obj == m_lastSelObj) m_lastSelObj = NULL;
	}
}

void CDiagramEntityContainer::LeftAlignSelected()
{
	if( GetSelectCount() > 1 )
	{
		CDiagramEntity* obj = GetSelectedObject();
		if( obj )
		{
			double left = obj->GetLeft();
			int count = 0;
			while (obj = GetAt( count++ ))
			{
				if (!obj->IsSelected()) continue;
				double width = obj->GetRight() - obj->GetLeft();
				double right = left + width;
				obj->SetRect( left, obj->GetTop(), right, obj->GetBottom() );
			}

		}
		SetModified( TRUE );
	}
}

void CDiagramEntityContainer::RightAlignSelected()
{
	if( GetSelectCount() > 1 )
	{
		CDiagramEntity* obj = GetSelectedObject();
		if( obj )
		{
			double right = obj->GetRight();
			int count = 0;
			while (obj = GetAt( count++ ))
			{
				if (!obj->IsSelected()) continue;
				double width = obj->GetRight() - obj->GetLeft();
				double left = right - width;
				obj->SetRect( left, obj->GetTop(), right, obj->GetBottom() );
			}

		}
		SetModified( TRUE );
	}
}

void CDiagramEntityContainer::TopAlignSelected()
{
	if( GetSelectCount() > 1 )
	{
		CDiagramEntity* obj = GetSelectedObject();
		if( obj )
		{
			double top = obj->GetTop();
			int count = 0;
			while (obj = GetAt( count++ ))
			{
				if (!obj->IsSelected()) continue;
				double height = obj->GetBottom() - obj->GetTop();
				double bottom = top + height;
				obj->SetRect( obj->GetLeft(), top, obj->GetRight(), bottom );
			}

		}
		SetModified( TRUE );
	}
}

void CDiagramEntityContainer::BottomAlignSelected()
{
	if( GetSelectCount() > 1 )
	{
		CDiagramEntity* obj = GetSelectedObject();
		if( obj )
		{
			double bottom = obj->GetBottom();
			int count = 0;
			while (obj = GetAt( count++ ))
			{
				if (!obj->IsSelected()) continue;
				double height = obj->GetBottom() - obj->GetTop();
				double top = bottom - height;
				obj->SetRect( obj->GetLeft(), top, obj->GetRight(), bottom );
			}

		}
		SetModified( TRUE );
	}
}

void CDiagramEntityContainer::MakeSameSizeSelected()
{
	if( GetSelectCount() > 1 )
	{
		CDiagramEntity* obj = GetSelectedObject();
		if( obj )
		{
			double width = obj->GetRight() - obj->GetLeft();
			double height = obj->GetBottom() - obj->GetTop();
			int count = 0;
			while (obj = GetAt( count++ ))
			{
				if (!obj->IsSelected()) continue;
				obj->SetRect( obj->GetLeft(), obj->GetTop(), obj->GetLeft() + width, obj->GetTop() + height );
			}

		}
		SetModified( TRUE );
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer property access

void CDiagramEntityContainer::SetVirtualSize( CSize size )
/* ============================================================
	Function :		CDiagramEntityContainer::SetVirtualSize
	Description :	Sets the current virtual paper size.
					
	Return :		void
	Parameters :	CSize size	-	The size to set
					
	Usage :			Call to set the paper size. Note that 
					SetModified( TRUE ) might have to be called 
					as well.

   ============================================================*/
{

	m_virtualSize = size;

}

CSize CDiagramEntityContainer::GetVirtualSize() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetVirtualSize
	Description :	Gets the virtual paper size.
					
	Return :		CSize	-	The current size
	Parameters :	none

	Usage :			Call to get the current paper size.

   ============================================================*/
{

	return m_virtualSize;

}

BOOL CDiagramEntityContainer::IsModified() const
/* ============================================================
	Function :		CDiagramEntityContainer::IsModified
	Description :	Returns the state of the modified-flag.
					
	Return :		BOOL	-	TRUE if data is changed
	Parameters :	none

	Usage :			Call to see if data is modified.

   ============================================================*/
{

	return m_dirty;

}

void CDiagramEntityContainer::SetModified( BOOL dirty )
/* ============================================================
	Function :		CDiagramEntityContainer::SetModified
	Description :	Sets the state of the modified flag
					
	Return :		void
	Parameters :	BOOL dirty	-	TRUE if data is changed.
					
	Usage :			Call to mark the data as modified.

   ============================================================*/
{

	m_dirty = dirty;

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer single object handlers

void CDiagramEntityContainer::Remove( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Remove
	Description :	Removes the object.
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to 
											remove.
					
	Usage :			Call to remove obj - if it exists - from the 
					container. Allocated memory is released.

   ============================================================*/
{
	if (obj == m_lastSelObj) m_lastSelObj = NULL;

	int index = Find( obj );
	if( index != -1 )
		RemoveAt( index );

}

void CDiagramEntityContainer::Duplicate( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Duplicate
	Description :	Duplicates the object and adds the new 
					one 10 pixels offset down and right.
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to 
											duplicate.	
					
	Usage :			Call to create a copy of the selected 
					element.

   ============================================================*/
{

	int index = Find( obj );
	if( index != -1 )
	{
		CDiagramEntity* newobj = obj->Clone();
		newobj->SetRect( newobj->GetLeft() + 10, newobj->GetTop() + 10, newobj->GetRight() + 10, newobj->GetBottom() + 10 );
		Add( newobj );
	}

}

void CDiagramEntityContainer::Cut( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Cut
	Description :	Cuts out the object and puts it into the 
					"clipboard"
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to cut.
					
	Usage :			Call in response to a Cut-command. See also 
					the functions for copy/paste below.

   ============================================================*/
{

	Copy( obj );
	Remove( obj );

}

void CDiagramEntityContainer::Copy( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Copy
	Description :	Copies the object to the "clipboard".
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to copy.	
					
	Usage :			Call in response to a Copy-command. Note 
					that obj will only be copied to the 
					clipboard, not the screen. See also the 
					functions for copy/paste below.

   ============================================================*/
{

	ASSERT( obj );

	if( m_clip == NULL )
		m_clip = &m_internalClip;

	if( obj )
		m_clip->Copy( obj );

}

void CDiagramEntityContainer::Up( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Up
	Description :	Moves the object one step up in the z-
					order.
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to move.	
					
	Usage :			Call to move obj in the z-order.

   ============================================================*/
{

	int index = Find( obj );
	Swap( index, index + 1);

}

void CDiagramEntityContainer::Down( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Down
	Description :	Moves the object one step down in the z-
					order.
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to move.	
					
	Usage :			Call to move obj in the z-order.

   ============================================================*/
{

	int index = Find( obj );
	Swap( index, index - 1);

}

void CDiagramEntityContainer::Front( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Front
	Description :	Moves obj to the top of the z-order.
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to move.	
					
	Usage :			Call to move obj in the z-order.

   ============================================================*/
{

	int index = Find( obj );
	m_objs.RemoveAt( index );
	m_objs.Add( obj );
	SetModified( TRUE );

}

void CDiagramEntityContainer::Bottom( CDiagramEntity* obj )
/* ============================================================
	Function :		CDiagramEntityContainer::Bottom
	Description :	Moves obj to the bottom of the z-order.
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to move.
					
	Usage :			Call to move obj in the z-order.

   ============================================================*/
{

	int index = Find( obj );
	m_objs.RemoveAt( index );
	m_objs.InsertAt( 0, obj );
	SetModified( TRUE );

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer copy/paste is implemented as separate class.

void CDiagramEntityContainer::SetClipboardHandler( CDiagramClipboardHandler* clip )
/* ============================================================
	Function :		CDiagramEntityContainer::SetClipboardHandler
	Description :	Sets the container clipboard class.
					
	Return :		void
	Parameters :	CDiagramClipboardHandler* clip	-	A pointer
														to the
														class
					
	Usage :			Call to set the clipboard handler for this 
					container. The same clipboard handler 
					instance can be used for several containers 
					to allow several editors (in an MDI-
					application) to share the same clipboard.

   ============================================================*/
{

	m_clip = clip;

}

CDiagramClipboardHandler* CDiagramEntityContainer::GetClipboardHandler()
/* ============================================================
	Function :		CDiagramEntityContainer::GetClipboardHandler
	Description :	Returns a pointer to the current clipboard 
					handler.
					
	Return :		CDiagramClipboardHandler*	-	Current handler.
	Parameters :	none

	Usage :			Call to get a pointer to the current handler.

   ============================================================*/
{

	return m_clip;

}

void CDiagramEntityContainer::CopyAllSelected()
/* ============================================================
	Function :		CDiagramEntityContainer::CopyAllSelected
	Description :	Clones all selected object to the paste 
					array.
					
	Return :		void
	Parameters :	none

	Usage :			Call to copy all selected objects to the 
					clipboard. Paste will put them on screen.

   ============================================================*/
{

	if( m_clip == NULL )
		m_clip = &m_internalClip;

	m_clip->CopyAllSelected( this );

}

int CDiagramEntityContainer::ObjectsInPaste()
/* ============================================================
	Function :		CDiagramEntityContainer::ObjectsInPaste
	Description :	Returns the number of objects in the paste 
					array.
					
	Return :		int		-	The number of objects.
	Parameters :	none

	Usage :			Call to get the number of objects in the 
					clipboard.

   ============================================================*/
{

	if( m_clip == NULL )
		m_clip = &m_internalClip;

	return m_clip->ObjectsInPaste();

}

void CDiagramEntityContainer::ClearPaste()
/* ============================================================
	Function :		CDiagramEntityContainer::ClearPaste
	Description :	Clears the paste-array.
					
	Return :		void
	Parameters :	none

	Usage :			Call to clear the clipboard. All memory is 
					released.

   ============================================================*/
{

	if( m_clip == NULL )
		m_clip = &m_internalClip;

	m_clip->ClearPaste();

}

void CDiagramEntityContainer::Paste()
/* ============================================================
	Function :		CDiagramEntityContainer::Paste
	Description :	Clones the contents of the paste array 
					into the container data array.

					
	Return :		void
	Parameters :	none

	Usage :			Call to paste the contents of the clipboard 
					to screen.

   ============================================================*/
{

	if( m_clip == NULL )
		m_clip = &m_internalClip;

	m_clip->Paste( this );

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer message handling

void CDiagramEntityContainer::SendMessageToObjects( int command, BOOL selected, CDiagramEntity* sender, CWnd* from )
/* ============================================================
	Function :		CDiagramEntityContainer::SendMessageToObjects
	Description :	Sends command to objects. 
					
	Return :		void
	Parameters :	int command				-	The command to send.
					BOOL selected			-	If TRUE, the command 
												will only be sent to 
												selected objects, 
												otherwise, it will be 
												sent to all objects.
					CDiagramEntity* sender	-	Original sender 
												or NULL if not 
												an object.

	Usage :			Call this member to send messages to 
					(selected) objects in the range CMD_START 
					to CMD_END inclusively (defined in 
					DiagramEntity.h). Calls the object DoCommand.

   ============================================================*/
{

	BOOL stop = FALSE;
	int max = m_objs.GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CDiagramEntity* obj = GetAt( t );
		//if( !stop && ( !selected || obj->IsSelected() ) )
		if( !stop )
		{
			stop = obj->DoMessage( command, sender, from );
			SetModified( TRUE );
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer private helpers

int CDiagramEntityContainer::Find( CDiagramEntity* testobj )
/* ============================================================
	Function :		CDiagramEntityContainer::Find
	Description :	Finds the index of object testobj in the 
					data array.
					
	Return :		int						-	Index of the 
												object or -1 
												if not found.
	Parameters :	CDiagramEntity* testobj	-	Object to find.
					
	Usage :			Internal function. 

   ============================================================*/
{

	int index = -1;
	CDiagramEntity* obj;
	int count = 0;
	while( ( obj = GetAt( count ) ) )
	{
		if( obj == testobj )
			index = count;
		count++;
	}

	return index;

}

void CDiagramEntityContainer::Swap( int index1, int index2 )
/* ============================================================
	Function :		CDiagramEntityContainer::Swap
	Description :	Swap the elements at index1 and index2.
					
	Return :		void
	Parameters :	int index1	-	First object to swap
					int index2	-	Second object to swap
					
	Usage :			Internal function. Used to move objects up 
					or down in the z-order.

   ============================================================*/
{

	int max = m_objs.GetSize();
	if( index1 >= 0 && index1 < max && index2 >= 0 && index2 < max )
	{
		CDiagramEntity* obj1 = GetAt( index1 );
		CDiagramEntity* obj2 = GetAt( index2 );
		SetAt( index1, obj2 );
		SetAt( index2, obj1 );
	}

}

/////////////////////////////////////////////////////////////////////////////
// The Undo-functionality is implemented as a simple FILO-stack of CObArray 
// pointers. Before any change that should be undoable, Snapshot is called 
// to add new entries to the Undo-stack. This is normally managed by the 
// editor, and need only be done manually for added functionality.
//
// Note that the CDiagramEntityContainer should normally not call Snapshot 
// itself - in the case of, for example, additions to m_objs, the container 
// can not and should not know if it is an undoable operation.
//

void CDiagramEntityContainer::Undo()
/* ============================================================
	Function :		CDiagramEntityContainer::Undo
	Description :	Sets the container data to the last entry 
					in the undo stack.
					
	Return :		void
	Parameters :	none

	Usage :			Call to undo the last operation

   ============================================================*/
{

	if( m_undo.GetSize() )
	{
		// We remove all current data
		RemoveAll();

		// We get the last entry from the undo-stack
		// and clone it into the container data
		CUndoItem* undo = static_cast< CUndoItem* >( m_undo.GetAt( m_undo.GetUpperBound() ) );
		int count = ( undo->arr )->GetSize();
		for( int t = 0 ; t < count ; t++ )
		{

			CDiagramEntity* obj = static_cast< CDiagramEntity* >( ( undo->arr )->GetAt( t ) );
			Add( obj->Clone() );

		}

		// Set the saved virtual size as well
		SetVirtualSize( *( undo->pt ) );

		// We remove the entry from the undo-stack
		delete undo;
		m_undo.RemoveAt( m_undo.GetUpperBound() );

	}

}

void CDiagramEntityContainer::Snapshot()
/* ============================================================
	Function :		CDiagramEntityContainer::Snapshot
	Description :	Copies the current state of the data to 
					the undo-stack.
					
	Return :		void
	Parameters :	none

	Usage :			Call to add the current state to the undo-stack. 
					If the undo stack has a maximum size and 
					the stack will grow above the stack limit, 
					the first undo array will be removed.

   ============================================================*/
{

	if( m_maxstacksize > 0 && m_undo.GetSize() == m_maxstacksize )
	{
		delete m_undo.GetAt( 0 );
		m_undo.RemoveAt( 0 );
	}

	CUndoItem* undo = new CUndoItem;

	while( !undo && m_undo.GetSize() )
	{

		// We seem - however unlikely -
		// to be out of memory.
		// Remove first element in
		// undo-stack and try again
		delete m_undo.GetAt( 0 );
		m_undo.RemoveAt( 0 );
		undo = new CUndoItem;

	}

	if( undo )
	{

		// Save current virtual size
		*( undo->pt ) = GetVirtualSize();

		// Save all objects
		int count = m_objs.GetSize();
		for( int t = 0 ; t < count ; t++ )
			( undo->arr )->Add( GetAt( t )->Clone() );

		// Add to undo stack
		m_undo.Add( undo );

	}

}

void CDiagramEntityContainer::ClearUndo()
/* ============================================================
	Function :		CDiagramEntityContainer::ClearUndo
	Description :	Remove all undo arrays from the undo stack
					
	Return :		void
	Parameters :	none

	Usage :			Call to clear the undo-stack. All memory will 
					be deleted.

   ============================================================*/
{

	int count = m_undo.GetSize() - 1;
	for( int t = count ; t >= 0 ; t-- )
	{
		CUndoItem* undo = static_cast< CUndoItem* >( m_undo.GetAt( t ) );

		// Remove all objects in the stack entry
		int max = ( undo->arr )->GetSize();
		for( int i = 0; i < max ; i++ )
			delete static_cast< CObArray* >( ( undo->arr )->GetAt( i ) );
		( undo->arr )->RemoveAll();

		// Remove the stack entry itself.
		delete undo;
	}

	m_undo.RemoveAll();

}

BOOL CDiagramEntityContainer::IsUndoPossible()
/* ============================================================
	Function :		CDiagramEntityContainer::IsUndoPossible
	Description :	Returns TRUE if it is possible to undo.
					
	Return :		BOOL	-	TRUE if undo is possible.
	Parameters :	none

	Usage :			Use this call for command enabling

   ============================================================*/
{

	return m_undo.GetSize();

}


void CDiagramEntityContainer::SetUndoStackSize( int maxstacksize )
/* ============================================================
	Function :		CDiagramEntityContainer::SetUndoStackSize
	Description :	Sets the size of the undo stack.
					
	Return :		void
	Parameters :	int maxstacksize	-	New size. -1 means 
											no limit, 0 no undo.
					
	Usage :			Call to set the max undo stack size.

   ============================================================*/
{

	m_maxstacksize = maxstacksize;

}

int CDiagramEntityContainer::GetUndoStackSize() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetUndoStackSize
	Description :	Returns the size of the undo-stack

	Return :		int	-	Current size
	Parameters :	none

	Usage :			Call to get the max undo stack size.

   ============================================================*/
{

	return m_maxstacksize;

}

#pragma warning( default : 4706 )
