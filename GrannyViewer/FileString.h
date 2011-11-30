#pragma once

class CFileString : public CString
{
public:
	CFileString() {}
	CFileString(const CString& s) : CString(s) {}
	virtual ~CFileString() {}

	// this   : "c:\\temp\\1.txt"
	// return : "c:\\temp"
	CString GetDirName();
	// return : "1.txt"
	CString GetFileName();
	// return : "txt"
	CString GetFileExt();
	static CString GetWorkingDir();
	static CString GetModuleDir();
};
