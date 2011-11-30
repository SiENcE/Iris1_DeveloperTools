#ifndef _DIAGRAMENTITYCONTAINER_H_
#define _DIAGRAMENTITYCONTAINER_H_
class CDiagramEntity;
#include "DiagramClipboardHandler.h"

class CUndoItem : public CObject 
{

public:
	CUndoItem() {

		arr = new CObArray;
		pt = new CPoint;

	};

	~CUndoItem() {

		int max = arr->GetSize();
		for (int t = 0 ; t < max ; t++ )
			delete arr->GetAt( t );
		arr->RemoveAll();

		delete arr;
		delete pt;

	}

	CObArray*	arr;
	CPoint*		pt;

};

class CDiagramEntityContainer {

public:

	// Construction/destruction/initialization
	CDiagramEntityContainer( CDiagramClipboardHandler* clip = NULL );
	virtual ~CDiagramEntityContainer();
	void Clear();
	virtual CString	GetString() const;
	virtual BOOL FromString( const CString& str );
	virtual void Export( CStringArray& stra, UINT format = 0 ) const;
	virtual void SetClipboardHandler( CDiagramClipboardHandler* clip );
	virtual CDiagramClipboardHandler* GetClipboardHandler();

	// Data access
	int				GetSize() const;
	int				GetSelectCount() const;
	void			Add( CDiagramEntity* obj );
	CDiagramEntity* GetSelectedObject() ;
	CDiagramEntity* GetAt( int index ) const;
	virtual void	RemoveAt( int index );
	void			RemoveAll();
	void			RemoveAllSelected();
	void			Remove( CDiagramEntity* obj );
	void			Select( CDiagramEntity* obj, BOOL selected);

	void			SetVirtualSize( CSize size );
	CSize			GetVirtualSize() const;

	void			SetModified( BOOL dirty );
	BOOL			IsModified() const;

	// Undo handling
	virtual void	Undo();
	virtual void	Snapshot();
	BOOL			IsUndoPossible();
	virtual void	ClearUndo();
	void			SetUndoStackSize( int maxstacksize );
	int				GetUndoStackSize() const;

	// Single object handlers
	virtual void			Duplicate( CDiagramEntity* obj );
	virtual void			Cut( CDiagramEntity* obj );
	virtual void			Copy( CDiagramEntity* obj );
	virtual void			Up( CDiagramEntity* obj );
	virtual void			Down( CDiagramEntity* obj );
	virtual void			Front( CDiagramEntity* obj );
	virtual void			Bottom( CDiagramEntity* obj );
	virtual void			Paste();

	virtual void			LeftAlignSelected();
	virtual void			RightAlignSelected();
	virtual void			TopAlignSelected();
	virtual void			BottomAlignSelected();
	virtual void			MakeSameSizeSelected();

	// Copy/paste
	virtual void			CopyAllSelected();
	virtual int				ObjectsInPaste();
	virtual void			ClearPaste();

	// Message handling
	virtual void			SendMessageToObjects( int command, BOOL selected = TRUE, CDiagramEntity* sender = NULL, CWnd* from = NULL );

	CObArray*				GetData()	{ return &m_objs; }

protected:

	CObArray*				GetPaste()	{ 
		CObArray* arr = NULL;
		if( m_clip )
			arr = m_clip->GetData();

		return arr; 
	};

	CObArray*				GetUndo()	{ return &m_undo; }
	int						Find( CDiagramEntity* obj );

private:

	// Data
	CDiagramEntity* m_lastSelObj;
	CObArray		m_objs;
	CObArray		m_undo;
	int				m_maxstacksize;
	CSize			m_virtualSize;

	CDiagramClipboardHandler*	m_clip;
	CDiagramClipboardHandler	m_internalClip;

	// State
	BOOL			m_dirty;

	// Helpers
	void			Swap( int index1, int index2 );
	void			SetAt( int index, CDiagramEntity* obj );

};

#endif // _DIAGRAMENTITYCONTAINER_H_
