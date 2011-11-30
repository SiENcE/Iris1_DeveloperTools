#include "StdAfx.h"
#include ".\gfx.h"
#include "MainFrm.h"
#include "GumpEditorDoc.h"
#include "FileString.h"
#include <direct.h>

static CString strAligns[] = { _T("left"), _T("center"), _T("right") };

CString GfxAligntoText(TEXT_ALIGN align)
{
	if (align > ALIGN_RIGHT) align = ALIGN_LEFT;
	
	return strAligns[align];
}

TEXT_ALIGN GfxTexttoAlign(CString align)
{
	for (int i=0; i < sizeof(strAligns)/sizeof(strAligns[0]); i++)
		if (align.CompareNoCase(strAligns[i])==0) return (TEXT_ALIGN)i;

	return ALIGN_LEFT;
}


int GfxAtoX(CString strNum)
{
	strNum.Trim();
	int iNum=atoi(strNum);
	if (strNum.Left(2).CompareNoCase(_T("0x"))==0)
		sscanf(strNum, _T("%x"), &iNum);
	return iNum;
}

CString GfxXtoA(int iNum)
{
	CString strNum;
	strNum.Format(_T("0x%X"), iNum);
	return strNum;
}

CString GfxGetFullPath(CString strPath)
{
	strPath.Replace(_T('/'), _T('\\'));
	
	if (strPath.Find(_T(":\\")) < 0)
	{
		char dir[_MAX_PATH]={0,};
		_getcwd(dir, sizeof(dir));

		if (strPath.Left(1) != _T("\\"))
			strPath = _T("\\") + strPath;

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

	TRACE(szBuffer);
	//CGrannyViewerDoc* pDoc = GfxGetDocument();
	//if (!pDoc) return;
	//pDoc->Log(szBuffer);
}


// '\\n' -> '\r\n'
CString GfxFormatText(CString strText)
{
	int i = 0;
	while ((i = strText.Find(_T("\\n"), i)) != -1) {
		if (i > 0 && strText[i-1] != _T(_T('\\'))) {
			strText.SetAt(i,   _T('\r'));
			strText.SetAt(i+1, _T('\n'));
		}
		i += 2;
	}

	return strText;
}

// 'c:\\aa\\bb.txt' -> { 'c:\\aa', 'bb', 'txt' }
void GfxSplitPathName(const CString& strPathName, CString& strDir, CString& strFile, CString& strExt)
{
	int a = strPathName.ReverseFind(_T('\\'));
	int b = strPathName.ReverseFind(_T('.'));
	int c = strPathName.GetLength();

	strDir = strFile = strExt = _T("");

	if (a > 0) strDir = strPathName.Left(a);

	if (b > 0 && b > a) strExt = strPathName.Right(c - b - 1);
	else b = c;

	strFile = strPathName.Mid(a+1, b - a - 1);
}

// 'c:\\aa\\bb.txt' -> 'bb.txt' (bIncludeExt = true), 'bb' (false)
CString GfxGetFileName(const CString& strPathName, bool bIncludeExt)
{
	CString d, f, e;
	GfxSplitPathName(strPathName, d, f, e);
	if (bIncludeExt && f.GetLength() > 0 && e.GetLength() > 0) return f + _T(".") + e;
	return f;
}

// 'c:\\aa\\bb.txt' -> 'c:\\aa'
CString GfxGetDirName(const CString& strPathName)
{
	CString d, f, e;
	GfxSplitPathName(strPathName, d, f, e);
	return d;
}

// 'c:\\aa\\bb.txt' -> 'txt'
CString GfxGetExtName(const CString& strPathName)
{
	CString d, f, e;
	GfxSplitPathName(strPathName, d, f, e);
	return e;
}


CString GfxExecuteExternalFile(CString csExeName, CString csArguments, bool bUsePipe)
{
	CString csExecute = csExeName + " " + csArguments;

	SECURITY_ATTRIBUTES secattr; 
	ZeroMemory(&secattr,sizeof(secattr));
	secattr.nLength = sizeof(secattr);
	secattr.bInheritHandle = TRUE;

	HANDLE rPipe, wPipe;

	//Create pipes to write and read data
	if (bUsePipe) CreatePipe(&rPipe,&wPipe,&secattr,0);
	//
	STARTUPINFO sInfo; 
	ZeroMemory(&sInfo,sizeof(sInfo));
	PROCESS_INFORMATION pInfo; 
	ZeroMemory(&pInfo,sizeof(pInfo));
	sInfo.cb=sizeof(sInfo);
	if (bUsePipe) {
		sInfo.dwFlags=STARTF_USESTDHANDLES;
		sInfo.hStdInput=NULL; 
		sInfo.hStdOutput=wPipe; 
		sInfo.hStdError=wPipe;
	}
	char command[1024]; 
	strcpy(command, csExecute.GetBuffer(csExecute.GetLength()));


	DWORD dwFlags = NORMAL_PRIORITY_CLASS;
	if (bUsePipe) dwFlags |= CREATE_NO_WINDOW;
	//Create the process here.
	CreateProcess(0, command,0,0,TRUE, dwFlags,0,0,&sInfo,&pInfo);
	
	CString csOutput, csTemp;

	if (bUsePipe) {
		CloseHandle(wPipe);

		//now read the output pipe here.
		char buf[100];
		DWORD reDword; 
		
		BOOL res;
		do
		{
			res=::ReadFile(rPipe,buf,100,&reDword,0);
			csTemp=buf;
			csOutput += csTemp.Left(reDword);
		}while(res);

		
	}

	return csOutput;
}

CString GfxResetWorkingDirectory()
{
	CString strDir = CFileString::GetModuleDir();
	SetCurrentDirectory(strDir);
	return strDir;
}

void GfxTest()
{
	CString d, f, e;
	GfxSplitPathName(_T("c:\\a\\b.txt"), d, f, e);
	ASSERT(d==_T("c:\\a"));
	ASSERT(f==_T("b"));
	ASSERT(e==_T("txt"));
	GfxSplitPathName(_T("c:\\a.c\\b"), d, f, e);
	ASSERT(d==_T("c:\\a.c"));
	ASSERT(f==_T("b"));
	ASSERT(e==_T(""));
	GfxSplitPathName(_T("\\a\\b..txt"), d, f, e);
	ASSERT(d==_T("\\a"));
	ASSERT(f==_T("b."));
	ASSERT(e==_T("txt"));
	GfxSplitPathName(_T("c:\\a\\"), d, f, e);
	ASSERT(d==_T("c:\\a"));
	ASSERT(f==_T(""));
	ASSERT(e==_T(""));
	GfxSplitPathName(_T("b.txt"), d, f, e);
	ASSERT(d==_T(""));
	ASSERT(f==_T("b"));
	ASSERT(e==_T("txt"));
	GfxSplitPathName(_T("b"), d, f, e);
	ASSERT(d==_T(""));
	ASSERT(f==_T("b"));
	ASSERT(e==_T(""));
	GfxSplitPathName(_T(""), d, f, e);
	ASSERT(d==_T(""));
	ASSERT(f==_T(""));
	ASSERT(e==_T(""));
}


