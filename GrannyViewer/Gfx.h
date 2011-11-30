#pragma once

int GfxAtoX(CString strNum);
CString GfxXtoA(int iNum);
CString GfxGetFullPath(CString strPath);

CString GfxSprintf(LPCTSTR lpszFormat, ...);
void GfxLog(LPCTSTR lpszFormat, ...);

class CGrannyViewerDoc;
CGrannyViewerDoc* GfxGetDocument();

const UINT INVALID_GRN_ID = 0xFFFFFFFF;

#include <algorithm>
#include <string>

std::string getFileName(const std::string& path_name, char split='/');

