// ScintillaFormat.h: Schnittstelle für die Klasse CScintillaFormat.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCINTILLAFORMAT_H__CF881535_3F6A_4B63_BD2A_11B02DBED773__INCLUDED_)
#define AFX_SCINTILLAFORMAT_H__CF881535_3F6A_4B63_BD2A_11B02DBED773__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CScintillaFormat  
{
public:
	CScintillaFormat();
	virtual ~CScintillaFormat();
protected:
   CString m_strExtension;
   CString m_strFormat;
};

#endif // !defined(AFX_SCINTILLAFORMAT_H__CF881535_3F6A_4B63_BD2A_11B02DBED773__INCLUDED_)
