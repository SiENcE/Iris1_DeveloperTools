#include "StdAfx.h"
#include ".\gfx.h"
#include "MainFrm.h"
#include "GrannyViewerDoc.h"
#include <direct.h>

int GfxAtoX(CString strNum)
{
	strNum.Trim();
	int iNum=atoi(strNum);
	if (strNum.Left(2).CompareNoCase("0x")==0)
		sscanf(strNum, "%x", &iNum);
	return iNum;
}

CString GfxXtoA(int iNum)
{
	CString strNum;
	strNum.Format("0x%X", iNum);
	return strNum;
}

CString GfxGetFullPath(CString strPath)
{
	strPath.Replace('/', '\\');
	
	if (strPath.Find(":\\") < 0)
	{
		char dir[_MAX_PATH]={0,};
		_getcwd(dir, sizeof(dir));

		if (strPath.Left(1) != "\\")
			strPath = "\\" + strPath;

		strPath = CString(dir) + strPath;
	}

	return strPath;
}


CString GfxSprintf(LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	TCHAR szBuffer[512];

	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);

	// was there an error? was the expanded string too long?
	ASSERT(nBuf >= 0);

	va_end(args);

	return CString(szBuffer);
}



void GfxLog(LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	TCHAR szBuffer[2048];

	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);

	// was there an error? was the expanded string too long?
	ASSERT(nBuf >= 0);

	va_end(args);

	CGrannyViewerDoc* pDoc = GfxGetDocument();
	if (!pDoc) return;
	pDoc->Log(szBuffer);
}



