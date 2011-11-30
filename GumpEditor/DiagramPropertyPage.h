#pragma once

#include "DiagramEntity.h"

class CDiagramEntity;
class CDiagramPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CDiagramPropertyPage)

public:
	CDiagramPropertyPage( UINT nIDTemplate);
	virtual ~CDiagramPropertyPage();

	//enum { IDD = IDD_DIAGRAMPROPERTYPAGE };

public:
	void			SetEntity( CDiagramEntity* entity ) { m_entity = entity; }
	CDiagramEntity*	GetEntity() const { return m_entity; }

	virtual CWnd*	GetRedrawWnd() { return m_redrawWnd; }
	virtual void	SetRedrawWnd( CWnd* redrawWnd ) { m_redrawWnd = redrawWnd; }
	virtual void	Redraw() { 
		CWnd* wnd = GetRedrawWnd();
		if( wnd )
			wnd->RedrawWindow() ;
	}

	virtual void	SetValues() = 0;
	virtual void	ApplyValues() = 0;
	virtual void	ResetValues() {}
private:

	CDiagramEntity*	m_entity;
	CWnd*			m_redrawWnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
};
