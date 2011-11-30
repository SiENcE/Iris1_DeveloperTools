#pragma once

#ifndef _GFX
#define _GFX

enum TEXT_ALIGN { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT };

CString GfxAligntoText(TEXT_ALIGN align);
TEXT_ALIGN GfxTexttoAlign(CString strAlign);

int GfxAtoX(CString strNum);
CString GfxXtoA(int iNum);
CString GfxGetFullPath(CString strPath);

void GfxUpdateStatusBar(int iGumpID, CPoint point);

CString GfxSprintf(LPCTSTR lpszFormat, ...);
void GfxLog(LPCTSTR lpszFormat, ...);

// '\\n' -> '\r\n'
CString GfxFormatText(CString strText);

// 'c:\\aa\\bb.txt' -> { 'c:\\aa', 'bb', 'txt' }
void GfxSplitPathName(const CString& strPathName, CString& strDir, CString& strFile, CString& strExt);

// 'c:\\aa\\bb.txt' -> 'bb.txt' (bIncludeExt = true), 'bb' (false)
CString GfxGetFileName(const CString& strPathName, bool bIncludeExt = true);

// 'c:\\aa\\bb.txt' -> 'c:\\aa'
CString GfxGetDirName(const CString& strPathName);

// 'c:\\aa\\bb.txt' -> 'txt'
CString GfxGetExtName(const CString& strPathName);

// 'iris -gui gfm/test.gfm', false
CString GfxExecuteExternalFile(CString csExeName, CString csArguments, bool bUsePipe);


CString GfxResetWorkingDirectory();

#endif