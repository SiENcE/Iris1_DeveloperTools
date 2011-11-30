/* ==========================================================================
	CTokeniz

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-31

	Purpose :		CTokenizer is a very simple class to tokenize a string 
					into a string array.	

	Description :	The string is chopped up and put into an internal 
					CStringArray. With GetAt, different types of data can 
					be read from the different elements.

	Usage :			

   ========================================================================*/
#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <tchar.h>

////////////////////////////////////////////////////////////////////////////
// CTokenizer is a class to tokenize a string
//

class CTokenizer
{
public:
	CTokenizer(CString strInput, const CString& strDelimiter = _T( "," ) )
	{
		Init(strInput, strDelimiter);
	}

	void Init(const CString& strInput, const CString& strDelimiter = _T( "," ) )
	{
		CString copy( strInput );
		m_stra.RemoveAll();
		int nFound = copy.Find( strDelimiter );

		while(nFound != -1)
		{
			CString strLeft;
			strLeft = copy.Left( nFound );
			copy = copy.Right( copy.GetLength() - ( nFound + 1 ) );

			m_stra.Add( strLeft );
			nFound = copy.Find( strDelimiter );
		}

		m_stra.Add( copy );
	}

	int GetSize() const
	{
		return m_stra.GetSize();
	}

	void GetAt( int nIndex, CString& str ) const
	{
		if( nIndex < m_stra.GetSize() )
			str = m_stra.GetAt( nIndex );
		else
			str = _T( "" );
	}

	void GetAt( int nIndex, int& var ) const
	{
		if( nIndex < m_stra.GetSize() )
			var = _ttoi( m_stra.GetAt( nIndex ) );
		else
			var = 0;
	}

	void GetAt( int nIndex, WORD& var ) const
	{
		if( nIndex < m_stra.GetSize() )
			var = ( WORD ) _ttoi( m_stra.GetAt( nIndex ) );
		else
			var = 0;
	}

	void GetAt( int nIndex, double& var ) const
	{
		char   *stop;
		if( nIndex < m_stra.GetSize() )
			var = _tcstod( m_stra.GetAt( nIndex ), &stop );
		else
			var = 0.0;
	}

	void GetAt( int nIndex, DWORD& var ) const
	{
		if( nIndex < m_stra.GetSize() )
			var = ( DWORD ) _ttoi( m_stra.GetAt( nIndex ) );
		else
			var = 0;
	}

private:

	CStringArray m_stra;

};

#endif // _TOKENIZER_H_
