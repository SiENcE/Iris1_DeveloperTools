#pragma once

#include <afxtempl.h>

struct sTagInfo {
	CString name;
	CString full_name;
	CString file;
	CString line;

	sTagInfo(LPCTSTR n="", LPCTSTR fn="", LPCTSTR f="", CString l="") : name(n), full_name(fn), file(f), line(l) {}
};

class CTagInfo : public CArray<sTagInfo, sTagInfo&> {
public:
	bool GenerateTagInfo(LPCTSTR cslfilname, LPCTSTR tagfilename);
	int ReadTagInfoFromFile(LPCTSTR filename);
	int ReadTagInfoFromBuf(LPCTSTR source);
	int FindTagLine(LPCTSTR tag);	
};