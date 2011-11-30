#if !defined(AFX_GOTOLINEDLG_H__97C30C1C_6FA9_42D7_9483_1C95215F5C5E__INCLUDED_)
#define AFX_GOTOLINEDLG_H__97C30C1C_6FA9_42D7_9483_1C95215F5C5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GotoLineDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CGotoLineDlg 

class CGotoLineDlg : public CDialog
{
// Konstruktion
public:
	CGotoLineDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CGotoLineDlg)
	enum { IDD = IDD_GOTOLINE };
	int		m_nLine;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CGotoLineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CGotoLineDlg)
		// HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_GOTOLINEDLG_H__97C30C1C_6FA9_42D7_9483_1C95215F5C5E__INCLUDED_
