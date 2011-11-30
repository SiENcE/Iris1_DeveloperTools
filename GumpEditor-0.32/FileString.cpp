#include "StdAfx.h"
#include ".\filestring.h"
#include <direct.h>

// this   : "c:\\temp\\1.txt"
// return : "c:\\temp"
CString CFileString::GetDirName()
{
	int iLoc = ReverseFind('\\');
	if (iLoc == -1) return "";

	return Left(iLoc);
}
// return : "1.txt"
CString CFileString::GetFileName()
{
	int iLen = GetLength(), iLoc = ReverseFind('\\');
	if (iLoc == -1) return "";

	return Right(iLen - iLoc - 1);
}
// return : "txt"
CString CFileString::GetFileExt()
{
	int iLen = GetLength(), iLoc = ReverseFind('.');
	if (iLoc == -1) return "";

	return Right(iLen - iLoc - 1);
}
CString CFileString::GetWorkingDir()
{
	char dir[_MAX_PATH];
	if (_getcwd(dir, sizeof(dir))) return dir;
	return "";
}

CString CFileString::GetModuleDir()
{
	TCHAR szPath[_MAX_PATH + 1];
	::GetModuleFileName(AfxGetApp()->m_hInstance, szPath, _MAX_PATH);
	
	return CFileString(szPath).GetDirName();
}
